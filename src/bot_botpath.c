// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void DM3CampLogic();
void DM4CampLogic();
void DM6CampLogic();
qbool DM6DoorClosed (fb_path_eval_t* eval);
void DM6MarkerTouchLogic (gedict_t* self, gedict_t* goalentity_marker);
qbool DM6LookAtDoor (gedict_t* self);
static float EvalPath (fb_path_eval_t* eval, qbool allowRocketJumps, qbool trace_bprint, float current_goal_time, float current_goal_time_125);
//static float best_score;

#define G_bprint_debug(...) if (trace_bprint) { G_bprint(__VA_ARGS__); }
#define STOP_DEBUGGING { self->fb.debug = false; }
//#define G_bprint_debug(...)
//#define STOP_DEBUGGING 

// FIXME: Globals
extern int new_path_state;

#define PATH_SCORE_NULL -1000000
#define PATH_NOISE_PENALTY 2.5
#define PATH_AVOID_PENALTY 2.5

static vec3_t rocket_endpos = { 0 };


static qbool HasItem (gedict_t* player, int mask)
{
	return ((int)player->s.v.items & mask);
}

static void BotRouteEvaluation (qbool be_quiet, float lookahead_time, gedict_t* from_marker, gedict_t* to_marker, qbool rocket_alert, qbool rocket_jump_routes_allowed, vec3_t player_origin, vec3_t player_direction, qbool path_normal, qbool trace_bprint, float current_goal_time, float current_goal_time_125, float goal_late_time, float* best_score, gedict_t** next_marker, int* next_description)
{ 
	fb_path_eval_t eval = { 0 };
	int i = 0;

	eval.goalentity_marker = to_marker;
	VectorCopy (player_origin, eval.player_origin);
	VectorCopy (player_direction, eval.player_direction);
	eval.rocket_alert = rocket_alert;
	eval.path_normal = path_normal;
	eval.touch_marker = from_marker;
	eval.goal_late_time = goal_late_time;
	eval.lookahead_time_ = lookahead_time;
	eval.be_quiet = be_quiet;

	for (i = 0; i < NUMBER_PATHS; ++i) {
		eval.description = from_marker->fb.paths[i].flags;
		eval.path_time = from_marker->fb.paths[i].time;
		eval.test_marker = from_marker->fb.paths[i].next_marker;

		if (eval.test_marker) {
			float path_score = 0;

			G_bprint_debug (2, "Path from %d > %d\n", eval.touch_marker->fb.index, eval.test_marker->fb.index);
			path_score = EvalPath(&eval, rocket_jump_routes_allowed, trace_bprint, current_goal_time, current_goal_time_125);
			G_bprint_debug (2, ">> path score %f vs %f\n", path_score, *best_score);
			if (path_score > *best_score) {
				*best_score = path_score;
				*next_marker = eval.test_marker;
				*next_description = eval.description;
			}
		}
	}
}

