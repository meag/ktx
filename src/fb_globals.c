// Converted from .qc on 05/02/2016
#include "g_local.h"
#include "fb_globals.h"

qbool marker_time = false;
float next_marker_time = 0;
qbool hazard_time = false;
float next_hazard_time = 0;

char* deathtype = 0;
float armorplay = 0;
int gamemode = 0;
int new_gamemode = 0;
float game_disable_autosteams = 0;
float game_disable_botchat = 0;
float game_lava_cheat = 0;
float game_hardestjump_disabled = 0;
gedict_t* targ = 0;
vec3_t extra_velocity = { 0 };
float damage = 0;
float t_radius_damage = 0;
gedict_t* ignore = 0;
float enemy_visible = 0;
float predict_dist = 0;
gedict_t* previous_spot = 0;
gedict_t* enemy_ = 0;
gedict_t* virtual_enemy = 0;
gedict_t* look_object_ = 0;
gedict_t* test_enemy = 0;
gedict_t* viewport = 0;
gedict_t* non_viewport = 0;
gedict_t* self_aiment = 0;
vec3_t targ_origin = { 0 };
vec3_t targ_centre = { 0 };
vec3_t inflictor_origin = { 0 };
gedict_t* inflictor = 0;
float damage_radius = 0;
float inflictor_dist = 0;
float numberofbots = 0;
float numberofplayers = 0;
float numberofclients = 0;
vec3_t oldvelocity_ = { 0 };
vec3_t delta_velocity = { 0 };
gedict_t* current_waiting_bot = 0;
gedict_t* first_item = 0;
gedict_t* first_ent = 0;
gedict_t* first_marker = 0;
float takedamage_exists = 0;
gedict_t* dropper = 0;
float fallheight = 0;
float mouse_friction = 0;
vec3_t pitch_tangent = { 0 };
vec3_t yaw_tangent = { 0 };
float mouse_vel = 0;
gedict_t* m_P = 0;
gedict_t* m_zone = 0;
float P_time = 0;
int m_D = 0;
float new_path_state = 0;
float old_path_state = 0;
float RA_type = 0;
float try_shoot = 0;
qbool markers_loaded = false;
float goal_index = 0;
gedict_t* current_load_position = 0;
gedict_t* load_position = 0;
float goal_number = 0;
gedict_t* test_goal = 0;
float same_zone = 0;
gedict_t* zone_stack_head = 0;

gedict_t* markers[NUMBER_MARKERS] = { 0 };
gedict_t* zone_head[NUMBER_ZONES] = { 0 };
gedict_t* zone_tail[NUMBER_ZONES] = { 0 };
int subzone_indexes[NUMBER_ZONES] = { 0 };

