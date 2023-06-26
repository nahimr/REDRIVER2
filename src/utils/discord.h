#ifndef DISCORD_H
#define DISCORD_H

enum EDriverActivity
{
	MENU = -1,
	UNDERCOVER = 0,
	TAKE_A_RIDE = 1,
	DIRECTOR = 4,
};

enum EDriverCity
{
	NONE = -1,
	CHICAGO = 0,
	HAVANA = 1,
	VEGAS = 2,
	RIO = 3,
};

extern int discord_init(void*);
extern void DiscordSetActivity(EDriverActivity type, EDriverCity city, int missionNumber);

#endif
