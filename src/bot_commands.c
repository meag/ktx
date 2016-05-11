
#include "g_local.h"
#include "fb_globals.h"

#define MAX_BOTS 32

typedef struct botcmd_s {
	int msec;
	vec3_t angles;
	int velocity[3];
	int buttons;
	int impulse;
} botcmd_t;

typedef struct bot_s {
	int              entity;

	char             name[64];
	botcmd_t         command;
} bot_t;

bot_t            bots[MAX_BOTS] = { 0 };

void FrogbotsAddbot() {
	int i = 0;

	for (i = 0; i < sizeof(bots) / sizeof(bots[0]); ++i) {
		if (bots[i].entity == 0) {
			int entity = 0;
			strlcpy(bots[i].name, va("frogbot %d", i+1), sizeof(bots[i].name));
			entity = trap_AddBot(bots[i].name, 3, 3, "base");

			if (entity == 0) {
				G_sprint(self, 2, "Error adding bot\n");
				return;
			}

			memset(&bots[i], 0, sizeof(bot_t));
			bots[i].entity = entity;
			memset(&bots[i].command, 0, sizeof(bots[i].command));
			self->fb.last_cmd_sent = g_globalvars.time;
			return;
		}
	}

	G_sprint(self, 2, "Bot limit reached\n");
}

void FrogbotsRemovebot() {
	int i = 0;
	bot_t* lastbot = NULL;

	for (i = 0; i < sizeof(bots) / sizeof(bots[0]); ++i) {
		if (bots[i].entity) {
			lastbot = &bots[i];
		}
	}

	if (lastbot == NULL) {
		return;
	}

	trap_RemoveBot(lastbot->entity);
	memset(lastbot, 0, sizeof(bot_t));
}

void BotStartFrame(int time_) {
	int i = 0;

	marker_time = (g_globalvars.time >= next_marker_time);
	if (marker_time) {
		next_marker_time = next_marker_time + 0.1;
		if (next_marker_time <= g_globalvars.time)
			next_marker_time = g_globalvars.time + 0.1;
	}

	hazard_time = (g_globalvars.time >= next_hazard_time);
	if (hazard_time) {
		next_hazard_time = next_hazard_time + 0.025;
		if (next_hazard_time <= g_globalvars.time)
			next_hazard_time = g_globalvars.time + 0.025;
	}

	FrogbotPrePhysics1 ();
	//FrameThink ();
	FrogbotPrePhysics2 ();
	FrogbotPostPhysics ();

	for (i = 0; i < sizeof(bots) / sizeof(bots[0]); ++i) {
		// meag testing...
		//bots[i].command.angles[0] = (int)(bots[i].command.angles[0] + 1) % 360;

		if (bots[i].entity) {
			BotSetCommand (&g_edicts[bots[i].entity]);
			/*
			trap_SetBotCMD(
				bots[i].entity,
				bots[i].command.msec,
				bots[i].command.angles[0],
				bots[i].command.angles[1],
				bots[i].command.angles[2],
				bots[i].command.velocity[0],
				bots[i].command.velocity[1],
				bots[i].command.velocity[2],
				bots[i].command.buttons,
				bots[i].command.impulse
			);*/
		}
	}
}