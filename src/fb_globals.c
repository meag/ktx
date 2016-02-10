// Converted from .qc on 05/02/2016
#include "g_local.h"
#include "fb_globals.h"

float time1 = 0;
float e_break = 0;
float counting_down = 0;
//float match_in_progress = 0;
float marker_time = 0;
float next_marker_time = 0;
float hazard_time = 0;
float next_hazard_time = 0;
float gameover = 0;
gedict_t* activator = 0;
gedict_t* attacker = 0;
float old_skill = 0;
float skill_ = 0;
float temp1 = 0;
char* str = 0;
float next_teamplay = 0;
float next_deathmatch = 0;
float team_index = 0;
float modelindex_head = 0;
gedict_t* runespawn = 0;
float realteam_ = 0;
float current_team = 0;
float current_frogbot = 0;
float axhitme = 0;
float axhitme_ = 0;
char* deathtype = 0;
float healthplay = 0;
float armorplay = 0;
int gamemode = 0;
int new_gamemode = 0;
float game_disable_powerups = 0;
float game_enable_runes = 0;
float game_not_rune_rj = 0;
float game_rl_pref = 0;
float game_lg_pref = 0;
float game_not_match = 0;
float game_disable_autoreport = 0;
float game_disable_dropweap = 0;
float game_disable_autosteams = 0;
float game_disable_botchat = 0;
float game_lava_cheat = 0;
float game_show_rules = 0;
float game_hardestjump_disabled = 0;
float game_qizmo = 0;
float msg_level = 0;
gedict_t* firstperson = 0;
float scoreboardsize = 0;
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
gedict_t* visible_object = 0;
gedict_t* goalentity_ = 0;
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
gedict_t* self_sound = 0;
float numberofbots = 0;
float numberofplayers = 0;
float numberofclients = 0;
vec3_t oldvelocity_ = { 0 };
vec3_t delta_velocity = { 0 };
float new_pitch = 0;
float average_frametime = 0;
float time_12 = 0;
gedict_t* current_waiting_bot = 0;
gedict_t* first_client = 0;
gedict_t* first_item = 0;
gedict_t* first_ent = 0;
gedict_t* first_teleport = 0;
gedict_t* first_takedamage = 0;
gedict_t* first_marker = 0;
float takedamage_exists = 0;
gedict_t* dropper = 0;
float fallheight = 0;
float mouse_friction = 0;
vec3_t pitch_tangent = { 0 };
vec3_t yaw_tangent = { 0 };
float mouse_vel = 0;
float pitchspeed_ = 0;
float yawspeed_ = 0;
float total_pitchspeed = 0;
float total_yawspeed = 0;
float real_pitch_ = 0;
gedict_t* m_P = 0;
gedict_t* m_zone = 0;
float P_time = 0;
int m_D = 0;
float new_path_state = 0;
float old_path_state = 0;
float RA_type = 0;
gedict_t* dm6_door = 0;
float nextthink_ = 0;
float try_shoot = 0;
float marker_index = 0;
float item_marker_index = 0;
float markers_loaded = 0;
float no_change = 0;
float goal_index = 0;
gedict_t* m = 0;
gedict_t* m_2 = 0;
vec3_t m_pos = { 0 };
vec3_t m_P_pos = { 0 };
gedict_t* current_load_position = 0;
gedict_t* load_position = 0;
float que_size = 0;
gedict_t* m1 = 0;
gedict_t* m2 = 0;
gedict_t* m3 = 0;
gedict_t* m4 = 0;
gedict_t* m5 = 0;
gedict_t* m6 = 0;
gedict_t* m7 = 0;
gedict_t* m8 = 0;
gedict_t* m9 = 0;
gedict_t* m10 = 0;
gedict_t* m11 = 0;
gedict_t* m12 = 0;
gedict_t* m13 = 0;
gedict_t* m14 = 0;
gedict_t* m15 = 0;
gedict_t* m16 = 0;
gedict_t* m17 = 0;
gedict_t* m18 = 0;
gedict_t* m19 = 0;
gedict_t* m20 = 0;
gedict_t* m21 = 0;
gedict_t* m22 = 0;
gedict_t* m23 = 0;
gedict_t* m24 = 0;
gedict_t* m25 = 0;
gedict_t* m26 = 0;
gedict_t* m27 = 0;
gedict_t* m28 = 0;
gedict_t* m29 = 0;
gedict_t* m30 = 0;
gedict_t* m31 = 0;
gedict_t* m32 = 0;
gedict_t* m33 = 0;
gedict_t* m34 = 0;
gedict_t* m35 = 0;
gedict_t* m36 = 0;
gedict_t* m37 = 0;
gedict_t* m38 = 0;
gedict_t* m39 = 0;
gedict_t* m40 = 0;
gedict_t* m41 = 0;
gedict_t* m42 = 0;
gedict_t* m43 = 0;
gedict_t* m44 = 0;
gedict_t* m45 = 0;
gedict_t* m46 = 0;
gedict_t* m47 = 0;
gedict_t* m48 = 0;
gedict_t* m49 = 0;
gedict_t* m50 = 0;
gedict_t* m51 = 0;
gedict_t* m52 = 0;
gedict_t* m53 = 0;
gedict_t* m54 = 0;
gedict_t* m55 = 0;
gedict_t* m56 = 0;
gedict_t* m57 = 0;
gedict_t* m58 = 0;
gedict_t* m59 = 0;
gedict_t* m60 = 0;
gedict_t* m61 = 0;
gedict_t* m62 = 0;
gedict_t* m63 = 0;
gedict_t* m64 = 0;
gedict_t* m65 = 0;
gedict_t* m66 = 0;
gedict_t* m67 = 0;
gedict_t* m68 = 0;
gedict_t* m69 = 0;
gedict_t* m70 = 0;
gedict_t* m71 = 0;
gedict_t* m72 = 0;
gedict_t* m73 = 0;
gedict_t* m74 = 0;
gedict_t* m75 = 0;
gedict_t* m76 = 0;
gedict_t* m77 = 0;
gedict_t* m78 = 0;
gedict_t* m79 = 0;
gedict_t* m80 = 0;
gedict_t* m81 = 0;
gedict_t* m82 = 0;
gedict_t* m83 = 0;
gedict_t* m84 = 0;
gedict_t* m85 = 0;
gedict_t* m86 = 0;
gedict_t* m87 = 0;
gedict_t* m88 = 0;
gedict_t* m89 = 0;
gedict_t* m90 = 0;
gedict_t* m91 = 0;
gedict_t* m92 = 0;
gedict_t* m93 = 0;
gedict_t* m94 = 0;
gedict_t* m95 = 0;
gedict_t* m96 = 0;
gedict_t* m97 = 0;
gedict_t* m98 = 0;
gedict_t* m99 = 0;
gedict_t* m100 = 0;
gedict_t* m101 = 0;
gedict_t* m102 = 0;
gedict_t* m103 = 0;
gedict_t* m104 = 0;
gedict_t* m105 = 0;
gedict_t* m106 = 0;
gedict_t* m107 = 0;
gedict_t* m108 = 0;
gedict_t* m109 = 0;
gedict_t* m110 = 0;
gedict_t* m111 = 0;
gedict_t* m112 = 0;
gedict_t* m113 = 0;
gedict_t* m114 = 0;
gedict_t* m115 = 0;
gedict_t* m116 = 0;
gedict_t* m117 = 0;
gedict_t* m118 = 0;
gedict_t* m119 = 0;
gedict_t* m120 = 0;
gedict_t* m121 = 0;
gedict_t* m122 = 0;
gedict_t* m123 = 0;
gedict_t* m124 = 0;
gedict_t* m125 = 0;
gedict_t* m126 = 0;
gedict_t* m127 = 0;
gedict_t* m128 = 0;
gedict_t* m129 = 0;
gedict_t* m130 = 0;
gedict_t* m131 = 0;
gedict_t* m132 = 0;
gedict_t* m133 = 0;
gedict_t* m134 = 0;
gedict_t* m135 = 0;
gedict_t* m136 = 0;
gedict_t* m137 = 0;
gedict_t* m138 = 0;
gedict_t* m139 = 0;
gedict_t* m140 = 0;
gedict_t* m141 = 0;
gedict_t* m142 = 0;
gedict_t* m143 = 0;
gedict_t* m144 = 0;
gedict_t* m145 = 0;
gedict_t* m146 = 0;
gedict_t* m147 = 0;
gedict_t* m148 = 0;
gedict_t* m149 = 0;
gedict_t* m150 = 0;
gedict_t* m151 = 0;
gedict_t* m152 = 0;
gedict_t* m153 = 0;
gedict_t* m154 = 0;
gedict_t* m155 = 0;
gedict_t* m156 = 0;
gedict_t* m157 = 0;
gedict_t* m158 = 0;
gedict_t* m159 = 0;
gedict_t* m160 = 0;
gedict_t* m161 = 0;
gedict_t* m162 = 0;
gedict_t* m163 = 0;
gedict_t* m164 = 0;
gedict_t* m165 = 0;
gedict_t* m166 = 0;
gedict_t* m167 = 0;
gedict_t* m168 = 0;
gedict_t* m169 = 0;
gedict_t* m170 = 0;
gedict_t* m171 = 0;
gedict_t* m172 = 0;
gedict_t* m173 = 0;
gedict_t* m174 = 0;
gedict_t* m175 = 0;
gedict_t* m176 = 0;
gedict_t* m177 = 0;
gedict_t* m178 = 0;
gedict_t* m179 = 0;
gedict_t* m180 = 0;
gedict_t* m181 = 0;
gedict_t* m182 = 0;
gedict_t* m183 = 0;
gedict_t* m184 = 0;
gedict_t* m185 = 0;
gedict_t* m186 = 0;
gedict_t* m187 = 0;
gedict_t* m188 = 0;
gedict_t* m189 = 0;
gedict_t* m190 = 0;
gedict_t* m191 = 0;
gedict_t* m192 = 0;
gedict_t* m193 = 0;
gedict_t* m194 = 0;
gedict_t* m195 = 0;
gedict_t* m196 = 0;
gedict_t* m197 = 0;
gedict_t* m198 = 0;
gedict_t* m199 = 0;
gedict_t* m200 = 0;
gedict_t* m201 = 0;
gedict_t* m202 = 0;
gedict_t* m203 = 0;
gedict_t* m204 = 0;
gedict_t* m205 = 0;
gedict_t* m206 = 0;
gedict_t* m207 = 0;
gedict_t* m208 = 0;
gedict_t* m209 = 0;
gedict_t* m210 = 0;
gedict_t* m211 = 0;
gedict_t* m212 = 0;
gedict_t* m213 = 0;
gedict_t* m214 = 0;
gedict_t* m215 = 0;
gedict_t* m216 = 0;
gedict_t* m217 = 0;
gedict_t* m218 = 0;
gedict_t* m219 = 0;
gedict_t* m220 = 0;
gedict_t* m221 = 0;
gedict_t* m222 = 0;
gedict_t* m223 = 0;
gedict_t* m224 = 0;
gedict_t* m225 = 0;
gedict_t* m226 = 0;
gedict_t* m227 = 0;
gedict_t* m228 = 0;
gedict_t* m229 = 0;
gedict_t* m230 = 0;
gedict_t* m231 = 0;
gedict_t* m232 = 0;
gedict_t* m233 = 0;
gedict_t* m234 = 0;
gedict_t* m235 = 0;
gedict_t* m236 = 0;
gedict_t* m237 = 0;
gedict_t* m238 = 0;
gedict_t* m239 = 0;
gedict_t* m240 = 0;
gedict_t* m241 = 0;
gedict_t* m242 = 0;
gedict_t* m243 = 0;
gedict_t* m244 = 0;
gedict_t* m245 = 0;
gedict_t* m246 = 0;
gedict_t* m247 = 0;
gedict_t* m248 = 0;
gedict_t* m249 = 0;
gedict_t* m250 = 0;
gedict_t* m251 = 0;
gedict_t* m252 = 0;
gedict_t* m253 = 0;
gedict_t* m254 = 0;
gedict_t* m255 = 0;
gedict_t* m256 = 0;
gedict_t* m257 = 0;
gedict_t* m258 = 0;
gedict_t* m259 = 0;
gedict_t* m260 = 0;
gedict_t* m261 = 0;
gedict_t* m262 = 0;
gedict_t* m263 = 0;
gedict_t* m264 = 0;
gedict_t* m265 = 0;
gedict_t* m266 = 0;
gedict_t* m267 = 0;
gedict_t* m268 = 0;
gedict_t* m269 = 0;
gedict_t* m270 = 0;
gedict_t* m271 = 0;
gedict_t* m272 = 0;
gedict_t* m273 = 0;
gedict_t* m274 = 0;
gedict_t* m275 = 0;
gedict_t* m276 = 0;
gedict_t* m277 = 0;
gedict_t* m278 = 0;
gedict_t* m279 = 0;
gedict_t* m280 = 0;
gedict_t* m281 = 0;
gedict_t* m282 = 0;
gedict_t* m283 = 0;
gedict_t* m284 = 0;
gedict_t* m285 = 0;
gedict_t* m286 = 0;
gedict_t* m287 = 0;
gedict_t* m288 = 0;
gedict_t* m289 = 0;
gedict_t* m290 = 0;
gedict_t* m291 = 0;
gedict_t* m292 = 0;
gedict_t* m293 = 0;
gedict_t* m294 = 0;
gedict_t* m295 = 0;
gedict_t* m296 = 0;
gedict_t* m297 = 0;
gedict_t* m298 = 0;
gedict_t* m299 = 0;
gedict_t* m300 = 0;
float goal_number = 0;
gedict_t* test_goal = 0;
float same_zone = 0;
float Z1_path_success = 0;
float Z8_path_success = 0;
gedict_t* zone_stack_head = 0;

