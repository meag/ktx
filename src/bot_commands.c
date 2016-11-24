
#include "g_local.h"
#include "fb_globals.h"

// Handles all "botcmd x" commands from the user

// Cripes.  Fix all these declarations
void SetAttribs (gedict_t* self);
void Bot_Print_Thinking (void);
void BotsFireInitialTriggers (gedict_t* client);
void PathScoringLogic (
	float respawn_time, qbool be_quiet, float lookahead_time, qbool path_normal, vec3_t player_origin, vec3_t player_direction, 
	gedict_t* touch_marker_, gedict_t* goalentity_marker, qbool rocket_alert, qbool rocket_jump_routes_allowed,
	qbool trace_bprint, float *best_score, gedict_t** linked_marker_, int* new_path_state
);
qbool BotDoorIsClosed (gedict_t* door);
qbool POVDMM4DontWalkThroughDoor (gedict_t* entity);
gedict_t* BotsFirstBot (void);
void RemovePath (gedict_t* marker, int path_number);
int AddPath (gedict_t* marker, gedict_t* next_marker);
void RemoveMarker (gedict_t* marker);
int DecodeMarkerFlagString (const char* string);
int DecodeMarkerPathFlagString (const char* string);
const char* EncodeMarkerPathFlags (int path_flags);
const char* EncodeMarkerFlags (int marker_flags);
void DM6Debug (gedict_t* self);

#define MAX_BOTS          32

#define UNLINKED_MARKER_MODEL "progs/w_g_key.mdl"
#define LINKED_MARKER_MODEL   "progs/w_s_key.mdl"
#define CURRENT_MARKER_MODEL  UNLINKED_MARKER_MODEL

#define EDITOR_BIDIRECTIONAL_COLOUR  EF_BLUE
#define EDITOR_UNIDIRECTIONAL_COLOUR EF_RED
#define EDITOR_SELECTED_NODE         EF_GREEN

// If the marker/path flag isn't set here, won't be included in .bot file
#define EXTERNAL_MARKER_PATH_FLAGS (WATERJUMP_ | DM6_DOOR | ROCKET_JUMP | JUMP_LEDGE | VERTICAL_PLATFORM)
#define EXTERNAL_MARKER_FLAGS (UNREACHABLE | MARKER_IS_DM6_DOOR | MARKER_FIRE_ON_MATCH_START | MARKER_DOOR_TOUCHABLE | MARKER_ESCAPE_ROUTE)

static qbool marker_time;
static float next_marker_time;
static qbool hazard_time;
static float next_hazard_time;

static gedict_t* saved_marker = NULL;
static gedict_t* last_touched_marker = NULL;

// FIXME: Globals
extern gedict_t* markers[];

qbool HasSavedMarker (void)
{
	return saved_marker != NULL;
}

qbool IsMarkerFrame (void)
{
	return marker_time;
}

qbool IsHazardFrame (void)
{
	return hazard_time;
}

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

static int CountTeamMembers (const char* teamName)
{
	int count = 0;
	gedict_t* ed;

	for (ed = world; ed = find_plr (ed); ) {
		if (streq (getteam (ed), teamName)) {
			++count;
		}
	}

	return count;
}

// FIXME: Autoteams vs manual...
static void FrogbotsAddbot(void) {
	int i = 0;

	if (!bots_enabled ()) {
		G_sprint (self, 2, "Bots are disabled by the server.\n");
		return;
	}

	for (i = 0; i < sizeof(bots) / sizeof(bots[0]); ++i) {
		if (bots[i].entity == 0) {
			int entity = 0;
			int topColor = 0;
			int bottomColor = 0;
			int red = CountTeamMembers ("red");
			int blue = CountTeamMembers ("blue");
			qbool red_team = red < blue || (red == blue && g_random () < 0.5);
			const char* teamName = red_team ? "red" : "blue";

			if (teamplay) {
				int teamCount = (int)CountTeams ();
				char* botName = SetTeamNetName (i, teamName);

				strlcpy (bots[i].name, botName, sizeof(bots[i].name));

				// Pick team
				topColor = bottomColor = (red_team ? 4 : 13);
			}
			else {
				strlcpy (bots[i].name, SetNetName(i), sizeof(bots[i].name));

				topColor = i_rnd (0, 13);
				bottomColor = i_rnd (0, 13);
			}
			entity = trap_AddBot(bots[i].name, topColor, bottomColor, "base");

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
			trap_SetBotUserInfo (entity, "team", teamName, 0);
			SetAttribs (&g_edicts[entity]);
			trap_SetBotUserInfo (entity, "k_nick", bots[i].name, 0);
			return;
		}
	}

	G_sprint(self, 2, "Bot limit reached\n");
}

