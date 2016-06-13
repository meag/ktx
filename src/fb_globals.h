// Converted from .qc on 05/02/2016

typedef void (*fb_spawn_func_t)(gedict_t* ent);

typedef struct fb_spawn_s {
	char* name;                 // classname
	fb_spawn_func_t func;       // initialisation function
} fb_spawn_t;

typedef struct fb_path_eval_s {
	gedict_t* touch_marker;
	gedict_t* test_marker;
	qbool rocket_alert;
	int description;
	float path_time;
	qbool path_normal;
	vec3_t player_origin;
	gedict_t* goalentity_marker;
	float goal_late_time;
	float lookahead_time_;
	vec3_t player_direction;
	qbool be_quiet;
} fb_path_eval_t;

// FIXME: Should be able to remove this
//#include "fb_externs.h"

// Globals used for general path-finding
extern qbool path_normal;
extern gedict_t* look_marker;
extern gedict_t* from_marker;
extern gedict_t* middle_marker;
extern gedict_t* to_marker;
extern gedict_t* next_marker;
extern gedict_t* prev_marker;
extern float traveltime;
extern float look_traveltime;
extern float zone_time;

// Globals for physics variables
extern float sv_maxspeed;
extern float sv_maxstrafespeed;
extern float sv_maxwaterspeed;
extern float half_sv_maxspeed;
extern float inv_sv_maxspeed;
extern float sv_accelerate;

#ifndef FL_ONGROUND_PARTIALGROUND
#define FL_ONGROUND_PARTIALGROUND (FL_ONGROUND | FL_PARTIALGROUND)
#endif
#ifndef IT_EITHER_NAILGUN
#define IT_EITHER_NAILGUN (IT_NAILGUN | IT_SUPER_NAILGUN)
#endif
#ifndef IT_NAILGUN_ROCKET
#define IT_NAILGUN_ROCKET (IT_ROCKET_LAUNCHER | IT_SUPER_NAILGUN | IT_NAILGUN)
#endif
#ifndef IT_VELOCITY
#define IT_VELOCITY (IT_ROCKET_LAUNCHER | IT_GRENADE_LAUNCHER | IT_SUPER_NAILGUN | IT_NAILGUN)
#endif
#ifndef IT_CONTINUOUS
#define IT_CONTINUOUS (IT_LIGHTNING | IT_SUPER_NAILGUN | IT_NAILGUN)
#endif
#ifndef IT_AXE_SHOTGUN
#define IT_AXE_SHOTGUN (IT_AXE | IT_SHOTGUN)
#endif
#ifndef IT_ALL_BUT_GRENADE
#define IT_ALL_BUT_GRENADE (IT_AXE | IT_SHOTGUN | IT_SUPER_SHOTGUN | IT_NAILGUN | IT_SUPER_NAILGUN | IT_ROCKET_LAUNCHER | IT_LIGHTNING)
#endif
#ifndef IT_ALL
#define IT_ALL (IT_AXE | IT_SHOTGUN | IT_SUPER_SHOTGUN | IT_NAILGUN | IT_SUPER_NAILGUN | IT_GRENADE_LAUNCHER | IT_ROCKET_LAUNCHER | IT_LIGHTNING)
#endif
#ifndef IT_NOT_AMMO
#define IT_NOT_AMMO 16773375
#endif
#ifndef IT_ARMOR
#define IT_ARMOR (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3)
#endif
#ifndef IT_NOT_ARMOR
#define IT_NOT_ARMOR (~IT_ARMOR)
#endif
#ifndef IT_INVULNERABILITY_QUAD
#define IT_INVULNERABILITY_QUAD (IT_INVULNERABILITY | IT_QUAD)
#endif
#ifndef IT_NOT_INVISIBILITY
#define IT_NOT_INVISIBILITY (~IT_INVISIBILITY)
#endif
#ifndef IT_NOT_INVULNERABILITY
#define IT_NOT_INVULNERABILITY (~IT_INVULNERABILITY)
#endif
#ifndef IT_NOT_SUIT
#define IT_NOT_SUIT (~IT_SUIT)
#endif
#ifndef IT_NOT_QUAD
#define IT_NOT_QUAD (~IT_QUAD)
#endif
#ifndef ITEM_RUNE_MASK
#define ITEM_RUNE_MASK (CTF_RUNE_RES | CTF_RUNE_STR | CTF_RUNE_HST | CTF_RUNE_RGN)
#endif
#ifndef NOT_EF_DIMLIGHT
#define NOT_EF_DIMLIGHT 16777207
#endif
#ifndef EF_DIMLIGHT_BLUE
#define EF_DIMLIGHT_BLUE (EF_DIMLIGHT | EF_BLUE)
#endif
#ifndef EF_DIMLIGHT_RED
#define EF_DIMLIGHT_RED (EF_DIMLIGHT | EF_RED)
#endif
#ifndef CLIENTKILL
#define CLIENTKILL 11
#endif

