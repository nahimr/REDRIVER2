#include "driver2.h"
#include "models.h"
#include "system.h"
#include "spool.h"
#include "mission.h"
#include "cars.h"

MODEL dummyModel = {0};

char* modelname_buffer = NULL;
char* car_models_lump = NULL;

MODEL* modelpointers[MAX_MODEL_SLOTS];
MODEL* pLodModels[MAX_MODEL_SLOTS];

int num_models_in_pack = 0;

u_short* Low2HighDetailTable = NULL;
u_short* Low2LowerDetailTable = NULL;

// [A]
int staticModelSlotBitfield[48];

// [A] returns freed slot count
int CleanSpooledModelSlots()
{
	int i;
	int num_freed;

	num_freed = 0;

	// assign model pointers
	for (i = 0; i < MAX_MODEL_SLOTS; i++) // [A] bug fix. Init with dummyModel
	{
		// if bit does not indicate usage - reset to dummy model
		if ((staticModelSlotBitfield[i >> 5] & 1 << (i & 31)) == 0)
		{
			if (modelpointers[i] != &dummyModel)
			{
				modelpointers[i] = &dummyModel;
				pLodModels[i] = &dummyModel;

				num_freed++;
			}
		}
	}

	return num_freed;
}

// [D] [T]
void ProcessMDSLump(char* lump_file, int lump_size)
{
	char* mdsfile;
	MODEL* model;
	MODEL* parentmodel;
	int modelAmts;
	int i, size;

	modelAmts = *(int*)lump_file;
	mdsfile = (lump_file + 4);
	num_models_in_pack = modelAmts;

	// [A] usage bits
	ClearMem((char*)staticModelSlotBitfield, sizeof(staticModelSlotBitfield));

	// assign model pointers
	for (i = 0; i < MAX_MODEL_SLOTS; i++) // [A] bug fix. Init with dummyModel
	{
		modelpointers[i] = &dummyModel;
		pLodModels[i] = &dummyModel;
	}

	for (i = 0; i < modelAmts; i++)
	{
		size = *(int*)mdsfile;
		mdsfile += sizeof(int);

		if (size)
		{
			// add the usage bit
			staticModelSlotBitfield[i >> 5] |= 1 << (i & 31);
			TOP_MODEL* mdl = (TOP_MODEL*)mdsfile;

			model = (MODEL*)mdsfile;

			OFFSET_MODEL* offs = (OFFSET_MODEL*)((char*)mdl + sizeof(TOP_MODEL));

			model->vertices = offs->vertices;
			model->normals = offs->normals;
			model->poly_block = offs->poly_block;
			model->point_normals = offs->point_normals;
			model->collision_block = offs->collision_block;

			modelpointers[i] = model;
		}

		mdsfile += size;
	}

	// process parent instances
	for (i = 0; i < modelAmts; i++)
	{
		MODEL* _model = modelpointers[i];

		if (_model->instance_number != -1)
		{
			parentmodel = modelpointers[_model->instance_number];

			// convert to real offsets
			_model->vertices = (s_int64_t)((char*)parentmodel + parentmodel->vertices);
			_model->normals = (s_int64_t)((char*)parentmodel + parentmodel->normals);
			_model->point_normals = (s_int64_t)((char*)parentmodel + parentmodel->point_normals);

			if ((uint)parentmodel->collision_block != 0)
				_model->collision_block = (s_int64_t)((char*)parentmodel + parentmodel->collision_block);
		}
	}

	// process models without parents
	for (i = 0; i < modelAmts; i++)
	{
		MODEL* _model = modelpointers[i];

		if (_model->instance_number == -1)
		{
			_model->vertices += (s_int64_t)_model;
			_model->normals += (s_int64_t)_model;
			_model->point_normals += (s_int64_t)_model;

			if ((uint)_model->collision_block != 0)
				_model->collision_block += (s_int64_t)_model;
		}

		_model->poly_block += (s_int64_t)(char*)_model;
	}
}

