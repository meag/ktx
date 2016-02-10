// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void EvalPath() {
	vec3_t temp;

	if (streq(test_marker->s.v.classname, "door")) {
		if (test_marker->fb.state != STATE_BOTTOM) {
			return;
		}
	}
	if (a_evalpath()) {
		return;
	}
	if (!path_normal) {
		if (!(description & REVERSIBLE)) {
			return;
		}
	}
	if (test_marker == dm6_door) {
		if (!dm6_door->s.v.takedamage) {
			return;
		}
	}
	VectorAdd(test_marker->s.v.absmin, test_marker->s.v.view_ofs, marker_pos);
	if (description & DM6_DOOR) {
		if (dm6_door->s.v.origin[0] > -64) {
			return;
		}
	}
	if (description & ROCKET_JUMP) {
		if (!self->fb.willRocketJumpThisTic) {
			return;
		}
	}

	VectorSubtract(marker_pos, origin_, temp);
	VectorNormalize(temp);
	same_dir = DotProduct(self_dir, temp);
	path_score = same_dir + random();
	if (g_globalvars.time < test_marker->fb.arrow_time) {
		avoid = TRUE;
	}
	else  {
		avoid = FALSE;
		if (rocket_alert) {
			vec3_t temp;
			VectorSubtract(marker_pos, rocket_endpos, temp);
			if (vlen(temp) < 200) {
				traceline(rocket_endpos[0], rocket_endpos[1], rocket_endpos[2], marker_pos[0], marker_pos[1], marker_pos[2], TRUE, self);
				if (g_globalvars.trace_fraction == 1) {
					avoid = TRUE;
				}
			}
		}
	}
	if (beQuiet) {
		if (test_marker->fb.pickup) {
			if (!test_marker->s.v.nextthink) {
				if (test_marker != goalentity_marker) {
					if (test_marker->fb.pickup()) {
						path_score = path_score - 2.5;
					}
				}
			}
		}
	}
	if (avoid) {
		path_score = path_score - 2.5;
	}
	else  {
		if (goalentity_marker) {
			from_marker = test_marker;
			path_normal = self->fb.path_normal_;
			goalentity_marker->fb.zone_marker();
			goalentity_marker->fb.sub_arrival_time();
			total_goal_time = path_time + traveltime;
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
		linked_marker_ = test_marker;
		new_path_state = description;
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
	if (self->fb.path_state & WAIT_GROUND) {
		if (!((int)self->s.v.flags & FL_ONGROUND)) {
			return;
		}
	}
	goalentity_marker = goalentity_->fb.touch_marker;
	if (self->fb.state & RUNAWAY) {
		goalentity_marker = world;
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
		from_marker = self->fb.old_linked_marker;
		to_marker = touch_marker_;
		if (ExistsPath()) {
			from_marker = touch_marker_;
			to_marker = linked_marker_;
			if (ExistsPath()) {
				self->fb.path_state = new_path_state;
				return;
			}
		}
		self->fb.state = self->fb.state & NOT_HURT_SELF;
	}
	self->fb.path_normal_ = TRUE;
	if (self->fb.state & RUNAWAY) {
		enemy_touch_marker = enemy_->fb.touch_marker;
		if (enemy_touch_marker) {
			int i = 0;

			best_away_marker = world;
			best_away_score = 0;
			to_marker = touch_marker_;
			enemy_touch_marker->fb.sight_from_time();
			look_traveltime_squared = look_traveltime * look_traveltime;
			path_normal = TRUE;

			for (i = 0; i < sizeof(touch_marker_->fb.runaway) / sizeof(touch_marker_->fb.runaway[0]); ++i) {
				to_marker = touch_marker_->fb.runaway[i].next_marker;
				if (to_marker) {
					RA_time = touch_marker_->fb.runaway[i].time;
					EvalCloseRunAway();
				}
			}

			if (!best_away_marker || best_away_marker == world) {
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
								HigherSightMarker();
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
	if (duel_mode) {
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
									HigherSightMarker();
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
		if (enemy_->fb.attack_finished <= g_globalvars.time + 0.2) {
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
	if (look_object_ == enemy_) {
		beQuiet = FALSE;
	}
	else  {
		if (enemy_) {
			beQuiet = !self->fb.allowedMakeNoise;
		}
		else  {
			beQuiet = FALSE;
		}
	}
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
		test_marker = touch_marker_;
		if (test_marker) {
			description = 0;
			path_time = 0;
			EvalPath();
		}
	}
	test_marker = touch_marker_->fb.P0;
	if (test_marker) {
		description = touch_marker_->fb.D0;
		path_time = touch_marker_->fb.P0_time;
		EvalPath();
	}
	test_marker = touch_marker_->fb.P1;
	if (test_marker) {
		description = touch_marker_->fb.D1;
		path_time = touch_marker_->fb.P1_time;
		EvalPath();
	}
	test_marker = touch_marker_->fb.P2;
	if (test_marker) {
		description = touch_marker_->fb.D2;
		path_time = touch_marker_->fb.P2_time;
		EvalPath();
	}
	test_marker = touch_marker_->fb.P3;
	if (test_marker) {
		description = touch_marker_->fb.D3;
		path_time = touch_marker_->fb.P3_time;
		EvalPath();
	}
	test_marker = touch_marker_->fb.P4;
	if (test_marker) {
		description = touch_marker_->fb.D4;
		path_time = touch_marker_->fb.P4_time;
		EvalPath();
	}
	test_marker = touch_marker_->fb.P5;
	if (test_marker) {
		description = touch_marker_->fb.D5;
		path_time = touch_marker_->fb.P5_time;
		EvalPath();
	}
	test_marker = touch_marker_->fb.P6;
	if (test_marker) {
		description = touch_marker_->fb.D6;
		path_time = touch_marker_->fb.P6_time;
		EvalPath();
	}
	test_marker = touch_marker_->fb.P7;
	if (test_marker) {
		description = touch_marker_->fb.D7;
		path_time = touch_marker_->fb.P7_time;
		EvalPath();
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
								self->fb.old_linked_marker = world;
							}
						}
					}
				}
				return;
			}
		}
	}
	if (self->fb.state & WAIT) {
		if (!look_object_->fb.client_) {
			//traceline(linked_marker_->s.v.absmin + linked_marker_->s.v.view_ofs + '0 0 32', look_object_->s.v.absmin + look_object_->s.v.view_ofs + '0 0 32', TRUE, self);
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
			self->fb.state = self->fb.state - (self->fb.state & WAIT);
		}
	}
	if (streq(g_globalvars.mapname, "dm3")) {
		if (numberofclients > 1) {
			if (teamplay && deathmatch <= 3) {
				if ((int)self->s.v.items & (IT_ROCKET_LAUNCHER | IT_LIGHTNING) && !self->fb.bot_evade) {
					if ((self->s.v.health > 60) && (self->s.v.armorvalue > 80)) {
						if ((self->s.v.ammo_cells > 15) || (self->s.v.ammo_rockets > 3)) {
							search_entity = ez_find(world, "item_artifact_super_damage");
							if (search_entity != world) {
								if (random() < 0.5) {
									if (search_entity->s.v.origin[2] <= self->s.v.origin[2] + 18) {
										vec3_t diff;
										VectorSubtract(search_entity->s.v.origin, self->s.v.origin, diff);
										if (vlen(diff) < 200) {
											if (random() < 0.9) {
												self->fb.camp_state |= CAMPBOT;
												linked_marker_ = touch_marker_;
											}
										}
										else  {
											self->fb.camp_state = self->fb.camp_state - (self->fb.camp_state & CAMPBOT);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else if (streq(g_globalvars.mapname, "dm4")) {
		if (duel_mode) {
			if ((int)self->s.v.items & (IT_ROCKET_LAUNCHER | IT_LIGHTNING) && !self->fb.bot_evade) {
				if ((self->s.v.health > 50) && (self->s.v.armorvalue > 30)) {
					if ((self->s.v.ammo_cells > 15) || (self->s.v.ammo_rockets > 3)) {
						if (random() < 0.985) {
							vec3_t above_lg = { 448, -176, 60 };
							vec3_t on_quad_stairs = { 280, -330, 60 };

							if ((enemy_->s.v.origin[0] < 700) && (VectorDistance(above_lg, self->s.v.origin) < 200)) {
								self->fb.camp_state |= CAMPBOT;
								linked_marker_ = touch_marker_;
							}
							else if ((enemy_->s.v.origin[0] >= 700) && (VectorDistance(on_quad_stairs, self->s.v.origin) < 200)) {
								self->fb.camp_state |= CAMPBOT;
								linked_marker_ = touch_marker_;
							}
							else  {
								self->fb.camp_state &= ~CAMPBOT;
							}
						}
					}
				}
			}
		}
	}
	else if (streq(g_globalvars.mapname, "dm6")) {
		// Camp the red armor...
		if (numberofclients > 1) {
			if ((int)self->s.v.items & (IT_ROCKET_LAUNCHER | IT_LIGHTNING) && !self->fb.bot_evade) {
				if ((self->s.v.health > 80) && (self->s.v.armorvalue > 100)) {
					if ((self->s.v.ammo_cells > 15) || (self->s.v.ammo_rockets > 3)) {
						search_entity = ez_find(world, "item_armorInv");
						if (search_entity != world) {
							if (search_entity->s.v.origin[2] <= self->s.v.origin[2] + 18) {
								if (VectorDistance(search_entity->s.v.origin, self->s.v.origin) < 200) {
									if (random() < 0.9) {
										self->fb.camp_state = self->fb.camp_state | CAMPBOT;
										linked_marker_ = touch_marker_;
									}
								}
								else  {
									self->fb.camp_state = self->fb.camp_state - (self->fb.camp_state & CAMPBOT);
								}
							}
						}
					}
				}
			}
		}
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
	if (goalentity_marker->fb.Z_ == 1) {
		if (touch_marker_->fb.zoneTask[0] & DM6_DOOR) {
			if (dm6_door->s.v.takedamage) {
				vec3_t temp, src;
				VectorAdd(dm6_door->s.v.absmin, dm6_door->s.v.view_ofs, temp);
				VectorSubtract(temp, origin_, temp);
				temp[2] -= 40;
				normalize(temp, direction);
				VectorCopy(origin_, src);
				src[2] += 16;
				traceline(src[0], src[1], src[2], src[0] + direction[0] * 2048, src[1] + direction[1] * 2048, src[2] + direction[2] * 2048, FALSE, self);
				if (g_globalvars.trace_ent == NUM_FOR_EDICT(dm6_door)) {
					self->fb.path_state |= DM6_DOOR;
				}
			}
			else  {
				self->fb.path_state &= ~DM6_DOOR;
			}
		}
	}
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
			self->fb.button2_ = TRUE;
		}
		return;
	}
	if (self->fb.path_state & DM6_DOOR) {
		self->fb.state = self->fb.state | NOTARGET_ENEMY;
		look_object_ = dm6_door;
		self->fb.look_object = look_object_;
		return;
	}
	if (look_object_->fb.client_) {
		return;
	}
	if ((self->s.v.waterlevel == 2) || (self->s.v.waterlevel == 1)) {
		self->fb.look_object = self->fb.linked_marker;
		return;
	}
	if (random() < self->fb.look_anywhere) {
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
				SightMarker();
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
	from_marker = linked_marker_->fb.P0;
	if (from_marker) {
		EvalLook();
	}
	from_marker = linked_marker_->fb.P1;
	if (from_marker) {
		EvalLook();
	}
	from_marker = linked_marker_->fb.P2;
	if (from_marker) {
		EvalLook();
	}
	from_marker = linked_marker_->fb.P3;
	if (from_marker) {
		EvalLook();
	}
	from_marker = linked_marker_->fb.P4;
	if (from_marker) {
		EvalLook();
	}
	from_marker = linked_marker_->fb.P5;
	if (from_marker) {
		EvalLook();
	}
	from_marker = linked_marker_->fb.P6;
	if (from_marker) {
		EvalLook();
	}
	from_marker = linked_marker_->fb.P7;
	if (from_marker) {
		EvalLook();
	}
	self->fb.look_object = look_object_;
	self->fb.predict_shoot = FALSE;
}

