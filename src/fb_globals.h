// Converted from .qc on 05/02/2016
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FL_FLY
#define FL_FLY 1
#endif
#ifndef FL_SWIM
#define FL_SWIM 2
#endif
#ifndef FL_INWATER
#define FL_INWATER 16
#endif
#ifndef FL_ITEM
#define FL_ITEM 256
#endif
#ifndef FL_ONGROUND
#define FL_ONGROUND 512
#endif
#ifndef FL_PARTIALGROUND
#define FL_PARTIALGROUND 1024
#endif
#ifndef FL_WATERJUMP
#define FL_WATERJUMP 2048
#endif
#ifndef FL_JUMPRELEASED
#define FL_JUMPRELEASED 4096
#endif
#ifndef FL_BODYQUE
#define FL_BODYQUE 262144
#endif
#ifndef FL_ONGROUND_PARTIALGROUND
#define FL_ONGROUND_PARTIALGROUND 1536
#endif
#ifndef FL_NOT_JUMPRELEASED
#define FL_NOT_JUMPRELEASED 16773119
#endif
#ifndef FL_NOT_ONGROUND
#define FL_NOT_ONGROUND 16776703
#endif
extern float time1;
extern float e_break;
extern float counting_down;
//extern float match_in_progress;
extern float marker_time;
extern float next_marker_time;
extern float hazard_time;
extern float next_hazard_time;
#ifndef START_SKILL
#define START_SKILL 1
#endif
#ifndef START_SKIN
#define START_SKIN 2
#endif
#ifndef START_SHIRT
#define START_SHIRT 3
#endif
#ifndef START_PANTS
#define START_PANTS 4
#endif
#ifndef START_TEAMSHIRT
#define START_TEAMSHIRT 5
#endif
#ifndef START_TEAMPANTS
#define START_TEAMPANTS 6
#endif
#ifndef START_TEAMPLAY
#define START_TEAMPLAY 7
#endif
#ifndef START_DEATHMATCH
#define START_DEATHMATCH 8
#endif
#ifndef START_TIMELIMIT
#define START_TIMELIMIT 9
#endif
#ifndef START_FRAGLIMIT
#define START_FRAGLIMIT 10
#endif
#ifndef START_LINES
#define START_LINES 11
#endif
#ifndef START_MSG
#define START_MSG 18
#endif
#ifndef START_BOTS
#define START_BOTS 12
#endif
#ifndef START_ADMIN1
#define START_ADMIN1 15
#endif
#ifndef START_ADMIN2
#define START_ADMIN2 16
#endif
#ifndef START_ADMIN3
#define START_ADMIN3 17
#endif
#ifndef IMP_START1
#define IMP_START1 14
#endif
#ifndef IMP_SKILL
#define IMP_SKILL 14
#endif
#ifndef IMP_SKIN
#define IMP_SKIN 15
#endif
#ifndef IMP_SHIRT
#define IMP_SHIRT 16
#endif
#ifndef IMP_PANTS
#define IMP_PANTS 17
#endif
#ifndef IMP_TEAMSHIRT
#define IMP_TEAMSHIRT 18
#endif
#ifndef IMP_TEAMPANTS
#define IMP_TEAMPANTS 19
#endif
#ifndef IMP_END1
#define IMP_END1 20
#endif
#ifndef IMP_LINES
#define IMP_LINES 24
#endif
#ifndef IMP_ADDBOT
#define IMP_ADDBOT 25
#endif
#ifndef IMP_ADD2BOTS
#define IMP_ADD2BOTS 26
#endif
#ifndef IMP_ADD3BOTS
#define IMP_ADD3BOTS 27
#endif
#ifndef IMP_ADD4BOTS
#define IMP_ADD4BOTS 28
#endif
#ifndef IMP_ADDBOT0
#define IMP_ADDBOT0 29
#endif
#ifndef IMP_ADDBOT1
#define IMP_ADDBOT1 30
#endif
#ifndef IMP_ADDBOT2
#define IMP_ADDBOT2 31
#endif
#ifndef IMP_ADDBOT3
#define IMP_ADDBOT3 32
#endif
#ifndef IMP_ADDBOT4
#define IMP_ADDBOT4 33
#endif
#ifndef IMP_ADDBOT5
#define IMP_ADDBOT5 34
#endif
#ifndef IMP_ADDBOT6
#define IMP_ADDBOT6 35
#endif
#ifndef IMP_ADDBOT7
#define IMP_ADDBOT7 36
#endif
#ifndef IMP_ADDBOT8
#define IMP_ADDBOT8 37
#endif
#ifndef IMP_ADDBOT9
#define IMP_ADDBOT9 38
#endif
#ifndef IMP_ADDBOT10
#define IMP_ADDBOT10 39
#endif
#ifndef IMP_ADDBOT11
#define IMP_ADDBOT11 40
#endif
#ifndef IMP_ADDBOT12
#define IMP_ADDBOT12 41
#endif
#ifndef IMP_ADDBOT13
#define IMP_ADDBOT13 42
#endif
#ifndef IMP_END2
#define IMP_END2 43
#endif
#ifndef IMP_MSG
#define IMP_MSG 44
#endif
#ifndef IMP_BOTS
#define IMP_BOTS 45
#endif
#ifndef IMP_ADMIN
#define IMP_ADMIN 46
#endif
#ifndef IMP_TIME
#define IMP_TIME 47
#endif
#ifndef IMP_RULES
#define IMP_RULES 48
#endif
#ifndef IMP_SETSKIN
#define IMP_SETSKIN 49
#endif
#ifndef IMP_FLASH
#define IMP_FLASH 50
#endif
#ifndef IMP_HIDE
#define IMP_HIDE 51
#endif
#ifndef IMP_REMOVEBOT
#define IMP_REMOVEBOT 52
#endif
#ifndef IMP_REMOVEALLBOTS
#define IMP_REMOVEALLBOTS 53
#endif
#ifndef IMP_LAVACHEAT
#define IMP_LAVACHEAT 54
#endif
#ifndef IMP_FRAMERATE
#define IMP_FRAMERATE 55
#endif
#ifndef IMP_POWERUP
#define IMP_POWERUP 56
#endif
#ifndef IMP_RUNE
#define IMP_RUNE 57
#endif
#ifndef IMP_READY
#define IMP_READY 58
#endif
#ifndef IMP_MATCH
#define IMP_MATCH 59
#endif
#ifndef IMP_OVERTIME
#define IMP_OVERTIME 60
#endif
#ifndef IMP_REPORT
#define IMP_REPORT 61
#endif
#ifndef IMP_AUTOREPORT
#define IMP_AUTOREPORT 62
#endif
#ifndef IMP_DROPWEAP
#define IMP_DROPWEAP 63
#endif
#ifndef IMP_AUTOSTEAMS
#define IMP_AUTOSTEAMS 64
#endif
#ifndef IMP_BOTCHAT
#define IMP_BOTCHAT 65
#endif
#ifndef IMP_WHO
#define IMP_WHO 66
#endif
#ifndef IMP_RL_PREF
#define IMP_RL_PREF 67
#endif
#ifndef IMP_LG_PREF
#define IMP_LG_PREF 68
#endif
#ifndef IMP_RUNE_RJ
#define IMP_RUNE_RJ 69
#endif
#ifndef IMP_NOSKINS
#define IMP_NOSKINS 70
#endif
#ifndef IMP_POVDMM4_TRAINING
#define IMP_POVDMM4_TRAINING 143
#endif
#ifndef IMP_LG_MODE_TOGGLE
#define IMP_LG_MODE_TOGGLE 144
#endif
#ifndef IMP_DEBUG_MARKERS
#define IMP_DEBUG_MARKERS 145
#endif
#ifndef IMP_DEBUG_ZONES
#define IMP_DEBUG_ZONES 146
#endif
#ifndef IMP_HUMANDAMAGE_DOWN
#define IMP_HUMANDAMAGE_DOWN 147
#endif
#ifndef IMP_HUMANDAMAGE_UP
#define IMP_HUMANDAMAGE_UP 148
#endif
#ifndef IMP_HARDEST_JUMP
#define IMP_HARDEST_JUMP 149
#endif
#ifndef IMP_RANDOM
#define IMP_RANDOM 250
#endif
#ifndef IMP_IMPULSE_0
#define IMP_IMPULSE_0 255
#endif
#ifndef MOVETYPE_NONE
#define MOVETYPE_NONE 0
#endif
#ifndef MOVETYPE_WALK
#define MOVETYPE_WALK 3
#endif
#ifndef MOVETYPE_STEP
#define MOVETYPE_STEP 4
#endif
#ifndef MOVETYPE_FLY
#define MOVETYPE_FLY 5
#endif
#ifndef MOVETYPE_TOSS
#define MOVETYPE_TOSS 6
#endif
#ifndef MOVETYPE_PUSH
#define MOVETYPE_PUSH 7
#endif
#ifndef MOVETYPE_NOCLIP
#define MOVETYPE_NOCLIP 8
#endif
#ifndef MOVETYPE_FLYMISSILE
#define MOVETYPE_FLYMISSILE 9
#endif
#ifndef MOVETYPE_BOUNCE
#define MOVETYPE_BOUNCE 10
#endif
#ifndef MOVETYPE_BOUNCEMISSILE
#define MOVETYPE_BOUNCEMISSILE 11
#endif
#ifndef SOLID_NOT
#define SOLID_NOT 0
#endif
#ifndef SOLID_TRIGGER
#define SOLID_TRIGGER 1
#endif
#ifndef SOLID_BBOX
#define SOLID_BBOX 2
#endif
#ifndef SOLID_SLIDEBOX
#define SOLID_SLIDEBOX 3
#endif
#ifndef SOLID_BSP
#define SOLID_BSP 4
#endif
#ifndef DEAD_NO
#define DEAD_NO 0
#endif
#ifndef DEAD_DYING
#define DEAD_DYING 1
#endif
#ifndef DEAD_DEAD
#define DEAD_DEAD 2
#endif
#ifndef DEAD_RESPAWNABLE
#define DEAD_RESPAWNABLE 3
#endif
#ifndef DAMAGE_NO
#define DAMAGE_NO 0
#endif
#ifndef DAMAGE_YES
#define DAMAGE_YES 1
#endif
#ifndef DAMAGE_AIM
#define DAMAGE_AIM 2
#endif
#ifndef IT_AXE
#define IT_AXE 4096
#endif
#ifndef IT_SHOTGUN
#define IT_SHOTGUN 1
#endif
#ifndef IT_SUPER_SHOTGUN
#define IT_SUPER_SHOTGUN 2
#endif
#ifndef IT_NAILGUN
#define IT_NAILGUN 4
#endif
#ifndef IT_SUPER_NAILGUN
#define IT_SUPER_NAILGUN 8
#endif
#ifndef IT_GRENADE_LAUNCHER
#define IT_GRENADE_LAUNCHER 16
#endif
#ifndef IT_ROCKET_LAUNCHER
#define IT_ROCKET_LAUNCHER 32
#endif
#ifndef IT_LIGHTNING
#define IT_LIGHTNING 64
#endif
#ifndef IT_EITHER_NAILGUN
#define IT_EITHER_NAILGUN 12
#endif
#ifndef IT_NAILGUN_ROCKET
#define IT_NAILGUN_ROCKET 44
#endif
#ifndef IT_VELOCITY
#define IT_VELOCITY 60
#endif
#ifndef IT_CONTINUOUS
#define IT_CONTINUOUS 76
#endif
#ifndef IT_AXE_SHOTGUN
#define IT_AXE_SHOTGUN 4097
#endif
#ifndef IT_ALL_BUT_GRENADE
#define IT_ALL_BUT_GRENADE 4207
#endif
#ifndef IT_ALL
#define IT_ALL 4223
#endif
#ifndef IT_SHELLS
#define IT_SHELLS 256
#endif
#ifndef IT_NAILS
#define IT_NAILS 512
#endif
#ifndef IT_ROCKETS
#define IT_ROCKETS 1024
#endif
#ifndef IT_CELLS
#define IT_CELLS 2048
#endif
#ifndef IT_NOT_AMMO
#define IT_NOT_AMMO 16773375
#endif
#ifndef IT_ARMOR1
#define IT_ARMOR1 8192
#endif
#ifndef IT_ARMOR2
#define IT_ARMOR2 16384
#endif
#ifndef IT_ARMOR3
#define IT_ARMOR3 32768
#endif
#ifndef IT_ARMOR
#define IT_ARMOR 57344
#endif
#ifndef IT_SUPERHEALTH
#define IT_SUPERHEALTH 65536
#endif
#ifndef IT_NOT_ARMOR
#define IT_NOT_ARMOR 16719871
#endif
#ifndef IT_KEY1
#define IT_KEY1 131072
#endif
#ifndef IT_KEY2
#define IT_KEY2 262144
#endif
#ifndef IT_INVISIBILITY
#define IT_INVISIBILITY 524288
#endif
#ifndef IT_INVULNERABILITY
#define IT_INVULNERABILITY 1048576
#endif
#ifndef IT_SUIT
#define IT_SUIT 2097152
#endif
#ifndef IT_QUAD
#define IT_QUAD 4194304
#endif
#ifndef IT_INVULNERABILITY_QUAD
#define IT_INVULNERABILITY_QUAD 5242880
#endif
#ifndef IT_POWERUP
#define IT_POWERUP 7864320
#endif
#ifndef IT_ARMOR3_INVULNERABILITY
#define IT_ARMOR3_INVULNERABILITY 1081344
#endif
#ifndef IT_NOT_INVISIBILITY
#define IT_NOT_INVISIBILITY 16252927
#endif
#ifndef IT_NOT_INVULNERABILITY
#define IT_NOT_INVULNERABILITY 15728639
#endif
#ifndef IT_NOT_SUIT
#define IT_NOT_SUIT 14680063
#endif
#ifndef IT_NOT_QUAD
#define IT_NOT_QUAD 12582911
#endif
#ifndef IT_SIGIL1
#define IT_SIGIL1 268435456
#endif
#ifndef IT_SIGIL2
#define IT_SIGIL2 536870912
#endif
#ifndef IT_SIGIL3
#define IT_SIGIL3 1073741824
#endif
#ifndef IT_SIGIL4
#define IT_SIGIL4 2147483648
#endif
#ifndef ITEM_RUNE1_FLAG
#define ITEM_RUNE1_FLAG 1
#endif
#ifndef ITEM_RUNE2_FLAG
#define ITEM_RUNE2_FLAG 2
#endif
#ifndef ITEM_RUNE3_FLAG
#define ITEM_RUNE3_FLAG 4
#endif
#ifndef ITEM_RUNE4_FLAG
#define ITEM_RUNE4_FLAG 8
#endif
#ifndef ITEM_RUNE_MASK
#define ITEM_RUNE_MASK 15
#endif
#ifndef CONTENT_EMPTY
#define CONTENT_EMPTY (-1)
#endif
#ifndef CONTENT_SOLID
#define CONTENT_SOLID (-2)
#endif
#ifndef CONTENT_WATER
#define CONTENT_WATER (-3)
#endif
#ifndef CONTENT_SLIME
#define CONTENT_SLIME (-4)
#endif
#ifndef CONTENT_LAVA
#define CONTENT_LAVA (-5)
#endif
#ifndef CONTENT_SKY
#define CONTENT_SKY (-6)
#endif
#ifndef STATE_TOP
#define STATE_TOP 0
#endif
#ifndef STATE_BOTTOM
#define STATE_BOTTOM 1
#endif
#ifndef STATE_UP
#define STATE_UP 2
#endif
#ifndef STATE_DOWN
#define STATE_DOWN 3
#endif
#ifndef TE_BLOOD
#define TE_BLOOD 12
#endif
#ifndef TE_LIGHTNINGBLOOD
#define TE_LIGHTNINGBLOOD 13
#endif
#ifndef VEC_ORIGIN
#define VEC_ORIGIN ('0 0 0')
#endif
#ifndef VEC_HULL2_MIN
#define VEC_HULL2_MIN ('-32 -32 -24')
#endif
#ifndef VEC_HULL2_MAX
#define VEC_HULL2_MAX ('32 32 64')
#endif
#ifndef SVC_SETVIEWPORT
#define SVC_SETVIEWPORT 5
#endif
#ifndef MSG_PRINT
#define MSG_PRINT 8
#endif
#ifndef MSG_STUFFCMD
#define MSG_STUFFCMD 9
#endif
#ifndef MSG_UPDATENAME
#define MSG_UPDATENAME 13
#endif
#ifndef MSG_UPDATEFRAGS
#define MSG_UPDATEFRAGS 14
#endif
#ifndef MSG_UPDATECOLORS
#define MSG_UPDATECOLORS 17
#endif
#ifndef SVC_TEMPENTITY
#define SVC_TEMPENTITY 23
#endif
#ifndef SVC_CENTERPRINT
#define SVC_CENTERPRINT 26
#endif
#ifndef SVC_KILLEDMONSTER
#define SVC_KILLEDMONSTER 27
#endif
#ifndef SVC_FOUNDSECRET
#define SVC_FOUNDSECRET 28
#endif
#ifndef SVC_INTERMISSION
#define SVC_INTERMISSION 30
#endif
#ifndef SVC_CDTRACK
#define SVC_CDTRACK 32
#endif
#ifndef SVC_SMALLKICK
#define SVC_SMALLKICK 34
#endif
#ifndef SVC_BIGKICK
#define SVC_BIGKICK 35
#endif
#ifndef SVC_UPDATEPING
#define SVC_UPDATEPING 36
#endif
#ifndef SVC_UPDATETIME
#define SVC_UPDATETIME 37
#endif
#ifndef SVC_MUZZLEFLASH
#define SVC_MUZZLEFLASH 39
#endif
#ifndef SVC_UPDATEUSERINFO
#define SVC_UPDATEUSERINFO 40
#endif
#ifndef SVC_PLAYERINFO
#define SVC_PLAYERINFO 42
#endif
#ifndef SVC_SETINFO
#define SVC_SETINFO 51
#endif
#ifndef TE_SPIKE
#define TE_SPIKE 0
#endif
#ifndef TE_SUPERSPIKE
#define TE_SUPERSPIKE 1
#endif
#ifndef TE_GUNSHOT
#define TE_GUNSHOT 2
#endif
#ifndef TE_EXPLOSION
#define TE_EXPLOSION 3
#endif
#ifndef TE_LIGHTNING2
#define TE_LIGHTNING2 6
#endif
#ifndef TE_LAVASPLASH
#define TE_LAVASPLASH 10
#endif
#ifndef TE_TELEPORT
#define TE_TELEPORT 11
#endif
#ifndef CHAN_AUTO
#define CHAN_AUTO 0
#endif
#ifndef CHAN_WEAPON
#define CHAN_WEAPON 1
#endif
#ifndef CHAN_VOICE
#define CHAN_VOICE 2
#endif
#ifndef CHAN_ITEM
#define CHAN_ITEM 3
#endif
#ifndef CHAN_BODY
#define CHAN_BODY 4
#endif
#ifndef CHAN_NO_PHS_ADD
#define CHAN_NO_PHS_ADD 8
#endif
#ifndef ATTN_NONE
#define ATTN_NONE 0
#endif
#ifndef ATTN_NORM
#define ATTN_NORM 1
#endif
#ifndef ATTN_IDLE
#define ATTN_IDLE 2
#endif
#ifndef ATTN_STATIC
#define ATTN_STATIC 3
#endif
#ifndef EF_MUZZLEFLASH
#define EF_MUZZLEFLASH 2
#endif
#ifndef EF_DIMLIGHT
#define EF_DIMLIGHT 8
#endif
#ifndef NOT_EF_DIMLIGHT
#define NOT_EF_DIMLIGHT 16777207
#endif
#ifndef EF_BLUE
#define EF_BLUE 64
#endif
#ifndef EF_RED
#define EF_RED 128
#endif
#ifndef EF_DIMLIGHT_BLUE
#define EF_DIMLIGHT_BLUE 72
#endif
#ifndef EF_DIMLIGHT_RED
#define EF_DIMLIGHT_RED 136
#endif
#ifndef NOT_EF_BLUE
#define NOT_EF_BLUE 16777151
#endif
#ifndef NOT_EF_RED
#define NOT_EF_RED 16777087
#endif
#ifndef MSG_BROADCAST
#define MSG_BROADCAST 0
#endif
#ifndef MSG_ONE
#define MSG_ONE 1
#endif
#ifndef MSG_ALL
#define MSG_ALL 2
#endif
#ifndef PRINT_LOW
#define PRINT_LOW 0
#endif
#ifndef PRINT_MEDIUM
#define PRINT_MEDIUM 1
#endif
#ifndef PRINT_HIGH
#define PRINT_HIGH 2
#endif
#ifndef MSG_MULTICAST
#define MSG_MULTICAST 4
#endif
#ifndef MULTICAST_PHS
#define MULTICAST_PHS 1
#endif
#ifndef MULTICAST_PVS
#define MULTICAST_PVS 2
#endif
#ifndef MAX_SKILL
#define MAX_SKILL 20
#endif
#ifndef MAX_TEAMPLAY
#define MAX_TEAMPLAY 5
#endif
#ifndef MAX_DEATHMATCH
#define MAX_DEATHMATCH 6
#endif
#ifndef enter
#define enter 10
#endif
#ifndef space
#define space 32
#endif
#ifndef quote
#define quote 34
#endif
#ifndef minus
#define minus 45
#endif
#ifndef _0
#define _0 48
#endif
#ifndef _1
#define _1 49
#endif
#ifndef _2
#define _2 50
#endif
#ifndef _3
#define _3 51
#endif
#ifndef _4
#define _4 52
#endif
#ifndef _5
#define _5 53
#endif
#ifndef _6
#define _6 54
#endif
#ifndef _7
#define _7 55
#endif
#ifndef _8
#define _8 56
#endif
#ifndef _9
#define _9 57
#endif
#ifndef colon
#define colon 58
#endif
#ifndef _A
#define _A 65
#endif
#ifndef _B
#define _B 66
#endif
#ifndef _C
#define _C 67
#endif
#ifndef _D
#define _D 68
#endif
#ifndef _E
#define _E 69
#endif
#ifndef _F
#define _F 70
#endif
#ifndef _G
#define _G 71
#endif
#ifndef _H
#define _H 72
#endif
#ifndef _I
#define _I 73
#endif
#ifndef _J
#define _J 74
#endif
#ifndef _K
#define _K 75
#endif
#ifndef _L
#define _L 76
#endif
#ifndef _M
#define _M 77
#endif
#ifndef _N
#define _N 78
#endif
#ifndef _O
#define _O 79
#endif
#ifndef _P
#define _P 80
#endif
#ifndef _Q
#define _Q 81
#endif
#ifndef _R
#define _R 82
#endif
#ifndef _S
#define _S 83
#endif
#ifndef _T
#define _T 84
#endif
#ifndef _U
#define _U 85
#endif
#ifndef _V
#define _V 86
#endif
#ifndef _W
#define _W 87
#endif
#ifndef _X
#define _X 88
#endif
#ifndef _Y
#define _Y 89
#endif
#ifndef _Z
#define _Z 90
#endif
#ifndef _a
#define _a 97
#endif
#ifndef _b
#define _b 98
#endif
#ifndef _c
#define _c 99
#endif
#ifndef _d
#define _d 100
#endif
#ifndef _e
#define _e 101
#endif
#ifndef _f
#define _f 102
#endif
#ifndef _g
#define _g 103
#endif
#ifndef _h
#define _h 104
#endif
#ifndef _i
#define _i 105
#endif
#ifndef _j
#define _j 106
#endif
#ifndef _k
#define _k 107
#endif
#ifndef _l
#define _l 108
#endif
#ifndef _m
#define _m 109
#endif
#ifndef _n
#define _n 110
#endif
#ifndef _o
#define _o 111
#endif
#ifndef _p
#define _p 112
#endif
#ifndef _q
#define _q 113
#endif
#ifndef _r
#define _r 114
#endif
#ifndef _s
#define _s 115
#endif
#ifndef _t
#define _t 116
#endif
#ifndef _u
#define _u 117
#endif
#ifndef _v
#define _v 118
#endif
#ifndef _w
#define _w 119
#endif
#ifndef _x
#define _x 120
#endif
#ifndef _y
#define _y 121
#endif
#ifndef _z
#define _z 122
#endif
#ifndef tilde
#define tilde 126
#endif
#ifndef backspace
#define backspace 127
#endif
#ifndef CLIENTKILL
#define CLIENTKILL 11
#endif
extern float gameover;
extern gedict_t* activator;
extern gedict_t* attacker;
extern float old_skill;
extern float skill_;
extern float temp1;
extern char* str;
extern float next_teamplay;
extern float next_deathmatch;
extern float team_index;
#ifndef TEAM_TOTAL_HEALTH_PROTECT
#define TEAM_TOTAL_HEALTH_PROTECT 1
#endif
#ifndef TEAM_FRAG_PENALTY
#define TEAM_FRAG_PENALTY 2
#endif
#ifndef TEAM_HEALTH_PROTECT
#define TEAM_HEALTH_PROTECT 3
#endif
#ifndef TEAM_HEALTH_MASK
#define TEAM_HEALTH_MASK 3
#endif
#ifndef TEAM_ARMOR_PROTECT
#define TEAM_ARMOR_PROTECT 4
#endif
extern float modelindex_head;
extern gedict_t* runespawn;
extern float realteam_;
extern float current_team;
extern float current_frogbot;
extern float axhitme;
extern float axhitme_;
extern char* deathtype;
extern float healthplay;
extern float armorplay;
extern int gamemode;
extern int new_gamemode;
extern float game_disable_powerups;
extern float game_enable_runes;
extern float game_not_rune_rj;
extern float game_rl_pref;
extern float game_lg_pref;
extern float game_not_match;
extern float game_disable_autoreport;
extern float game_disable_dropweap;
extern float game_disable_autosteams;
extern float game_disable_botchat;
extern float game_lava_cheat;
extern float game_show_rules;
extern float game_hardestjump_disabled;
#ifndef game_fastnail
#define game_fastnail 0
#endif
extern float game_qizmo;
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
#ifndef GAME_RL_PREF
#define GAME_RL_PREF 128
#endif
#ifndef GAME_LAVA_CHEAT
#define GAME_LAVA_CHEAT 256
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
#ifndef GAME_ENABLE_BOTCHAT
#define GAME_ENABLE_BOTCHAT 4096
#endif
#ifndef GAME_HIDE_RULES
#define GAME_HIDE_RULES 8192
#endif
#ifndef GAME_LG_PREF
#define GAME_LG_PREF 524288
#endif
float random();
float vlen(vec3_t v);
float vectoyaw(vec3_t v);
gedict_t* spawn();
//gedict_t* find(gedict_t* start, char* fld, char* match);
char* precache_sound(char* s);
char* precache_model(char* s);
char* ftos(float f);
//float walkmove(float yaw, float dist);
//float droptofloor();
float rint(float v);
float oldfloor(float v);
//float floor(float a);
//float ceil(float v);
//float pointcontents(vec3_t v);
//float fabs(float f);
//float cvar(char* s);
gedict_t* nextent(gedict_t* e);
//vec3_t vectoangles(vec3_t v);
char* precache_model2(char* s);
char* precache_sound2(char* s);
//char* infokey(gedict_t* e, char* key);
//float stof(char* s);
extern float msg_level;
extern gedict_t* firstperson;
#ifndef PF_FIRSTPERSON
#define PF_FIRSTPERSON 1
#endif
extern float scoreboardsize;
//float CanDamage();
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
extern gedict_t* visible_object;
extern gedict_t* goalentity_;
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
extern gedict_t* self_sound;
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
extern float new_pitch;
extern float average_frametime;
extern float time_12;
extern gedict_t* current_waiting_bot;
extern gedict_t* first_client;
extern gedict_t* first_item;
extern gedict_t* first_ent;
extern gedict_t* first_teleport;
extern gedict_t* first_takedamage;
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
extern float pitchspeed_;
extern float yawspeed_;
extern float total_pitchspeed;
extern float total_yawspeed;
extern float real_pitch_;
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
#ifndef SAVED_T
#define SAVED_T 1
#endif
extern gedict_t* dm6_door;
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
extern float marker_index;
extern float item_marker_index;
extern float markers_loaded;
extern float no_change;
extern float goal_index;
extern gedict_t* m;
extern gedict_t* m_2;
extern vec3_t m_pos;
extern vec3_t m_P_pos;
extern gedict_t* current_load_position;
extern gedict_t* load_position;
extern float que_size;
extern gedict_t* m1;
extern gedict_t* m2;
extern gedict_t* m3;
extern gedict_t* m4;
extern gedict_t* m5;
extern gedict_t* m6;
extern gedict_t* m7;
extern gedict_t* m8;
extern gedict_t* m9;
extern gedict_t* m10;
extern gedict_t* m11;
extern gedict_t* m12;
extern gedict_t* m13;
extern gedict_t* m14;
extern gedict_t* m15;
extern gedict_t* m16;
extern gedict_t* m17;
extern gedict_t* m18;
extern gedict_t* m19;
extern gedict_t* m20;
extern gedict_t* m21;
extern gedict_t* m22;
extern gedict_t* m23;
extern gedict_t* m24;
extern gedict_t* m25;
extern gedict_t* m26;
extern gedict_t* m27;
extern gedict_t* m28;
extern gedict_t* m29;
extern gedict_t* m30;
extern gedict_t* m31;
extern gedict_t* m32;
extern gedict_t* m33;
extern gedict_t* m34;
extern gedict_t* m35;
extern gedict_t* m36;
extern gedict_t* m37;
extern gedict_t* m38;
extern gedict_t* m39;
extern gedict_t* m40;
extern gedict_t* m41;
extern gedict_t* m42;
extern gedict_t* m43;
extern gedict_t* m44;
extern gedict_t* m45;
extern gedict_t* m46;
extern gedict_t* m47;
extern gedict_t* m48;
extern gedict_t* m49;
extern gedict_t* m50;
extern gedict_t* m51;
extern gedict_t* m52;
extern gedict_t* m53;
extern gedict_t* m54;
extern gedict_t* m55;
extern gedict_t* m56;
extern gedict_t* m57;
extern gedict_t* m58;
extern gedict_t* m59;
extern gedict_t* m60;
extern gedict_t* m61;
extern gedict_t* m62;
extern gedict_t* m63;
extern gedict_t* m64;
extern gedict_t* m65;
extern gedict_t* m66;
extern gedict_t* m67;
extern gedict_t* m68;
extern gedict_t* m69;
extern gedict_t* m70;
extern gedict_t* m71;
extern gedict_t* m72;
extern gedict_t* m73;
extern gedict_t* m74;
extern gedict_t* m75;
extern gedict_t* m76;
extern gedict_t* m77;
extern gedict_t* m78;
extern gedict_t* m79;
extern gedict_t* m80;
extern gedict_t* m81;
extern gedict_t* m82;
extern gedict_t* m83;
extern gedict_t* m84;
extern gedict_t* m85;
extern gedict_t* m86;
extern gedict_t* m87;
extern gedict_t* m88;
extern gedict_t* m89;
extern gedict_t* m90;
extern gedict_t* m91;
extern gedict_t* m92;
extern gedict_t* m93;
extern gedict_t* m94;
extern gedict_t* m95;
extern gedict_t* m96;
extern gedict_t* m97;
extern gedict_t* m98;
extern gedict_t* m99;
extern gedict_t* m100;
extern gedict_t* m101;
extern gedict_t* m102;
extern gedict_t* m103;
extern gedict_t* m104;
extern gedict_t* m105;
extern gedict_t* m106;
extern gedict_t* m107;
extern gedict_t* m108;
extern gedict_t* m109;
extern gedict_t* m110;
extern gedict_t* m111;
extern gedict_t* m112;
extern gedict_t* m113;
extern gedict_t* m114;
extern gedict_t* m115;
extern gedict_t* m116;
extern gedict_t* m117;
extern gedict_t* m118;
extern gedict_t* m119;
extern gedict_t* m120;
extern gedict_t* m121;
extern gedict_t* m122;
extern gedict_t* m123;
extern gedict_t* m124;
extern gedict_t* m125;
extern gedict_t* m126;
extern gedict_t* m127;
extern gedict_t* m128;
extern gedict_t* m129;
extern gedict_t* m130;
extern gedict_t* m131;
extern gedict_t* m132;
extern gedict_t* m133;
extern gedict_t* m134;
extern gedict_t* m135;
extern gedict_t* m136;
extern gedict_t* m137;
extern gedict_t* m138;
extern gedict_t* m139;
extern gedict_t* m140;
extern gedict_t* m141;
extern gedict_t* m142;
extern gedict_t* m143;
extern gedict_t* m144;
extern gedict_t* m145;
extern gedict_t* m146;
extern gedict_t* m147;
extern gedict_t* m148;
extern gedict_t* m149;
extern gedict_t* m150;
extern gedict_t* m151;
extern gedict_t* m152;
extern gedict_t* m153;
extern gedict_t* m154;
extern gedict_t* m155;
extern gedict_t* m156;
extern gedict_t* m157;
extern gedict_t* m158;
extern gedict_t* m159;
extern gedict_t* m160;
extern gedict_t* m161;
extern gedict_t* m162;
extern gedict_t* m163;
extern gedict_t* m164;
extern gedict_t* m165;
extern gedict_t* m166;
extern gedict_t* m167;
extern gedict_t* m168;
extern gedict_t* m169;
extern gedict_t* m170;
extern gedict_t* m171;
extern gedict_t* m172;
extern gedict_t* m173;
extern gedict_t* m174;
extern gedict_t* m175;
extern gedict_t* m176;
extern gedict_t* m177;
extern gedict_t* m178;
extern gedict_t* m179;
extern gedict_t* m180;
extern gedict_t* m181;
extern gedict_t* m182;
extern gedict_t* m183;
extern gedict_t* m184;
extern gedict_t* m185;
extern gedict_t* m186;
extern gedict_t* m187;
extern gedict_t* m188;
extern gedict_t* m189;
extern gedict_t* m190;
extern gedict_t* m191;
extern gedict_t* m192;
extern gedict_t* m193;
extern gedict_t* m194;
extern gedict_t* m195;
extern gedict_t* m196;
extern gedict_t* m197;
extern gedict_t* m198;
extern gedict_t* m199;
extern gedict_t* m200;
extern gedict_t* m201;
extern gedict_t* m202;
extern gedict_t* m203;
extern gedict_t* m204;
extern gedict_t* m205;
extern gedict_t* m206;
extern gedict_t* m207;
extern gedict_t* m208;
extern gedict_t* m209;
extern gedict_t* m210;
extern gedict_t* m211;
extern gedict_t* m212;
extern gedict_t* m213;
extern gedict_t* m214;
extern gedict_t* m215;
extern gedict_t* m216;
extern gedict_t* m217;
extern gedict_t* m218;
extern gedict_t* m219;
extern gedict_t* m220;
extern gedict_t* m221;
extern gedict_t* m222;
extern gedict_t* m223;
extern gedict_t* m224;
extern gedict_t* m225;
extern gedict_t* m226;
extern gedict_t* m227;
extern gedict_t* m228;
extern gedict_t* m229;
extern gedict_t* m230;
extern gedict_t* m231;
extern gedict_t* m232;
extern gedict_t* m233;
extern gedict_t* m234;
extern gedict_t* m235;
extern gedict_t* m236;
extern gedict_t* m237;
extern gedict_t* m238;
extern gedict_t* m239;
extern gedict_t* m240;
extern gedict_t* m241;
extern gedict_t* m242;
extern gedict_t* m243;
extern gedict_t* m244;
extern gedict_t* m245;
extern gedict_t* m246;
extern gedict_t* m247;
extern gedict_t* m248;
extern gedict_t* m249;
extern gedict_t* m250;
extern gedict_t* m251;
extern gedict_t* m252;
extern gedict_t* m253;
extern gedict_t* m254;
extern gedict_t* m255;
extern gedict_t* m256;
extern gedict_t* m257;
extern gedict_t* m258;
extern gedict_t* m259;
extern gedict_t* m260;
extern gedict_t* m261;
extern gedict_t* m262;
extern gedict_t* m263;
extern gedict_t* m264;
extern gedict_t* m265;
extern gedict_t* m266;
extern gedict_t* m267;
extern gedict_t* m268;
extern gedict_t* m269;
extern gedict_t* m270;
extern gedict_t* m271;
extern gedict_t* m272;
extern gedict_t* m273;
extern gedict_t* m274;
extern gedict_t* m275;
extern gedict_t* m276;
extern gedict_t* m277;
extern gedict_t* m278;
extern gedict_t* m279;
extern gedict_t* m280;
extern gedict_t* m281;
extern gedict_t* m282;
extern gedict_t* m283;
extern gedict_t* m284;
extern gedict_t* m285;
extern gedict_t* m286;
extern gedict_t* m287;
extern gedict_t* m288;
extern gedict_t* m289;
extern gedict_t* m290;
extern gedict_t* m291;
extern gedict_t* m292;
extern gedict_t* m293;
extern gedict_t* m294;
extern gedict_t* m295;
extern gedict_t* m296;
extern gedict_t* m297;
extern gedict_t* m298;
extern gedict_t* m299;
extern gedict_t* m300;
extern float goal_number;
extern gedict_t* test_goal;
extern float same_zone;
extern float Z1_path_success;
extern float Z8_path_success;
extern gedict_t* zone_stack_head;

