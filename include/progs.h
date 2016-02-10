/*
 *  QWProgs-DM
 *  Copyright (C) 2004  [sd] angel
 *
 *  This code is based on QuakeWorld DM mod code by Id Software, Inc.
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 *  $Id$
 */

#include "progdefs.h"

#define MAX_ROUTE_NODES		20 // max race checkpoints per race (including start and finish checkpoints)
#define MAX_ROUTES			20 // max race route per map

typedef struct shared_edict_s {
	void			*ptr; // this points to sv_edict_t but mod should NOT bother about that...
	entvars_t       v;	// C exported fields from progs
// other fields from progs come immediately after
} edict_t;

struct gedict_s;
typedef void (*th_die_func_t)();
typedef void (*th_pain_func_t)(struct gedict_s *, float);

// { SP
typedef void (*th_sp_func_t)();
// }

typedef enum
{
	ctNone = 0,
	ctPlayer,
	ctSpec
} clientType_t;

typedef enum
{
	wpNONE = 0,
	wpAXE,
	wpSG,
	wpSSG,
	wpNG,
	wpSNG,
	wpGL,
	wpRL,
	wpLG,
	wpMAX
} weaponName_t;

typedef struct wpType_s {
	int hits;			// hits with this weapon, for SG and SSG this is count of bullets
	int rhits;			// real hits for this weapon (direct + splash), used for RL and GL only
	int vhits;			// virtual hits for this weapon (direct + splash, do not care about pent and such), used for RL and GL only
	int attacks;		// all attacks with this weapon, for SG and SSG this is count of bullets

	int kills;			// kills with this weapon
	int deaths;			// deaths from this weapon
	int tkills;			// team kills with this weapon
	int ekills;			// killed enemys which contain this weapon in inventory
	int drops;			// number of packs dropped which contain this weapon
	int tooks;			// took this weapon and does't have this weapon before took (weapon from packs counted too)
	int ttooks;			// total tooked, even u alredy have this weapon

} wpType_t;

typedef enum
{
	itNONE = 0,
	itHEALTH_15,
	itHEALTH_25,
	itHEALTH_100,
	itGA,
	itYA,
	itRA,
	itQUAD,
	itPENT,
	itRING,
	itMAX
} itemName_t;

typedef struct itType_s {
	int tooks; // taken count
	float time; // total time u have some item

} itType_t;

// store player statistic here, like taken armors etc...
typedef struct player_stats_s {
	wpType_t wpn[wpMAX];
	itType_t itm[itMAX];

	float    dmg_t; // damage taken
	float    dmg_g; // damage given
	float    dmg_g_rl; // virtual given rl damage
	float    dmg_team;  // damage to team
	float    dmg_self;  // damage to own player
	float    dmg_eweapon;  // damage to enemy weapons
// { k_dmgfrags
	float    dmg_frags; // frags awarded from damage (CA)
// }

	int		ot_a;	 // overtime armor value
//	float	ot_at;   // overtime armor type
	int     ot_items;// overtime items
	int		ot_h;	 // overtime health

	int		spawn_frags;
	int		handicap;
	int		transferred_packs;

	// ctf stats
	int ctf_points; // use frags - this to calculate efficiency for ctf
	int caps;		// flag captures
	int f_defends;  // flag defends
	int c_defends;	// carrier defends
	int c_frags;	// frags on enemy carriers
	int pickups;	// flag pickups
	int returns;	// flag returns
	float res_time;	// time with resistance rune
	float str_time;	// time with strength rune
	float hst_time;	// time with haste rune
	float rgn_time;	// time with regen rune

	// midair stats
	int mid_stomps;	// stomps done on other players
	int mid_bronze;	// bronze midairs
	int mid_silver;	// silver midairs
	int mid_gold;	// gold midairs
	int mid_platinum;	// platinum midairs
	int mid_total;	// total of midairs
	int mid_bonus;	// total of bonuses
	float mid_totalheight; // maximum height of midairs
	float mid_maxheight; // maximum height of midairs
	float mid_avgheight;	// average height of midairs

	// spree stats
	int spree_current;		// number of frags since our last death
	int spree_current_q;	// number of frags in current quad run
	int spree_max;			// largest spree throughout game
	int spree_max_q;		// largest quad spree throughout game

	// time stats
	float control_time; // time spent in control

	// rocket arena
	int wins;	//number of wins they have
	int loses;	//number of loses they have

	// velocity stats
	float velocity_max;	// maximum velocity
	float velocity_sum;	// summary velocity from all frames
	int vel_frames;		// number of frames

	// instagib stats
	int i_height;	// Cumulated height  of airgibs
	int i_maxheight;	
	int i_cggibs;
	int i_axegibs;
	int i_stompgibs;
	int i_multigibs;
	int i_airgibs;		// Number of airgibs
	int i_maxmultigibs;
	int i_rings;

} player_stats_t;


typedef enum
{
	etNone = 0,
	etCaptain,
	etAdmin
} electType_t;

// store player votes here
typedef struct vote_s {

	int brk;
	int elect;
	int map;
	int pickup;
	int rpickup;
	int teamoverlay;
	int nospecs;
	int coop;
	int antilag;

	electType_t elect_type; // election type
	float elect_block_till;	// block election for this time
} vote_t;

// demo marker
typedef struct demo_marker_s {
	float time;
	char markername[64];
} demo_marker_t;

// cmd flood protection

#define MAX_FP_CMDS (10)

typedef struct fp_cmd_s {
	float locked;
	float cmd_time[MAX_FP_CMDS];
	int   last_cmd;
	int   warnings;
} fp_cmd_t;