float path_normal = 0;
float leave = 0;
gedict_t* from_marker = 0;
gedict_t* middle_marker = 0;
gedict_t* next_marker = 0;
gedict_t* to_marker = 0;
gedict_t* look_marker = 0;
gedict_t* test_marker = 0;
gedict_t* prev_marker = 0;
float two_teams = 0;
float first_team = 0;
float second_team = 0;
float new_team = 0;
vec3_t to_marker_pos = { 0 };
gedict_t* goal_entity = 0;
float weapon_refresh_time_ = 0;
float weapon_ = 0;
float am = 0;
float save = 0;
float take = 0;
float maxplayers = 0;
gedict_t* touch_marker_ = 0;
float lookahead_time_ = 0;
float prediction_error_ = 0;
vec3_t origin_ = { 0 };
float impulse_ = 0;
float time_start = 0;
float framecount_start = 0;
float real_frametime = 0;
float sv_accelerate = 0;
float sv_maxfriction = 0;
float sv_accelerate_frametime = 0;
float sv_maxspeed = 0;
float sv_maxwaterspeed = 0;
float half_sv_maxspeed = 0;
float inv_sv_maxspeed = 0;
float sv_maxstrafespeed = 0;
float friction_factor = 0;
float old_time = 0;
float distance = 0;
vec3_t hor_direction = { 0 };
vec3_t dir_forward = { 0 };
float current_maxspeed = 0;
float max_accel_forward = 0;
vec3_t desired_accel = { 0 };
vec3_t hor_velocity = { 0 };
vec3_t new_velocity = { 0 };
vec3_t new_origin = { 0 };
vec3_t last_clear_hor_velocity = { 0 };
float last_clear_hor_speed = 0;
vec3_t jump_velocity = { 0 };
vec3_t jump_origin = { 0 };
float oldflags = 0;
float current_arrow = 0;
float content = 0;
float content1 = 0;
float content2 = 0;
float content3 = 0;
float fall = 0;
float new_fall = 0;
float current_fallspot = 0;
vec3_t edge_normal = { 0 };
vec3_t self_view = { 0 };
vec3_t testplace = { 0 };
float tries = 0;
vec3_t last_clear_point = { 0 };
vec3_t last_clear_velocity = { 0 };
float jumpspeed = 0;
float path_score = 0;
float total_goal_time = 0;
vec3_t self_dir = { 0 };
float look_score = 0;
vec3_t dir_look = { 0 };
/*
gedict_t* best_goal = 0;
float best_goal_desire = 0;
float best_goal_time = 0;
float best_score2 = 0;
gedict_t* best_goal2 = 0;
float goal_desire = 0;
float enemy_desire = 0;
float enemy_repel = 0;
*/
float best_goal_score = 0;
float current_goal_time = 0;
float current_goal_time_125 = 0;
float best_respawn_time = 0;
gedict_t* goalentity_marker = 0;
float goal_late_time = 0;
gedict_t* linked_marker_ = 0;
vec3_t linked_marker_origin = { 0 };
float goal_score = 0;
float goal_score2 = 0;
float same_dir = 0;
float avoid = 0;
float enemy_score = 0;
vec3_t rel_pos = { 0 };
vec3_t rel_pos2 = { 0 };
vec3_t rel_dir = { 0 };
float rel_dist = 0;
float rel_time = 0;
vec3_t rel_hor_dir = { 0 };
float hor_component = 0;
float min_angle_error = 0;
gedict_t* enemy_touch_marker = 0;
float new_health = 0;
vec3_t src = { 0 };
vec3_t direction = { 0 };
float risk = 0;
float hit_radius = 0;
float risk_factor = 0;
float risk_strength = 0;
vec3_t rocket_origin = { 0 };
vec3_t enemy_angles = { 0 };
gedict_t* bot = 0;
float rnd = 0;
gedict_t* spawn_pos = 0;
gedict_t* spots = 0;
float pcount = 0;
gedict_t* thing = 0;
float numspots = 0;
float totalspots = 0;
vec3_t vec1 = { 0 };
vec3_t vec2 = { 0 };
vec3_t vec_ = { 0 };
gedict_t* think_ent = 0;
gedict_t* trigger = 0;
vec3_t cmins = { 0 };
vec3_t cmaxs = { 0 };
gedict_t* item = 0;
vec3_t tmin = { 0 };
vec3_t tmax = { 0 };
float or = 0;
float nr = 0;
float best_weapon = 0;
float score_count = 0;
vec3_t item_pos = { 0 };
vec3_t marker_pos = { 0 };
gedict_t* marker_ = 0;
gedict_t* marker2 = 0;
float zone_time = 0;
float real_yaw_ = 0;
gedict_t* fireball = 0;
gedict_t* bubble = 0;
gedict_t* bubble_spawner = 0;
vec3_t org_ = { 0 };
float rnd1 = 0;
float rnd2 = 0;
float rnd3 = 0;
float character = 0;
float block_ = 0;
float char_count = 0;
float word_count = 0;
float spawnflags_ = 0;
float digit = 0;
float exponent = 0;
float previous_exponent = 0;
gedict_t* target_ = 0;
float traveltime = 0;
float traveltime2 = 0;
float traveltime3 = 0;
float look_traveltime = 0;
float look_traveltime_squared = 0;
gedict_t* flag_self = 0;
float flag_pos = 0;
gedict_t* flag1 = 0;
gedict_t* flag2 = 0;
gedict_t* tfog = 0;
vec3_t weapons_vel = { 0 };
gedict_t* death = 0;
gedict_t* trace_ent1 = 0;
gedict_t* trace_ent2 = 0;
gedict_t* old_self = 0;
gedict_t* old_other = 0;
float ledge_backup = 0;
float try_jump_ledge = 0;
float being_blocked = 0;
float forward = 0;
char* deathstring = 0;
char* deathstring2 = 0;
gedict_t* removebot_self = 0;
vec3_t start = { 0 };
vec3_t end = { 0 };
int description = 0;
//float goal_time = 0;
float path_time = 0;
float component_speed = 0;
float do_jump = 0;
float dm = 0;
float count_ = 0;
gedict_t* array_sub_object_ = 0;
float bind_char = 0;
float dodge_factor = 0;
gedict_t* dodge_missile = 0;
gedict_t* killed_self = 0;
gedict_t* rune_self = 0;
gedict_t* door_enemy = 0;
gedict_t* multi_ent = 0;
//float intermission_running = 0;
//float intermission_exittime = 0;
gedict_t* bodyque_head = 0;
float rocket_alert = 0;
vec3_t rocket_endpos = { 0 };
//char* nextmap = 0;
float frogbot_number = 0;
gedict_t* prephysics = 0;
gedict_t* postphysics = 0;
gedict_t* frogbot_spawned = 0;
float use_ammo = 0;
int available_weapons = 0;                      // bitmask of weapons on the current map
float check_parm_time = 0;
float unstick_time = 0;
float no_bots_stuck = 0;
float first_trace_fraction = 0;
vec3_t first_trace_plane_normal = { 0 };
float turning_speed = 0;
vec3_t hor_normal_vec = { 0 };
vec3_t last_clear_angle = { 0 };
vec3_t velocity_hor_angle = { 0 };
float beQuiet = 0;
float runaway_time = 0;
float runaway_time_temp = 0;
float runaway_score = 0;
float runaway_score_temp = 0;
gedict_t* runaway_dest = 0;
float runaway_fail = 0;
gedict_t* runaway_marker_temp = 0;
gedict_t* runaway_prev_temp = 0;
float min_traveltime = 0;
gedict_t* to_zone = 0;
float test_away_score = 0;
float best_away_score = 0;
gedict_t* best_away_marker = 0;
gedict_t* search_entity = 0;
float RA_time = 0;
float enemy_time_squared = 0;
float intermission_getting_closer = 0;
float total_entity_count = 0;
float ar_time = 0;
float lock = 0;
float duel_mode = 0;
float team_mode = 0;
float a_rep = 0;
char* string_null = 0;
float pingupdate = 0;
gedict_t* team0 = 0;
gedict_t* team1 = 0;
gedict_t* team2 = 0;
gedict_t* team3 = 0;
gedict_t* team4 = 0;
gedict_t* updatebot = 0;
float cvar_ = 0;
float print_ = 0;
float reset_ = 0;
char* varg = 0;
float var_ = 0;
float min_ = 0;
float max_ = 0;
float idleprint_time = 0;
float last_action_time = 0;
float force_time = 0;
float users = 0;
float inter_print = 0;
float loadbot_time = 0;
float loadingbots = 0;
float check_loadingbots = 0;
float game_drop = 0;
float game_damage = 0;
float game_raspawn = 0;
float teams = 0;
float initialized = 0;
float updatestuff = 0;
float a_sounds = 0;
char* a_spawn = 0;