extern gedict_t* zone_head[NUMBER_ZONES];
extern gedict_t* zone_tail[NUMBER_ZONES];
extern int subzone_indexes[NUMBER_ZONES];

extern float S_index;
extern float path_normal;
extern float leave;
extern gedict_t* from_marker;
extern gedict_t* middle_marker;
extern gedict_t* next_marker;
extern gedict_t* to_marker;
extern gedict_t* look_marker;
extern gedict_t* test_marker;
extern gedict_t* prev_marker;
extern float two_teams;
extern float first_team;
extern float second_team;
extern float new_team;
extern vec3_t to_marker_pos;
extern gedict_t* goal_entity;
extern float goal_refresh_time_;
extern float weapon_refresh_time_;
extern float toughness_;
extern int items_;
extern float weapon_;
extern float am;
extern float attackbonus;
extern float firepower_;
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
//extern float timelimit;
//extern float fraglimit;
extern float sv_maxspeed;
extern float sv_maxwaterspeed;
extern float half_sv_maxspeed;
extern float inv_sv_maxspeed;
extern float sv_maxstrafespeed;
extern float quad_factor;
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
extern float velocity_forward;
extern float accel_forward;
extern float max_accel_forward;
extern vec3_t desired_accel;
extern int arrow_;
extern float yaw;
extern float dist;
extern vec3_t hor_velocity;
extern float hor_speed;
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
extern float normal_comp;
extern float best_dotproduct;
extern float best_arrow;
extern float test_forward;
extern float test_forward_left;
extern float test_forward_right;
extern float test_right;
extern float test_left;
extern float test_back;
extern float test_back_left;
extern float test_back_right;
extern vec3_t self_view;
extern vec3_t testplace;
extern vec3_t testplace_mins;
extern vec3_t testplace_maxs;
extern vec3_t test_enemy_mins;
extern vec3_t test_enemy_maxs;
extern float tries;
extern vec3_t last_clear_point;
extern vec3_t last_clear_velocity;
extern float jumpspeed;
extern vec3_t dir_move;
extern float path_score;
extern float total_goal_time;
extern gedict_t* goal_marker2;
extern vec3_t self_dir;
extern float best_score;
extern float look_score;
extern vec3_t dir_look;
extern gedict_t* best_goal;
extern gedict_t* best_goal_marker;
extern float best_goal_desire;
extern float best_goal_time;
extern float best_score2;
extern gedict_t* best_goal2;
extern float goal_desire;
extern float enemy_desire;
extern float enemy_repel;
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
extern float goal_time2;
extern float same_dir;
extern float avoid;
extern float enemy_score;
extern vec3_t rel_pos;
extern vec3_t rel_pos2;
extern vec3_t rel_dir;
extern float rel_dist;
extern float rel_time;
extern vec3_t rel_hor_dir;
extern vec3_t desired_angle;
extern float hor_component;
extern vec3_t angle_error;
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
extern float splash_damage;
extern float shaft_available;
extern vec3_t enemy_angles;
extern float avoid_rockets;
extern gedict_t* grenade_marker;
extern float desired_weapon;
extern gedict_t* bot;
extern float rnd;
extern float minutes;
extern float seconds;
extern float time_to_print;
extern gedict_t* spawn_pos;
extern float attacker_damaged;
extern float attacker_damage;
extern gedict_t* spots;
extern float pcount;
extern gedict_t* thing;
extern float numspots;
extern float totalspots;
extern gedict_t* t;
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
extern gedict_t* closest_marker;
extern float shortest_distance;
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
extern float min_first;
extern float min_second;
extern vec3_t start;
extern vec3_t end;
extern int description;
extern float goal_time;
extern float path_time;
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
extern gedict_t* fallspot_self;
extern gedict_t* door_enemy;
extern gedict_t* multi_ent;
extern float multi_damage;
extern vec3_t blood_org;
extern float blood_count;
extern vec3_t puff_org;
extern float puff_count;
extern float clientpants;
extern float clientshirt;
extern float load_frogbots;
extern float frogbot_removetime;
extern float invalid_map;
extern float frogbot_load_stage;
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
extern gedict_t* new_bot;
extern gedict_t* prephysics;
extern gedict_t* postphysics;
extern gedict_t* first_frogbot;
extern gedict_t* frogbot_spawned;
extern float use_ammo;
extern int available_weapons;
extern float pre_game;
extern float start_time;
extern float real_time;
extern float time_left;
extern float minutes_left;
extern int spawnbit0_;
extern int spawnbit1_;
extern float registered;
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
extern float predict_spot;
extern float beQuiet;
#ifndef PREF_FLASH
#define PREF_FLASH 1
#endif
extern float w_switch;
extern float b_switch;
extern float max_dead_time;
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
extern float humanDamageMultiplier;
extern float i;
extern float j;
extern float lg_mode;
#ifndef LG_FRACTIONAL
#define LG_FRACTIONAL 1
#endif
#ifndef LG_HALF_FRACTIONAL
#define LG_HALF_FRACTIONAL 2
#endif
#ifndef LG_DAMAGE_MOMENTUM
#define LG_DAMAGE_MOMENTUM 64
#endif
#ifndef LG_DAMAGE_ENHANCED
#define LG_DAMAGE_ENHANCED 128
#endif
#ifndef LG_DAMAGE_MODEL_FLAGS
#define LG_DAMAGE_MODEL_FLAGS 63
#endif
extern float povTraining;
#ifndef POV_NOTRAINING
#define POV_NOTRAINING 0
#endif
#ifndef POV_HIGHSPAWNS_STD
#define POV_HIGHSPAWNS_STD 1
#endif
#ifndef POV_HIGHSPAWNS_LEVEL1
#define POV_HIGHSPAWNS_LEVEL1 2
#endif
#ifndef POV_HIGHSPAWNS_LEVEL2
#define POV_HIGHSPAWNS_LEVEL2 3
#endif
#ifndef POV_HIGHSPAWNS_LEVEL3
#define POV_HIGHSPAWNS_LEVEL3 4
#endif
#ifndef POV_HIGHSPAWNS_LEVEL4
#define POV_HIGHSPAWNS_LEVEL4 5
#endif
float IsVisible(gedict_t* ent);
gedict_t* spawn_apply();
extern gedict_t* search_item;
extern gedict_t* q_gamer;
extern gedict_t* p_gamer;
extern gedict_t* i_gamer;
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
float ExistsPath();
float RocketSafe();
float BotShouldDischarge();
float WaterCombat();
float boomstick_only();
#ifndef TEAM_COLOR1
#define TEAM_COLOR1 5
#endif
#ifndef TEAM_COLOR2
#define TEAM_COLOR2 14
#endif
#ifndef TEAM_COLOR3
#define TEAM_COLOR3 1
#endif
#ifndef TEAM_COLOR4
#define TEAM_COLOR4 2
#endif
#ifndef TEAM_COLOR5
#define TEAM_COLOR5 3
#endif
#ifndef TEAM_COLOR6
#define TEAM_COLOR6 4
#endif
#ifndef TEAM_COLOR7
#define TEAM_COLOR7 6
#endif
#ifndef TEAM_COLOR8
#define TEAM_COLOR8 7
#endif
#ifndef TEAM_COLOR9
#define TEAM_COLOR9 8
#endif
#ifndef TEAM_COLOR10
#define TEAM_COLOR10 9
#endif
#ifndef TEAM_COLOR11
#define TEAM_COLOR11 10
#endif
#ifndef TEAM_COLOR12
#define TEAM_COLOR12 11
#endif
#ifndef TEAM_COLOR13
#define TEAM_COLOR13 12
#endif
#ifndef TEAM_COLOR14
#define TEAM_COLOR14 13
#endif
extern float last_say_q;
extern float last_say_p;
extern float last_say_i;
extern float game_arena;
//extern float k_scores1;
//extern float k_scores2;
extern float k_standby;
extern float load_delay_time;
extern float ar_time;
extern float sudden_death;
extern float break_match;
extern float timelimit_user;
extern float line_one;
extern float line_two;
extern float line_three;
extern float line_four;
#ifndef open_bracket
#define open_bracket 40
#endif
#ifndef close_bracket
#define close_bracket 41
#endif
#ifndef MSG_HIGH
#define MSG_HIGH 3
#endif
extern float overtime_begun;
extern float overtime_stats;
extern float k_vbreak;
extern float dist_sfl;
extern float lock;
extern float duel_mode;
extern float team_mode;
//extern float no_lg;
extern float k_shutdown;
extern float a_rep;
extern float dm_pline;
extern float t1_pline;
extern float t2_pline;
extern float door_open;
extern char* string_null;
char* GetTeamName(float g);
char* ClientTeamName(gedict_t* p);
float ControlRPlayers();
float CountTeams();
float rndm(float fw, float fh);
char* gNum(float n);
float ent_fspeed(gedict_t* p);
float IsDanger();
float self_have_rl_or_lg();
float enemy_have_rl_or_lg();
float enemy_defenceless();
float enemy_shaft_attack();
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
extern float game_botjump;
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
extern float round;
extern float rounds;
extern float a_health;
extern float a_armorvalue;
extern float a_armortype;
extern float a_ammo_shells;
extern float a_ammo_nails;
extern float a_ammo_rockets;
extern float a_ammo_cells;
extern float a_items;
extern float arenastate;
#ifndef A_IDLE
#define A_IDLE 0
#endif
#ifndef A_STARTING
#define A_STARTING 1
#endif
#ifndef A_PREROUND
#define A_PREROUND 2
#endif
#ifndef A_PLAYING
#define A_PLAYING 3
#endif
#ifndef A_POSTROUND
#define A_POSTROUND 4
#endif
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
#ifndef BIT_30
#define BIT_30 1073741824
#endif
#ifndef BIT_29
#define BIT_29 536870912
#endif
#ifndef BIT_28
#define BIT_28 268435456
#endif
#ifndef BIT_27
#define BIT_27 134217728
#endif
#ifndef BIT_26
#define BIT_26 67108864
#endif
#ifndef BIT_25
#define BIT_25 33554432
#endif
#ifndef BIT_24
#define BIT_24 16777216
#endif
#ifndef BIT_23
#define BIT_23 8388608
#endif
#ifndef BIT_22
#define BIT_22 4194304
#endif
#ifndef BIT_21
#define BIT_21 2097152
#endif
#ifndef BIT_20
#define BIT_20 1048576
#endif
#ifndef BIT_19
#define BIT_19 524288
#endif
#ifndef BIT_18
#define BIT_18 262144
#endif
#ifndef BIT_17
#define BIT_17 131072
#endif
#ifndef BIT_16
#define BIT_16 65536
#endif
#ifndef BIT_15
#define BIT_15 32768
#endif
#ifndef BIT_14
#define BIT_14 16384
#endif
#ifndef BIT_13
#define BIT_13 8192
#endif
#ifndef BIT_12
#define BIT_12 4096
#endif
#ifndef BIT_11
#define BIT_11 2048
#endif
#ifndef BIT_10
#define BIT_10 1024
#endif
#ifndef BIT_09
#define BIT_09 512
#endif
#ifndef BIT_08
#define BIT_08 256
#endif
#ifndef BIT_07
#define BIT_07 128
#endif
#ifndef BIT_06
#define BIT_06 64
#endif
#ifndef BIT_05
#define BIT_05 32
#endif
#ifndef BIT_04
#define BIT_04 16
#endif
#ifndef BIT_03
#define BIT_03 8
#endif
#ifndef BIT_02
#define BIT_02 4
#endif
#ifndef BIT_01
#define BIT_01 2
#endif
#ifndef BIT_00
#define BIT_00 1
#endif
float players_();
float putselfinserver();
float best_rl();
float best_gl();
float a_prethink();
float a_postthink();
float a_newitems();
float toss_player();
float scratch1();
float W_BestWeapon();
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
float check_cvar();
float loadbot_time_();
float utilFloatToLong(float n);
float T_Damage2();
float lowest_userid();
float a_item();
float TeamArmorDam();
gedict_t* LocateMarker(vec3_t org);
float NoItemTouch();
#ifndef H_ROTTEN
#define H_ROTTEN 1
#endif
#ifndef H_MEGA
#define H_MEGA 2
#endif
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
float G_is_connected();
//char* GetMapName(float f2);
float map_was_voted(float val);
float count_map_votes(float val);
char* rndffamap();
float RemoveBot(float remove_name);
float crandom();
gedict_t* RemoveFromList(gedict_t* first_in_list, gedict_t* ent);
vec_t* VelocityForDamage();
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
float door_target();
float EntitiesTouching(gedict_t* e1, gedict_t* e2);
#ifndef SECRET_OPEN_ONCE
#define SECRET_OPEN_ONCE 1
#endif
#ifndef SECRET_1ST_LEFT
#define SECRET_1ST_LEFT 2
#endif
#ifndef SECRET_1ST_DOWN
#define SECRET_1ST_DOWN 4
#endif
#ifndef SECRET_NO_SHOOT
#define SECRET_NO_SHOOT 8
#endif
#ifndef SECRET_YES_SHOOT
#define SECRET_YES_SHOOT 16
#endif
float secret_target();
#ifndef SPAWNFLAG_NOTOUCH
#define SPAWNFLAG_NOTOUCH 1
#endif
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
#ifndef SPAWNFLAG_SUPERSPIKE
#define SPAWNFLAG_SUPERSPIKE 1
#endif
#ifndef SPAWNFLAG_LASER
#define SPAWNFLAG_LASER 2
#endif
gedict_t* EntityAt(gedict_t* start_entity, float index_);
float near_teammate(gedict_t* me);
float right_direction();
float able_rj();
float a_evalpath();
float checkboven();
float checkground();
float JumpInWater();
float EntVisible(vec3_t vec);
float VisibleEntity(gedict_t* ent);
gedict_t* identify_teammate_(gedict_t* me);
float anglemod(float v);
float visible_teammate(gedict_t* me);
gedict_t* HelpTeammate();
float rndval();
float ShotForLuck(vec3_t object);
float BotCanReachMarker();
float BotSwimDown();
float BotSwimUp();
float BotGoUpForAir();
#ifndef fakenick_prefix
#define fakenick_prefix ("$\\")
#endif
gedict_t* check_wp_stats(gedict_t* pl);
float AddRandomBot();
float BotExists();
float BeenSpawned(gedict_t* client, float value);
char* SetNetName();
char* EnemyTeamName();
char* FriendTeamName();
char* SetTeamNetName();
float SpawnBot();