static void FrogbotsRemovebot(void) {
	int i = 0;
	bot_t* lastbot = NULL;
	gedict_t* e = NULL;

	for (i = 0; i < sizeof(bots) / sizeof(bots[0]); ++i) {
		if (bots[i].entity) {
			lastbot = &bots[i];
			e = &g_edicts[bots[i].entity];
		}
	}

	if (lastbot == NULL) {
		return;
	}

	G_bprint( PRINT_HIGH, "%s left the game with %.0f frags\n", e->s.v.netname, e->s.v.frags );
	sound( self, CHAN_BODY, "player/tornoff2.wav", 1, ATTN_NONE );
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
	if (trap_CmdArgc () == 2) {
		Bot_Print_Thinking ();
	}
	else {
		char sub_command[64];

		trap_CmdArgv (2, sub_command, sizeof (sub_command));

		if (streq (sub_command, "door")) {
			if (streq (g_globalvars.mapname, "povdmm4")) {
				if (markers[0]->fb.door_entity && markers[4]->fb.door_entity) {
					G_sprint (self, 2, "Low-spawn door is %s @ %f %f %f\n", BotDoorIsClosed (markers[0]) ? "closed" : "open", PASSVEC3(markers[0]->s.v.origin));
					G_sprint (self, 2, "High-spawn door is %s @ %f %f %f\n", BotDoorIsClosed (markers[4]) ? "closed" : "open", PASSVEC3(markers[4]->s.v.origin));
					G_sprint (self, 2, "Low-spawn YA is %s\n", POVDMM4DontWalkThroughDoor (markers[2]) ? "blocked" : "available");
					G_sprint (self, 2, "High-spawn YA is %s\n", POVDMM4DontWalkThroughDoor (markers[5]) ? "blocked" : "available");
				}
			}
			else {
				G_sprint (self, 2, "Only available on povdmm4.\n");
			}
		}

		if (match_in_progress)
			return;

		if (streq (sub_command, "markers")) {
			int i = 0;
			
			for (i = 0; i < NUMBER_MARKERS; ++i) {
				if (markers[i]) {
					G_sprint (self, 2, "%d / %d: %s\n", i, markers[i]->fb.index, markers[i]->s.v.classname);
				}
			}
		}
		else if (streq (sub_command, "entity")) {
			gedict_t* e = NULL;
			int ent = 0;

			trap_CmdArgv (3, sub_command, sizeof (sub_command));
			ent = atoi (sub_command);

			if (ent > 0 && ent < MAX_EDICTS)
				G_sprint (self, 2, "%d: %s [%f %f %f]\n", atoi (sub_command), g_edicts[ent].s.v.classname ? g_edicts[ent].s.v.classname : "?", PASSVEC3(g_edicts[ent].s.v.origin));
			else
				G_sprint (self, 2, "%d - out of range\n", atoi (sub_command));
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
				G_sprint (self, 2, "> mins [%f %f %f] maxs [%f %f %f]\n", PASSVEC3 (marker->s.v.mins), PASSVEC3 (marker->s.v.maxs));
				G_sprint (self, 2, "> absmin [%f %f %f] absmax [%f %f %f]\n", PASSVEC3 (marker->s.v.absmin), PASSVEC3 (marker->s.v.absmax));
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
					G_sprint (self, 2, "%s -> %s\n", from->s.v.classname, to->s.v.classname);
					G_sprint (self, 2, "From zone %d, subzone %d to zone %d subzone %d\n", from->fb.Z_, from->fb.S_, to->fb.Z_, to->fb.S_);
					from_marker = from;
					ZoneMarker (from_marker, to, path_normal);
					traveltime = SubZoneArrivalTime (zone_time, middle_marker, to);
					G_sprint (self, 2, "Travel time %f, zone_time %f\n", traveltime, zone_time);
					G_sprint (self, 2, "Middle marker %d (zone %d subzone %d), time %f\n", middle_marker->fb.index, middle_marker->fb.Z_, middle_marker->fb.S_, middle_marker->fb.subzones[to->fb.S_].time);

					{
						float best_score = -1000000;
						gedict_t* linked_marker_ = NULL;
						int new_path_state = 0;
						vec3_t player_direction = { 0, 0, 0 }; // Standing still, for sake of argument

						PathScoringLogic (to->fb.goal_respawn_time, false, 30, true, from->s.v.origin, player_direction, from, to, false, true, true, &best_score, &linked_marker_, &new_path_state);

						G_sprint (self, 2, "Finished: next marker %d (%s), best_score %f\n", (linked_marker_ ? linked_marker_->fb.index : -1), (linked_marker_ ? linked_marker_->s.v.classname : "null"), best_score);
					}
				}
			}
		}
		else if (streq (sub_command, "random")) {
			RunRandomTrials (0, 6, 2);
		}
		else if (streq (sub_command, "startmap")) {
			if (!k_practice) {
				SetPractice (1, NULL);
			}

			if (!k_practice) {
				G_sprint (self, 2, "Map must be in practice mode\n");
				return;
			}

			BotsFireInitialTriggers (self);
		}
		else if (streq (sub_command, "botpath")) {
			gedict_t* first_bot = BotsFirstBot ();
			gedict_t* marker = NULL;
			gedict_t* target = NULL;
			vec3_t teleport_angles = { 0, 0, 0 };
			vec3_t teleport_location = { 0, 0, 0 };
			int i = 0;

			if (!k_practice) {
				SetPractice (1, NULL);
			}

			if (first_bot == NULL || !k_practice) {
				G_sprint (self, 2, "Map must be in practice mode and have one bot\n");
				return;
			}

			trap_CmdArgv (3, sub_command, sizeof (sub_command));
			marker = markers[(int)bound (0, atoi (sub_command), NUMBER_MARKERS - 1)];

			if (marker == NULL || marker->fb.index != atoi(sub_command)) {
				G_sprint (self, 2, "(marker #%d not present)\n", atoi(sub_command));
				return;
			}

			trap_CmdArgv (4, sub_command, sizeof (sub_command));
			target = markers[(int)bound (0, atoi (sub_command), NUMBER_MARKERS - 1)];

			if (target == NULL || target->fb.index != atoi(sub_command)) {
				G_sprint (self, 2, "(marker #%d not present)\n", atoi(sub_command));
				return;
			}

			first_bot->fb.fixed_goal = target;
			first_bot->fb.debug = true;
			first_bot->fb.debug_path = true;
			first_bot->fb.touch_marker_time = first_bot->fb.linked_marker_time = first_bot->fb.goal_refresh_time = 0;
			first_bot->fb.old_linked_marker = NULL;

			VectorCopy (marker->s.v.origin, teleport_location);
			if (!streq (marker->s.v.classname, "marker")) {
				teleport_location[2] += 32;
			}
			first_bot->fb.dbg_countdown = 33;
			teleport_player (first_bot, teleport_location, teleport_angles, TFLAGS_SND_DST | TFLAGS_FOG_DST);
		}
		else if (streq (sub_command, "dm6")) {
			DM6Debug (self);
		}
	}
}

