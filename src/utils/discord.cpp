#include <platform.h>

#ifdef DISCORD_SUPPORT

#include <cstdio>
#include "PsyX/PsyX_globals.h"
#include <discord.h>
#include "discord.h"
#include <Windows.h>
#include <cassert>
#include <ctime>

#define DISCORD_REQUIRE(x) assert(x == DiscordResult_Ok)

const char DriverCity[4][10] = {
	"Chicago",
	"Havana",
	"Las Vegas",
	"Rio"
};

struct Application
{
	IDiscordCore* core;
	IDiscordActivityManager* activities;
	DiscordUserId user_id;
};

Application app;

void DISCORD_CALLBACK UpdateActivityCallback(void* data, const EDiscordResult result)
{
	DISCORD_REQUIRE(result);
}

void DISCORD_CALLBACK UpdateActivity(DiscordActivity* activity)
{
	app.activities->update_activity(app.activities, activity, nullptr, UpdateActivityCallback);
}

// TODO: Add other methods, cause it's updating activity but need more data or on what it's based ??
// Current Mission or City, GameType and Level (would be better only with currentMissionNumber)

void DiscordSetActivity(EDriverActivity type, EDriverCity city, int missionNumber = -1)
{
#ifdef WIN32
	const std::time_t result = std::time(nullptr);

	DiscordActivity activity{};
	activity.type = EDiscordActivityType::DiscordActivityType_Playing;
	activity.timestamps.start = result;

	if (type == EDriverActivity::MENU)
	{
		sprintf(activity.state, "Menu");
		sprintf(activity.assets.large_image, "app-logo");
	}
	else if (type == EDriverActivity::TAKE_A_RIDE && city >= 0)
	{
		sprintf(activity.details, "Take a Ride");
		sprintf(activity.state, "Roaming in %s", DriverCity[city]);
		sprintf(activity.assets.small_image, "app-logo");
	}
	else if (type == EDriverActivity::DIRECTOR)
	{
		sprintf(activity.state, "Director Mode");
		sprintf(activity.assets.small_image, "app-logo");
	}
	else if (type == EDriverActivity::UNDERCOVER && city >= 0)
	{
		sprintf(activity.details, "Undercover in %s", DriverCity[city]);
		sprintf(activity.state, "Mission %d", missionNumber);
		sprintf(activity.assets.small_image, "app-logo");
	}

	UpdateActivity(&activity);
#endif
}

int discord_init(void*)
{
#ifdef WIN32
	memset(&app, 0, sizeof(app));

	IDiscordActivityEvents activities_events = {};

	DiscordCreateParams params{};
	DiscordCreateParamsSetDefault(&params);
	params.client_id = 1113931444004323409;
	params.flags = DiscordLogLevel_Debug;
	params.event_data = &app;
	params.activity_events = &activities_events;
	DISCORD_REQUIRE(DiscordCreate(DISCORD_VERSION, &params, &app.core));

	app.activities = app.core->get_activity_manager(app.core);

	app.core->set_log_hook(app.core, DiscordLogLevel_Debug, nullptr,
	                       [](void* hook_data, const EDiscordLogLevel level, const char* message)
	                       {
		                       PsyX_Log_Error("[Discord] Log(%d): %s\n", static_cast<uint32_t>(level), message);
	                       });

	DiscordSetActivity(EDriverActivity::MENU, EDriverCity::NONE);

	for (;;)
	{
		DISCORD_REQUIRE(app.core->run_callbacks(app.core));
		Sleep(16);
	}

#endif
	return 0;
}

#endif