void PathScoringLogic(
	float goal_respawn_time, qbool be_quiet, float lookahead_time, qbool path_normal, vec3_t player_origin, vec3_t player_direction, gedict_t* touch_marker_,
	gedict_t* goalentity_marker, qbool rocket_alert, qbool rocket_jump_routes_allowed,
	qbool trace_bprint, float *best_score, gedict_t** linked_marker_, int* new_path_state
)
{
	float current_goal_time = 0;
	float current_goal_time_125 = 0;
	float goal_late_time = 0;

	G_bprint_debug (2, "PathScoringLogic(\n");
	G_bprint_debug (2, "  goal_respawn_time = %f\n", goal_respawn_time);
	G_bprint_debug (2, "  path_normal = %s\n", path_normal ? "true" : "false");
	G_bprint_debug (2, "  player_origin = [%f %f %f]\n", PASSVEC3(player_origin));
	G_bprint_debug (2, "  touch_marker_ = %d (%s)\n", touch_marker_->fb.index, touch_marker_->s.v.classname);
	G_bprint_debug (2, "  goalentity_marker  = %d (%s)\n", (goalentity_marker ? goalentity_marker->fb.index : -1), goalentity_marker ? goalentity_marker->s.v.classname : "(null)");
	G_bprint_debug (2, "  rocket_alert = %s\n", rocket_alert ? "true" : "false");
	G_bprint_debug (2, "  rj_allowed = %s\n", rocket_jump_routes_allowed ? "true" : "false");
	G_bprint_debug (2, "  *best_score = %f\n", *best_score);
	G_bprint_debug (2, "  *linked_marker = %d (%s)\n", (*linked_marker_) ? (*linked_marker_)->fb.index : -1, (*linked_marker_) ? (*linked_marker_)->s.v.classname : "(null)");
	G_bprint_debug (2, ") = \n");

	if (goalentity_marker) {
		from_marker = touch_marker_;
		goalentity_marker->fb.zone_marker();
		goalentity_marker->fb.sub_arrival_time();
		current_goal_time = traveltime;
		current_goal_time_125 = traveltime + 1.25;

		G_bprint_debug (2, "Init: goal_time %f, \n", current_goal_time);

		// FIXME: Estimating respawn times should be skill-based
		if (current_goal_time < 2.5) {
			goal_late_time = (goal_respawn_time - (random () * 5)) - g_globalvars.time;
		}
		else {
			goal_late_time = (goal_respawn_time - (random () * 10)) - g_globalvars.time;
		}
	}

	if (goalentity_marker && touch_marker_) {
		float path_score = 0;
		fb_path_eval_t eval = { 0 };

		eval.path_normal = path_normal;
		eval.rocket_alert = rocket_alert;
		eval.touch_marker = touch_marker_;
		eval.goalentity_marker = goalentity_marker;
		eval.goal_late_time = goal_late_time;
		eval.lookahead_time_ = lookahead_time;
		eval.be_quiet = be_quiet;
		VectorCopy (player_origin, eval.player_origin);
		VectorCopy (player_direction, eval.player_direction);

		eval.test_marker = touch_marker_;
		eval.description = 0;
		eval.path_time = 0;

		G_bprint_debug (2, "Marker > GoalEntity (%s) %d\n", goalentity_marker->s.v.classname, goalentity_marker->fb.index);
		path_score = EvalPath(&eval, rocket_jump_routes_allowed, trace_bprint, current_goal_time, current_goal_time_125);
		G_bprint_debug (2, ">> path score %f vs %f\n", path_score, *best_score);
		if (path_score > *best_score) {
			*best_score = path_score;
			*linked_marker_ = eval.test_marker;
			*new_path_state = eval.description;
		}
	}

	// Evaluate all paths from touched marker to the goal entity
	BotRouteEvaluation (be_quiet, lookahead_time, touch_marker_, goalentity_marker, rocket_alert, rocket_jump_routes_allowed, player_origin, player_direction, path_normal, trace_bprint, current_goal_time, current_goal_time_125, goal_late_time, best_score, linked_marker_, new_path_state);
}

// FIXME: Globals
static void BotWaitLogic(gedict_t* self) {
	gedict_t* touch_marker = self->fb.touch_marker;
	gedict_t* look_object = self->fb.look_object;

	// if we're not looking at a player
	if (look_object->ct != ctPlayer) {
		vec3_t linkedPos, lookPos;
		VectorAdd(self->fb.linked_marker->s.v.absmin, self->fb.linked_marker->s.v.view_ofs, linkedPos);
		VectorAdd(look_object->s.v.absmin, look_object->s.v.view_ofs, lookPos);
		traceline(linkedPos[0], linkedPos[1], linkedPos[2] + 32, lookPos[0], lookPos[1], lookPos[2] + 32, true, self);
		if (g_globalvars.trace_fraction != 1) {
			self->fb.linked_marker = touch_marker;
			new_path_state = 0;
		}
	}
	else  {
		// stop waiting
		self->fb.state &= ~WAIT;
	}
}

static qbool DetectIncomingRocket(qbool rocket_alert, vec3_t marker_pos) {
	// if the path location is too close to an incoming rocket, 
	if (rocket_alert && VectorDistance(marker_pos, rocket_endpos) < 200) {
		traceline(rocket_endpos[0], rocket_endpos[1], rocket_endpos[2], marker_pos[0], marker_pos[1], marker_pos[2], true, self);
		return (g_globalvars.trace_fraction == 1);
	}

	return false;
}