// store wregs here

#define MAX_WREGS    (256)
#define MAX_WREG_IMP (9)

typedef struct wreg_s {
	qbool init;

	int attack;
	int impulse[MAX_WREG_IMP];
} wreg_t;

// {

#define MAX_PLRFRMS (77*15) /* 77 frames for each of 15 seconds */

typedef struct
{
	float	time;
//	float	modelindex;
	vec3_t	origin;
	vec3_t	angles;
	float	frame;
	float	effects;
//	vec3_t	v_angle;
	float	colormap;
} plrfrm_t;

// }

// player position

#define MAX_POSITIONS (5)

typedef struct pos_s {

	vec3_t velocity;
	vec3_t origin;
	vec3_t v_angle;

} pos_t;

typedef enum
{
	atNone = 0,
	atBest,
	atPow,
	atKTPRO
} autoTrackType_t;

// { rocket arena
typedef enum
{
	raNone = 0,
	raWinner,
	raLoser,
	raQue
} raPlayerType_t;
// }

#define MAX_SPAWN_WEIGHTS (64)

// 
typedef void (*fb_void_func_t)();
typedef qbool (*fb_bool_func_t)();

typedef struct fb_runaway_route_s {
	struct gedict_s* next_marker;
	struct gedict_s* prev_marker;
	float time;
	float score;
} fb_runaway_route_t;

typedef struct fb_path_s {
	struct gedict_s* next_marker;
	float time;
	int flags;
} fb_path_t;

typedef struct fb_goal_s {
	struct gedict_s* next_marker;
	float time;
} fb_goal_t;