// [D] [T]
int ProcessCarModelLump(char* lump_ptr, int lump_size)
{
	int size;
	int* offsets;
	char* models_offset;


	MODEL* model;
	int model_number;
	int i;

	int specMemReq;

	specMemReq = 0;

	models_offset = lump_ptr + 4 + 160; // also skip model count
	offsets = (int*)(lump_ptr + 100);

	// compute special memory requirement for spooling
	for (i = 8; i < 13; i++)
	{
		int cleanOfs = offsets[0];
		int damOfs = offsets[1];
		int lowOfs = offsets[2];

		if (cleanOfs != -1)
		{
			size = ((LOADED_MODEL*)(models_offset + cleanOfs))->poly_block;

			if (damOfs != -1)
				size += ((LOADED_MODEL*)(models_offset + damOfs))->normals;

			if (lowOfs != -1)
				size += ((LOADED_MODEL*)(models_offset + lowOfs))->poly_block;

			size = (size + 2048) + 2048;
			if (size > specMemReq)
				specMemReq = size;

			size = (damOfs - cleanOfs) + 2048;
			if (size > specMemReq)
				specMemReq = size;

			size = (lowOfs - damOfs) + 2048;
			if (size > specMemReq)
				specMemReq = size;

			if (i != 11) // what the fuck is this hack about?
			{
				// next model offset?
				size = (offsets[3] - lowOfs) + 2048;

				if (size > specMemReq)
					specMemReq = size;
			}
		}

		offsets += 3;
	}

	for (i = 0; i < 5; i++)
	{
		gCarCleanModelPtr[i] = NULL;
		gCarDamModelPtr[i] = NULL;
		gCarLowModelPtr[i] = NULL;

		if (i == 4)
			specmallocptr = (char*)mallocptr;

		model_number = MissionHeader->residentModels[i];

		if (model_number == 13)
		{
			model_number = 10 - (MissionHeader->residentModels[0] + MissionHeader->residentModels[1] + MissionHeader->
				residentModels[2]);

			if (model_number < 1)
				model_number = 1;
			else if (model_number > 4)
				model_number = 4;
		}

		if (model_number != -1)
		{
			offsets = (int*)(lump_ptr + 4 + model_number * sizeof(int) * 3);

			int cleanOfs = offsets[0];
			int damOfs = offsets[1];
			int lowOfs = offsets[2];

			if (cleanOfs != -1)
			{
				D_MALLOC_BEGIN();
					model = GetCarModel(models_offset + cleanOfs, (char**)&mallocptr, 1, model_number, CAR_MODEL_CLEAN);
					gCarCleanModelPtr[i] = model;
				D_MALLOC_END();
			}

			if (damOfs != -1)
			{
				D_MALLOC_BEGIN();
					model = GetCarModel(models_offset + damOfs, (char**)&mallocptr, 0, model_number, CAR_MODEL_DAMAGED);
					gCarDamModelPtr[i] = model;
				D_MALLOC_END();
			}

			if (lowOfs != -1)
			{
				D_MALLOC_BEGIN();
					model = GetCarModel(models_offset + lowOfs, (char**)&mallocptr, 1, model_number,
					                    CAR_MODEL_LOWDETAIL);
					gCarLowModelPtr[i] = model;
				D_MALLOC_END();
			}
		}
	}

	D_MALLOC_BEGIN();
		mallocptr = specmallocptr + specMemReq;
		specLoadBuffer = specmallocptr + specMemReq - 2048;
	D_MALLOC_END();

	buildNewCars();

	return 0;
}


char* CarModelTypeNames[] = {
	"CLEAN",
	"DAMAGED",
	"LOW",
};

#ifndef PSX
// [A] loads car model from file
char* LoadCarModelFromFile(char* dest, int modelNumber, int type)
{
	char* mem;
	char filename[64];

	sprintf(filename, "LEVELS\\%s\\CARMODEL_%d_%s.DMODEL", LevelNames[GameLevel], modelNumber,
	        CarModelTypeNames[type - 1]);
	if (FileExists(filename))
	{
		mem = (char*)(dest ? dest : (_other_buffer + modelNumber * 0x10000 + (type - 1) * 0x4000));

		// get model from file
		Loadfile(filename, mem);
		return mem;
	}

	return NULL;
}
#endif

// [D] [T]
MODEL* GetCarModel(char* src, char** dest, int KeepNormals, int modelNumber, int type)
{
	int size;
	TOP_MODEL* top_mdl;
	OFFSET_MODEL* offs_mdl;
	// LOADED_MODEL* model;
	MODEL* _mdl;
	char* mem;

#ifndef PSX
	mem = LoadCarModelFromFile(NULL, modelNumber, type);

	if (!mem) // fallback to lump
		mem = src;
#else
	mem = src;
#endif

	// model = (LOADED_MODEL*)*dest; // Model in memory with offsets (loaded from file)
	_mdl = (MODEL*)*dest;
	top_mdl = (TOP_MODEL*)*dest;
	offs_mdl = (OFFSET_MODEL*)((char*)top_mdl + sizeof(TOP_MODEL));

	if (KeepNormals == 0)
	{
		size = ((LOADED_MODEL*)mem)->normals;
	}
	else
	{
		size = ((LOADED_MODEL*)mem)->poly_block;
	}

	// if loaded externally don't copy from source lump
	memcpy((u_char*)*dest, (u_char*)mem, size);

	if (KeepNormals == 0)
		size = offs_mdl->normals;
	else
		size = offs_mdl->poly_block;

	//*dest += size + 2;
	// *dest = (char*)((int)model + size + 3 & 0xfffffffc); // 32 bits
	// *dest = (char*)((unsigned long long)model + size + 3L & 0xfffffffffffffffc); // 64 bits
	*dest = (char*)((s_int64_t)_mdl + size + 3 & ~3); // [A] 64 and 32 bits

	// Set values to 64bits
	_mdl->vertices = offs_mdl->vertices;
	_mdl->poly_block = offs_mdl->poly_block;
	_mdl->normals = offs_mdl->normals;
	_mdl->point_normals = offs_mdl->point_normals;
	_mdl->collision_block = offs_mdl->collision_block;

	_mdl->vertices += (s_int64_t)_mdl;
	_mdl->normals += (s_int64_t)_mdl;
	_mdl->poly_block = (s_int64_t)mem + _mdl->poly_block;

	if (KeepNormals == 0)
		_mdl->point_normals = 0;
	else
		_mdl->point_normals += (s_int64_t)_mdl;

	// D_MALLOC_END()

	return _mdl;
}

// [D] [T]
MODEL* FindModelPtrWithName(char* name)
{
	int idx;
	idx = FindModelIdxWithName(name);

	return idx >= 0 ? modelpointers[idx] : NULL;
}


// [D] [T]
int FindModelIdxWithName(char* name)
{
	char* str;
	int i;

	i = 0;
	str = modelname_buffer;

	while (i < num_models_in_pack)
	{
		if (!strcmp(str, name))
			return i;

		while (*str++)
		{
		} // go to next string

		i++;
	}

	return -1;
}