void Visible_360();
void NewVelocityForArrow();
void VelocityForArrow();
void Z_marker_error();
void S_time_error();
void S_path_marker_error();
void Z_path_marker_error();
void Z_next_zone_marker_error();
void Z_sight_from_marker_error();
void Z_higher_sight_from_marker_error();
void Z_sight_from_time_error();

//
void spawn_marker(vec3_t org);

// load.qc
void spawn_load();
void set_load();
void remove_load();

void InitialiseMarkerRoutes();

// route_fields.qc
void EnterZone(int zoneNumber, float teamNumber, float strong);
void LeaveZone(int zoneNumber, float teamNumber, float strong);

// items.qc
void SUB_regen();
void goal_health0();
void goal_health2();
void goal_armor1();
void goal_armor2();
void goal_armorInv();
void goal_supershotgun1();
void goal_supershotgun2();
void goal_nailgun1();
void goal_nailgun2();
void goal_supernailgun1();
void goal_supernailgun2();
void goal_grenadelauncher1();
void goal_grenadelauncher2();
void goal_rocketlauncher1();
void goal_rocketlauncher2();
void goal_lightning1();
void goal_lightning2();
void goal_shells();
void goal_spikes();
void goal_rockets();
void goal_cells();
void goal_artifact_invulnerability();
void goal_artifact_invisibility();
void goal_artifact_super_damage();