typedef struct fb_entvars_s {
	char* wad;
	char* map;
	float worldtype;
	char* killtarget;
	float light_lev;
	float style;
	float count;
	vec3_t mangle;
	float speed;
	float wait;
	float delay;
	float dmg;
	float lip;
	float height;
	char* mdl;
	float client_;
	float player;
	float frogbot;
	float fl_thud;
	float fl_ontrain;
	float ready;
	float p_break;
	float teamflag;
	float oldwaterlevel;
	float oldwatertype;
	float realteam;
	float score_pos;
	float deaths;
	float efficiency;
	float healamount;
	float healtype;
	float ammo_shells;
	float ammo_nails;
	float ammo_rockets;
	float ammo_cells;
	float total_armor;
	func_t th_pain;
	func_t th_die;
	float walkframe;
	float attack_finished;
	float pain_finished;
	float invincible_finished;
	float invisible_finished;
	float super_damage_finished;
	float radsuit_finished;
	float invincible_time;
	float invincible_sound;
	float invisible_time;
	float invisible_sound;
	float super_time;
	float super_sound;
	float regeneration_sound;
	float haste_sound;
	float rad_time;
	float fly_sound;
	float jump_flag;
	float swim_flag;
	float air_finished;
	float bubble_count;
	int player_flag;
	float regen_time;
	float t_length;
	float t_width;
	vec3_t dest;
	char* noise4;
	float aflag;
	func_t think1;
	vec3_t finaldest;
	vec3_t finalangle;
	int state;
	int camp_state;
	vec3_t pos1;
	vec3_t pos2;
	struct gedict_s* clink;
	float pflags;
	float arrow;
	float wasinwater;
	float swim_arrow;
	float button0_;
	float button2_;
	float arrow_time;
	float arrow_time2;
	float enemy_time;
	float linked_marker_time;
	float touch_marker_time;
	float enemy_dist;
	float index;
	float print_framerate;
	vec3_t oldvelocity;
	vec3_t obstruction_normal;
	vec3_t velocity_normal;
	struct gedict_s* marker_link;           // linked list, this points to next marker
	float fl_marker;                        // 
	struct gedict_s* previous;
	struct gedict_s* next;
	struct gedict_s* next_load;
	float oldsolid;
	float real_pitch;
	float real_yaw;
	float pitchspeed;
	float yawspeed;
	float pitchaccel;
	float yawaccel;
	float track_pitchspeed;
	float track_yawspeed;
	float _highermarker;

	fb_path_t paths[8];
	int path_state;

	fb_runaway_route_t runaway[8];
	int T;
	struct gedict_s* near_teleport;
	struct gedict_s* linked_marker;
	struct gedict_s* old_linked_marker;
	struct gedict_s* look_object;
	//struct gedict_s* friend;
	float frogbot_nextthink;
	float fire_nextthink;

	fb_goal_t goals[24];

	float G_;
	struct gedict_s* virtual_goal;
	struct gedict_s* Z1_;
	struct gedict_s* Z2_;
	struct gedict_s* Z3_;
	struct gedict_s* Z4_;
	struct gedict_s* Z5_;
	struct gedict_s* Z6_;
	struct gedict_s* Z7_;
	struct gedict_s* Z8_;
	struct gedict_s* Z9_;
	struct gedict_s* Z10_;
	struct gedict_s* Z11_;
	struct gedict_s* Z12_;
	struct gedict_s* Z13_;
	struct gedict_s* Z14_;
	struct gedict_s* Z15_;
	struct gedict_s* Z16_;
	struct gedict_s* Z17_;
	struct gedict_s* Z18_;
	struct gedict_s* Z19_;
	struct gedict_s* Z20_;
	struct gedict_s* Z21_;
	struct gedict_s* Z22_;
	struct gedict_s* Z23_;
	struct gedict_s* Z24_;
	struct gedict_s* Z1_rev;
	struct gedict_s* Z2_rev;
	struct gedict_s* Z3_rev;
	struct gedict_s* Z4_rev;
	struct gedict_s* Z5_rev;
	struct gedict_s* Z6_rev;
	struct gedict_s* Z7_rev;
	struct gedict_s* Z8_rev;
	struct gedict_s* Z9_rev;
	struct gedict_s* Z10_rev;
	struct gedict_s* Z11_rev;
	struct gedict_s* Z12_rev;
	struct gedict_s* Z13_rev;
	struct gedict_s* Z14_rev;
	struct gedict_s* Z15_rev;
	struct gedict_s* Z16_rev;
	struct gedict_s* Z17_rev;
	struct gedict_s* Z18_rev;
	struct gedict_s* Z19_rev;
	struct gedict_s* Z20_rev;
	struct gedict_s* Z21_rev;
	struct gedict_s* Z22_rev;
	struct gedict_s* Z23_rev;
	struct gedict_s* Z24_rev;
	float Z1_time;
	float Z2_time;
	float Z3_time;
	float Z4_time;
	float Z5_time;
	float Z6_time;
	float Z7_time;
	float Z8_time;
	float Z9_time;
	float Z10_time;
	float Z11_time;
	float Z12_time;
	float Z13_time;
	float Z14_time;
	float Z15_time;
	float Z16_time;
	float Z17_time;
	float Z18_time;
	float Z19_time;
	float Z20_time;
	float Z21_time;
	float Z22_time;
	float Z23_time;
	float Z24_time;
	float Z1_time_rev;
	float Z2_time_rev;
	float Z3_time_rev;
	float Z4_time_rev;
	float Z5_time_rev;
	float Z6_time_rev;
	float Z7_time_rev;
	float Z8_time_rev;
	float Z9_time_rev;
	float Z10_time_rev;
	float Z11_time_rev;
	float Z12_time_rev;
	float Z13_time_rev;
	float Z14_time_rev;
	float Z15_time_rev;
	float Z16_time_rev;
	float Z17_time_rev;
	float Z18_time_rev;
	float Z19_time_rev;
	float Z20_time_rev;
	float Z21_time_rev;
	float Z22_time_rev;
	float Z23_time_rev;
	float Z24_time_rev;
	struct gedict_s* Z1_next_zone;
	struct gedict_s* Z2_next_zone;
	struct gedict_s* Z3_next_zone;
	struct gedict_s* Z4_next_zone;
	struct gedict_s* Z5_next_zone;
	struct gedict_s* Z6_next_zone;
	struct gedict_s* Z7_next_zone;
	struct gedict_s* Z8_next_zone;
	struct gedict_s* Z9_next_zone;
	struct gedict_s* Z10_next_zone;
	struct gedict_s* Z11_next_zone;
	struct gedict_s* Z12_next_zone;
	struct gedict_s* Z13_next_zone;
	struct gedict_s* Z14_next_zone;
	struct gedict_s* Z15_next_zone;
	struct gedict_s* Z16_next_zone;
	struct gedict_s* Z17_next_zone;
	struct gedict_s* Z18_next_zone;
	struct gedict_s* Z19_next_zone;
	struct gedict_s* Z20_next_zone;
	struct gedict_s* Z21_next_zone;
	struct gedict_s* Z22_next_zone;
	struct gedict_s* Z23_next_zone;
	struct gedict_s* Z24_next_zone;
	struct gedict_s* Z1_next;
	struct gedict_s* Z2_next;
	struct gedict_s* Z3_next;
	struct gedict_s* Z4_next;
	struct gedict_s* Z5_next;
	struct gedict_s* Z6_next;
	struct gedict_s* Z7_next;
	struct gedict_s* Z8_next;
	struct gedict_s* Z9_next;
	struct gedict_s* Z10_next;
	struct gedict_s* Z11_next;
	struct gedict_s* Z12_next;
	struct gedict_s* Z13_next;
	struct gedict_s* Z14_next;
	struct gedict_s* Z15_next;
	struct gedict_s* Z16_next;
	struct gedict_s* Z17_next;
	struct gedict_s* Z18_next;
	struct gedict_s* Z19_next;
	struct gedict_s* Z20_next;
	struct gedict_s* Z21_next;
	struct gedict_s* Z22_next;
	struct gedict_s* Z23_next;
	struct gedict_s* Z24_next;
	struct gedict_s* Z1_next_rev;
	struct gedict_s* Z2_next_rev;
	struct gedict_s* Z3_next_rev;
	struct gedict_s* Z4_next_rev;
	struct gedict_s* Z5_next_rev;
	struct gedict_s* Z6_next_rev;
	struct gedict_s* Z7_next_rev;
	struct gedict_s* Z8_next_rev;
	struct gedict_s* Z9_next_rev;
	struct gedict_s* Z10_next_rev;
	struct gedict_s* Z11_next_rev;
	struct gedict_s* Z12_next_rev;
	struct gedict_s* Z13_next_rev;
	struct gedict_s* Z14_next_rev;
	struct gedict_s* Z15_next_rev;
	struct gedict_s* Z16_next_rev;
	struct gedict_s* Z17_next_rev;
	struct gedict_s* Z18_next_rev;
	struct gedict_s* Z19_next_rev;
	struct gedict_s* Z20_next_rev;
	struct gedict_s* Z21_next_rev;
	struct gedict_s* Z22_next_rev;
	struct gedict_s* Z23_next_rev;
	struct gedict_s* Z24_next_rev;
	float Z1_from_time;
	float Z2_from_time;
	float Z3_from_time;
	float Z4_from_time;
	float Z5_from_time;
	float Z6_from_time;
	float Z7_from_time;
	float Z8_from_time;
	float Z9_from_time;
	float Z10_from_time;
	float Z11_from_time;
	float Z12_from_time;
	float Z13_from_time;
	float Z14_from_time;
	float Z15_from_time;
	float Z16_from_time;
	float Z17_from_time;
	float Z18_from_time;
	float Z19_from_time;
	float Z20_from_time;
	float Z21_from_time;
	float Z22_from_time;
	float Z23_from_time;
	float Z24_from_time;
	struct gedict_s* Z1_sight_from;
	struct gedict_s* Z2_sight_from;
	struct gedict_s* Z3_sight_from;
	struct gedict_s* Z4_sight_from;
	struct gedict_s* Z5_sight_from;
	struct gedict_s* Z6_sight_from;
	struct gedict_s* Z7_sight_from;
	struct gedict_s* Z8_sight_from;
	struct gedict_s* Z9_sight_from;
	struct gedict_s* Z10_sight_from;
	struct gedict_s* Z11_sight_from;
	struct gedict_s* Z12_sight_from;
	struct gedict_s* Z13_sight_from;
	struct gedict_s* Z14_sight_from;
	struct gedict_s* Z15_sight_from;
	struct gedict_s* Z16_sight_from;
	struct gedict_s* Z17_sight_from;
	struct gedict_s* Z18_sight_from;
	struct gedict_s* Z19_sight_from;
	struct gedict_s* Z20_sight_from;
	struct gedict_s* Z21_sight_from;
	struct gedict_s* Z22_sight_from;
	struct gedict_s* Z23_sight_from;
	struct gedict_s* Z24_sight_from;
	float Z1_sight_from_time_;
	float Z2_sight_from_time_;
	float Z3_sight_from_time_;
	float Z4_sight_from_time_;
	float Z5_sight_from_time_;
	float Z6_sight_from_time_;
	float Z7_sight_from_time_;
	float Z8_sight_from_time_;
	float Z9_sight_from_time_;
	float Z10_sight_from_time_;
	float Z11_sight_from_time_;
	float Z12_sight_from_time_;
	float Z13_sight_from_time_;
	float Z14_sight_from_time_;
	float Z15_sight_from_time_;
	float Z16_sight_from_time_;
	float Z17_sight_from_time_;
	float Z18_sight_from_time_;
	float Z19_sight_from_time_;
	float Z20_sight_from_time_;
	float Z21_sight_from_time_;
	float Z22_sight_from_time_;
	float Z23_sight_from_time_;
	float Z24_sight_from_time_;
	struct gedict_s* Z1_higher_sight_from;
	struct gedict_s* Z2_higher_sight_from;
	struct gedict_s* Z3_higher_sight_from;
	struct gedict_s* Z4_higher_sight_from;
	struct gedict_s* Z5_higher_sight_from;
	struct gedict_s* Z6_higher_sight_from;
	struct gedict_s* Z7_higher_sight_from;
	struct gedict_s* Z8_higher_sight_from;
	struct gedict_s* Z9_higher_sight_from;
	struct gedict_s* Z10_higher_sight_from;
	struct gedict_s* Z11_higher_sight_from;
	struct gedict_s* Z12_higher_sight_from;
	struct gedict_s* Z13_higher_sight_from;
	struct gedict_s* Z14_higher_sight_from;
	struct gedict_s* Z15_higher_sight_from;
	struct gedict_s* Z16_higher_sight_from;
	struct gedict_s* Z17_higher_sight_from;
	struct gedict_s* Z18_higher_sight_from;
	struct gedict_s* Z19_higher_sight_from;
	struct gedict_s* Z20_higher_sight_from;
	struct gedict_s* Z21_higher_sight_from;
	struct gedict_s* Z22_higher_sight_from;
	struct gedict_s* Z23_higher_sight_from;
	struct gedict_s* Z24_higher_sight_from;
	float Z1_higher_sight_from_time_;
	float Z2_higher_sight_from_time_;
	float Z3_higher_sight_from_time_;
	float Z4_higher_sight_from_time_;
	float Z5_higher_sight_from_time_;
	float Z6_higher_sight_from_time_;
	float Z7_higher_sight_from_time_;
	float Z8_higher_sight_from_time_;
	float Z9_higher_sight_from_time_;
	float Z10_higher_sight_from_time_;
	float Z11_higher_sight_from_time_;
	float Z12_higher_sight_from_time_;
	float Z13_higher_sight_from_time_;
	float Z14_higher_sight_from_time_;
	float Z15_higher_sight_from_time_;
	float Z16_higher_sight_from_time_;
	float Z17_higher_sight_from_time_;
	float Z18_higher_sight_from_time_;
	float Z19_higher_sight_from_time_;
	float Z20_higher_sight_from_time_;
	float Z21_higher_sight_from_time_;
	float Z22_higher_sight_from_time_;
	float Z23_higher_sight_from_time_;
	float Z24_higher_sight_from_time_;
	float Z1_height_equal_time;
	float Z8_height_equal_time;
	float Z_;
	struct gedict_s* zone_stack_next;
	struct gedict_s* Z_head;
	struct gedict_s* Z_next;
	float S1_time;
	float S2_time;
	float S3_time;
	float S4_time;
	float S5_time;
	float S6_time;
	float S7_time;
	float S8_time;
	float S9_time;
	float S10_time;
	float S11_time;
	float S12_time;
	float S13_time;
	float S14_time;
	float S15_time;
	float S16_time;
	float S17_time;
	float S18_time;
	float S19_time;
	float S20_time;
	float S21_time;
	float S22_time;
	float S23_time;
	float S24_time;
	float S25_time;
	float S26_time;
	float S27_time;
	float S28_time;
	float S29_time;
	float S30_time;
	float S31_time;
	float S32_time;
	struct gedict_s* S1_next;
	struct gedict_s* S2_next;
	struct gedict_s* S3_next;
	struct gedict_s* S4_next;
	struct gedict_s* S5_next;
	struct gedict_s* S6_next;
	struct gedict_s* S7_next;
	struct gedict_s* S8_next;
	struct gedict_s* S9_next;
	struct gedict_s* S10_next;
	struct gedict_s* S11_next;
	struct gedict_s* S12_next;
	struct gedict_s* S13_next;
	struct gedict_s* S14_next;
	struct gedict_s* S15_next;
	struct gedict_s* S16_next;
	struct gedict_s* S17_next;
	struct gedict_s* S18_next;
	struct gedict_s* S19_next;
	struct gedict_s* S20_next;
	struct gedict_s* S21_next;
	struct gedict_s* S22_next;
	struct gedict_s* S23_next;
	struct gedict_s* S24_next;
	struct gedict_s* S25_next;
	struct gedict_s* S26_next;
	struct gedict_s* S27_next;
	struct gedict_s* S28_next;
	struct gedict_s* S29_next;
	struct gedict_s* S30_next;
	struct gedict_s* S31_next;
	struct gedict_s* S32_next;
	float S_;
	int zoneTask[24];
	float path_normal_;
	fb_void_func_t zone_marker;
	fb_void_func_t sub_arrival_time;
	fb_void_func_t sub_path_marker;
	fb_void_func_t path_marker;
	fb_void_func_t next_zone_marker;
	fb_void_func_t sight_from_marker;
	fb_void_func_t higher_sight_from_marker;
	fb_void_func_t sight_from_time;
	fb_void_func_t desire;
	fb_bool_func_t pickup;
	float saved_goal_desire;
	float saved_respawn_time;
	float saved_goal_time;
	float saved_enemy_time_squared;
	float goal_respawn_time;
	float goal_refresh_time;
	float weapon_refresh_time;
	float total_damage;
	float firepower;
	float desire_armor1;
	float desire_armor2;
	float desire_armorInv;
	float desire_health0;
	float desire_health2;
	float desire_supershotgun;
	float desire_nailgun;
	float desire_supernailgun;
	float desire_grenadelauncher;
	float desire_rocketlauncher;
	float desire_lightning;
	float desire_rockets;
	float desire_cells;
	float desire_nails;
	float desire_shells;
	struct gedict_s* touch_marker;
	float touch_distance;
	int color_;
	float teamcolor;
	float bot_skill;
	float admin_code;
	float lines;
	float input_time;
	float fast_aim;
	float dodge_amount;
	float lookahead_time;
	float prediction_error;
	float look_anywhere;
	float stop_turn_speed;
	float accuracy;
	float firing_reflex;
	vec3_t virtual_mins;
	vec3_t virtual_maxs;
	vec3_t dir_move_;
	float ledge_backup_time;
	float hit_z;
	float number_bots;
	float dmgtime;
	float k_msgcount;
	float skin0;
	float skin1;
	float skin2;
	int spawnbit0;
	int spawnbit1;
	int number_spawnbits;
	struct gedict_s* movetarget;
	vec3_t v_forward_;
	vec3_t v_right_;
	struct gedict_s* touchPlayer;
	float touchPlayerTime;
	float tread_water_count;
	vec3_t predict_origin;
	float predict_success;
	float predict_shoot;
	float allowedMakeNoise;
	int preferences;
	float dead_time;
	float desire_backpack;
	float lastwepfired;
	float willRocketJumpThisTic;
	float team1_zone_players;
	float team1_zone_strong_players;
	float team2_zone_players;
	float team2_zone_strong_players;
	float team3_zone_players;
	float team3_zone_strong_players;
	float team4_zone_players;
	float team4_zone_strong_players;
	float team5_zone_players;
	float team5_zone_strong_players;
	float team6_zone_players;
	float team6_zone_strong_players;
	float team7_zone_players;
	float team7_zone_strong_players;
	float team8_zone_players;
	float team8_zone_strong_players;
	float team9_zone_players;
	float team9_zone_strong_players;
	float team10_zone_players;
	float team10_zone_strong_players;
	float team11_zone_players;
	float team11_zone_strong_players;
	float team12_zone_players;
	float team12_zone_strong_players;
	float team13_zone_players;
	float team13_zone_strong_players;
	float is_strong;
	float total_players;
	float kf_speed;
	float s_sg;
	float s_ssg;
	float s_lg;
	float h_sg;
	float h_ssg;
	float h_rl;
	float h_lg;
	float s_RA;
	float s_YA;
	float s_GA;
	float s_QUAD;
	float s_PENT;
	float s_RING;
	float s_dmg;
	float t_dmg;
	float k_vote;
	float k_flag;
	float t_health;
	float t_armortype;
	float t_armorvalue;
	float chat_time;
	float reporteditems;
	float friendly;
	float bot_evade;
	float help_teammate_time;
	float frogwatermove_time;
	float kick;
	float k_stuff;
	float k_accepted;
	float suicide_time;
	float spawn_frag_time;
	float s_frags;
	float cprint;
	float up_finished;
	float botnumber;
	float old_bot;
	char* stringname;
	float entertime;
	float ping;
	float ping_;
	float packetloss;
	float packetloss_;
	float itemstarted;
	float botchose;
	float aggressive;
	float invisible;
	float trackprint_time;
	float tracking;
	float printmotd;
	float connected;
	float scoreboard;
	float rocketjumping;
	float userid;
	float alias1time;
	float alias2time;
	float alias3time;
	float alias4time;
	float damfrags;
	float players;
	float wins;
	float teamnumber;
	float shirt1;
	float shirt2;
	float shirt3;
	float shirtcolor;
	float shirtcolor_;
	float pantscolor;
	float pantscolor_;
	float botpantscolor;
	float nextthink2;
	float gravity;
	float a_button0;
	float a_button2;
	vec3_t oldangles;
	struct gedict_s* chasing;
	func_t think2;
} fb_entvars_t;

