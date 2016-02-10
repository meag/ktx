// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void ResetGoalEntity() {
	goalentity_ = &g_edicts[self->s.v.goalentity];
	if (goalentity_ != world) {
		goalentity_->fb.teamflag = goalentity_->fb.teamflag - ((int)goalentity_->fb.teamflag & (int)self->fb.teamflag);
		self->s.v.goalentity = NUM_FOR_EDICT(world);
	}
}

void UpdateGoalEntity() {
	test_enemy = first_client;
	while (test_enemy) {
		if (test_enemy->s.v.goalentity == NUM_FOR_EDICT(self)) {
			goal_refresh_time_ = g_globalvars.time + random();
			if (test_enemy->fb.goal_refresh_time > goal_refresh_time_) {
				test_enemy->fb.goal_refresh_time = goal_refresh_time_;
			}
		}
		test_enemy = test_enemy->fb.next;
	}
}

void EvalGoal() {
	goal_entity->fb.desire();
	goal_entity->fb.saved_goal_desire = goal_desire;
	if (goal_desire > 0) {
		if (streq(g_globalvars.mapname, "aerowalk")) {
			if (pre_game) {
				if (!game_disable_powerups) {
					if (streq(goal_entity->s.v.classname, "dynamic_item")) {
						goal_entity->fb.saved_goal_desire = 0;
						return;
					}
				}
			}
		}
		if (streq(g_globalvars.mapname, "povdmm4")) {
			if (!door_open) {
				if (streq(goal_entity->s.v.classname, "item_armor2")) {
					goal_entity->fb.saved_goal_desire = 0;
					return;
				}
			}
		}
		if (g_globalvars.time < goal_entity->fb.touchPlayerTime) {
			if (goal_entity->s.v.nextthink > 0) {
				if (goal_entity->fb.touchPlayer->fb.realteam == realteam_) {
					if (goal_entity->fb.touchPlayer != self) {
						goal_entity->fb.saved_goal_desire = 0;
						return;
					}
				}
			}
		}
		from_marker = touch_marker_;
		to_marker = goal_entity->fb.touch_marker;
		to_marker->fb.zone_marker();
		to_marker->fb.sub_arrival_time();
		goal_time = traveltime;
		if (enemy_repel) {
			from_marker = enemy_touch_marker;
			to_marker->fb.zone_marker();
			to_marker->fb.sub_arrival_time();
			if (traveltime <= (goal_time - 1.25)) {
				goal_desire = goal_desire + enemy_repel;
				goal_entity->fb.saved_goal_desire = goal_desire;
				if (goal_desire <= 0) {
					return;
				}
			}
		}
		if (goal_entity->fb.Z_ == 1) {
			if (touch_marker_->fb.zoneTask[0] & DM6_DOOR) {
				if (dm6_door->s.v.takedamage) {
					if (enemy_ == look_object_) {
						if (!self->fb.invincible_time) {
							if (enemy_->fb.firepower >= 50) {
								goal_entity->fb.saved_goal_desire = 0;
								return;
							}
						}
					}
				}
			}
		}
		goal_entity->fb.saved_respawn_time = goal_entity->fb.goal_respawn_time + (goal_time * prediction_error_ * random()) - g_globalvars.time;
		if (goal_time < goal_entity->fb.saved_respawn_time) {
			goal_time = goal_entity->fb.saved_respawn_time;
		}
		goal_entity->fb.saved_goal_time = goal_time;
		if (self->fb.bot_evade) {
			if (enemy_repel) {
				from_marker = enemy_touch_marker;
				to_marker->fb.zone_marker();
				to_marker->fb.sub_arrival_time();
				goal_entity->fb.saved_enemy_time_squared = traveltime * traveltime;
			}
			if ((enemy_time_squared + (goal_time * goal_time)) >= goal_entity->fb.saved_enemy_time_squared) {
				goal_entity->fb.saved_goal_desire = 0;
				return;
			}
		}
		if (goal_time < lookahead_time_) {
			goal_score = goal_desire * (lookahead_time_ - goal_time) / (goal_time + 5);
			if (goal_score > best_score) {
				best_score = goal_score;
				best_goal = goal_entity;
			}
		}
	}
}