gedict_t* zone_head[NUMBER_ZONES] = { 0 };
gedict_t* zone_tail[NUMBER_ZONES] = { 0 };
int subzone_indexes[NUMBER_ZONES];

float S_index = 0;
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
float goal_refresh_time_ = 0;
float weapon_refresh_time_ = 0;
float toughness_ = 0;
int items_ = 0;
float weapon_ = 0;
float am = 0;
float attackbonus = 0;
float firepower_ = 0;
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
float quad_factor = 0;
float sv_friction = 0;
float sv_friction_frametime = 0;
float inv_sv_friction_frametime = 0;
float sv_friction_frametime_100 = 0;
float friction_factor = 0;
float old_time = 0;
float distance = 0;
vec3_t hor_direction = { 0 };
vec3_t dir_forward = { 0 };
float current_maxspeed = 0;
float velocity_forward = 0;
float accel_forward = 0;
float max_accel_forward = 0;
vec3_t desired_accel = { 0 };
int arrow_ = 0;
float yaw = 0;
float dist = 0;
vec3_t hor_velocity = { 0 };
float hor_speed = 0;
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
float normal_comp = 0;
float best_dotproduct = 0;
float best_arrow = 0;
float test_forward = 0;
float test_forward_left = 0;
float test_forward_right = 0;
float test_right = 0;
float test_left = 0;
float test_back = 0;
float test_back_left = 0;
float test_back_right = 0;
vec3_t self_view = { 0 };
vec3_t testplace = { 0 };
vec3_t testplace_mins = { 0 };
vec3_t testplace_maxs = { 0 };
vec3_t test_enemy_mins = { 0 };
vec3_t test_enemy_maxs = { 0 };
float tries = 0;
vec3_t last_clear_point = { 0 };
vec3_t last_clear_velocity = { 0 };
float jumpspeed = 0;
vec3_t dir_move = { 0 };
float path_score = 0;
float total_goal_time = 0;
gedict_t* goal_marker2 = 0;
vec3_t self_dir = { 0 };
float best_score = 0;
float look_score = 0;
vec3_t dir_look = { 0 };
gedict_t* best_goal = 0;
gedict_t* best_goal_marker = 0;
float best_goal_desire = 0;
float best_goal_time = 0;
float best_score2 = 0;
gedict_t* best_goal2 = 0;
float goal_desire = 0;
float enemy_desire = 0;
float enemy_repel = 0;
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
float goal_time2 = 0;
float same_dir = 0;
float avoid = 0;
float enemy_score = 0;
vec3_t rel_pos = { 0 };
vec3_t rel_pos2 = { 0 };
vec3_t rel_dir = { 0 };
float rel_dist = 0;
float rel_time = 0;
vec3_t rel_hor_dir = { 0 };
vec3_t desired_angle = { 0 };
float hor_component = 0;
vec3_t angle_error = { 0 };
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
float splash_damage = 0;
float shaft_available = 0;
vec3_t enemy_angles = { 0 };
float avoid_rockets = 0;
gedict_t* grenade_marker = 0;
float desired_weapon = 0;
gedict_t* bot = 0;
float rnd = 0;
float minutes = 0;
float seconds = 0;
float time_to_print = 0;
gedict_t* spawn_pos = 0;
float attacker_damaged = 0;
float attacker_damage = 0;
gedict_t* spots = 0;
float pcount = 0;
gedict_t* thing = 0;
float numspots = 0;
float totalspots = 0;
gedict_t* t = 0;
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
gedict_t* closest_marker = 0;
float shortest_distance = 0;
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
float min_first = 0;
float min_second = 0;
vec3_t start = { 0 };
vec3_t end = { 0 };
int description = 0;
float goal_time = 0;
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
gedict_t* fallspot_self = 0;
gedict_t* door_enemy = 0;
gedict_t* multi_ent = 0;
float multi_damage = 0;
vec3_t blood_org = { 0 };
float blood_count = 0;
vec3_t puff_org = { 0 };
float puff_count = 0;
float clientpants = 0;
float clientshirt = 0;
float load_frogbots = 0;
float frogbot_removetime = 0;
float invalid_map = 0;
float frogbot_load_stage = 0;
//float intermission_running = 0;
//float intermission_exittime = 0;
gedict_t* bodyque_head = 0;
float rocket_alert = 0;
vec3_t rocket_endpos = { 0 };
//char* nextmap = 0;
float frogbot_number = 0;
gedict_t* new_bot = 0;
gedict_t* prephysics = 0;
gedict_t* postphysics = 0;
gedict_t* first_frogbot = 0;
gedict_t* frogbot_spawned = 0;
float use_ammo = 0;
int available_weapons = 0;                      // bitmask of weapons on the current map
float pre_game = 0;
float start_time = 0;
float real_time = 0;
float time_left = 0;
float minutes_left = 0;
int spawnbit0_ = 0;
int spawnbit1_ = 0;
float registered = 0;
float check_parm_time = 0;
float unstick_time = 0;
float no_bots_stuck = 0;
float first_trace_fraction = 0;
vec3_t first_trace_plane_normal = { 0 };
float turning_speed = 0;
vec3_t hor_normal_vec = { 0 };
vec3_t last_clear_angle = { 0 };
vec3_t velocity_hor_angle = { 0 };
float predict_spot = 0;
float beQuiet = 0;
float w_switch = 0;
float b_switch = 0;
float max_dead_time = 0;
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
float humanDamageMultiplier = 0;
float i = 0;
float j = 0;
float lg_mode = 0;
float povTraining = 0;
gedict_t* search_item = 0;
gedict_t* q_gamer = 0;
gedict_t* p_gamer = 0;
gedict_t* i_gamer = 0;
float intermission_getting_closer = 0;
float total_entity_count = 0;
float last_say_q = 0;
float last_say_p = 0;
float last_say_i = 0;
float game_arena = 0;
//float k_scores1 = 0;
//float k_scores2 = 0;
float k_standby = 0;
float load_delay_time = 0;
float ar_time = 0;
float sudden_death = 0;
float break_match = 0;
float timelimit_user = 0;
float line_one = 0;
float line_two = 0;
float line_three = 0;
float line_four = 0;
float overtime_begun = 0;
float overtime_stats = 0;
float k_vbreak = 0;
float dist_sfl = 0;
float lock = 0;
float duel_mode = 0;
float team_mode = 0;
//float no_lg = 0;
float k_shutdown = 0;
float a_rep = 0;
float dm_pline = 0;
float t1_pline = 0;
float t2_pline = 0;
float door_open = 0;
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
float game_botjump = 0;
float game_damage = 0;
float game_raspawn = 0;
float teams = 0;
float round = 0;
float rounds = 0;
float a_health = 0;
float a_armorvalue = 0;
float a_armortype = 0;
float a_ammo_shells = 0;
float a_ammo_nails = 0;
float a_ammo_rockets = 0;
float a_ammo_cells = 0;
float a_items = 0;
float arenastate = 0;
float a_nextthink = 0;
float initialized = 0;
float updatestuff = 0;
float a_sounds = 0;
char* a_spawn = 0;