static int FindPathIndex (gedict_t* saved_marker, gedict_t* nearest)
{
	int i = 0;

	if (saved_marker == NULL || nearest == NULL)
		return -1;

	for (i = 0; i < NUMBER_PATHS; ++i) {
		if (saved_marker->fb.paths[i].next_marker == nearest) {
			return i;
		}
	}

	return -1;
}

static void BotFileGenerate (void)
{
	fileHandle_t file;
	char* entityFile = cvar_string ("k_entityfile");
	char date[64];
	char fileName[128];
	int i;

	if (!QVMstrftime (date, sizeof (date), "%Y%m%d-%H%M%S", 0))
		snprintf (date, sizeof (date), "%d", i_rnd (0, 9999));

	snprintf (fileName, sizeof (fileName), "bots/maps/%s[%s].bot", strnull (entityFile) ? g_globalvars.mapname : entityFile, date);
	file = std_fwopen (fileName);
	if (file == -1) {
		G_sprint (self, PRINT_HIGH, "Failed to open botfile.  Check bots/maps/ directory is writable\n");
		return;
	}

	for (i = 0; i < NUMBER_MARKERS; ++i) {
		if (markers[i] && streq (markers[i]->s.v.classname, "marker")) {
			std_fprintf (file, "CreateMarker %d %d %d\n", PASSINTVEC3(markers[i]->s.v.origin));
			if (markers[i]->s.v.view_ofs[2] && markers[i]->s.v.view_ofs[2] != 24) {
				std_fprintf (file, "SetMarkerViewOffset %d %d\n", markers[i]->fb.index + 1, (int)markers[i]->s.v.view_ofs[2]);
			}
		}
	}

	for (i = 0; i < NUMBER_MARKERS; ++i) {
		if (markers[i] && ! (markers[i]->fb.T & MARKER_DYNAMICALLY_ADDED)) {
			int p;

			if (markers[i]->fb.G_) {
				std_fprintf (file, "SetGoal %d %d\n", markers[i]->fb.index + 1, markers[i]->fb.G_);
			}
			if (markers[i]->fb.Z_) {
				std_fprintf (file, "SetZone %d %d\n", markers[i]->fb.index + 1, markers[i]->fb.Z_);
			}
			if (markers[i]->fb.T & EXTERNAL_MARKER_FLAGS) {
				std_fprintf (file, "SetMarkerFlag %d %s\n", markers[i]->fb.index + 1, EncodeMarkerFlags (markers[i]->fb.T & EXTERNAL_MARKER_FLAGS));
			}
			for (p = 0; p < NUMBER_PATHS; ++p) {
				if (markers[i]->fb.paths[p].next_marker && ! (markers[i]->fb.paths[p].next_marker->fb.T & MARKER_DYNAMICALLY_ADDED)) {
					std_fprintf (file, "SetMarkerPath %d %d %d\n", markers[i]->fb.index + 1, p, markers[i]->fb.paths[p].next_marker->fb.index + 1);
					if (markers[i]->fb.paths[p].flags & EXTERNAL_MARKER_PATH_FLAGS) {
						std_fprintf (file, "SetMarkerPathFlags %d %d %s\n", markers[i]->fb.index + 1, p, EncodeMarkerPathFlags (markers[i]->fb.paths[p].flags & EXTERNAL_MARKER_PATH_FLAGS));
					}
				}
			}
		}
	}

	std_fclose (file);
	G_sprint (self, PRINT_HIGH, "Created file %s\n", fileName);
}

static gedict_t* MarkerIndicator (gedict_t* marker)
{
	gedict_t* indicator;

	if (marker == NULL || streq (marker->s.v.classname, "marker")) {
		return marker;
	}

	for (indicator = world; indicator = ez_find (indicator, "marker_indicator"); ) {
		if (indicator->fb.index == marker->fb.index) {
			return indicator;
		}
	}

	return NULL;
}

static void SelectMarker (gedict_t* marker)
{
	gedict_t* indicator;
	int i;

	indicator = MarkerIndicator (marker);
	if (indicator) {
		indicator->s.v.effects = (int)indicator->s.v.effects | EDITOR_SELECTED_NODE;
		setmodel (indicator, CURRENT_MARKER_MODEL);
	}

	for (i = 0; i < NUMBER_PATHS; ++i) {
		gedict_t* next = marker->fb.paths[i].next_marker;

		if (next) {
			int j;
			int effect = EDITOR_UNIDIRECTIONAL_COLOUR;
			gedict_t* next_indicator = MarkerIndicator (next);

			if (next_indicator != NULL) {
				for (j = 0; j < NUMBER_PATHS; ++j) {
					if (next->fb.paths[j].next_marker == marker) {
						effect = EDITOR_BIDIRECTIONAL_COLOUR;
					}
				}

				next_indicator->s.v.effects = ((int)next_indicator->s.v.effects & ~(EDITOR_UNIDIRECTIONAL_COLOUR | EDITOR_BIDIRECTIONAL_COLOUR)) | effect;
				setmodel (next_indicator, LINKED_MARKER_MODEL);
			}
		}
	}
}