void EvalGoal2() {
	goal_desire = goal_entity->fb.saved_goal_desire;
	if (goal_desire > 0) {
		goal_time2 = goal_entity->fb.saved_goal_time;
		if (goal_time2 <= 5) {
			goal_marker2 = goal_entity->fb.touch_marker;
			from_marker = goal_marker2;
			best_goal_marker->fb.zone_marker();
			best_goal_marker->fb.sub_arrival_time();
			traveltime2 = goal_time2 + traveltime;
			if (traveltime2 < best_respawn_time) {
				traveltime2 = best_respawn_time;
			}
			if (self->fb.bot_evade) {
				if (enemy_repel) {
					if ((enemy_time_squared + traveltime2 * traveltime2) >= best_goal->fb.saved_enemy_time_squared) {
						traveltime2 = 1000000;
					}
				}
			}
			if (traveltime2 < lookahead_time_) {
				goal_score2 = (goal_desire * (lookahead_time_ - goal_time2) / (goal_time2 + 5)) + (best_goal_desire * (lookahead_time_ - traveltime2) / (traveltime2 + 5));
				if (goal_score2 > best_score2) {
					best_score2 = goal_score2;
					best_goal2 = goal_entity;
				}
			}
			from_marker = best_goal_marker;
			goal_marker2->fb.zone_marker();
			goal_marker2->fb.sub_arrival_time();
			traveltime2 = best_goal_time + traveltime;
			if (traveltime2 < goal_entity->fb.saved_respawn_time) {
				traveltime2 = goal_entity->fb.saved_respawn_time;
			}
			if (self->fb.bot_evade) {
				if (enemy_repel) {
					if ((enemy_time_squared + traveltime2 * traveltime2) >= goal_entity->fb.saved_enemy_time_squared) {
						return;
					}
				}
			}
			if (traveltime2 < lookahead_time_) {
				goal_score2 = best_score + (goal_desire * (lookahead_time_ - traveltime2) / (traveltime2 + 5));
				if (goal_score2 > best_score2) {
					best_score2 = goal_score2;
					best_goal2 = best_goal;
				}
			}
		}
	}
}

