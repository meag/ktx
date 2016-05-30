
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

void PathScoringLogic (
	float respawn_time, qbool be_quiet, qbool path_normal, vec3_t player_origin, vec3_t player_direction, 
	gedict_t* touch_marker_, gedict_t* goalentity_marker, qbool rocket_alert, qbool rocket_jump_routes_allowed,
	qbool trace_bprint, float *best_score, gedict_t** linked_marker_, int* new_path_state
);

qbool CanJumpOver (gedict_t* self, vec3_t jump_origin, vec3_t jump_velocity, vec3_t last_clear_velocity, vec3_t last_clear_point);

static void FrogbotsDebug (void)
{
	if (trap_CmdArgc () == 2) {
		gedict_t* bot = &g_edicts[bots[0].entity];

		G_sprint (self, 2, "Bot: %s\n", bot->s.v.netname);
		G_sprint (self, 2, "  %s: %s (%d)\n", redtext ("Touch"), bot->fb.touch_marker ? bot->fb.touch_marker->s.v.classname : "(none)", bot->fb.touch_marker ? bot->fb.touch_marker->fb.index : -1);
		G_sprint (self, 2, "  %s: %s\n", redtext ("Enemy"), bot->s.v.enemy == 0 ? "(none)" : g_edicts[bot->s.v.enemy].s.v.netname);
		G_sprint (self, 2, "  %s: %s\n", redtext ("Looking"), bot->fb.look_object ? bot->fb.look_object->s.v.classname : "(nothing)");
		G_sprint (self, 2, "  %s: %s\n", redtext ("VirtGoal"), bot->fb.virtual_goal ? bot->fb.virtual_goal->s.v.classname : "(nothing)");
		G_sprint (self, 2, "  %s: %s\n", redtext ("GoalEnt"), bot->s.v.goalentity == 0 ? "(none)" : g_edicts[bot->s.v.goalentity].s.v.classname);

		bot->fb.debug = true;
	}
	else {
		char sub_command[64];

		trap_CmdArgv (2, sub_command, sizeof (sub_command));

		if (streq (sub_command, "markers")) {
			int i = 0;
			
			for (i = 0; i < NUMBER_MARKERS; ++i) {
				if (markers[i]) {
					G_sprint (self, 2, "%d / %d: %s\n", i, markers[i]->fb.index, markers[i]->s.v.classname);
				}
			}
		}
		else if (streq (sub_command, "marker")) {
			gedict_t* marker = NULL;
			int i = 0;

			trap_CmdArgv (3, sub_command, sizeof (sub_command));
			marker = markers[(int)bound (0, atoi (sub_command), NUMBER_MARKERS - 1)];

			if (marker == NULL) {
				G_sprint (self, 2, "(marker #%d not present)\n", atoi(sub_command));
			}
			else {
				G_sprint (self, 2, "Marker %d, %s, position %f %f %f\n", marker->fb.index, marker->s.v.classname, PASSVEC3 (marker->s.v.origin));
				G_sprint (self, 2, "Zone %d, Subzone %d\n", marker->fb.Z_, marker->fb.S_);
				G_sprint (self, 2, "Paths:\n");
				for (i = 0; i < NUMBER_PATHS; ++i) {
					gedict_t* next = marker->fb.paths[i].next_marker;

					if (next != NULL) {
						G_sprint (self, 2, "  %d: %d (%s), time %f\n", i, next->fb.index, next->s.v.classname, marker->fb.paths[i].time);
					}
				}
			}
		}
		else if (streq (sub_command, "path") && trap_CmdArgc() == 5) {
			int start, end;

			trap_CmdArgv (3, sub_command, sizeof (sub_command));
			start = atoi (sub_command);
			trap_CmdArgv (4, sub_command, sizeof (sub_command));
			end = atoi (sub_command);

			if (start >= 0 && start < NUMBER_MARKERS && end >= 0 && end < NUMBER_MARKERS) {
				gedict_t *from = markers[start];
				gedict_t *to = markers[end];

				if (from && to) {
					G_sprint (self, 2, "From zone %d, subzone %d to zone %d subzone %d\n", from->fb.Z_, from->fb.S_, to->fb.Z_, to->fb.S_);
					from_marker = from;
					to->fb.zone_marker();
					to->fb.sub_arrival_time();
					G_sprint (self, 2, "Travel time %f, zone_time %f\n", traveltime, zone_time);
					G_sprint (self, 2, "Middle marker %d (zone %d subzone %d), time %f\n", middle_marker->fb.index, middle_marker->fb.Z_, middle_marker->fb.S_, middle_marker->fb.subzones[to->fb.S_].time);

					{
						float best_score = -1000000;
						gedict_t* linked_marker_ = NULL;
						int new_path_state = 0;
						vec3_t player_direction = { 0, 0, 0 }; // Standing still, for sake of argument

						lookahead_time_ = 30;
						PathScoringLogic (to->fb.goal_respawn_time, false, true, from->s.v.origin, player_direction, from, to, false, true, true, &best_score, &linked_marker_, &new_path_state);

						G_sprint (self, 2, "Finished: next marker %d (%s), best_score %f\n", (linked_marker_ ? linked_marker_->fb.index : -1), (linked_marker_ ? linked_marker_->s.v.classname : "null"), best_score);
					}
				}
			}
		}
		else if (streq (sub_command, "jump")) {
			vec3_t jumpo = { -502.6, 39.1, 280.0 };
			vec3_t jumpv = { -324.2, 217.3, 270 };
			vec3_t clearo = { -618.5, 124.3, 225.6 };
			vec3_t clearv = { -292.8, 258.0, -328.0 };

			qbool result;
			
			self->fb.oldsolid = self->s.v.solid;

			fallheight = 242;
			current_fallspot = 0;
			result = CanJumpOver (self, jumpo, jumpv, clearv, clearo);
			if (result)
				G_sprint (self, 2, "CanJumpOver\n");
			else
				G_sprint (self, 2, "CanJumpOver: failed\n");

			self->s.v.solid = self->fb.oldsolid;
		}
	}
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

static qbool TimeTrigger (float *next_time, float time_increment)
{
	qbool triggered = (g_globalvars.time >= *next_time);
	if (triggered) {
		*next_time += time_increment;
		if (*next_time <= g_globalvars.time)
			*next_time = g_globalvars.time + time_increment;
	}
	return triggered;
}

void BotStartFrame(int time_) {
	int i = 0;

	marker_time = TimeTrigger (&next_marker_time, 0.1);
	hazard_time = TimeTrigger (&next_hazard_time, 0.025);

	FrogbotPrePhysics1 ();
	//FrameThink ();
	FrogbotPrePhysics2 ();
	FrogbotPostPhysics ();

	for (i = 0; i < MAX_BOTS; ++i) {
		if (bots[i].entity) {
			BotSetCommand (&g_edicts[bots[i].entity]);
		}
	}
}