qbool pickup_health0();
qbool pickup_health2();
qbool pickup_armor1();
qbool pickup_armor2();
qbool pickup_armorInv();
qbool pickup_true();
qbool pickup_supershotgun2();
qbool pickup_nailgun2();
qbool pickup_supernailgun2();
qbool pickup_grenadelauncher2();
qbool pickup_rocketlauncher2();
qbool pickup_lightning2();
qbool pickup_shells();
qbool pickup_spikes();
qbool pickup_rockets();
qbool pickup_cells();

// marker_util.qc
void marker_touch();
void BecomeMarker(gedict_t* marker);

// route_calc.qc
void CheckWaterColumn(gedict_t* m, vec3_t m_pos, vec3_t testplace);

// route_lookup.qc
void SightMarker();

#define SubZoneFunction(x) \
	void S1_ ## x (); \
	void S2_ ## x (); \
	void S3_ ## x (); \
	void S4_ ## x (); \
	void S5_ ## x (); \
	void S6_ ## x (); \
	void S7_ ## x (); \
	void S8_ ## x (); \
	void S9_ ## x (); \
	void S10_ ## x (); \
	void S11_ ## x (); \
	void S12_ ## x (); \
	void S13_ ## x (); \
	void S14_ ## x (); \
	void S15_ ## x (); \
	void S16_ ## x (); \
	void S17_ ## x (); \
	void S18_ ## x (); \
	void S19_ ## x (); \
	void S20_ ## x (); \
	void S21_ ## x (); \
	void S22_ ## x (); \
	void S23_ ## x (); \
	void S24_ ## x (); \
	void S25_ ## x (); \
	void S26_ ## x (); \
	void S27_ ## x (); \
	void S28_ ## x (); \
	void S29_ ## x (); \
	void S30_ ## x (); \
	void S31_ ## x (); \
	void S32_ ## x ();