#define FB_PREFER_ROCKET_LAUNCHER 1
#define FB_PREFER_LIGHTNING_GUN   2

#ifndef GAME_ENABLE_POWERUPS
#define GAME_ENABLE_POWERUPS 1
#endif
#ifndef GAME_ENABLE_RUNES
#define GAME_ENABLE_RUNES 2
#endif
#ifndef GAME_RUNE_RJ
#define GAME_RUNE_RJ 4
#endif
#ifndef GAME_MATCH
#define GAME_MATCH 64
#endif
#ifndef GAME_ENABLE_DROPWEAP
#define GAME_ENABLE_DROPWEAP 512
#endif
#ifndef GAME_ENABLE_AUTOREPORT
#define GAME_ENABLE_AUTOREPORT 1024
#endif
#ifndef GAME_ENABLE_AUTOSTEAMS
#define GAME_ENABLE_AUTOSTEAMS 2048
#endif
#ifndef FORWARD
#define FORWARD 1
#endif
#ifndef BACK
#define BACK 2
#endif
#ifndef LEFT
#define LEFT 4
#endif
#ifndef RIGHT
#define RIGHT 8
#endif
#ifndef FORWARD_LEFT
#define FORWARD_LEFT 5
#endif
#ifndef FORWARD_RIGHT
#define FORWARD_RIGHT 9
#endif
#ifndef BACK_LEFT
#define BACK_LEFT 6
#endif
#ifndef BACK_RIGHT
#define BACK_RIGHT 10
#endif
#ifndef UP
#define UP 16
#endif
#ifndef DOWN
#define DOWN 32
#endif
#ifndef JUMPSPEED
#define JUMPSPEED 270
#endif

// Fall results
#ifndef FALL_FALSE
#define FALL_FALSE 0
#endif
#ifndef FALL_BLOCKED
#define FALL_BLOCKED 1
#endif
#ifndef FALL_LAND
#define FALL_LAND 2
#endif
#ifndef FALL_DEATH
#define FALL_DEATH 3
#endif

// Path flags
#ifndef WATERJUMP_
#define WATERJUMP_ (1 << 1)
#endif
#ifndef DM6_DOOR
#define DM6_DOOR (1 << 8)
#endif
#ifndef ROCKET_JUMP
#define ROCKET_JUMP (1 << 9)
#endif
#ifndef JUMP_LEDGE
#define JUMP_LEDGE (1 << 10)
#endif
#ifndef VERTICAL_PLATFORM
#define VERTICAL_PLATFORM (1 << 11)
#endif
#ifndef SAVED_DESCRIPTION
#define SAVED_DESCRIPTION (DM6_DOOR | ROCKET_JUMP | JUMP_LEDGE | VERTICAL_PLATFORM)
#endif
#ifndef NOT_ROCKET_JUMP
#define NOT_ROCKET_JUMP (~ROCKET_JUMP)
#endif
#ifndef REVERSIBLE
#define REVERSIBLE (1 << 14)
#endif
#ifndef WATER_PATH
#define WATER_PATH (1 << 15)
#endif
#ifndef DELIBERATE_AIR
#define DELIBERATE_AIR (1 << 17)
#endif
#ifndef WAIT_GROUND
#define WAIT_GROUND (1 << 18)
#endif
#ifndef STUCK_PATH
#define STUCK_PATH (1 << 19)
#endif
#ifndef AIR_ACCELERATION
#define AIR_ACCELERATION (1 << 20)
#endif
#ifndef NO_DODGE
#define NO_DODGE (1 << 21)
#endif
#ifndef DELIBERATE_AIR_WAIT_GROUND
#define DELIBERATE_AIR_WAIT_GROUND (DELIBERATE_AIR | WAIT_GROUND)
#endif