qbool fb_lg_disabled()
{
	return (qbool) ((int)cvar("k_disallow_weapons") & IT_LIGHTNING) != 0;
}

float random()
{
	return g_random();
}

// from defs2.qc
float boomstick_only() {
	return (((int)self->s.v.items & IT_SHOTGUN) && !((int)self->s.v.items & (IT_NAILGUN | IT_SUPER_NAILGUN | IT_SUPER_SHOTGUN | IT_ROCKET_LAUNCHER | IT_GRENADE_LAUNCHER | IT_LIGHTNING)));
}

// taken from pr1 implementation
float rint(float f) {
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
	float n,
	      d,
	      g;
	if (f > 999 || f < -999) {
		bprint_ftos(lev, f);
		return;
	}
	f = floor(f);
	if (f < 0) {
		f = f * -1;
		bprint_fb(lev, "-");
	}
	n = rint(f);
	d = floor(n / 100);
	if (d) {
		bprint_ftos(lev, d);
	}
	n = n - d * 100;
	g = floor(n / 10);
	if (g || d) {
		bprint_ftos(lev, g);
	}
	n = n - g * 10;
	bprint_ftos(lev, n);
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
void GameSettings() {
	pre_game = FALSE;
	if (deathmatch != 4)
		use_ammo = TRUE;
}

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
	return ((enemy_->s.v.weapon == IT_LIGHTNING) && (self->fb.enemy_dist < 630) && (g_globalvars.time < enemy_->fb.attack_finished));
}

