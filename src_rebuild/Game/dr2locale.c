#include "driver2.h"
#include "C/system.h"
#include "C/pres.h"
#include "platform.h"

#ifndef PSX
#include <stdio.h>
#include <malloc.h>
#endif


int gUserLanguage = 0;

char* gMisssionLanguageBuffer = NULL;
char* gGameLanguageBuffer = NULL;

char* gGameLangTable[MAX_LANGUAGE_TEXT];
char* gMissionLangTable[MAX_LANGUAGE_TEXT];

char* LanguageNames[5] =
{
	"EN",
	"IT",
	"GE",
	"FR",
	"SP",
};

#ifndef PSX
// [D] Driver 1 PC function
int InitStringMng()
{
	char filename[80];

	if (gUserLanguage < 0 || gUserLanguage >= 6)
		gUserLanguage = 0;

	sprintf(filename, "%sLANG\\%s_GAME.LTXT", gDataFolder, LanguageNames[gUserLanguage]);
	FS_FixPathSlashes(filename);

	if(InitStringLanguage(filename, 0) == -1)
		return 0;
	
	sprintf(filename, "%sLANG\\%s_MISSION.LTXT", gDataFolder, LanguageNames[gUserLanguage]);
	FS_FixPathSlashes(filename);
	
	if(InitStringLanguage(filename, 1) == -1)
		return 0;

#ifdef DEBUG_OPTIONS
	{
		int i;
		int nchars;
		char table[256];
		char* file;

		file = (char*)_frontend_buffer;
		
		Loadfile("GFX\\FONT2.FNT", file);

		nchars = *(int *)file;
		file += sizeof(int);

		// skip character info
		file += nchars * sizeof(OUT_FONTINFO);

		// get ASCII table
		memcpy((u_char*)table, file, 256);

		printInfo("Performing '%s' LANG to FONT text check...\n", LanguageNames[gUserLanguage]);
		for (i = 0; i < MAX_LANGUAGE_TEXT; i++)
		{
			char* base = gGameLangTable[i];
			char* testPtr = base;
			while (*testPtr)
			{
				u_char chr = *testPtr;

				if (chr != 32 && (chr < 32 || chr > 138 || chr < 128) && table[chr] == -1)
				{
					printWarning("WARN - incorrect GAME text symbol '%c', line %d, col %d\n", chr, i + 1, testPtr - base + 1);
				}
				testPtr++;
			}
		}

		for (i = 0; i < MAX_LANGUAGE_TEXT; i++)
		{
			char* base = gMissionLangTable[i];
			char* testPtr = base;
			while (*testPtr)
			{
				u_char chr = *testPtr;

				if (chr != 32 && (chr < 32 || chr > 138 || chr < 128) && table[chr] == -1)
				{
					printWarning("WARN - incorrect MISSION text symbol '%c', line %d, col %d\n", chr, i + 1, testPtr - base + 1);
				}
				testPtr++;
			}
		}
	}
#endif
	return 1;
}

void DeinitStringMng()
{
	if (gGameLanguageBuffer)
		free(gGameLanguageBuffer);

	if (gMisssionLanguageBuffer)
		free(gMisssionLanguageBuffer);

	gGameLanguageBuffer = NULL;
	gMisssionLanguageBuffer = NULL;
}

// [D] Driver 1 PC function
int InitStringLanguage(char *filename, int mission)
{
	unsigned char *buffer;
	unsigned char *cptr;
	unsigned char *ln;
	int i;
	int size;
	
	FILE* fp = PsyX_FS_OpenFile(filename, "rb");
	if (!fp)
	{
		printError("ERROR! Cannot open language file '%s'!\n", filename);
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	buffer = (unsigned char*)malloc(size);
	fread(buffer, 1, size, fp);
	fclose(fp);
	
	if (mission)
		gMisssionLanguageBuffer = (char*)buffer;
	else
		gGameLanguageBuffer = (char*)buffer;

	cptr = buffer;
	for (i = 0; i < MAX_LANGUAGE_TEXT; i++)
	{	
		if (size <= 0)
		{
			if (mission)
				gMissionLangTable[i] = "Mission.? missing!";
			else
				gGameLangTable[i] = "Lang.? missing!";
		}
		else
		{
			if (mission)
				gMissionLangTable[i] = (char*)cptr;
			else
				gGameLangTable[i] = (char*)cptr;

			// go to next line
			ln = cptr;
			do
			{
				if (*cptr == '\n' || *cptr == '\r')
				{
					while(*cptr == '\r')
					{
						*cptr++;
						size--;
					}
			
					break;
				}
				
				if (*cptr >= 32)
					ln = cptr + 1;
				
				size--;
				cptr++;
			} while (size > 1);
	
			*ln = 0; // zero on end of the line
			
			cptr++;
			size--;
		}
	}

	return i;
}
#endif