// Bot flags
#ifndef BOTFLAG_UNREACHABLE
#define BOTFLAG_UNREACHABLE 1
#endif

// Marker flags
#ifndef UNREACHABLE
#define UNREACHABLE 1
#endif
#ifndef T_WATER
#define T_WATER 2
#endif
#ifndef T_NO_AIR
#define T_NO_AIR 4
#endif
#ifndef MARKER_IS_DM6_DOOR
#define MARKER_IS_DM6_DOOR 8
#endif
#ifndef NOTARGET_ENEMY
#define NOTARGET_ENEMY 32
#endif
#ifndef AWARE_SURROUNDINGS
#define AWARE_SURROUNDINGS 128
#endif
#ifndef HURT_SELF
#define HURT_SELF 1024
#endif
#ifndef CHASE_ENEMY
#define CHASE_ENEMY 2048
#endif
#ifndef RUNAWAY
#define RUNAWAY 4096
#endif
#ifndef WAIT
#define WAIT 8192
#endif
#ifndef NOT_HURT_SELF
#define NOT_HURT_SELF (~HURT_SELF)
#endif
#ifndef NOT_NOTARGET_ENEMY
#define NOT_NOTARGET_ENEMY ~(NOTARGET_ENEMY)
#endif
#ifndef NOT_AWARE_SURROUNDINGS
#define NOT_AWARE_SURROUNDINGS ~(AWARE_SURROUNDINGS)
#endif

#ifndef CAMPBOT
#define CAMPBOT 1
#endif
#ifndef SHOT_FOR_LUCK
#define SHOT_FOR_LUCK 2
#endif
#ifndef IT_ROCKET_LIGHTNING
#define IT_ROCKET_LIGHTNING (IT_ROCKET_LAUNCHER | IT_LIGHTNING)
#endif
#ifndef HELP_TEAMMATE
#define HELP_TEAMMATE 128
#endif

gedict_t* AddToList(gedict_t* first_in_list, gedict_t* ent);

qbool IsVisible(gedict_t* ent);

qbool ExistsPath(gedict_t* from_marker, gedict_t* to_marker, int* new_path_state);
float boomstick_only(void);

float CountTeams(void);
qbool EnemyDefenceless (gedict_t* self);

float enemy_shaft_attack(void);
float W_BestWeapon(void);

gedict_t* LocateMarker(vec3_t org);
float RankForWeapon(float w);
float WeaponCode(float w);

float crandom(void);
gedict_t* RemoveFromList(gedict_t* first_in_list, gedict_t* ent);

float EntitiesTouching(gedict_t* e1, gedict_t* e2);
gedict_t* EntityAt(gedict_t* start_entity, float index_);
float near_teammate(gedict_t* me);

qbool able_rj(gedict_t* self);
qbool EntVisible(vec3_t vec);
qbool VisibleEntity(gedict_t* ent);
gedict_t* identify_teammate_(gedict_t* me);
float anglemod(float v);
qbool visible_teammate(gedict_t* me);
gedict_t* HelpTeammate(void);

// 
char* SetNetName(void);
char* EnemyTeamName(void);
char* FriendTeamName(void);
char* SetTeamNetName(void);

qbool Visible_360(gedict_t* self, gedict_t* visible_object);
void VelocityForArrow(gedict_t* self);
void Z_marker_error(void);
void S_time_error(void);
void S_path_marker_error(void);
void Z_path_marker_error(void);
void Z_next_zone_marker_error(void);
void Z_sight_from_marker_error(void);
void Z_higher_sight_from_marker_error(void);
void Z_sight_from_time_error(void);

// route_calc.qc
void InitialiseMarkerRoutes(void);

// route_fields.qc
void EnterZone(int zoneNumber, float teamNumber, float strong);
void LeaveZone(int zoneNumber, float teamNumber, float strong);