SubZoneFunction(time_)
SubZoneFunction(path_marker)

#define ZoneFunction(x) \
	void Z1_ ## x (); \
	void Z2_ ## x (); \
	void Z3_ ## x (); \
	void Z4_ ## x (); \
	void Z5_ ## x (); \
	void Z6_ ## x (); \
	void Z7_ ## x (); \
	void Z8_ ## x (); \
	void Z9_ ## x (); \
	void Z10_ ## x (); \
	void Z11_ ## x (); \
	void Z12_ ## x (); \
	void Z13_ ## x (); \
	void Z14_ ## x (); \
	void Z15_ ## x (); \
	void Z16_ ## x (); \
	void Z17_ ## x (); \
	void Z18_ ## x (); \
	void Z19_ ## x (); \
	void Z20_ ## x (); \
	void Z21_ ## x (); \
	void Z22_ ## x (); \
	void Z23_ ## x (); \
	void Z24_ ## x ();

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
void BestArrowForDirection();
void FallSpotAir();
void CanJumpOver(); // only used in bothazd?
void FallSpotGround(); // Same
void TestTopBlock(); // same
void PredictSpot(); // botthink

// weapons.qc
void GrenadeExplode();
void Missile_Remove();

// items.qc
void goal_NULL();

// client.qc
void NewItems();
void InitBodyQue();
void ClientConnect_apply();
void PlayerPreThink_apply();
void PlayerPostThink_apply();
void PlayerDeathThink();
void CheckLand();