static void DeselectMarker (gedict_t* marker)
{
	gedict_t* indicator = MarkerIndicator (marker);
	int i = 0;

	if (indicator) {
		indicator->s.v.effects = (int)indicator->s.v.effects & ~EDITOR_SELECTED_NODE;
	}

	for (i = 0; i < NUMBER_PATHS; ++i) {
		gedict_t* next = marker->fb.paths[i].next_marker;

		if (next) {
			gedict_t* next_indicator = MarkerIndicator (next);
			if (next_indicator) {
				next_indicator->s.v.effects = (int)next_indicator->s.v.effects & ~(EDITOR_UNIDIRECTIONAL_COLOUR | EDITOR_BIDIRECTIONAL_COLOUR);
				setmodel (next_indicator, UNLINKED_MARKER_MODEL);
			}
		}
	}
}

void FrogbotEditorMarkerTouched (gedict_t* marker)
{
	if (saved_marker == NULL) {
		if (last_touched_marker != marker) {
			if (last_touched_marker) {
				DeselectMarker (last_touched_marker);
			}
			SelectMarker (marker);
		}
	}
	last_touched_marker = marker;
}

static void FrogbotAddMarker (void)
{
	vec3_t pos, nearest_pos;
	gedict_t* nearest = LocateMarker (self->s.v.origin);
	gedict_t* spawn = NULL;

	if (nearest) {
		VectorAdd (self->s.v.origin, self->s.v.view_ofs, pos);
		VectorAdd (nearest->s.v.origin, nearest->s.v.view_ofs, nearest_pos);
		if (VectorDistance (nearest_pos, pos) < 30) {
			G_sprint (self, PRINT_HIGH, "Too close to marker #%d [%s]\n", nearest->fb.index + 1, nearest->s.v.classname);
			return;
		}
	}

	spawn = CreateNewMarker (self->s.v.origin);
	G_sprint (self, PRINT_HIGH, "Created marker #%d\n", spawn->fb.index + 1);
}

static void FrogbotRemoveMarker (void) {
	gedict_t* nearest = LocateMarker (self->s.v.origin);

	if (! nearest) {
		G_sprint (self, PRINT_HIGH, "No marker found nearby\n");
		return;
	}

	if (!streq (nearest->s.v.classname, "marker")) {
		G_sprint (self, PRINT_HIGH, "Cannot remove non-manual markers\n");
		return;
	}

	if (saved_marker == nearest) {
		DeselectMarker (nearest);
		saved_marker = NULL;
	}
	RemoveMarker (nearest);
}

static void FrogbotSaveMarker (void)
{
	if (saved_marker == NULL) {
		gedict_t* nearest = LocateMarker (self->s.v.origin);

		if (nearest != NULL) {
			if (last_touched_marker) {
				DeselectMarker (last_touched_marker);
			}
			SelectMarker (saved_marker = nearest);

			G_sprint (self, PRINT_HIGH, "Marker #%d [%s] is saved\n", nearest->fb.index + 1, nearest->s.v.classname);
		}
	}
	else {
		DeselectMarker (saved_marker);
		saved_marker = NULL;

		if (last_touched_marker) {
			SelectMarker (last_touched_marker);
		}

		G_sprint (self, PRINT_HIGH, "Cleared saved marker\n");
	}
}

static void FrogbotAddPath (void)
{
	gedict_t* nearest = LocateMarker (self->s.v.origin);
	gedict_t* nearest_indicator;
	int source_to_target_path = FindPathIndex (saved_marker, nearest);
	int target_to_source_path = FindPathIndex (nearest, saved_marker);

	if (nearest == NULL) {
		G_sprint (self, PRINT_HIGH, "Could not locate marker nearby\n");
		return;
	}

	if (saved_marker == NULL) {
		G_sprint (self, PRINT_HIGH, "Save a marker before creating path\n");
		return;
	}

	if (nearest == saved_marker) {
		G_sprint (self, PRINT_HIGH, "Cannot link a marker to itself\n");
		return;
	}

	if (source_to_target_path >= 0 && target_to_source_path >= 0) {
		RemovePath (saved_marker, source_to_target_path);
		RemovePath (nearest, target_to_source_path);

		nearest_indicator = MarkerIndicator (nearest);
		if (nearest_indicator) {
			nearest_indicator->s.v.effects = (int)nearest_indicator->s.v.effects & ~(EDITOR_BIDIRECTIONAL_COLOUR | EDITOR_UNIDIRECTIONAL_COLOUR);
			setmodel (nearest_indicator, UNLINKED_MARKER_MODEL);
		}

		G_sprint (self, PRINT_HIGH, "Both paths cleared - no link\n");
		return;
	}

	if (source_to_target_path >= 0) {
		if (AddPath (nearest, saved_marker) >= 0) {
			G_sprint (self, PRINT_HIGH, "Marker %d > %d linked (bi-directional)\n", nearest->fb.index, saved_marker->fb.index);
			nearest_indicator = MarkerIndicator (nearest);
			if (nearest_indicator) {
				nearest_indicator->s.v.effects = ((int)nearest_indicator->s.v.effects & ~EDITOR_UNIDIRECTIONAL_COLOUR) | EDITOR_BIDIRECTIONAL_COLOUR;
				setmodel (nearest_indicator, LINKED_MARKER_MODEL);
			}
		}
		else {
			G_sprint (self, PRINT_HIGH, "{&cf00ERROR&cfff}: Unable to link (maximum paths hit?)\n");
		}

		return;
	}

	if (AddPath (saved_marker, nearest) >= 0) {
		G_sprint (self, PRINT_HIGH, "Marker %d > %d linked (uni-directional)\n", saved_marker->fb.index, nearest->fb.index);
		nearest_indicator = MarkerIndicator (nearest);
		if (nearest_indicator) {
			nearest_indicator->s.v.effects = ((int)nearest_indicator->s.v.effects & ~EDITOR_BIDIRECTIONAL_COLOUR) | EDITOR_UNIDIRECTIONAL_COLOUR;
			setmodel (nearest_indicator, LINKED_MARKER_MODEL);
		}
	}
	else {
		G_sprint (self, PRINT_HIGH, "{&cf00ERROR&cfff}: Unable to link (maximum paths hit?)\n");
	}
}