static float EvalPath(fb_path_eval_t* eval, qbool allowRocketJumps, qbool trace_bprint, float current_goal_time, float current_goal_time_125) {
	vec3_t direction_to_marker;
	vec3_t marker_position;
	float path_score;
	float same_dir;
	
	// don't try and pass through closed doors
	if (streq(eval->test_marker->s.v.classname, "door")) {
		if (eval->test_marker->fb.state != STATE_BOTTOM) {
			G_bprint_debug (2, "> Door closed, ignoring...\n");
			return PATH_SCORE_NULL;
		}
	}

	// if it's a rocket jump, calculate path time (TODO: fix this for horizontal rocket jumps, also precalculate)
	// FIXME: /sv_maxspeed is utter crap, should be based on velocity of flight
	if ((eval->description & ROCKET_JUMP) && allowRocketJumps) {
		vec3_t m_P_pos, m_pos;

		VectorAdd(eval->touch_marker->s.v.absmin, eval->touch_marker->s.v.view_ofs, m_pos);
		VectorAdd(eval->test_marker->s.v.absmin, eval->test_marker->s.v.view_ofs, m_P_pos);
		eval->path_time = (VectorDistance(m_P_pos, m_pos) / sv_maxspeed);
	}

	//
	if (!eval->path_normal && !(eval->description & REVERSIBLE)) {
		G_bprint_debug (2, "> path not normal and not reversible, stopping\n");
		return PATH_SCORE_NULL;
	}

	// FIXME: Map specific logic
	if (DM6DoorClosed(eval)) {
		G_bprint_debug (2, "> DM6 Door Closed, stopping\n");
		return PATH_SCORE_NULL;
	}

	if ((eval->description & ROCKET_JUMP) && !allowRocketJumps) {
		G_bprint_debug (2, "> RJ required, stopping\n");
		return PATH_SCORE_NULL;
	}

	// FIXME: This code gives a bonus to routes carrying the player in the same direction
	VectorAdd(eval->test_marker->s.v.absmin, eval->test_marker->s.v.view_ofs, marker_position);
	VectorSubtract(marker_position, eval->player_origin, direction_to_marker);
	VectorNormalize(direction_to_marker);
	same_dir = DotProduct(eval->player_direction, direction_to_marker);

	path_score = same_dir + random();
	self->fb.avoiding = (g_globalvars.time < eval->test_marker->fb.arrow_time) || DetectIncomingRocket(eval->rocket_alert, marker_position);
	G_bprint_debug (2, "> Temp path_score = %f\n", path_score);

	// If we'd pickup an item as we travel, negatively impact score
	if (eval->be_quiet && eval->test_marker->fb.pickup && ! WaitingToRespawn(eval->test_marker)) {
		if (eval->test_marker != eval->goalentity_marker) {
			if (eval->test_marker->fb.pickup()) {
				path_score -= PATH_NOISE_PENALTY;
				G_bprint_debug (2, "> be_quiet penalty, new path_score %f\n", path_score);
			}
		}
	}

	if (self->fb.avoiding) {
		path_score -= PATH_AVOID_PENALTY;
		G_bprint_debug (2, "> avoid penalty, final path_score %f\n", path_score);
	}
	else if (eval->goalentity_marker) {
		float total_goal_time;

		// Calculate time from marker > goal entity
		from_marker = eval->test_marker;
		path_normal = eval->path_normal;
		eval->goalentity_marker->fb.zone_marker();
		eval->goalentity_marker->fb.sub_arrival_time();
		total_goal_time = eval->path_time + traveltime;
		G_bprint_debug (2, "> total_goal_time = %f + %f = %f\n", eval->path_time, traveltime, total_goal_time);

		if (total_goal_time > eval->goal_late_time) {
			G_bprint_debug (2, "> total_goal time > goal_late_time (%f)\n", eval->goal_late_time);
			if (traveltime < current_goal_time) {
				path_score += eval->lookahead_time_ - total_goal_time;
				G_bprint_debug (2, "> += %f - %f\n", eval->lookahead_time_, total_goal_time);
			}
			else if (total_goal_time > current_goal_time_125) {
				path_score -= total_goal_time;
				G_bprint_debug (2, "> -= %f\n", total_goal_time);
			}
		}
	}

	return path_score;
}

static void EvalLook(gedict_t* self, float* best_score, vec3_t dir_look, vec3_t linked_marker_origin) {
	vec3_t temp;
	float look_score;

	VectorAdd(from_marker->s.v.absmin, from_marker->s.v.view_ofs, temp);
	VectorSubtract(temp, linked_marker_origin, temp);
	VectorNormalize(temp);

	look_score = DotProduct(dir_look, temp);
	look_score = look_score + random();           // FIXME: Skill
	if (look_score > *best_score) {
		*best_score = look_score;
		self->fb.look_object = from_marker;
	}
}

