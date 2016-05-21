// Converted from .qc on 05/02/2016
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FL_ONGROUND_PARTIALGROUND
#define FL_ONGROUND_PARTIALGROUND 1536
#endif
extern qbool marker_time;
extern float next_marker_time;
extern qbool hazard_time;
extern float next_hazard_time;

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
extern char* deathtype;
extern float armorplay;
extern int gamemode;
extern int new_gamemode;
extern float game_rl_pref;
extern float game_lg_pref;
extern float game_disable_autosteams;
extern float game_disable_botchat;
extern float game_lava_cheat;
extern float game_hardestjump_disabled;

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
extern gedict_t* targ;
extern vec3_t extra_velocity;
extern float damage;
extern float t_radius_damage;
extern gedict_t* ignore;
gedict_t* AddToList(gedict_t* first_in_list, gedict_t* ent);
extern float enemy_visible;
extern float predict_dist;
extern gedict_t* previous_spot;
extern gedict_t* enemy_;
extern gedict_t* virtual_enemy;
extern gedict_t* look_object_;
extern gedict_t* test_enemy;
extern gedict_t* viewport;
extern gedict_t* non_viewport;
extern gedict_t* self_aiment;
extern vec3_t targ_origin;
extern vec3_t targ_centre;
extern vec3_t inflictor_origin;
extern gedict_t* inflictor;
extern float damage_radius;
extern float inflictor_dist;
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
extern float numberofbots;
extern float numberofplayers;
extern float numberofclients;
extern vec3_t oldvelocity_;
extern vec3_t delta_velocity;
extern gedict_t* current_waiting_bot;
extern gedict_t* first_item;
extern gedict_t* first_ent;
extern gedict_t* first_marker;
extern float takedamage_exists;
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
extern gedict_t* dropper;
extern float fallheight;
extern float mouse_friction;
extern vec3_t pitch_tangent;
extern vec3_t yaw_tangent;
extern float mouse_vel;
extern gedict_t* m_P;
extern gedict_t* m_zone;
extern float P_time;
extern int m_D;
extern float new_path_state;
extern float old_path_state;
#ifndef WATERJUMP_
#define WATERJUMP_ 2
#endif
#ifndef DM6_DOOR
#define DM6_DOOR 256
#endif
#ifndef ROCKET_JUMP
#define ROCKET_JUMP 512
#endif
#ifndef JUMP_LEDGE
#define JUMP_LEDGE 1024
#endif
#ifndef VERTICAL_PLATFORM
#define VERTICAL_PLATFORM 2048
#endif
#ifndef SAVED_DESCRIPTION
#define SAVED_DESCRIPTION 3840
#endif
#ifndef NOT_ROCKET_JUMP
#define NOT_ROCKET_JUMP 65023
#endif
#ifndef REVERSIBLE
#define REVERSIBLE 16384
#endif
#ifndef WATER_PATH
#define WATER_PATH 32768
#endif
#ifndef DELIBERATE_AIR
#define DELIBERATE_AIR 131072
#endif
#ifndef WAIT_GROUND
#define WAIT_GROUND 262144
#endif
#ifndef STUCK_PATH
#define STUCK_PATH 524288
#endif
#ifndef AIR_ACCELERATION
#define AIR_ACCELERATION 1048576
#endif
#ifndef NO_DODGE
#define NO_DODGE 2097152
#endif
#ifndef DELIBERATE_AIR_WAIT_GROUND
#define DELIBERATE_AIR_WAIT_GROUND 393216
#endif
//extern float RA_type;
//#ifndef THROUGH_NODE
//#define THROUGH_NODE 1
//#endif
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
#define NOT_HURT_SELF 16776191
#endif
#ifndef NOT_NOTARGET_ENEMY
#define NOT_NOTARGET_ENEMY 16777183
#endif
#ifndef NOT_AWARE_SURROUNDINGS
#define NOT_AWARE_SURROUNDINGS 16777087
#endif
#ifndef CAMPBOT
#define CAMPBOT 1
#endif
#ifndef SHOT_FOR_LUCK
#define SHOT_FOR_LUCK 2
#endif

extern float nextthink_;
extern float try_shoot;
extern qbool markers_loaded;
extern float goal_index;
extern gedict_t* current_load_position;
extern gedict_t* load_position;

