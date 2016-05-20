// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void DM3CampLogic();
void DM4CampLogic();
void DM6CampLogic();
qbool DM6DoorClosed (fb_path_eval_t* eval);
void DM6MarkerTouchLogic (gedict_t* self, gedict_t* goalentity_marker, gedict_t* touch_marker_);
qbool DM6LookAtDoor (gedict_t* self);

static float best_score;

static void BotWaitLogic(gedict_t* touch_marker_) {
	// if we're not looking at a player
	if (!look_object_->ct == ctPlayer) {
		vec3_t linkedPos, lookPos;
		VectorAdd(linked_marker_->s.v.absmin, linked_marker_->s.v.view_ofs, linkedPos);
		VectorAdd(look_object_->s.v.absmin, look_object_->s.v.view_ofs, lookPos);
		traceline(linkedPos[0], linkedPos[1], linkedPos[2] + 32, lookPos[0], lookPos[1], lookPos[2] + 32, TRUE, self);
		if (g_globalvars.trace_fraction != 1) {
			linked_marker_ = touch_marker_;
			new_path_state = 0;
		}
	}
	else  {
		// stop waiting
		self->fb.state = self->fb.state - (self->fb.state & WAIT);
	}
}

static qbool DetectIncomingRocket(vec3_t marker_pos) {
	// if the path location is too close to an incoming rocket, 
	if (rocket_alert && VectorDistance(marker_pos, rocket_endpos) < 200) {
		traceline(rocket_endpos[0], rocket_endpos[1], rocket_endpos[2], marker_pos[0], marker_pos[1], marker_pos[2], TRUE, self);
		return (g_globalvars.trace_fraction == 1);
	}

	return false;
}

static void EvalPath(fb_path_eval_t* eval) {
	vec3_t direction_to_marker;
	vec3_t marker_position;
	float path_score;
	
	// don't try and pass through closed doors
	if (streq(eval->test_marker->s.v.classname, "door")) {
		if (eval->test_marker->fb.state != STATE_BOTTOM) {
			return;
		}
	}

	// if it's a rocket jump, calculate path time (TODO: fix this for horizontal rocket jumps, also precalculate)
	if ((eval->description & ROCKET_JUMP) && self->fb.willRocketJumpThisTic) {
		vec3_t m_P_pos, m_pos;

		VectorAdd(eval->touch_marker->s.v.absmin, eval->touch_marker->s.v.view_ofs, m_pos);
		VectorAdd(eval->test_marker->s.v.absmin, eval->test_marker->s.v.view_ofs, m_P_pos);
		eval->path_time = ((VectorDistance(m_P_pos, m_pos) / sv_maxspeed));
	}

	//
	if (!eval->path_normal && !(eval->description & REVERSIBLE)) {
		return;
	}

	if (DM6DoorClosed(eval)) {
		return;
	}

	if (eval->description & ROCKET_JUMP && !self->fb.willRocketJumpThisTic) {
		return;
	}

	VectorAdd(eval->test_marker->s.v.absmin, eval->test_marker->s.v.view_ofs, marker_position);
	VectorSubtract(marker_position, self->s.v.origin, direction_to_marker);
	VectorNormalize(direction_to_marker);

	same_dir = DotProduct(self_dir, marker_position);
	path_score = same_dir + random();
	avoid = (g_globalvars.time < eval->test_marker->fb.arrow_time) || DetectIncomingRocket(marker_position);

	// If we'd pickup an item as we travel, negatively impact score
	if (beQuiet && eval->test_marker->fb.pickup && !eval->test_marker->s.v.nextthink) {
		if (eval->test_marker != goalentity_marker) {
			if (eval->test_marker->fb.pickup()) {
				path_score = path_score - 2.5;
			}
		}
	}

	if (avoid) {
		path_score = path_score - 2.5;
	}
	else  {
		if (goalentity_marker) {
			from_marker = eval->test_marker;
			path_normal = self->fb.path_normal_;
			goalentity_marker->fb.zone_marker();
			goalentity_marker->fb.sub_arrival_time();
			total_goal_time = eval->path_time + traveltime;
			if (total_goal_time > goal_late_time) {
				if (traveltime < current_goal_time) {
					path_score = path_score + lookahead_time_ - total_goal_time;
				}
				else if (total_goal_time > current_goal_time_125) {
					path_score = path_score - total_goal_time;
				}
			}
		}
	}
	if (path_score > best_score) {
		best_score = path_score;
		linked_marker_ = eval->test_marker;
		new_path_state = eval->description;
	}
}