qbool fb_lg_disabled() {
	return (qbool) ((int)cvar("k_disallow_weapons") & IT_LIGHTNING) != 0;
}

float random() {
	return g_random();
}

// from defs2.qc
float boomstick_only() {
	return (((int)self->s.v.items & IT_SHOTGUN) && !((int)self->s.v.items & (IT_NAILGUN | IT_SUPER_NAILGUN | IT_SUPER_SHOTGUN | IT_ROCKET_LAUNCHER | IT_GRENADE_LAUNCHER | IT_LIGHTNING)));
}

// taken from pr1 implementation
float pr1_rint(float f) {
	if (f > 0)
		return (int)(f + 0.5);
	else
		return (int)(f - 0.5);
}

void bprint_fb(int level, char* text)
{
	G_bprint(level, "%s", text);
}

void bprint_ftos(int level, float value)
{
	G_bprint(level, "%f", value);
}

void bprint_g(int lev, float f)
{
	G_bprint (lev, "%d", (int)f);
}

// weapons.qc
void Missile_Remove() {
	self->s.v.owner = NUM_FOR_EDICT(world);
	remove_self();
}

void remove_self() {
	trap_remove(NUM_FOR_EDICT(self));
}

// match.qc
void NewItems() {
	// Replace...
}

void ClientConnect_apply() {
	// Replace...
}

void CopyBot(gedict_t* start, gedict_t* f) {
	// Replace...
}

float enemy_shaft_attack() {
	return ((enemy_->s.v.weapon == IT_LIGHTNING) && (self->fb.enemy_dist < 630) && (g_globalvars.time < enemy_->attack_finished));
}

void PlayerPreThink_apply() {
	// remove
}

void PlayerPostThink_apply() {
	// remove
}

qbool bots_enabled() {
	return TRUE;	// FIXME: make a variable
}

qbool SameTeam(gedict_t* p1, gedict_t* p2) {
	return p1 == p2 || (teamplay && streq( ezinfokey(p1, "team"), ezinfokey(p2, "team") ));
}

static qbool HasRLOrLG (gedict_t* self)
{
	return ((((int)self->s.v.items & IT_ROCKET_LAUNCHER) && (self->s.v.ammo_rockets > 1)) || (((int)self->s.v.items & IT_LIGHTNING) && (self->s.v.ammo_cells > 5)));
}

static qbool EnemyHasRLorLG (gedict_t* self)
{
	gedict_t* enemy = &g_edicts[self->s.v.enemy];
	if (self->s.v.enemy == 0)
		return FALSE;

	return ((((int)enemy->s.v.items & IT_ROCKET_LAUNCHER) && (enemy->s.v.ammo_rockets > 1)) || (((int)enemy->s.v.items & IT_LIGHTNING) && (enemy->s.v.ammo_cells > 5)));
}

static qbool IsDanger (gedict_t* self)
{
	gedict_t* enemy = &g_edicts[self->s.v.enemy];
	if (self->s.v.enemy == 0)
		return FALSE;
	
	if ((self->s.v.health < enemy->s.v.health) && 
		(self->s.v.armorvalue < enemy->s.v.armorvalue) && 
		(self->s.v.armortype < enemy->s.v.armortype) && 
		(self->fb.firepower < enemy->fb.firepower))
		return TRUE;
	if (((int)enemy->s.v.items & (IT_INVULNERABILITY | IT_QUAD | IT_INVISIBILITY)) && (!((int)self->s.v.items & (IT_INVULNERABILITY | IT_INVISIBILITY))))
		return TRUE;
	return FALSE;
}

qbool EnemyDefenceless(gedict_t* self)
{
	if (!EnemyHasRLorLG(self) && HasRLOrLG (self)) {
		return (!IsDanger(self) && (self->s.v.health > 50) && (self->s.v.armorvalue >= 50));
	}
	return FALSE; 
}