// match.qc
void GameSettings();
void DoCountDown();
void NextLevel();

// botphys.qc
void FrogbotPrePhysics2();
void thud_touch();
void FrogbotPostPhysics();

// names.qc
void CopyBot(gedict_t* e, gedict_t* f);

// lists.qc
void SetColorName(float to, gedict_t* client);
void Add_takedamage(gedict_t* ent);

// defs2.qc
void remove_self();

// bothazd.qc
void AvoidHazards();

// route_lookup.qc
void HigherSightMarker();

// botweap.qc
void UpdateWeapons();
void SelectWeapon();
void AttackRespawns();
void SetFireButton();

// marker_util.qc
void set_marker(gedict_t* client, gedict_t* marker);
void Visible_infront();
void AssignVirtualGoal_apply(gedict_t* marker_);
void adjust_view_ofs_z();

// botenemy.qc
void ClearLookObject(gedict_t* client);
void LookEnemy();
void BestEnemy();

// botjump.qc
void CheckCombatJump();
void BotInLava();

// arena.qc (move to botweap.qc)
void a_attackfix();

// botgoal.qc
void UpdateGoal();

// botpath.qc
void frogbot_marker_touch();

// marker_load.qc
void N(float x, float y, float z);
void LSQ();

void SetZone(int zone, gedict_t* marker);

