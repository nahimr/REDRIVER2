#ifndef HANDLING_H
#define HANDLING_H

extern int ghost_mode;
extern int playerghost;
extern int playerhitcopsanyway;

extern void InitCarPhysics(CAR_DATA *cp, LONGVECTOR4* startpos, int direction); // 0x0005381C
extern void TempBuildHandlingMatrix(CAR_DATA *cp, int init); // 0x000539E8

extern void FixCarCos(CAR_COSMETICS *carCos, int externalModelNumber); // 0x00056C24

extern void GlobalTimeStep(); // 0x00053C00

extern void SetShadowPoints(CAR_DATA *c0, VECTOR* outpoints); // 0x000551EC

extern void LongQuaternion2Matrix(LONGQUATERNION* qua, MATRIX *m); // 0x000553CC
extern void RebuildCarMatrix(RigidBodyState *st, CAR_DATA *cp); // 0x00056AE4

extern void StepCarPhysics(CAR_DATA *cp); // 0x00056A0C

extern void InitialiseCarHandling(); // 0x00056A6C

extern void CheckCarToCarCollisions(); // 0x000556E0

extern void ProcessCarPad(CAR_DATA *cp, u_int pad, char PadSteer, char use_analogue); // 0x00055A9C

extern void InitSkidding(); // 0x00056CB8
extern void TerminateSkidding(int player_id); // 0x000562AC

extern void CheckCarEffects(CAR_DATA *cp, int player_id); // 0x00056350

extern int GetPlayerId(CAR_DATA *cp); // 0x00056CEC


#endif