//typedef (void(*)(gedict_t *)) one_edict_func;
typedef struct gedict_s {
	edict_t         s;

//custom fields

	int				isMissile; //treat some objects as missiles.

	float           vw_index;
	float           maxspeed, gravity;
	int             isBot;
	float 			brokenankle; // player can't jump for a while after falling and get damaged
	char           *mdl;
	char           *killtarget;
	int             worldtype;	// 0=medieval 1=metal 2=base
	char           *map;
//player
	int             walkframe;

//
// quakeed fields
//
	int             style;
	float           speed;
	int             state;
	int             voided;
//items.c
	float           healamount, healtype;
//ammo
	int             aflag;
//
// doors, etc
//
	vec3_t		movement;
	vec3_t          dest, dest1, dest2;
	vec3_t          pos1, pos2;
	vec3_t          mangle;
	float           t_length, t_width, height;
	float           wait;	// time from firing to restarting
	float           delay;	// time from activation to firing
	struct gedict_s *trigger_field;	// door's trigger entity
	struct gedict_s *movetarget;
	float           pausetime;
	char           *noise4;
	float           count;
//
// misc
//
	float           cnt;	// misc flag

	void            ( *think1 ) ();	//calcmove
	vec3_t          finaldest;
//combat
	float           dmg;
	float           lip;
	float           attack_finished;
	float           pain_finished;

	float           invincible_finished;
	float           invisible_finished;
	float           super_damage_finished;
	float           radsuit_finished;


	float           invincible_time, invincible_sound;
	float           invisible_time, invisible_sound;
	float           super_time, super_sound;
	float           rad_time;
	float           fly_sound;

	float           axhitme;
	float           show_hostile;	// set to time+0.2 whenever a client fires a
	// weapon or takes damage.  Used to alert
	// monsters that otherwise would let the player go
	float           jump_flag;	// player jump flag
	float           swim_flag;	// player swimming sound flag
	float           air_finished;	// when time > air_finished, start drowning
	float           bubble_count;	// keeps track of the number of bubbles
	int				deathtype;	// keeps track of how the player died
	float           dmgtime;

	th_die_func_t   th_die;
	th_pain_func_t  th_pain;

// below is KTX fields

// { SP

	//
	// monster ai
	//
	th_sp_func_t	th_stand;
	th_sp_func_t	th_walk;
	th_sp_func_t	th_run;
	th_sp_func_t	th_missile;
	th_sp_func_t	th_melee;

	th_sp_func_t	th_respawn; // for nightmare mode

	struct gedict_s *oldenemy;	// mad at this player before taking damage

	float	lefty;

	float	search_time;
	float	attack_state;

	float	monster_desired_spawn_time; // in nightmare mode monster desire respawn at this time after last death
	vec3_t  oldangles; // for nightmare skill, need remember monster angles before respawn again

// }

	float	cnt2;            // NOT_SURE: cnt2 = seconds left?
	float	dead_time;       // time at which player last died - used in autorespawn
	float	spawn_time;		 // time at which player last spawned,
							 // NOTE: ->spawn_time also used in spawn() function, so we know when edict was spawned
	float	connect_time;	 // time at which player connect
	float	deaths;          // number of times player died
	float	efficiency;      // stores player efficiency rating
	float	friendly;        // number of times player killed teammates
	float	ready;           // if a player is ready or not
	char	*killer;         // name of player who last killed player
	char	*victim;         // name of player last killed

	char    *backpack_player_name; // player who dropped the backpack
	char    backpack_team_name[32 /*MAX_TEAM_NAME*/]; // team associated with the backpack

	struct gedict_s *k_lastspawn;    // NOT_SURE: spot at which player last spawned?
// { kick mode
	struct gedict_s *k_playertokick; // player selected to be kicked
	float	k_kicking;       // if player is in kick mode
// }
	float	k_1spawn;        // NOT_SURE: used in kteams respawn code...
	float	k_accepted;      // NOT_SURE:
	int		k_added;         // NOT_SURE: stores the entered admin code?
	int		k_adminc;        // number of digits of admin code still to enter
	int 	k_admin;         // if player is an admin, flags
	float   k_adm_lasttime;	 // store time of last attempt getting admin rights
	int		k_captain;       // if player is team captain
	float	k_flag;          // flagvalue to customise settings such as sounds/autoscreenshot
	float	k_msgcount;      // NOT_SURE: last time mod printed a message to client?
	float	k_picked;        // NOT_SURE: 
	int		k_stuff;         // if player has received stuffed aliases|commands
// k_stuff flags
#define STUFF_MAPS     (1<<0)
#define STUFF_COMMANDS (1<<1)

	float	k_teamnumber;    // team the player is a member of, 1 = team1, 2 = team2
	float	k_teamnum;       // NOT_SURE:
	
	float lastwepfired;
	
//	float maxspeed;       // Used to set Maxspeed on a player moved from old qw defs.qc

	float suicide_time;	// can't suicide sooner than this

// ILLEGALFPS[
	// Zibbo's frametime checking code
	float real_time;
	float uptimebugpolicy;

	float fAverageFrameTime;
	float fFrameCount;
	float fDisplayIllegalFPS;
	float fCurrentFrameTime;
	float fLowestFrameTime;
	float fHighestFrameTime;
	float fIllegalFPSWarnings;
// ILLEGALFPS]

	float shownick_time;	// used to force centerprint is off at desired time
	clientType_t ct;		// client type for client edicts
// { timing
	float k_lastPostThink;  // last time PlayerPostThink or SpectatorThink was called
	float k_timingWarnTime; // time of last client is timing warning
	int   k_timingTakedmg;  // .... so we can restore
	int   k_timingSolid;    // .... so we can restore
	int   k_timingMovetype; // .... so we can restore
// }

// { stats
	int   wp_stats;			// show +wp_stats or not
	float wp_stats_time;    // used to force centerprint is off at desired time
	int   sc_stats;			// show +scores or not
	float sc_stats_time;    // used to force centerprint is off at desired time

	player_stats_t ps;		// store player statistic here, like taken armors etc...

	float control_start_time; // time when player gained control
	float q_pickup_time;	// time then u took quad
	float p_pickup_time;	// time then u took pent
	float r_pickup_time;	// time then u took ring
// }

// { mvd demo weapon stats
	int   wpstats_mask;     // bit mask, like ( 1 << wpSG | 1 << wpLG ), tell us which weapon stats need to be sent to demo at some point
// }

	int   need_clearCP;		// if this true, clear center print at certain cases

// { ghost stuff
	int   ghost_slot;		// now ktx put ghost in players scoreboards in one of free slots - store this slot
	float ghost_dt;         // ghost drop time - time when player dropped
	int   ghost_clr;		// color of dropped player
// }

	vote_t v;				// player votes stored here

// { spec stuff
	int   favx[MAX_CLIENTS];    // here stored players number for appropriate favX_add/Xfav_go commands
	int   fav[MAX_CLIENTS];     // here stored players number for fav_add/next_fav commands
	autoTrackType_t autotrack;  // is autotrack or auto_pow or ktpro autorack
	qbool apply_ktpro_autotrack; // if we use ktpro's autotrack, that a hint to apply pov switch
	struct gedict_s *autotrack_hint; // per spectator autotrack hint, this helps switch pov more precise
	struct gedict_s *wizard;    // for specs, link to the entity which represent wizard
	int   last_goal;            // just store here self->s.v.goal from last spec frame, so we can catch pov switch

// }

	pos_t	pos[MAX_POSITIONS];	// for player pos_save / pos_move
	float	pos_move_time;		// for player pos_save / pos_move

// { CTF
	struct gedict_s *hook;         // grapple
	qbool ctf_sound;            // sound stuff
	qbool on_hook;              // are we on the grapple?
	qbool hook_out;             // is the grapple in flight?
	int ctf_flag;                  // do we have a rune or flag?
	float ctf_freeze;              // removes jitterness from grapple 
	float regen_time;              // time to update health if regen rune
	float rune_sound_time;         // dont spam rune sounds (1 per second)
	float carrier_frag_time;       // used for carrier assists
	float return_flag_time;        // used for flag return assists
	float rune_notify_time;        // already have a rune spam prevention
	float carrier_hurt_time;       // time we last hurt enemy carrier
	float rune_pickup_time;        // time we picked up current rune
	char *last_rune;			   // name of last rune we send to client
	float	items2;				   // using  ZQ_ITEMS2 extension in mvdsv we can use per client sigils for runes

//
// TF -- well, we does not support TF but require it for loading TF map as CTF map.
//
	int team_no; // team number, as I got TF support it from 1 to 4. (up to 4 teams).
// }
//
	int i_agmr; 		// Instagib AirGib Master rune

	qbool was_jump;

// { wreg
	qbool wreg_attack;		   // client simulate +attack via "cmd wreg" feature
	wreg_t *wreg; // [256]
// }

// { communication commands
	struct gedict_s *s_last_to;    // last direct msg to
	struct gedict_s *s_last_from;  // last direct msf from
// }

	fp_cmd_t fp_c; // cmd flood protection
	fp_cmd_t fp_s; // say flood protection

	float tdeath_time;	// when player invoke spawn_tdeath(...), help reduce double telefrags

// { rocket arena
	float idletime;		// how long they can idle
	float lasttime;		// last time idle was checked
	float laststattime;	// time of last status update
	raPlayerType_t ra_pt; // ra player type
// }

// { Clan Arena
	qbool ca_alive;
// }

// {
	float pb_time;
	float pb_old_time;
	int pb_frame; // frame which currently must be or already played
	qbool is_playback;

	float rec_start_time; // time when record starts
	int rec_count; // count of recorded frames
	qbool is_recording;

	struct gedict_s *pb_ent; // enitity which used to show our model during playback

	plrfrm_t *plrfrms; // [MAX_PLRFRMS] for record trix
// }

// { for /cmd callalias <alias time>
	char *callalias;		// store alias name
	float callalias_time;	// time when we must activate that alias
// }

	char *f_checkbuf;	// for /cmd check f_xxx

	// Yawnmode variables
	vec3_t old_vel;								// store pre physicsthink velocity
	float  spawn_weights[MAX_SPAWN_WEIGHTS];	// spawn point weights used by "fair respawns"

	// yeah its lame, but better than checking setinfo each time.
	int ezquake_version;

// midair
	float		mid_top_height;
	float		mid_top_avgheight;
	float		mid_top_score;
	float		mid_top_kills;
	float		mid_top_stomps;
	float		mid_top_streak;
	float		mid_top_spawnfrag;
	float		mid_top_rl_efficiency;

// { race
	int 		race_id; 			// used by checkpoints, 
				 					// start checkpoint have id = 0,
				 					// intermediate checkpoints have it from 1 to xxx,
				 					// and end checkpoint have id xxx + 1
	float		race_volume; 		// how loud to play sound() when you tocuh this checkpoint
	int			race_effects; 		// apply this effects when checkpoint is touched
	int			race_RouteNodeType; // this is actually must be raceRouteNodeType_t 
									// but unwilling to move type definition out of race.c so using int

	int			race_ready; 		// is player ready for race
	int			race_chasecam; 		// does player want to follow other racers in line
	int			race_chasecam_freelook; // disable server forcing v_angle on client
	int			race_chasecam_view;	// does follow uses the chasecam or 1st person view
	int			race_afk;			// counter for afk starts
	qbool		racer;				// this player do race right now
	qbool		muted;				// this player produces no sound
	int			hideentity;			// links to entity to hide in eye chasecam

	// race_route_start entity fields
	char*       race_route_name;            // the name of the route
	char*       race_route_description;     // the description of the route
	int         race_route_timeout;         // the maximum time to complete the route
	int         race_route_weapon_mode;     // the weapon mode - see raceWeapoMode_t
	int         race_route_falsestart_mode; // the falsestart mode - see raceFalseStartMode_t
	float       race_route_start_yaw;       // the player's initial yaw angle
	float       race_route_start_pitch;     // the player's initial pitch angle
// }

	int			trackent;			// pseudo spectating for players.
// { bloodfest
	qbool		bloodfest_boss;		// is monsters is bloodfest boss?
// }

	qbool		dropitem;			// true if placed with "dropitem" command.
// { hoonymode
//	struct gedict_s *k_hoonyspawn; // hoonymode: on odd-number points, we switch the spawns
// }

// { frogbots
	fb_entvars_t fb;
// }
} gedict_t;