// items.qc
qbool pickup_health0(void);
qbool pickup_health2(void);
qbool pickup_armor1(void);
qbool pickup_armor2(void);
qbool pickup_armorInv(void);
qbool pickup_true(void);
qbool pickup_supershotgun2(void);
qbool pickup_nailgun2(void);
qbool pickup_supernailgun2(void);
qbool pickup_grenadelauncher2(void);
qbool pickup_rocketlauncher2(void);
qbool pickup_lightning2(void);
qbool pickup_shells(void);
qbool pickup_spikes(void);
qbool pickup_rockets(void);
qbool pickup_cells(void);

// marker_util.qc
void marker_touch(void);
void BecomeMarker(gedict_t* marker);

// route_calc.qc
void CheckWaterColumn(gedict_t* m, vec3_t m_pos, vec3_t testplace);

#define SubZoneFunction(x) \
	void S1_ ## x (void); \
	void S2_ ## x (void); \
	void S3_ ## x (void); \
	void S4_ ## x (void); \
	void S5_ ## x (void); \
	void S6_ ## x (void); \
	void S7_ ## x (void); \
	void S8_ ## x (void); \
	void S9_ ## x (void); \
	void S10_ ## x (void); \
	void S11_ ## x (void); \
	void S12_ ## x (void); \
	void S13_ ## x (void); \
	void S14_ ## x (void); \
	void S15_ ## x (void); \
	void S16_ ## x (void); \
	void S17_ ## x (void); \
	void S18_ ## x (void); \
	void S19_ ## x (void); \
	void S20_ ## x (void); \
	void S21_ ## x (void); \
	void S22_ ## x (void); \
	void S23_ ## x (void); \
	void S24_ ## x (void); \
	void S25_ ## x (void); \
	void S26_ ## x (void); \
	void S27_ ## x (void); \
	void S28_ ## x (void); \
	void S29_ ## x (void); \
	void S30_ ## x (void); \
	void S31_ ## x (void); \
	void S32_ ## x (void);

SubZoneFunction(time_)
SubZoneFunction(path_marker)

#define ZoneFunction(x) \
	void Z1_ ## x (void); \
	void Z2_ ## x (void); \
	void Z3_ ## x (void); \
	void Z4_ ## x (void); \
	void Z5_ ## x (void); \
	void Z6_ ## x (void); \
	void Z7_ ## x (void); \
	void Z8_ ## x (void); \
	void Z9_ ## x (void); \
	void Z10_ ## x (void); \
	void Z11_ ## x (void); \
	void Z12_ ## x (void); \
	void Z13_ ## x (void); \
	void Z14_ ## x (void); \
	void Z15_ ## x (void); \
	void Z16_ ## x (void); \
	void Z17_ ## x (void); \
	void Z18_ ## x (void); \
	void Z19_ ## x (void); \
	void Z20_ ## x (void); \
	void Z21_ ## x (void); \
	void Z22_ ## x (void); \
	void Z23_ ## x (void); \
	void Z24_ ## x (void);

ZoneFunction(marker)
ZoneFunction(next_zone_marker)
ZoneFunction(path_marker)
ZoneFunction(sight_from_marker)
ZoneFunction(higher_sight_from_marker)
ZoneFunction(sight_from_time)

// defs.qc
void bprint_fb(int level, char* text);
void bprint_ftos(int level, float value);
void bprint_g(int level, float f);

// botutil.qc
float BestArrowForDirection(gedict_t* self, vec3_t dir_move);

// botwater.qc
void BotWaterMove (gedict_t* self);

// weapons.qc
void GrenadeExplode(void);
void Missile_Remove(void);

// items.qc
float goal_NULL(gedict_t* self);

// client.qc
void NewItems(void);
void InitBodyQue(void);
void ClientConnect_apply(void);
void PlayerPreThink_apply(void);
void PlayerPostThink_apply(void);
void PlayerDeathThink(void);
void CheckLand(void);
void BotDeathThink(void);
void BotClientEntersEvent(gedict_t* self, gedict_t* spawn_pos);
void BotPreThink(gedict_t* self);
void BotClientConnectedEvent (gedict_t* self);
void BotOutOfWater (gedict_t* self);
void BotSetCommand (gedict_t* self);
void ThinkTime (gedict_t* self);
void ResetGoalEntity(gedict_t* self);
void ResetEnemy(gedict_t* self);