extern float goal_number;
extern gedict_t* test_goal;
extern float same_zone;
extern gedict_t* zone_stack_head;

extern gedict_t* markers[NUMBER_MARKERS];
extern gedict_t* zone_head[NUMBER_ZONES];
extern gedict_t* zone_tail[NUMBER_ZONES];
extern int subzone_indexes[NUMBER_ZONES];

extern float path_normal;
extern float leave;
extern gedict_t* from_marker;
extern gedict_t* middle_marker;
extern gedict_t* next_marker;
extern gedict_t* to_marker;
extern gedict_t* look_marker;
//extern gedict_t* test_marker;
extern gedict_t* prev_marker;
extern float two_teams;
extern float first_team;
extern float second_team;
extern float new_team;
extern vec3_t to_marker_pos;
extern gedict_t* goal_entity;
extern float weapon_refresh_time_;
extern float weapon_;
extern float am;
extern float save;
extern float take;
extern float maxplayers;
extern gedict_t* touch_marker_;
extern float lookahead_time_;
extern float prediction_error_;
extern vec3_t origin_;
extern float impulse_;
extern float time_start;
extern float framecount_start;
extern float real_frametime;
extern float sv_accelerate;
extern float sv_maxfriction;
extern float sv_accelerate_frametime;
extern float sv_maxspeed;
extern float sv_maxwaterspeed;
extern float half_sv_maxspeed;
extern float inv_sv_maxspeed;
extern float sv_maxstrafespeed;
extern float sv_friction;
extern float sv_friction_frametime;
extern float inv_sv_friction_frametime;
extern float sv_friction_frametime_100;
extern float friction_factor;
extern float old_time;
extern float distance;
extern vec3_t hor_direction;
extern vec3_t dir_forward;
extern float current_maxspeed;
//extern float velocity_forward;
//extern float accel_forward;
//extern float max_accel_forward;
extern vec3_t desired_accel;
extern vec3_t hor_velocity;
extern vec3_t new_velocity;
extern vec3_t new_origin;
extern vec3_t last_clear_hor_velocity;
extern float last_clear_hor_speed;
extern vec3_t jump_velocity;
extern vec3_t jump_origin;
extern float oldflags;
extern float current_arrow;
extern float content;
extern float content1;
extern float content2;
extern float content3;
extern float fall;
extern float new_fall;
extern float current_fallspot;
extern vec3_t edge_normal;
extern vec3_t self_view;
extern vec3_t testplace;
extern float tries;
extern vec3_t last_clear_point;
extern vec3_t last_clear_velocity;
extern float jumpspeed;
//extern vec3_t dir_move;
//extern float path_score;
extern float total_goal_time;
extern vec3_t self_dir;
extern float look_score;
extern vec3_t dir_look;
/*
extern float best_goal_desire;
extern float best_goal_time;
extern float best_score2;
extern gedict_t* best_goal2;
extern float goal_desire;
extern float enemy_desire;
extern float enemy_repel;
*/
extern float best_goal_score;
extern float current_goal_time;
extern float current_goal_time_125;
extern float best_respawn_time;
extern gedict_t* goalentity_marker;
extern float goal_late_time;
extern gedict_t* linked_marker_;
extern vec3_t linked_marker_origin;
extern float goal_score;
extern float goal_score2;
extern float same_dir;
extern float avoid;
extern float enemy_score;
extern vec3_t rel_pos;
extern vec3_t rel_pos2;
extern vec3_t rel_dir;
extern float rel_dist;
extern float rel_time;
extern vec3_t rel_hor_dir;
extern float hor_component;
extern float min_angle_error;
extern gedict_t* enemy_touch_marker;
extern float new_health;
extern vec3_t src;
extern vec3_t direction;
extern float risk;
extern float hit_radius;
extern float risk_factor;
extern float risk_strength;
extern vec3_t rocket_origin;
extern vec3_t enemy_angles;
extern gedict_t* bot;
extern float rnd;
extern gedict_t* spawn_pos;
extern gedict_t* spots;
extern float pcount;
extern gedict_t* thing;
extern float numspots;
extern float totalspots;
extern vec3_t vec1;
extern vec3_t vec2;
extern vec3_t vec_;
extern gedict_t* think_ent;
extern gedict_t* trigger;
extern vec3_t cmins;
extern vec3_t cmaxs;
extern gedict_t* item;
extern vec3_t tmin;
extern vec3_t tmax;
extern float or;
extern float nr;
extern float best_weapon;
extern float score_count;
extern vec3_t item_pos;
extern vec3_t marker_pos;
extern gedict_t* marker_;
extern gedict_t* marker2;
extern float zone_time;
extern float real_yaw_;
extern gedict_t* fireball;
extern gedict_t* bubble;
extern gedict_t* bubble_spawner;
extern vec3_t org_;
extern float rnd1;
extern float rnd2;
extern float rnd3;
extern float character;
extern float block_;
extern float char_count;
extern float word_count;
extern float spawnflags_;
extern float digit;
extern float exponent;
extern float previous_exponent;
extern gedict_t* target_;
extern float traveltime;
extern float traveltime2;
extern float traveltime3;
extern float look_traveltime;
extern float look_traveltime_squared;
extern gedict_t* flag_self;
extern float flag_pos;
extern gedict_t* flag1;
extern gedict_t* flag2;
extern gedict_t* tfog;
extern vec3_t weapons_vel;
extern gedict_t* death;
extern gedict_t* trace_ent1;
extern gedict_t* trace_ent2;
extern gedict_t* old_self;
extern gedict_t* old_other;
extern float ledge_backup;
extern float try_jump_ledge;
extern float being_blocked;
extern float forward;
extern char* deathstring;
extern char* deathstring2;
extern gedict_t* removebot_self;
extern vec3_t start;
extern vec3_t end;
//extern int description;
//extern float goal_time;
//extern float path_time;
extern float component_speed;
extern float do_jump;
extern float dm;
extern float count_;
extern gedict_t* array_sub_object_;
extern float bind_char;
extern float dodge_factor;
extern gedict_t* dodge_missile;
extern gedict_t* killed_self;
extern gedict_t* rune_self;
extern gedict_t* door_enemy;
extern gedict_t* multi_ent;
#ifndef NUMBER_LOAD_STAGE
#define NUMBER_LOAD_STAGE 9
#endif
#ifndef NUMBERHARDBOTS
#define NUMBERHARDBOTS 16
#endif
//extern float intermission_running;
//extern float intermission_exittime;
extern gedict_t* bodyque_head;
extern float rocket_alert;
extern vec3_t rocket_endpos;
//extern char* nextmap;
extern float frogbot_number;
extern gedict_t* prephysics;
extern gedict_t* postphysics;
extern gedict_t* frogbot_spawned;
extern float use_ammo;
extern int available_weapons;
extern float check_parm_time;
#ifndef SPAWN_SIZE
#define SPAWN_SIZE 1
#endif
extern float unstick_time;
extern float no_bots_stuck;
extern float first_trace_fraction;
extern vec3_t first_trace_plane_normal;
extern float turning_speed;
extern vec3_t hor_normal_vec;
extern vec3_t last_clear_angle;
extern vec3_t velocity_hor_angle;
extern float beQuiet;
#ifndef PREF_FLASH
#define PREF_FLASH 1
#endif
extern float runaway_time;
extern float runaway_time_temp;
extern float runaway_score;
extern float runaway_score_temp;
extern gedict_t* runaway_dest;
extern float runaway_fail;
extern gedict_t* runaway_marker_temp;
extern gedict_t* runaway_prev_temp;
extern float min_traveltime;
extern gedict_t* to_zone;
extern float test_away_score;
extern float best_away_score;
extern gedict_t* best_away_marker;
extern gedict_t* search_entity;
extern float RA_time;
extern float enemy_time_squared;
float IsVisible(gedict_t* ent);
gedict_t* spawn_apply(void);
#ifndef IT_ROCKET_LIGHTNING
#define IT_ROCKET_LIGHTNING 96
#endif
#ifndef HELP_TEAMMATE
#define HELP_TEAMMATE 128
#endif
#ifndef CP_WPSTATS
#define CP_WPSTATS 1
#endif
extern float intermission_getting_closer;
extern float total_entity_count;
float ExistsPath(gedict_t* from_marker, gedict_t* to_marker);
float boomstick_only(void);