static void FrogbotRemovePath (void)
{
	gedict_t* nearest = LocateMarker (self->s.v.origin);
	gedict_t* nearest_indicator;
	int source_to_target_path = FindPathIndex (saved_marker, nearest);
	int target_to_source_path = FindPathIndex (nearest, saved_marker);

	if (nearest == NULL) {
		G_sprint (self, PRINT_HIGH, "Could not locate marker nearby\n");
		return;
	}

	if (saved_marker == NULL) {
		G_sprint (self, PRINT_HIGH, "Save a marker before creating path\n");
		return;
	}

	if (source_to_target_path >= 0) {
		RemovePath (saved_marker, source_to_target_path);
	}
	if (target_to_source_path >= 0) {
		RemovePath (nearest, target_to_source_path);
	}

	nearest_indicator = MarkerIndicator (nearest);
	if (nearest_indicator)
		nearest_indicator->s.v.effects = (int)nearest_indicator->s.v.effects & ~(EDITOR_UNIDIRECTIONAL_COLOUR | EDITOR_BIDIRECTIONAL_COLOUR);
}

static void FrogbotRemoveAllPaths (void)
{
	gedict_t* nearest = LocateMarker (self->s.v.origin);
	int i;

	if (nearest == NULL) {
		G_sprint (self, PRINT_HIGH, "Could not locate marker nearby\n");
		return;
	}

	for (i = 0; i < NUMBER_PATHS; ++i) {
		gedict_t* next = nearest->fb.paths[i].next_marker;
		if (next) {
			gedict_t* indicator = MarkerIndicator (next);
			if (indicator) {
				indicator->s.v.effects = (int)indicator->s.v.effects & ~(EDITOR_UNIDIRECTIONAL_COLOUR | EDITOR_BIDIRECTIONAL_COLOUR);
			}
		}
		RemovePath (nearest, i);
	}
}

static void FrogbotSetZone(void)
{
	gedict_t* nearest = LocateMarker (self->s.v.origin);
	int zone = 0;

	if (!nearest) {
		G_sprint (self, PRINT_HIGH, "No marker found nearby\n");
		return;
	}

	zone = nearest->fb.Z_ + 1;
	if (zone > NUMBER_ZONES)
		zone = 1;
	if (trap_CmdArgc () >= 3) {
		char param[64];

		trap_CmdArgv (2, param, sizeof (param));

		if (atoi (param) != 0) {
			zone = bound (1, atoi (param), NUMBER_ZONES);
		}
	}

	nearest->fb.Z_ = zone;
	G_sprint (self, PRINT_HIGH, "Marker #%d now has zone %d\n", nearest->fb.index + 1, nearest->fb.Z_);
}

static void FrogbotSetMarkerFlag (void)
{
	char param[64];
	gedict_t* nearest = LocateMarker (self->s.v.origin);
	int flags;

	if (nearest == NULL) {
		G_sprint (self, PRINT_HIGH, "No marker nearby\n");
		return;
	}

	if (trap_CmdArgc () < 3) {
		G_sprint (self, PRINT_HIGH, "Provide marker flags: " FROGBOT_MARKER_FLAG_OPTIONS "\n");
		return;
	}

	trap_CmdArgv (2, param, sizeof (param));
	flags = DecodeMarkerFlagString (param);
	if (flags) {
		nearest->fb.T |= flags;
		G_sprint (self, PRINT_HIGH, "Marker flags set, now: %s\n", EncodeMarkerFlags (nearest->fb.T));
	}
	else {
		G_sprint (self, PRINT_HIGH, "Marker flags invalid, options are %s\n", FROGBOT_MARKER_FLAG_OPTIONS);
	}
}

static void FrogbotClearMarkerFlag (void)
{
	char param[64];
	gedict_t* nearest = LocateMarker (self->s.v.origin);
	int flags;

	if (nearest == NULL) {
		G_sprint (self, PRINT_HIGH, "No marker nearby\n");
		return;
	}

	if (trap_CmdArgc () < 3) {
		G_sprint (self, PRINT_HIGH, "Provide marker flags: " FROGBOT_MARKER_FLAG_OPTIONS "\n");
		return;
	}

	trap_CmdArgv (2, param, sizeof (param));
	flags = DecodeMarkerFlagString (param);
	if (flags) {
		nearest->fb.T &= ~flags;
		G_sprint (self, PRINT_HIGH, "Marker flags cleared, now: %s\n", EncodeMarkerFlags (nearest->fb.T));
	}
	else {
		G_sprint (self, PRINT_HIGH, "Marker flags invalid, options are %s\n", FROGBOT_MARKER_FLAG_OPTIONS);
	}
}

static void FrogbotSetPathFlag (void)
{
	gedict_t* nearest = LocateMarker (self->s.v.origin);
	int source_to_target_path = FindPathIndex (saved_marker, nearest);
	char param[64];

	if (nearest == NULL) {
		G_sprint (self, PRINT_HIGH, "No marker nearby\n");
		return;
	}

	if (trap_CmdArgc () < 3) {
		G_sprint (self, PRINT_HIGH, "Provide path flags: " FROGBOT_PATH_FLAG_OPTIONS "\n");
		return;
	}

	trap_CmdArgv (2, param, sizeof (param));
	if (source_to_target_path >= 0) {
		int flags = DecodeMarkerPathFlagString (param);

		if (flags) {
			saved_marker->fb.paths[source_to_target_path].flags |= flags;
			G_sprint (self, PRINT_HIGH, "Path flags set, now: %s\n", EncodeMarkerPathFlags (saved_marker->fb.paths[source_to_target_path].flags));
		}
		else {
			G_sprint (self, PRINT_HIGH, "Path flags invalid, options are %s\n", FROGBOT_PATH_FLAG_OPTIONS);
		}
	}
	else {
		G_sprint (self, PRINT_HIGH, "No path linked to add flag\n");
	}
}