void PlayerPreThink_apply() {
	// remove
}

void PlayerPostThink_apply() {
	// remove
}

void a_attackfix() {
	// remove
}

void StartItems() {
	gedict_t* marker_;

	for (marker_ = first_item; marker_ && marker_ != world; marker_ = marker_->fb.next) {
		if (! marker_->s.v.touch) {
			marker_->s.v.touch = (func_t) marker_touch;
			marker_->s.v.nextthink = -1;
		}
	}
}

// defs.qc
char* GetTeamName(float g) {
	char buffer[1024] = { 0 };

	if (g == TEAM_COLOR1) {
		return infokey(world, "team4", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR2) {
		return infokey(world, "team13", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR3) {
		return infokey(world, "team0", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR4) {
		return infokey(world, "team1", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR5) {
		return infokey(world, "team2", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR6) {
		return infokey(world, "team3", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR7) {
		return infokey(world, "team5", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR8) {
		return infokey(world, "team6", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR9) {
		return infokey(world, "team7", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR10) {
		return infokey(world, "team8", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR11) {
		return infokey(world, "team9", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR12) {
		return infokey(world, "team10", buffer, sizeof(buffer));
	}
	else if (g == TEAM_COLOR13) {
		return infokey(world, "team11", buffer, sizeof(buffer));
	}
	else  {
		return infokey(world, "team12", buffer, sizeof(buffer));
	}
}

qbool bots_enabled() {
	return true;	// FIXME: make a variable
}