// match.qc
//void DoCountDown(void);
void NextLevel(void);

// botwater.qc
qbool BotShouldDischarge(void);

// botphys.qc
void FrogbotPrePhysics1 (void);
void FrogbotPrePhysics2(void);
void FrogbotPostPhysics(void);
//void thud_touch(void);

// names.qc
void CopyBot(gedict_t* e, gedict_t* f);

// defs2.qc
void remove_self(void);

// bothazd.qc
void AvoidHazards(void);
void NewVelocityForArrow (gedict_t* self, vec3_t dir_move);

// route_lookup.qc
void SightMarker(gedict_t* from_marker);
void HigherSightMarker(gedict_t* from_marker);

// botweap.qc
void UpdateWeapons(void);
void SelectWeapon(void);
void AttackRespawns(void);
void SetFireButton(gedict_t* self, vec3_t rel_pos, float rel_dist);
void DelayUpdateWeapons(gedict_t* self);

// marker_util.qc
qbool Visible_infront(gedict_t* self, gedict_t* visible_object);
void AssignVirtualGoal (gedict_t* item);
void AssignVirtualGoal_apply(gedict_t* marker_);
void adjust_view_ofs_z(gedict_t* ent);

// botenemy.qc
void ClearLookObject(gedict_t* client);
void LookEnemy(gedict_t* player, gedict_t* enemy);
void BestEnemy(gedict_t* self);
void CheckCombatEnemy(gedict_t* attacker, gedict_t* targ);

// botjump.qc
void CheckCombatJump(void);
//void BotInLava(void);
void a_rocketjump (gedict_t* self);

// botgoal.qc
void UpdateGoal(void);

// botpath.qc
void frogbot_marker_touch(void);

// marker_load.qc
void CreateMarker(float x, float y, float z);
void AllMarkersLoaded(void);

void SetZone(int zone_number, int marker_number);
void SetMarkerFlag(int marker_number, int flags);
void AddToQue(gedict_t* ent);
void SetGoal(int goal, int marker_number);
void SetGoalForMarker(int goal, gedict_t* marker);
void SetMarkerPathFlags(int marker_number, int path_index, int flags);
void SetMarkerPath(int source_marker, int path_index, int next_marker);
void SetMarkerViewOffset (int marker, float zOffset);

// added for ktx
qbool fb_lg_disabled(void);
float random(void);
//void StartItems(void);

// maps
void LoadMap(void);
qbool FrogbotsCheckMapSupport (void);

void map_aerowalk(void);
void map_amphi2(void);
void map_dm4(void);
void map_dm3(void);
void map_dm6(void);
void map_e1m2(void);
void map_frobodm2(void);
void map_pkeg1(void);
void map_povdmm4(void);
void map_spinev2(void);
void map_ukooldm2(void);
void map_ultrav(void);
void map_ztndm3(void);
void map_ztndm4(void);
void map_ztndm5(void);

// 
float pr1_rint (float f);

// 
void BotEvadeLogic(gedict_t* self);
qbool SameTeam(gedict_t* p1, gedict_t* p2);

// botstat.qc
void UpdateTotalDamage (gedict_t* client);
void UpdateGoalEntity (gedict_t* item);

// bot_client.qc
void BotPlayerDeathEvent (gedict_t* self);

// bot_items.qc
int ItemSpawnFunctionCount(void);
qbool NoItemTouch (gedict_t* self, gedict_t* other);
qbool BotsPreTeleport (gedict_t* self, gedict_t* other);

#define FB_CVAR_GAMEMODE "k_fb_gamemode"
#define FB_CVAR_SKILL "k_fb_skill"

// FBCA code just set nextthink to 0 when item respawned...
qbool WaitingToRespawn (gedict_t* ent);

int NumberOfClients (void);
qbool TimeTrigger (float *next_time, float time_increment);

qbool IsMarkerFrame (void);
qbool IsHazardFrame (void);