void EvalLook() {
	vec3_t temp;

	VectorAdd(from_marker->s.v.absmin, from_marker->s.v.view_ofs, temp);
	VectorSubtract(temp, linked_marker_origin, temp);
	VectorNormalize(temp);

	look_score = DotProduct(dir_look, temp);
	look_score = look_score + random();
	if (look_score > best_score) {
		best_score = look_score;
		look_object_ = from_marker;
	}
}

void EvalCloseRunAway() {
	from_marker = enemy_touch_marker;
	to_marker->fb.zone_marker();
	to_marker->fb.sub_arrival_time();
	traveltime2 = traveltime;
	from_marker = touch_marker_;
	to_marker->fb.zone_marker();
	to_marker->fb.sub_arrival_time();
	if (look_traveltime) {
		test_away_score = random() * RA_time * ((traveltime2 * traveltime2) - (look_traveltime_squared + (traveltime * traveltime))) / (look_traveltime * traveltime);
	}
	else  {
		test_away_score = random() * RA_time * (traveltime2 - traveltime);
	}
	if (test_away_score > best_away_score) {
		best_away_marker = to_marker;
		best_away_score = test_away_score;
	}
}

void frogbot_marker_touch() {
	gedict_t* goalentity_ = &g_edicts[self->s.v.goalentity];

	if (self->fb.path_state & WAIT_GROUND) {
		if (!((int)self->s.v.flags & FL_ONGROUND)) {
			return;
		}
	}
	goalentity_marker = goalentity_->fb.touch_marker;
	if (self->fb.state & RUNAWAY) {
		goalentity_marker = NULL;
	}
	if (linked_marker_ == touch_marker_) {
		if (goalentity_ == touch_marker_) {
			if (!touch_marker_->s.v.nextthink) {
				return;
			}
		}
		else if (goalentity_marker == touch_marker_) {
			if (streq(goalentity_->s.v.classname, "dynamic_item")) {
				if (IsVisible(goalentity_)) {
					linked_marker_ = self->fb.linked_marker = goalentity_;
					self->fb.linked_marker_time = g_globalvars.time + 5;
					self->fb.old_linked_marker = touch_marker_;
					return;
				}
			}
		}
	}
	else  {
		if (ExistsPath(self->fb.old_linked_marker, touch_marker_)) {
			if (ExistsPath(touch_marker_, linked_marker_)) {
				self->fb.path_state = new_path_state;
				return;
			}
		}
		self->fb.state = self->fb.state & NOT_HURT_SELF;
	}
	self->fb.path_normal_ = true;
	if (self->fb.state & RUNAWAY) {
		enemy_touch_marker = enemy_->fb.touch_marker;
		if (enemy_touch_marker) {
			int i = 0;

			best_away_marker = NULL;
			best_away_score = 0;
			to_marker = touch_marker_;
			enemy_touch_marker->fb.sight_from_time();
			look_traveltime_squared = look_traveltime * look_traveltime;
			path_normal = true;

			for (i = 0; i < sizeof(touch_marker_->fb.runaway) / sizeof(touch_marker_->fb.runaway[0]); ++i) {
				to_marker = touch_marker_->fb.runaway[i].next_marker;
				if (to_marker) {
					RA_time = touch_marker_->fb.runaway[i].time;
					EvalCloseRunAway();
				}
			}

			if (!best_away_marker) {
				best_away_marker = touch_marker_;
			}
			self->fb.goal_respawn_time = 0;
			goalentity_marker = best_away_marker;
			self->fb.path_normal_ = TRUE;
		}
	}
	if (random() < 0.5) {
		if ((int)self->s.v.items & IT_ROCKET_LAUNCHER) {
			if (self->s.v.ammo_rockets) {
				if ((self->fb.firepower < enemy_->fb.firepower) && (self->s.v.armorvalue < enemy_->s.v.armorvalue)) {
					if (!avoid) {
						to_marker = enemy_->fb.touch_marker;
						if (to_marker) {
							from_marker = touch_marker_;
							from_marker->fb.higher_sight_from_marker();
							if (!look_marker) {
								self->fb._highermarker = 0;
								HigherSightMarker(from_marker);
							}
							if (look_marker) {
								goalentity_marker = look_marker;
								self->fb._highermarker = 1;
							}
						}
					}
				}
			}
		}
	}
	if (isDuel()) {
		if (enemy_->s.v.weapon == IT_GRENADE_LAUNCHER) {
			if (enemy_->s.v.ammo_rockets > 6) {
				if ((self->s.v.origin[2] + 18) < (enemy_->s.v.absmin[2] + enemy_->s.v.view_ofs[2])) {
					vec3_t diff;
					VectorSubtract(self->s.v.origin, enemy_->s.v.origin, diff);
					if (vlen(diff) < 200) {
						if (enemy_->s.v.button0) {
							to_marker = enemy_->fb.touch_marker;
							if (to_marker) {
								from_marker = touch_marker_;
								from_marker->fb.higher_sight_from_marker();
								if (!look_marker) {
									self->fb._highermarker = 0;
									HigherSightMarker(from_marker);
								}
								if (look_marker) {
									goalentity_marker = look_marker;
									self->fb._highermarker = 1;
								}
							}
						}
					}
				}
			}
		}
	}

	best_score = -1000000;
	normalize(self->s.v.velocity, self_dir);
	rocket_alert = FALSE;
	if ((int)enemy_->s.v.items & IT_ROCKET_LAUNCHER && !((int)self->s.v.items & IT_INVULNERABILITY)) {
		if (enemy_->attack_finished <= g_globalvars.time + 0.2) {
			if (enemy_->s.v.ammo_rockets) {
				if (random() < 0.5) {
					VectorCopy(enemy_->s.v.origin, src);
					src[2] += 16;
					traceline(src[0], src[1], src[2], origin_[0], origin_[1], origin_[2], TRUE, self);
					if (g_globalvars.trace_fraction != 1) {
						rocket_alert = TRUE;
						trap_makevectors(enemy_->s.v.v_angle);
						traceline(src[0], src[1], src[2], src[0] + g_globalvars.v_forward[0] * 500, src[1] + g_globalvars.v_forward[1] * 500, src[2] + g_globalvars.v_forward[2] * 500, TRUE, self);
						VectorCopy(g_globalvars.trace_endpos, rocket_endpos);
					}
				}
			}
		}
	}

	beQuiet = (look_object_ != enemy_ && enemy_ && !self->fb.allowedMakeNoise);

	if (goalentity_marker) {
		from_marker = touch_marker_;
		path_normal = self->fb.path_normal_;
		goalentity_marker->fb.zone_marker();
		goalentity_marker->fb.sub_arrival_time();
		current_goal_time = traveltime;
		current_goal_time_125 = current_goal_time + 1.25;
		if (current_goal_time < 2.5) {
			goal_late_time = (self->fb.goal_respawn_time - (random() * 5)) - g_globalvars.time;
		}
		else  {
			goal_late_time = (self->fb.goal_respawn_time - (random() * 10)) - g_globalvars.time;
		}
		
		if (touch_marker_) {
			fb_path_eval_t eval = { 0 };

			eval.description = 0;
			eval.path_normal = self->fb.path_normal_;
			eval.path_time = 0;
			eval.rocket_alert = rocket_alert;
			eval.test_marker = touch_marker_;
			eval.touch_marker = touch_marker_;
			
			EvalPath(&eval);
		}
	}

	// Evaluate all paths from the newly touched marker
	{ 
		int i = 0;

		for (i = 0; i < NUMBER_PATHS; ++i) {
			gedict_t* test_marker = touch_marker_->fb.paths[i].next_marker;
			if (test_marker) {
				fb_path_eval_t eval = { 0 };

				eval.description = touch_marker_->fb.paths[i].flags;
				eval.path_normal = self->fb.path_normal_;
				eval.path_time = touch_marker_->fb.paths[i].time;
				eval.rocket_alert = rocket_alert;
				eval.test_marker = test_marker;
				eval.touch_marker = touch_marker_;

				EvalPath(&eval);
			}
		}
	}

	if (streq(touch_marker_->s.v.classname, "door")) {
		if (streq(deathtype, "squish")) {
			if (linked_marker_->s.v.absmin[2] + linked_marker_->s.v.view_ofs[2] > self->s.v.origin[2] + 18) {
				if (teamplay) {
					if (self->fb.state & HELP_TEAMMATE) {
						self->fb.state = self->fb.state - HELP_TEAMMATE;
					}
				}
				if (self->s.v.absmin[0] >= touch_marker_->s.v.absmin[0]) {
					if (self->s.v.absmax[0] <= touch_marker_->s.v.absmax[0]) {
						if (self->s.v.absmin[1] >= touch_marker_->s.v.absmin[1]) {
							if (self->s.v.absmax[1] <= touch_marker_->s.v.absmax[1]) {
								linked_marker_ = self->fb.linked_marker = touch_marker_;
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
	}

	if (self->fb.state & WAIT) {
		BotWaitLogic(touch_marker_);
	}

	if (streq(g_globalvars.mapname, "dm3")) {
		DM3CampLogic();
	}
	else if (streq(g_globalvars.mapname, "dm4")) {
		DM4CampLogic();
	}
	else if (streq(g_globalvars.mapname, "dm6")) {
		DM6CampLogic();
	}
	self->fb.linked_marker = linked_marker_;
	self->fb.path_state = new_path_state;
	if (touch_marker_ == linked_marker_) {
		self->fb.linked_marker_time = g_globalvars.time + 0.3;
	}
	else  {
		self->fb.linked_marker_time = g_globalvars.time + 5;
	}
	self->fb.old_linked_marker = touch_marker_;
	DM6MarkerTouchLogic (self, goalentity_marker, touch_marker_);
	self->fb.state = self->fb.state & NOT_NOTARGET_ENEMY;
	if ((int)self->s.v.flags & FL_ONGROUND && self->fb.wasinwater) {
		self->fb.wasinwater = FALSE;
		self->fb.path_state = self->fb.path_state - (self->fb.path_state & WATERJUMP_);
		self->fb.state = self->fb.state & NOT_NOTARGET_ENEMY;
	}
	if (self->fb.path_state & WATERJUMP_) {
		self->fb.wasinwater = TRUE;
		self->fb.state = self->fb.state | NOTARGET_ENEMY;
		look_object_ = self->fb.linked_marker;
		self->fb.look_object = look_object_;
		if (random() < 0.003) {
			self->fb.jumping = true;
		}
		return;
	}
	if (DM6LookAtDoor (self))
		return;
	if (look_object_ && look_object_->ct == ctPlayer) {
		return;
	}
	if ((self->s.v.waterlevel == 2) || (self->s.v.waterlevel == 1)) {
		self->fb.look_object = self->fb.linked_marker;
		return;
	}
	if (random() < self->fb.skill.look_anywhere) {
		from_marker = enemy_->fb.touch_marker;
		if (!from_marker) {
			from_marker = goalentity_marker;
		}
		if (from_marker) {
			to_marker = linked_marker_;
			from_marker->fb.sight_from_marker();
			if (look_marker) {
				path_normal = TRUE;
				look_marker->fb.zone_marker();
				look_marker->fb.sub_arrival_time();
				look_traveltime = traveltime;
			}
			else  {
				SightMarker(from_marker);
			}
			if (look_marker) {
				to_marker = from_marker;
				from_marker = linked_marker_;
				path_normal = TRUE;
				to_marker->fb.zone_marker();
				to_marker->fb.sub_arrival_time();
				if (look_traveltime < traveltime) {
					self->fb.look_object = look_object_ = look_marker;
					self->fb.predict_shoot = TRUE;
					return;
				}
			}
		}
	}
	if (touch_marker_ == linked_marker_) {
		VectorClear(dir_look);
	}
	else {
		vec3_t diff;
		VectorAdd(linked_marker_->s.v.absmin, linked_marker_->s.v.view_ofs, linked_marker_origin);
		VectorSubtract(linked_marker_origin, origin_, diff);
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
	VectorAdd(linked_marker_->s.v.absmin, linked_marker_->s.v.view_ofs, linked_marker_origin);
	best_score = -1000000;

	{
		int i = 0;
		for (i = 0; i < NUMBER_PATHS; ++i) {
			from_marker = linked_marker_->fb.paths[i].next_marker;
			if (from_marker) {
				EvalLook();
			}
		}
	}
	self->fb.look_object = look_object_;
	self->fb.predict_shoot = false;
}