typedef enum
{
	raceWeaponUnknown = 0,	// this must never happens
	raceWeaponNo,			// weapon not allowed. NOTE: must be second in this enum!!!
	raceWeaponAllowed,		// weapon allowed
	raceWeapon2s,			// weapon allowed after 2s
	raceWeaponMAX

} raceWeapoMode_t;

typedef enum
{
	raceFalseStartUnknown = 0,	// this must never happens
	raceFalseStartNo,			// no false start, player is stuck until 'go'
	raceFalseStartYes,			// false start allowed, player can move anytime before 'go'
	raceFalseStartMAX

} raceFalseStartMode_t;

typedef enum
{
	nodeUnknown = 0,		// this node is fucked by lame coding, I mean this must never happens
	nodeStart,				// this node is start of race route
	nodeCheckPoint,			// this node is intermediate
	nodeEnd,				// this node is end of race route
	nodeMAX

} raceRouteNodeType_t;

typedef struct
{
	raceRouteNodeType_t		type;			// race route node type
	vec3_t					ang;			// this is only need for start node, so we can set player view angles
	vec3_t					org;			// node origin in 3D space

} raceRouteNode_t;

typedef struct
{
	char					name[128]; 				// NOTE: this will probably FUCK QVM!!!
	char					desc[128]; 				// NOTE: this will probably FUCK QVM!!!
	int						cnt;				   	// how much nodes we actually have, no more than MAX_ROUTE_NODES
	float					timeout;				// when timeout of race must occur
	raceWeapoMode_t			weapon;					// weapon mode
	raceFalseStartMode_t	falsestart;				// start mode
	raceRouteNode_t			node[MAX_ROUTE_NODES];	// nodes of this route, fixed array, yeah I'm lazy
} raceRoute_t;
 