static void EvalCloseRunAway(float runaway_time, gedict_t* enemy_touch_marker, float look_traveltime_squared, float *best_away_score, gedict_t **best_away_marker, gedict_t* touch_marker) {
	float test_away_score = 0;
	float traveltime2;

	from_marker = enemy_touch_marker;
	to_marker->fb.zone_marker();
	to_marker->fb.sub_arrival_time();
	traveltime2 = traveltime;
	from_marker = touch_marker;
	to_marker->fb.zone_marker();
	to_marker->fb.sub_arrival_time();
	if (look_traveltime) {
		test_away_score = random() * runaway_time * ((traveltime2 * traveltime2) - (look_traveltime_squared + (traveltime * traveltime))) / (look_traveltime * traveltime);
	}
	else {
		test_away_score = random() * runaway_time * (traveltime2 - traveltime);
	}
	if (test_away_score > *best_away_score) {
		*best_away_marker = to_marker;
		*best_away_score = test_away_score;
	}
}

static qbool CheckForRocketEnemyAim (gedict_t* self)
{
	gedict_t* enemy_ = &g_edicts[self->s.v.enemy];

	if (enemy_ != world && HasItem(enemy_, IT_ROCKET_LAUNCHER) && !HasItem(self, IT_INVULNERABILITY)) {
		// FIXME: random() call to determine behaviour, move threshold to bot's skill
		if (enemy_->attack_finished <= g_globalvars.time + 0.2 && enemy_->s.v.ammo_rockets && random() < 0.5) {
			vec3_t src;
			VectorCopy(enemy_->s.v.origin, src);
			src[2] += 16;
			traceline(src[0], src[1], src[2], self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], true, self);

			// Fixme: Only avoid rocket aim if there's a direct line, rather than if they are within range of the explosion...
			if (g_globalvars.trace_fraction != 1) {
				trap_makevectors(enemy_->s.v.v_angle);
				traceline(src[0], src[1], src[2], src[0] + g_globalvars.v_forward[0] * 500, src[1] + g_globalvars.v_forward[1] * 500, src[2] + g_globalvars.v_forward[2] * 500, true, self);
				VectorCopy(g_globalvars.trace_endpos, rocket_endpos);
				return true;
			}
		}
	}

	return false;
}