static void FrogbotClearPathFlag (void)
{
	gedict_t* nearest = LocateMarker (self->s.v.origin);
	int source_to_target_path = FindPathIndex (saved_marker, nearest);
	char param[64];

	if (nearest == NULL) {
		G_sprint (self, PRINT_HIGH, "No marker nearby\n");
		return;
	}

	if (trap_CmdArgc () < 3) {
		G_sprint (self, PRINT_HIGH, "Provide path flags: " FROGBOT_PATH_FLAG_OPTIONS "\n");
		return;
	}

	trap_CmdArgv (2, param, sizeof (param));
	if (source_to_target_path >= 0) {
		int flags = DecodeMarkerPathFlagString (param);

		if (flags) {
			saved_marker->fb.paths[source_to_target_path].flags &= ~flags;
			G_sprint (self, PRINT_HIGH, "Path flags cleared, now: %s\n", EncodeMarkerPathFlags (saved_marker->fb.paths[source_to_target_path].flags));
		}
		else {
			G_sprint (self, PRINT_HIGH, "Path flags invalid, options are %s\n", FROGBOT_PATH_FLAG_OPTIONS);
		}
	}
	else {
		G_sprint (self, PRINT_HIGH, "No path linked to add flag\n");
	}
}

static void FrogbotSaveBotFile (void)
{
	int i = 0, j = 0;
	gedict_t* new_markers[NUMBER_MARKERS] = { 0 };

	// Renumber all markers
	for (i = 0; i < NUMBER_MARKERS; ++i) {
		if (markers[i]) {
			markers[i]->fb.index = j;
			new_markers[j++] = markers[i];
		}
	}

	memcpy (markers, new_markers, sizeof (new_markers));

	BotFileGenerate ();
}

static void FrogbotShowInfo (void)
{
	char message[1024] = { 0 };
	int i = 0;
	gedict_t* marker = self->fb.touch_marker;
	const char* marker_flags = 0;

	if (trap_CmdArgc () >= 3) {
		char temp[10];

		trap_CmdArgv (2, temp, sizeof (temp));

		if (atoi (temp) >= 1 && atoi (temp) <= NUMBER_MARKERS) {
			marker = markers[atoi (temp) - 1];

			if (marker == NULL) {
				G_sprint (self, PRINT_HIGH, "No such marker #%d found\n", atoi (temp));
				return;
			}
		}
	}

	if (marker == NULL)
		marker = LocateMarker (self->s.v.origin);

	if (marker == NULL) {
		G_sprint (self, PRINT_HIGH, "Unable to find nearby marker\n");
		return;
	}

	marker_flags = EncodeMarkerFlags (marker->fb.T);

	if (g_globalvars.time < self->fb.last_spec_cp)
		return;

	if (!marker)
		return;

	strlcpy (message, va ("Marker #%3d [%s]\n", marker->fb.index + 1, marker->s.v.classname), sizeof (message));
	strlcat (message, va ("Origin #%3d %3d %3d\n", PASSINTVEC3(marker->s.v.origin)), sizeof (message));
	strlcat (message, va ("Zone #%2d, Goal #%2d\n", marker->fb.Z_, marker->fb.G_), sizeof (message));
	strlcat (message, va ("Flags: %s\n", strnull (marker_flags) ? "(none)" : marker_flags), sizeof (message));
	strlcat (message, "Paths:\n", sizeof (message));
	for (i = 0; i < NUMBER_PATHS; ++i) {
		gedict_t* next = marker->fb.paths[i].next_marker;

		if (next) {
			const char* path_flags = EncodeMarkerPathFlags (marker->fb.paths[i].flags);

			strlcat (message, va ("  %3d: %s [%s]\n", next->fb.index + 1, next->s.v.classname, strnull (path_flags) ? "(none)" : path_flags), sizeof (message));
		}
	}

	G_centerprint (self, message);
	self->fb.last_spec_cp = g_globalvars.time + 0.2;
}

static void FrogbotsFillServer (void)
{
	int max_clients = cvar ("maxclients");
	int plr_count = CountPlayers ();
	int i;

	for (i = 0; i < min(max_clients - plr_count, 8); ++i) {
		FrogbotsAddbot ();
	}
}

static void FrogbotsRemoveAll (void)
{
	int bot_count = CountBots ();

	while (bot_count-- > 0) {
		FrogbotsRemovebot ();
	}
}