typedef struct
{
	float					time;
	char					racername[64];
	char					demoname[64];
	float					distance;
	float					maxspeed;
	float					avgspeed;
	float					avgcount;
	char					date[64];
	raceWeapoMode_t			weaponmode;					// weapon mode
	raceFalseStartMode_t	startmode;				// start mode
} raceRecord_t;
 
typedef enum
{
       raceNone = 0,                // race is inactive
       raceCD,                                // race in count down state
       raceActive,                        // race is active
} raceStatus_t;
 
#define NUM_CHASECAMS	4 // number of camera views defined
#define NUM_BESTSCORES	10

typedef struct
{
	raceRecord_t records[NUM_BESTSCORES];	// array of best scores information
	raceRecord_t currentrace;	// curent score information

	int						cnt;				// how much we actually have of routes, no more than MAX_ROUTES
	int						active_route;		// which route active right now
	raceRoute_t				route[MAX_ROUTES];	// fixed array of routes

// { count down
	int						cd_cnt;				// 4 3 2 1 GO!
	float					cd_next_think;		//
// }

	int						timeout_setting;	// just how long timeout
	float					timeout;			// when timeout of race must occur
	float					start_time;			// when race was started

// {
	char					top_nick[64];		// NOTE: this will probably FUCK QVM!!!
	int						top_time;
// }

	int						next_race_time;		// used for centerprint, help us print it not each server frame but more rare, each 100ms or something

	qbool				warned;				// do we warned why we can't start race
	qbool				race_recording;		// is race being recorded?
	int						next_racer;			// this is queue of racers

	raceWeapoMode_t			weapon;				// weapon mode
	
	raceFalseStartMode_t	falsestart;			// start type

	raceStatus_t			status;				// race status

} race_t;

extern race_t			race; // whole race struct
