#ifndef COSMETIC_H
#define COSMETIC_H

extern CAR_COSMETICS car_cosmetics[MAX_CAR_MODELS];
extern CAR_COSMETICS dummyCosmetics;

extern int gcar_num;

extern void LoadCosmetics(int level); // 0x00031160
extern void SetupSpecCosmetics(char *loadbuffer); // 0x00031360

extern void AddReverseLight(CAR_DATA* cp); // 0x0002F994
extern void AddIndicatorLight(CAR_DATA *cp, int Type); // 0x0002FAEC
extern void AddBrakeLight(CAR_DATA *cp); // 0x0002FDE4
extern void AddCopCarLight(CAR_DATA *cp); // 0x00030148
extern void AddNightLights(CAR_DATA *cp); // 0x00030544

extern void AddSmokingEngine(CAR_DATA *cp, int black_smoke, int WheelSpeed); // 0x00030D9C
extern void AddExhaustSmoke(CAR_DATA *cp, int black_smoke, int WheelSpeed);
extern void AddFlamingEngine(CAR_DATA *cp); // 0x00030FAC


#endif