static void FrogbotSummary (void)
{
	int marker_count = 0;
	int goal_count[NUMBER_GOALS] = { 0 };
	int zone_count[NUMBER_ZONES] = { 0 };
	int i, j;

	G_sprint (self, PRINT_HIGH, "Marker summary:\n");
	for (i = 0; i < NUMBER_MARKERS; ++i) {
		if (markers[i]) {
			int path_count = 0;

			++marker_count;
			for (j = 0; j < NUMBER_PATHS; ++j) {
				if (markers[i]->fb.paths[j].next_marker) {
					++path_count;
				}
			}

			if (path_count == 0) {
				G_sprint (self, PRINT_HIGH, "  %3d: %s: no paths%s\n", markers[i]->fb.index + 1, markers[i]->s.v.classname, markers[i]->fb.Z_ ? "" : " and no zone");
			}
			else if (!markers[i]->fb.Z_) {
				G_sprint (self, PRINT_HIGH, "  %3d: %s: no zone\n", markers[i]->fb.index + 1, markers[i]->s.v.classname);
			}

			if (markers[i]->fb.G_) {
				++goal_count[markers[i]->fb.G_ - 1];
			}
			if (markers[i]->fb.Z_) {
				++zone_count[markers[i]->fb.Z_ - 1];
			}
		}
	}
	G_sprint (self, PRINT_HIGH, "  %d markers in total\n", marker_count);

	G_sprint (self, PRINT_HIGH, "Goal summary:\n");
	for (i = 0; i < NUMBER_GOALS; ++i) {
		if (goal_count[i]) {
			G_sprint (self, PRINT_HIGH, "  %2d: %d markers\n", i + 1, goal_count[i]);
		}
	}

	G_sprint (self, PRINT_HIGH, "Zone summary:\n");
	for (i = 0; i < NUMBER_ZONES; ++i) {
		if (zone_count[i]) {
			G_sprint (self, PRINT_HIGH, "  %2d: %d markers\n", i + 1, zone_count[i]);
		}
	}
}

typedef struct frogbot_cmd_s {
	char* name;
	void (*func) (void);
	char* description;
} frogbot_cmd_t;

static frogbot_cmd_t std_commands[] = {
	{ "skill", FrogbotsSetSkill, "Set skill level for next bot added" },
	{ "addbot", FrogbotsAddbot, "Adds a bot to the server" },
	{ "fill", FrogbotsFillServer, "Fills the server (max 8 bots at a time)" },
	{ "removebot", FrogbotsRemovebot, "Removes a single bot" },
	{ "removeall", FrogbotsRemoveAll, "Removes all bots from server" },
	{ "debug", FrogbotsDebug, "Debugging commands" }
};

static frogbot_cmd_t editor_commands[] = {
	{ "addmarker", FrogbotAddMarker, "Adds a routing marker to the map" },
	{ "removemarker", FrogbotRemoveMarker, "Removes a routing marker from the map" },
	{ "savemarker", FrogbotSaveMarker, "Saves current marker" },
	{ "addpath", FrogbotAddPath, "Adds a path between markers" },
	{ "removepath", FrogbotRemovePath, "Removes a path between markers" },
	{ "removeallpaths", FrogbotRemoveAllPaths, "Removes all paths from this marker" },
	{ "setzone", FrogbotSetZone, "Sets a marker's zone #" },
	{ "setmarkerflag", FrogbotSetMarkerFlag, "Flags a path between two markers" },
	{ "clearmarkerflag", FrogbotClearMarkerFlag, "Clears flag on a parth between two markers" },
	{ "setpathflag", FrogbotSetPathFlag, "Flags a path between two markers" },
	{ "clearpathflag", FrogbotClearPathFlag, "Clears flag on a parth between two markers" },
	{ "save", FrogbotSaveBotFile, "Saves current routing as a .bot file" },
	{ "info", FrogbotShowInfo, "Shows information about the current marker" },
	{ "summary", FrogbotSummary, "Shows summary of current map" }
};

#define NUM_EDITOR_COMMANDS (sizeof (editor_commands) / sizeof (editor_commands[0]))
#define NUM_STANDARD_COMMANDS (sizeof (std_commands) / sizeof (std_commands[0]))

static void PrintAvailableCommands (frogbot_cmd_t* commands, int num_commands)
{
	int i;
	int max_length = 0;

	G_sprint (self, PRINT_HIGH, "Available commands:\n");
	for (i = 0; i < num_commands; ++i) {
		max_length = max (max_length, strlen (commands[i].name));
	}

	for (i = 0; i < num_commands; ++i) {
		G_sprint (self, PRINT_HIGH, "  &cff0%s&cfff %*s%s\n", commands[i].name, max_length - strlen(commands[i].name), "", commands[i].description);
	}
}

void FrogbotsCommand (void)
{
	frogbot_cmd_t* commands = FrogbotOptionEnabled (FB_OPTION_EDITOR_MODE) ? editor_commands : std_commands;
	int num_commands = FrogbotOptionEnabled (FB_OPTION_EDITOR_MODE) ? NUM_EDITOR_COMMANDS : NUM_STANDARD_COMMANDS;
	char command[64];
	int i;

	if (trap_CmdArgc () <= 1) {
		PrintAvailableCommands (commands, num_commands);
		return;
	}

	if (!FrogbotOptionEnabled (FB_OPTION_EDITOR_MODE) && !FrogbotsCheckMapSupport ()) {
		G_sprint (self, PRINT_HIGH, "Bots not supported on this map.\n");
		return;
	}

	trap_CmdArgv (1, command, sizeof (command));

	for (i = 0; i < num_commands; ++i) {
		if (streq (commands[i].name, command)) {
			commands[i].func ();
			return;
		}
	}

	G_sprint (self, 2, "Command not known.\n");
	PrintAvailableCommands (commands, num_commands);
}