extern float ar_time;
#ifndef MSG_HIGH
#define MSG_HIGH 3
#endif
extern float lock;
extern float duel_mode;
extern float team_mode;
extern float a_rep;
extern char* string_null;
char* GetTeamName(float g);
char* ClientTeamName(gedict_t* p);
float ControlRPlayers(void);
float CountTeams(void);
float rndm(float fw, float fh);
char* gNum(float n);
float ent_fspeed(gedict_t* p);
/*qbool IsDanger(void);
float self_have_rl_or_lg(void);
float enemy_have_rl_or_lg(void);
float enemy_defenceless(void);*/
qbool EnemyDefenceless (gedict_t* self);

float enemy_shaft_attack(void);
extern float pingupdate;
#ifndef SVC_UPDATEPACKETLOSS
#define SVC_UPDATEPACKETLOSS 53
#endif
#ifndef SVC_SETANGLE
#define SVC_SETANGLE 10
#endif
extern gedict_t* team0;
extern gedict_t* team1;
extern gedict_t* team2;
extern gedict_t* team3;
extern gedict_t* team4;
extern gedict_t* updatebot;
extern float cvar_;
extern float print_;
extern float reset_;
extern char* varg;
extern float var_;
extern float min_;
extern float max_;
extern float idleprint_time;
extern float last_action_time;
extern float force_time;
extern float users;
extern float inter_print;
extern float loadbot_time;
extern float loadingbots;
extern float check_loadingbots;
extern float game_drop;
extern float game_damage;
extern float game_raspawn;
#ifndef GAME_DROP
#define GAME_DROP 524288
#endif
#ifndef GAME_ARENA
#define GAME_ARENA 1048576
#endif
#ifndef GAME_DAMAGE
#define GAME_DAMAGE 2097152
#endif
#ifndef GAME_RASPAWN
#define GAME_RASPAWN 4194304
#endif
#ifndef GAME_BOTJUMP
#define GAME_BOTJUMP 8388608
#endif
extern float teams;
extern float a_nextthink;
extern float initialized;
extern float updatestuff;
#ifndef exclamation
#define exclamation 33
#endif
#ifndef period
#define period 46
#endif
extern float a_sounds;
extern char* a_spawn;
float players_(void);
float putselfinserver(void);
float best_rl(void);
float best_gl(void);
float a_prethink(void);
float a_postthink(void);
float a_newitems(void);
float toss_player(void);
float scratch1(void);
float W_BestWeapon(void);
#ifndef IMP_DROP
#define IMP_DROP 71
#endif
#ifndef IMP_ARENA
#define IMP_ARENA 72
#endif
#ifndef IMP_DAMAGE
#define IMP_DAMAGE 73
#endif
#ifndef IMP_SETTINGS
#define IMP_SETTINGS 74
#endif
#ifndef IMP_DEFAULTS
#define IMP_DEFAULTS 75
#endif
#ifndef IMP_RESTART
#define IMP_RESTART 76
#endif
#ifndef IMP_FORCE
#define IMP_FORCE 77
#endif
#ifndef IMP_SOUNDS
#define IMP_SOUNDS 78
#endif
#ifndef IMP_ARMORTYPE
#define IMP_ARMORTYPE 79
#endif
#ifndef IMP_SET_DEATHMATCH
#define IMP_SET_DEATHMATCH 80
#endif
#ifndef IMP_SET_TEAMPLAY
#define IMP_SET_TEAMPLAY 81
#endif
#ifndef IMP_SET_TIMELIMIT
#define IMP_SET_TIMELIMIT 82
#endif
#ifndef IMP_SET_FRAGLIMIT
#define IMP_SET_FRAGLIMIT 83
#endif
#ifndef IMP_SET_SKILL
#define IMP_SET_SKILL 84
#endif
#ifndef IMP_SET_TEAMS
#define IMP_SET_TEAMS 85
#endif
#ifndef IMP_SET_ROUNDS
#define IMP_SET_ROUNDS 86
#endif
#ifndef IMP_SET_HEALTH
#define IMP_SET_HEALTH 87
#endif
#ifndef IMP_SET_ARMOR
#define IMP_SET_ARMOR 88
#endif
#ifndef IMP_SET_SHELLS
#define IMP_SET_SHELLS 89
#endif
#ifndef IMP_SET_NAILS
#define IMP_SET_NAILS 90
#endif
#ifndef IMP_SET_ROCKETS
#define IMP_SET_ROCKETS 91
#endif
#ifndef IMP_SET_CELLS
#define IMP_SET_CELLS 92
#endif
#ifndef IMP_SG
#define IMP_SG 93
#endif
#ifndef IMP_SSG
#define IMP_SSG 94
#endif
#ifndef IMP_NG
#define IMP_NG 95
#endif
#ifndef IMP_SNG
#define IMP_SNG 96
#endif
#ifndef IMP_GL
#define IMP_GL 97
#endif
#ifndef IMP_RL
#define IMP_RL 98
#endif
#ifndef IMP_LG
#define IMP_LG 99
#endif
#ifndef IMP_NOTREADY
#define IMP_NOTREADY 100
#endif
#ifndef IMP_OLDTEAM_BLUE
#define IMP_OLDTEAM_BLUE 101
#endif
#ifndef IMP_OLDTEAM_RED
#define IMP_OLDTEAM_RED 102
#endif
#ifndef IMP_OLDTEAM_YELLOW
#define IMP_OLDTEAM_YELLOW 103
#endif
#ifndef IMP_OLDTEAM_GREEN
#define IMP_OLDTEAM_GREEN 104
#endif
#ifndef IMP_RA
#define IMP_RA 105
#endif
#ifndef IMP_BOTJUMP
#define IMP_BOTJUMP 106
#endif
#ifndef IMP_ALIASES
#define IMP_ALIASES 107
#endif
#ifndef IMP_COMMANDS
#define IMP_COMMANDS 108
#endif
#ifndef IMP_BREAK
#define IMP_BREAK 109
#endif
#ifndef IMP_TIMEDOWN
#define IMP_TIMEDOWN 110
#endif
#ifndef IMP_TIMEUP
#define IMP_TIMEUP 111
#endif
#ifndef IMP_FRAGSDOWN
#define IMP_FRAGSDOWN 112
#endif
#ifndef IMP_FRAGSUP
#define IMP_FRAGSUP 113
#endif
#ifndef IMP_SKILLDOWN
#define IMP_SKILLDOWN 114
#endif
#ifndef IMP_SKILLUP
#define IMP_SKILLUP 115
#endif
#ifndef IMP_JOIN
#define IMP_JOIN 116
#endif
#ifndef IMP_OBSERVE
#define IMP_OBSERVE 117
#endif
#ifndef IMP_LOCK
#define IMP_LOCK 118
#endif
#ifndef IMP_ABOUT
#define IMP_ABOUT 119
#endif
#ifndef IMP_SCORES
#define IMP_SCORES 120
#endif
#ifndef IMP_CHDM
#define IMP_CHDM 121
#endif
#ifndef IMP_CHTP
#define IMP_CHTP 122
#endif
#ifndef IMP_SILENCE
#define IMP_SILENCE 124
#endif
#ifndef IMP_NO_LG
#define IMP_NO_LG 125
#endif
#ifndef IMP_WS_ON
#define IMP_WS_ON 126
#endif
#ifndef IMP_WS_OFF
#define IMP_WS_OFF 127
#endif
#ifndef IMP_WS_RES
#define IMP_WS_RES 128
#endif
#ifndef IMP_SH_SPEED
#define IMP_SH_SPEED 129
#endif
#ifndef IMP_NOCLIP
#define IMP_NOCLIP 131
#endif
#ifndef IMP_END4
#define IMP_END4 149
#endif
//float int(float a);
float oint(float a);
float imaginary(float a, float b);
float dig(float a, float b);
//float pow(float a, float b);
float anglemod2(float a);
float check_cvar(void);
float loadbot_time_(void);
float utilFloatToLong(float n);
float T_Damage2(void);
float a_item(void);
float TeamArmorDam(void);
gedict_t* LocateMarker(vec3_t org);
float RankForWeapon(float w);
float WeaponCode(float w);
#ifndef WEAPON_BIG2
#define WEAPON_BIG2 1
#endif
#ifndef WEAPON_SHOTGUN
#define WEAPON_SHOTGUN 1
#endif
#ifndef WEAPON_ROCKET
#define WEAPON_ROCKET 2
#endif
#ifndef WEAPON_SPIKES
#define WEAPON_SPIKES 4
#endif
#ifndef WEAPON_BIG
#define WEAPON_BIG 8
#endif
float G_is_connected(void);
//char* GetMapName(float f2);
float map_was_voted(float val);
float count_map_votes(float val);
char* rndffamap(void);
float RemoveBot(float remove_name);
float crandom(void);
gedict_t* RemoveFromList(gedict_t* first_in_list, gedict_t* ent);
vec_t* VelocityForDamage(void);
#ifndef DOOR_START_OPEN
#define DOOR_START_OPEN 1
#endif
#ifndef DOOR_DONT_LINK
#define DOOR_DONT_LINK 4
#endif
#ifndef DOOR_SILVER_GOLD_KEY
#define DOOR_SILVER_GOLD_KEY 24
#endif
#ifndef DOOR_TOGGLE
#define DOOR_TOGGLE 32
#endif
float door_target(void);
float EntitiesTouching(gedict_t* e1, gedict_t* e2);
float secret_target(void);
#ifndef SILENT
#define SILENT 2
#endif
#ifndef PUSH_ONCE
#define PUSH_ONCE 1
#endif
#ifndef PLAT_LOW_TRIGGER
#define PLAT_LOW_TRIGGER 1
#endif
#ifndef START_OFF
#define START_OFF 1
#endif
gedict_t* EntityAt(gedict_t* start_entity, float index_);
float near_teammate(gedict_t* me);

