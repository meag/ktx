
#ifndef FB_EXTERNS_H
#define FB_EXTERNS_H

extern fb_spawn_t itemSpawnFunctions[];
extern float predict_dist;

extern gedict_t* enemy_;
extern gedict_t* virtual_enemy;
extern gedict_t* look_object_;
extern gedict_t* test_enemy;

extern vec3_t oldvelocity_;
extern gedict_t* current_waiting_bot;
extern gedict_t* first_ent;
extern gedict_t* first_marker;

extern gedict_t* dropper;
extern float fallheight;
extern gedict_t* m_P;
extern float P_time;
extern int m_D;
extern int new_path_state;
extern float nextthink_;
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

extern qbool path_normal;
extern gedict_t* from_marker;
extern gedict_t* middle_marker;
extern gedict_t* next_marker;
extern gedict_t* to_marker;
extern gedict_t* look_marker;
//extern gedict_t* test_marker;
extern gedict_t* prev_marker;
extern vec3_t to_marker_pos;
extern gedict_t* goal_entity;
extern float weapon_refresh_time_;
extern float lookahead_time_;
extern vec3_t origin_;
extern float impulse_;
extern float time_start;
extern float framecount_start;
extern float sv_accelerate;
extern float sv_maxfriction;
extern float sv_accelerate_frametime;
extern float sv_maxspeed;
extern float sv_maxwaterspeed;
extern float half_sv_maxspeed;
extern float inv_sv_maxspeed;
extern float sv_maxstrafespeed;
extern float friction_factor;
extern float old_time;
extern float distance;
extern vec3_t dir_forward;
extern float current_maxspeed;
extern vec3_t desired_accel;
extern vec3_t hor_velocity;
extern vec3_t new_velocity;
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
extern vec3_t last_clear_velocity;
extern float jumpspeed;
extern float look_score;
extern float best_goal_score;
extern float current_goal_time;
extern float current_goal_time_125;
extern float best_respawn_time;
//extern gedict_t* linked_marker_;
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
extern vec3_t src;
extern vec3_t direction;
extern float risk;
extern float risk_factor;
extern float risk_strength;
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
extern float being_blocked;
extern float forward;
extern vec3_t start;
extern vec3_t end;
extern float component_speed;
extern float dm;
extern float count_;
extern gedict_t* array_sub_object_;
extern float bind_char;
extern float dodge_factor;
extern gedict_t* dodge_missile;

extern gedict_t* bodyque_head;
extern float first_trace_fraction;
extern vec3_t first_trace_plane_normal;
extern float turning_speed;
extern vec3_t hor_normal_vec;
extern vec3_t last_clear_angle;
extern vec3_t velocity_hor_angle;
extern gedict_t* to_zone;
extern gedict_t* search_entity;

#endif