#define Z1(m) SetZone(1, m)
#define Z2(m) SetZone(2, m)
#define Z3(m) SetZone(3, m)
#define Z4(m) SetZone(4, m)
#define Z5(m) SetZone(5, m)
#define Z6(m) SetZone(6, m)
#define Z7(m) SetZone(7, m)
#define Z8(m) SetZone(8, m)
#define Z9(m) SetZone(9, m)
#define Z10(m) SetZone(10, m)
#define Z11(m) SetZone(11, m)
#define Z12(m) SetZone(12, m)
#define Z13(m) SetZone(13, m)
#define Z14(m) SetZone(14, m)
#define Z15(m) SetZone(15, m)
#define Z16(m) SetZone(16, m)
#define Z17(m) SetZone(17, m)
#define Z18(m) SetZone(18, m)
#define Z19(m) SetZone(19, m)
#define Z20(m) SetZone(20, m)
#define Z21(m) SetZone(21, m)
#define Z22(m) SetZone(22, m)
#define Z23(m) SetZone(23, m)
#define Z24(m) SetZone(24, m)

void AddToQue(gedict_t* ent);
void SetGoal(int goal, gedict_t* marker);

#define G1(m) SetGoal(1, m)
#define G2(m) SetGoal(2, m)
#define G3(m) SetGoal(3, m)
#define G4(m) SetGoal(4, m)
#define G5(m) SetGoal(5, m)
#define G6(m) SetGoal(6, m)
#define G7(m) SetGoal(7, m)
#define G8(m) SetGoal(8, m)
#define G9(m) SetGoal(9, m)
#define G10(m) SetGoal(10, m)
#define G11(m) SetGoal(11, m)
#define G12(m) SetGoal(12, m)
#define G13(m) SetGoal(13, m)
#define G14(m) SetGoal(14, m)
#define G15(m) SetGoal(15, m)
#define G16(m) SetGoal(16, m)
#define G17(m) SetGoal(17, m)
#define G18(m) SetGoal(18, m)
#define G19(m) SetGoal(19, m)
#define G20(m) SetGoal(20, m)
#define G21(m) SetGoal(21, m)
#define G22(m) SetGoal(22, m)
#define G23(m) SetGoal(23, m)
#define G24(m) SetGoal(24, m)

// added for ktx
qbool fb_lg_disabled();
float random();
void StartItems();

// maps
void LoadMap();
void map_aerowalk();
void map_amphi2();
void map_dm4();
void map_dm3();
void map_dm6();
void map_e1m2();
void map_frobodm2();
void map_pkeg1();
void map_povdmm4();
void map_spinev2();
void map_ukooldm2();
void map_ultrav();
void map_ztndm3();
void map_ztndm4();
void map_ztndm5();