void Bot_Print_Thinking (void)
{
	// Spectator is watching a bot - display bot's thinking
	qbool isSpectator = self->ct == ctSpec && self->s.v.goalentity;
	gedict_t* bot = self->isBot ? self :
		isSpectator ? PROG_TO_EDICT( self->s.v.goalentity ) :
		bots[0].entity ? &g_edicts[bots[0].entity] :
		self;
	gedict_t* linked = bot->fb.linked_marker;
	gedict_t* oldlink = bot->fb.old_linked_marker;
	char data[1024] = { 0 };

	if (g_globalvars.time < self->fb.last_spec_cp)
		return;

	strlcat(data, va("Bot: %s\n", bot->s.v.netname), sizeof(data));
	strlcat(data, va("  %s: %s (%d)\n", redtext ("Touch"), bot->fb.touch_marker ? bot->fb.touch_marker->s.v.classname : "(none)", bot->fb.touch_marker ? bot->fb.touch_marker->fb.index : -1), sizeof(data));
	strlcat(data, va("  %s: %s\n", redtext ("Looking"), bot->fb.look_object ? bot->fb.look_object->s.v.classname : "(nothing)"), sizeof(data));
	strlcat(data, va("  %s: %s (%d)\n", redtext ("Linked"), linked ? linked->s.v.classname : "?", linked ? linked->fb.index : -1), sizeof (data));
	strlcat(data, va("  %s: %s (%d)\n", redtext ("OldLinked"), oldlink ? oldlink->s.v.classname : "?", oldlink ? oldlink->fb.index : -1), sizeof (data));
	strlcat(data, va("  %s: %s\n", redtext ("GoalEnt"), bot->s.v.goalentity ? va("%s (%d) (%f)", g_edicts[bot->s.v.goalentity].s.v.classname, g_edicts[bot->s.v.goalentity].fb.index, g_edicts[bot->s.v.goalentity].fb.saved_goal_desire) : "(none)"), sizeof(data));
	//strlcat(data, va("  %s: armor %d, damage %d\n", redtext ("Strength"), (int)bot->fb.total_armor, (int)bot->fb.total_damage), sizeof(data));
	//strlcat(data, va("  %s: RA %d YA %d GA %d\n", redtext ("Desire"), (int)bot->fb.desire_armorInv, (int)bot->fb.desire_armor2, (int)bot->fb.desire_armor1), sizeof(data));
	//strlcat(data, va("  %s: LG %d RL %d\n", redtext ("Desire"), (int)bot->fb.desire_lightning, (int)bot->fb.desire_rocketlauncher), sizeof(data));
	strlcat(data, "\n", sizeof(data));
	strlcat (data, va ("  %s: %f %f %f\n", redtext ("Velocity"), PASSVEC3 (bot->s.v.velocity)), sizeof(data));
	strlcat (data, va ("  %s: %f %f %f\n", redtext ("Obstruction"), PASSVEC3 (bot->fb.obstruction_normal)), sizeof (data));
	strlcat (data, va ("  %s: %f %f %f\n", redtext ("LastDirection"), PASSVEC3 (bot->fb.last_cmd_direction)), sizeof (data));

	if (bot->s.v.enemy) {
		gedict_t* enemy = &g_edicts[bot->s.v.enemy];

		strlcat(data, va("\n%s: %s\n", redtext ("Enemy"), enemy->s.v.netname), sizeof(data));
		strlcat(data, va("  %s: armor %d, damage %d\n", redtext ("Strength"), (int)enemy->fb.total_armor, (int)enemy->fb.total_damage), sizeof(data));
		strlcat(data, va("  %s: RA %d YA %d GA %d\n", redtext ("Desire"), (int)enemy->fb.desire_armorInv, (int)enemy->fb.desire_armor2, (int)bot->fb.desire_armor1), sizeof(data));
		strlcat(data, va("  %s: LG %d RL %d\n", redtext ("Desire"), (int)enemy->fb.desire_lightning, (int)enemy->fb.desire_rocketlauncher), sizeof(data));
	}

	G_centerprint (self, data);
	self->fb.last_spec_cp = g_globalvars.time + 0.2;
}






qbool TimeTrigger (float *next_time, float time_increment)
{
	qbool triggered = (g_globalvars.time >= *next_time);
	if (triggered) {
		*next_time += time_increment;
		if (*next_time <= g_globalvars.time)
			*next_time = g_globalvars.time + time_increment;
	}
	return triggered;
}

static void BotInitialiseServer (void)
{
	dropper = spawn();
	setsize(dropper, PASSVEC3( VEC_HULL_MIN ), PASSVEC3( VEC_HULL_MAX ));
	dropper->fb.desire = goal_NULL;
	dropper->fb.virtual_goal = dropper;
	dropper->s.v.classname = "fb_dropper";

	sv_accelerate = cvar("sv_accelerate");
	sv_maxspeed = cvar("sv_maxspeed");
	sv_maxstrafespeed = sv_maxspeed;
	sv_maxwaterspeed = sv_maxspeed * 0.7;
	half_sv_maxspeed = sv_maxspeed * 0.5;
	inv_sv_maxspeed = 1 / sv_maxspeed;
}

void BotStartFrame(int framecount) {
	extern void BotsFireLogic (void);

	if ( framecount == 3 ) {
		BotInitialiseServer();
	}
	else if ( framecount == 20 ) {
		LoadMap();
	}
	else if (framecount > 20) {
		marker_time = TimeTrigger (&next_marker_time, 0.1);
		hazard_time = TimeTrigger (&next_hazard_time, 0.025);

		FrogbotPrePhysics1 ();
		FrogbotPrePhysics2 ();

		for (self = world; self = find_plr (self); ) {
			// Logic that gets called every frame for every frogbot
			if (self->isBot) {
				self->fb.willRocketJumpThisTic = able_rj(self);

				SelectWeapon ();

				BotsFireLogic ();

				if (FrogbotOptionEnabled (FB_OPTION_SHOW_THINKING)) {
					Bot_Print_Thinking ();
				}

				BotSetCommand (self);
			}
		}
	}
}

// This is stored in statistics files
int BotVersionNumber (void)
{
	return 1;
}

qbool FrogbotOptionEnabled (int option)
{
	return ((int)cvar ("k_fb_options")) & option;
}
