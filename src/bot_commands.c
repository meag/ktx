
#include "g_local.h"
#include "fb_globals.h"

void SetAttribs (gedict_t* self);

#define MAX_BOTS          32
#define MIN_FROGBOT_SKILL  0
#define MAX_FROGBOT_SKILL 20

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

static int FrogbotSkillLevel (void)
{
	return (int)cvar ("k_fb_skill");
}

static void FrogbotsAddbot(void) {
	int i = 0;

	if (!bots_enabled ()) {
		G_sprint (self, 2, "Bots are disabled by the server.\n");
		return;
	}

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
			g_edicts[entity].fb.last_cmd_sent = g_globalvars.time;
			g_edicts[entity].fb.skill.skill_level = FrogbotSkillLevel();
			g_edicts[entity].fb.botnumber = i;
			SetAttribs (&g_edicts[entity]);
			return;
		}
	}

	G_sprint(self, 2, "Bot limit reached\n");
}

static void FrogbotsRemovebot(void) {
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

static void FrogbotsSetSkill (void)
{
	if (!bots_enabled ()) {
		G_sprint (self, 2, "Bots are disabled by the server.\n");
		return;
	}

	if (trap_CmdArgc () <= 2) {
		G_sprint (self, 2, "Usage: /botcmd skill <skill>\n");
		G_sprint (self, 2, "       <skill> must be in range %d and %d\n", MIN_FROGBOT_SKILL, MAX_FROGBOT_SKILL);
		G_sprint (self, 2, "bot skill is currently \"%d\"\n", FrogbotSkillLevel());
	}
	else {
		char argument[32];
		int new_skill = 0;
		int old_skill = FrogbotSkillLevel();
		
		trap_CmdArgv (2, argument, sizeof (argument));
		new_skill = bound(MIN_FROGBOT_SKILL, atoi (argument), MAX_FROGBOT_SKILL);

		if (new_skill != old_skill) {
			cvar_fset ("k_fb_skill", new_skill);
			G_sprint (self, 2, "bot skill changed to \"%d\"\n", FrogbotSkillLevel());
		}
	}
}

static void FrogbotsDebug (void)
{
	gedict_t* bot = &g_edicts[bots[0].entity];

	G_sprint (self, 2, "Bot: %s\n", bot->s.v.netname);
	G_sprint (self, 2, "  %s: %s (%d)\n", redtext("Touch"), bot->fb.touch_marker ? bot->fb.touch_marker->s.v.classname : "(none)", bot->fb.touch_marker ? bot->fb.touch_marker->fb.index : -1);
	G_sprint (self, 2, "  %s: %s\n", redtext("Enemy"), bot->s.v.enemy == 0 ? "(none)" : g_edicts[bot->s.v.enemy].s.v.netname);
	G_sprint (self, 2, "  %s: %s\n", redtext("Looking"), bot->fb.look_object ? bot->fb.look_object->s.v.classname : "(nothing)");
	G_sprint (self, 2, "  %s: %s\n", redtext ("VirtGoal"), bot->fb.virtual_goal ? bot->fb.virtual_goal->s.v.classname : "(nothing)");
	G_sprint (self, 2, "  %s: %s\n", redtext ("GoalEnt"), bot->s.v.goalentity == 0 ? "(none)" : g_edicts[bot->s.v.goalentity].s.v.classname);

	bot->fb.debug = true;
}

void FrogbotsCommand (void)
{
	char command[64];

	if (trap_CmdArgc () <= 1) {
		G_sprint (self, 2, "Available commands:\n");
		G_sprint (self, 2, "  &cff0skill&r <skill> .... set bot skill\n");
		G_sprint (self, 2, "  &cff0add&r .............. add bot\n");
		G_sprint (self, 2, "  &cff0remove&r ........... remove bot\n");
		G_sprint (self, 2, "  &cff0debug&r ............ debug bots\n");
		return;
	}

	if (!FrogbotsCheckMapSupport ()) {
		return;
	}

	trap_CmdArgv (1, command, sizeof (command));

	if (streq (command, "skill")) {
		FrogbotsSetSkill ();
	}
	else if (streq (command, "add")) {
		FrogbotsAddbot ();
	}
	else if (streq (command, "remove")) {
		FrogbotsRemovebot ();
	}
	else if (streq (command, "debug")) {
		FrogbotsDebug ();
	}
	else {
		G_sprint (self, 2, "Command not known.\n");
	}
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

	for (i = 0; i < MAX_BOTS; ++i) {
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