void frogbot_marker_touch(void) {
	gedict_t* goalentity_ = &g_edicts[self->s.v.goalentity];
	gedict_t* goalentity_marker = (goalentity_ == world || (self->fb.state & RUNAWAY) ? NULL : goalentity_->fb.touch_marker);
	qbool trace_bprint = self->fb.debug;
	qbool rocket_jump_routes_allowed = self->fb.willRocketJumpThisTic;
	qbool rocket_alert = false;
	float best_score = PATH_SCORE_NULL;
	vec3_t player_direction;

	// Wait until we hit the ground
	if ((self->fb.path_state & WAIT_GROUND) && !((int)self->s.v.flags & FL_ONGROUND))
		return;

	if (self->fb.linked_marker == self->fb.touch_marker) {
		if (goalentity_ == self->fb.touch_marker) {
			if (! WaitingToRespawn(self->fb.touch_marker)) {
				return;
			}
		}
		else if (goalentity_marker == self->fb.touch_marker) {
			// FIXME: Create IsDynamicItem(ent)... should be dropped quads etc as well
			if (streq(goalentity_->s.v.classname, "backpack")) {
				if (IsVisible(goalentity_)) {
					self->fb.linked_marker = goalentity_;
					self->fb.linked_marker_time = g_globalvars.time + 5;
					self->fb.old_linked_marker = self->fb.touch_marker;
					return;
				}
			}
		}
	}
	else {
		if (ExistsPath(self->fb.old_linked_marker, self->fb.touch_marker)) {
			if (ExistsPath(self->fb.touch_marker, self->fb.linked_marker)) {
				self->fb.path_state = new_path_state;
				return;
			}
		}
		self->fb.state &= NOT_HURT_SELF;
	}

	self->fb.path_normal_ = true;
	if (self->fb.state & RUNAWAY) {
		gedict_t* enemy_touch_marker = g_edicts[self->s.v.enemy].fb.touch_marker;
		if (enemy_touch_marker) {
			int i = 0;
			float best_away_score = 0;
			float look_traveltime_squared = 0;
			gedict_t* best_away_marker = NULL;
			
			to_marker = self->fb.touch_marker;
			enemy_touch_marker->fb.sight_from_time();
			look_traveltime_squared = look_traveltime * look_traveltime;
			path_normal = true;

			for (i = 0; i < NUMBER_PATHS; ++i) {
				to_marker = self->fb.touch_marker->fb.runaway[i].next_marker;
				if (to_marker) {
					EvalCloseRunAway(self->fb.touch_marker->fb.runaway[i].time, enemy_touch_marker, look_traveltime_squared, &best_away_score, &best_away_marker, self->fb.touch_marker);
				}
			}

			self->fb.goal_respawn_time = 0;
			goalentity_marker = (best_away_marker ? best_away_marker : self->fb.touch_marker);
			self->fb.path_normal_ = true;
		}
	}

	if (random() < 0.5) {
		gedict_t* enemy_ = &g_edicts[self->s.v.enemy];

		if (HasItem(self, IT_ROCKET_LAUNCHER)) {
			if (self->s.v.ammo_rockets) {
				if ((self->fb.firepower < g_edicts[self->s.v.enemy].fb.firepower) && (self->s.v.armorvalue < enemy_->s.v.armorvalue)) {
					if (!self->fb.avoiding) {
						to_marker = enemy_->fb.touch_marker;
						if (to_marker) {
							from_marker = self->fb.touch_marker;
							from_marker->fb.higher_sight_from_marker();
							if (!look_marker) {
								self->fb._highermarker = false;
								HigherSightMarker(from_marker);
							}

							if (look_marker) {
								goalentity_marker = look_marker;
								self->fb._highermarker = true;
							}
						}
					}
				}
			}
		}
	}

	if (isDuel()) {
		gedict_t* enemy_ = &g_edicts[self->s.v.enemy];

		if (HasItem(enemy_, IT_GRENADE_LAUNCHER)) {
			if (enemy_->s.v.ammo_rockets > 6) {
				if ((self->s.v.origin[2] + 18) < (enemy_->s.v.absmin[2] + enemy_->s.v.view_ofs[2])) {
					vec3_t diff;
					VectorSubtract(self->s.v.origin, enemy_->s.v.origin, diff);
					if (vlen(diff) < 200) {
						if (enemy_->s.v.button0) {
							to_marker = enemy_->fb.touch_marker;
							if (to_marker) {
								from_marker = self->fb.touch_marker;
								from_marker->fb.higher_sight_from_marker();
								if (!look_marker) {
									self->fb._highermarker = false;
									HigherSightMarker(from_marker);
								}
								if (look_marker) {
									goalentity_marker = look_marker;
									self->fb._highermarker = true;
								}
							}
						}
					}
				}
			}
		}
	}

	rocket_alert = CheckForRocketEnemyAim (self);
	normalize (self->s.v.velocity, player_direction);
	self->fb.be_quiet = (self->s.v.enemy && &g_edicts[self->s.v.enemy] != self->fb.look_object && !self->fb.allowedMakeNoise);

	PathScoringLogic (self->fb.goal_respawn_time, self->fb.be_quiet, self->fb.skill.lookahead_time, self->fb.path_normal_, self->s.v.origin, self->s.v.velocity, self->fb.touch_marker, goalentity_marker, rocket_alert, rocket_jump_routes_allowed, trace_bprint, &best_score, &self->fb.linked_marker, &new_path_state);
	STOP_DEBUGGING

	// "check if fully on lift - if not then continue moving to linked_marker_"
	if (streq(self->fb.touch_marker->s.v.classname, "door") && self->deathtype == dtSQUISH) {
		if (self->fb.linked_marker->s.v.absmin[2] + self->fb.linked_marker->s.v.view_ofs[2] > self->s.v.origin[2] + 18) {
			if (teamplay)
				self->fb.state &= ~HELP_TEAMMATE;

			if (self->s.v.absmin[0] >= self->fb.touch_marker->s.v.absmin[0]) {
				if (self->s.v.absmax[0] <= self->fb.touch_marker->s.v.absmax[0]) {
					if (self->s.v.absmin[1] >= self->fb.touch_marker->s.v.absmin[1]) {
						if (self->s.v.absmax[1] <= self->fb.touch_marker->s.v.absmax[1]) {
							self->fb.linked_marker = self->fb.touch_marker;
							self->fb.path_state = 0;
							self->fb.linked_marker_time = g_globalvars.time + 5;
							self->fb.old_linked_marker = NULL;
						}
					}
				}
			}
			return;
		}
	}

	if (self->fb.state & WAIT) {
		BotWaitLogic(self);
	}

	// FIXME: Map specific waiting points
	if (streq(g_globalvars.mapname, "dm3")) {
		DM3CampLogic();
	}
	else if (streq(g_globalvars.mapname, "dm4")) {
		DM4CampLogic();
	}
	else if (streq(g_globalvars.mapname, "dm6")) {
		DM6CampLogic();
	}

	G_bprint_debug (2, "New linked marker: %d\n", self->fb.linked_marker->fb.index);
	self->fb.path_state = new_path_state;
	self->fb.linked_marker_time = g_globalvars.time + (self->fb.touch_marker == self->fb.linked_marker ? 0.3 : 5);
	self->fb.old_linked_marker = self->fb.touch_marker;

	// Logic past this point appears to be deciding what to look at...
	DM6MarkerTouchLogic (self, goalentity_marker);

	self->fb.state &= ~NOTARGET_ENEMY;
	if ((int)self->s.v.flags & FL_ONGROUND && self->fb.wasinwater) {
		self->fb.wasinwater = false;
		self->fb.path_state &= ~WATERJUMP_;
		self->fb.state &= ~NOTARGET_ENEMY;
	}

	if (self->fb.path_state & WATERJUMP_) {
		self->fb.wasinwater = true;
		self->fb.state |= NOTARGET_ENEMY;
		self->fb.look_object = self->fb.linked_marker;
		self->fb.jumping |= (random() < 0.003); // FIXME
		return;
	}

	// FIXME: Map-specific
	if (DM6LookAtDoor (self))
		return;

	if (self->fb.look_object && self->fb.look_object->ct == ctPlayer) {
		return;
	}

	// When in water, look at the next waypoint
	if ((self->s.v.waterlevel == 2) || (self->s.v.waterlevel == 1)) {
		self->fb.look_object = self->fb.linked_marker;
		return;
	}

	if (random() < self->fb.skill.look_anywhere) {
		from_marker = g_edicts[self->s.v.enemy].fb.touch_marker;
		if (!from_marker) {
			from_marker = goalentity_marker;
		}
		if (from_marker) {
			to_marker = self->fb.linked_marker;
			from_marker->fb.sight_from_marker();
			if (look_marker) {
				path_normal = true;
				look_marker->fb.zone_marker();
				look_marker->fb.sub_arrival_time();
				look_traveltime = traveltime;
			}
			else {
				SightMarker(from_marker);
			}

			if (look_marker) {
				to_marker = from_marker;
				from_marker = self->fb.linked_marker;
				path_normal = true;
				to_marker->fb.zone_marker();
				to_marker->fb.sub_arrival_time();
				if (look_traveltime < traveltime) {
					self->fb.look_object = look_marker;
					self->fb.predict_shoot = true;
					return;
				}
			}
		}
	}

	// Either look at the next marker or the marker after that...
	{
		vec3_t dir_look = { 0 };

		if (self->fb.touch_marker != self->fb.linked_marker) {
			vec3_t diff, linked_marker_origin;

			VectorAdd(self->fb.linked_marker->s.v.absmin, self->fb.linked_marker->s.v.view_ofs, linked_marker_origin);
			VectorSubtract (linked_marker_origin, self->s.v.origin, diff);
			normalize(diff, dir_look);
		}

		trap_makevectors(self->s.v.v_angle);
		{
			vec3_t temp;
			VectorScale(self->s.v.velocity, inv_sv_maxspeed, temp);
			VectorAdd(temp, g_globalvars.v_forward, temp);
			VectorAdd(temp, dir_look, temp);
			normalize(temp, dir_look);
		}

		// Look at the next marker...
		{
			int i = 0;
			vec3_t linked_marker_origin = { 0 };

			VectorAdd(self->fb.linked_marker->s.v.absmin, self->fb.linked_marker->s.v.view_ofs, linked_marker_origin);
			best_score = PATH_SCORE_NULL;
			for (i = 0; i < NUMBER_PATHS; ++i) {
				from_marker = self->fb.linked_marker->fb.paths[i].next_marker;
				if (from_marker) {
					EvalLook(self, &best_score, dir_look, linked_marker_origin);
				}
			}
		}
	}
	self->fb.predict_shoot = false;
}