void UpdateGoal() {
	int i = 0;

	self->fb.goal_refresh_time = g_globalvars.time + 2 + random();
	prediction_error_ = self->fb.prediction_error;
	best_score = 0;
	best_goal = world;
	enemy_ = &g_edicts[self->s.v.enemy];
	enemy_touch_marker = enemy_->fb.touch_marker;
	items_ = self->s.v.items;
	enemy_desire = enemy_repel = 0;
	self->fb.bot_evade = FALSE;
	if (deathmatch <= 3 && !game_arena) {
		if (numberofclients == 2) {
			if (random() < 0.08) {
				if ((self->s.v.origin[2] + 18) > (enemy_->s.v.absmin[2] + enemy_->s.v.view_ofs[2])) {
					if ((int)self->s.v.items & IT_ROCKET_LAUNCHER) {
						if (self->s.v.ammo_rockets > 4) {
							if (!self->s.v.waterlevel) {
								if ((self->s.v.health > 70) && (self->s.v.armorvalue > 100) && !enemy_visible) {
									self->fb.bot_evade = TRUE;
								}
							}
						}
					}
				}
			}
		}
		else if (numberofclients > 2) {
			if (random() < 0.1) {
				if ((self->s.v.origin[2] + 18) > (enemy_->s.v.absmin[2] + enemy_->s.v.view_ofs[2])) {
					if (((int)self->s.v.items & IT_ROCKET_LAUNCHER) || ((int)self->s.v.items & IT_LIGHTNING)) {
						if ((self->s.v.ammo_cells >= 20) || (self->s.v.ammo_rockets > 3)) {
							if (!self->s.v.waterlevel) {
								if ((self->s.v.health > 70) && (self->s.v.armorvalue > 90)) {
									if (!((int)self->s.v.items & (IT_INVULNERABILITY | IT_INVISIBILITY | IT_QUAD))) {
										self->fb.bot_evade = TRUE;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (enemy_touch_marker) {
		virtual_enemy = enemy_;
		enemy_->fb.desire();
		if (enemy_desire > 0) {
			from_marker = touch_marker_;
			enemy_touch_marker->fb.zone_marker();
			enemy_touch_marker->fb.sub_arrival_time();
			enemy_->fb.saved_respawn_time = 0;
			enemy_->fb.saved_goal_time = traveltime;
			if (traveltime < lookahead_time_) {
				goal_score = enemy_desire * (lookahead_time_ - traveltime) / (traveltime + 5);
				if (goal_score > best_score) {
					best_score = goal_score;
					best_goal = enemy_;
					enemy_->fb.saved_goal_desire = enemy_desire;
				}
			}
		}
		else  {
			if (enemy_->s.v.enemy == NUM_FOR_EDICT(self)) {
				enemy_repel = enemy_desire;
			}
		}
	}
	else  {
		virtual_enemy = dropper;
	}

	for (i = 0; i < sizeof(m->fb.goals) / sizeof(m->fb.goals[0]); ++i) {
		goal_entity = touch_marker_->fb.goals[i].next_marker->fb.virtual_goal;
		EvalGoal();
	}

	goal_entity = ez_find(world, "dynamic_item");
	while (goal_entity) {
		if (goal_entity->fb.touch_marker) {
			EvalGoal();
		}
		goal_entity = ez_find(goal_entity, "dynamic_item");
	}
	if (teamplay && !game_arena) {
		search_entity = HelpTeammate();
		if (search_entity != world) {
			if (random() < 0.25) {
				best_goal = search_entity;
			}
		}
	}
	if (best_goal) {
		best_goal_desire = best_goal->fb.saved_goal_desire;
		best_goal->fb.saved_goal_desire = 0;
		best_goal_time = best_goal->fb.saved_goal_time;
		best_goal_marker = best_goal->fb.touch_marker;
		best_respawn_time = best_goal->fb.saved_respawn_time;
		best_score2 = best_score;
		best_goal2 = best_goal;
		if (enemy_desire > 0) {
			goal_time2 = enemy_->fb.saved_goal_time;
			if (goal_time2 <= 5) {
				goal_marker2 = enemy_->fb.touch_marker;
				from_marker = goal_marker2;
				best_goal_marker->fb.zone_marker();
				best_goal_marker->fb.sub_arrival_time();
				traveltime2 = goal_time2 + traveltime;
				if (traveltime2 < best_respawn_time) {
					traveltime2 = best_respawn_time;
				}
				if (traveltime2 < lookahead_time_) {
					goal_score2 = (enemy_desire * (lookahead_time_ - goal_time2) / (goal_time2 + 5)) + (best_goal_desire * (lookahead_time_ - traveltime2) / (traveltime2 + 5));
					if (goal_score2 > best_score2) {
						best_score2 = goal_score2;
						best_goal2 = enemy_;
					}
				}
				from_marker = best_goal_marker;
				goal_marker2->fb.zone_marker();
				goal_marker2->fb.sub_arrival_time();
				traveltime2 = best_goal_time + traveltime;
				if (traveltime2 < enemy_->fb.saved_respawn_time) {
					traveltime2 = enemy_->fb.saved_respawn_time;
				}
				if (traveltime2 < lookahead_time_) {
					goal_score2 = best_score + (enemy_desire * (lookahead_time_ - traveltime2) / (traveltime2 + 5));
					if (goal_score2 > best_score2) {
						best_score2 = goal_score2;
						best_goal2 = best_goal;
					}
				}
			}
		}

		for (i = 0; i < sizeof(m->fb.goals) / sizeof(m->fb.goals[0]); ++i) {
			goal_entity = touch_marker_->fb.goals[i].next_marker->fb.virtual_goal;
			EvalGoal2();
		}

		goal_entity = ez_find(world, "dynamic_item");
		while (goal_entity) {
			if (goal_entity->fb.touch_marker) {
				EvalGoal2();
			}
			goal_entity = ez_find(goal_entity, "dynamic_item");
		}
		goalentity_ = best_goal2;
		self->s.v.goalentity = NUM_FOR_EDICT(best_goal2);
		self->fb.goal_respawn_time = g_globalvars.time + best_goal2->fb.saved_respawn_time;
	}
	else  {
		goalentity_ = world;
		self->s.v.goalentity = NUM_FOR_EDICT(world);
	}
}