qbool able_rj(void);
float EntVisible(vec3_t vec);
float VisibleEntity(gedict_t* ent);
gedict_t* identify_teammate_(gedict_t* me);
float anglemod(float v);
float visible_teammate(gedict_t* me);
gedict_t* HelpTeammate(void);
gedict_t* check_wp_stats(gedict_t* pl);
float AddRandomBot(void);
float BeenSpawned(gedict_t* client, float value);

// 
char* SetNetName(void);
char* EnemyTeamName(void);
char* FriendTeamName(void);
char* SetTeamNetName(void);
float SpawnBot(void);

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

//
void spawn_marker(vec3_t org);

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

// route_lookup.qc
void SightMarker(gedict_t* from_marker);

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
void TestTopBlock (void); // same
void TestBottomBlock (void);

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
qbool JumpInWater(void);
qbool BotShouldDischarge(void);

// botphys.qc
void FrogbotPrePhysics1 (void);
void FrogbotPrePhysics2(void);
void FrogbotPostPhysics(void);
//void thud_touch(void);

// names.qc
void CopyBot(gedict_t* e, gedict_t* f);

// lists.qc
void SetColorName(float to, gedict_t* client);
//void Add_takedamage(gedict_t* ent);

// defs2.qc
void remove_self(void);

// bothazd.qc
void AvoidHazards(void);
void NewVelocityForArrow (gedict_t* self, vec3_t dir_move);

// route_lookup.qc
void HigherSightMarker(gedict_t* from_marker);

// botweap.qc
void UpdateWeapons(void);
void SelectWeapon(void);
void AttackRespawns(void);
void SetFireButton(gedict_t* self);

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
typedef void (*fb_spawn_func_t)(gedict_t* ent);

typedef struct fb_spawn_s {
	char* name;                 // classname
	fb_spawn_func_t func;       // initialisation function
} fb_spawn_t;

extern fb_spawn_t itemSpawnFunctions[];
int ItemSpawnFunctionCount(void);
qbool NoItemTouch (gedict_t* self, gedict_t* other);
qbool BotsPreTeleport (gedict_t* self, gedict_t* other);

#define FB_CVAR_GAMEMODE "k_fb_gamemode"
#define FB_CVAR_SKILL "k_fb_skill"

typedef struct fb_path_eval_s {
	gedict_t* touch_marker;
	gedict_t* test_marker;
	qbool rocket_alert;
	int description;
	float path_time;
	qbool path_normal;
} fb_path_eval_t;
