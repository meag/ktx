// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

static float best_score;

// FIXME: Called during client.qc[ResetItems], maybe when player dies?
void ResetGoalEntity(gedict_t* self) {
	if (self->s.v.goalentity) {
		gedict_t* ent = &g_edicts[self->s.v.goalentity];
		ent->fb.teamflag = ent->fb.teamflag - ((int)ent->fb.teamflag & (int)self->fb.teamflag);
		self->s.v.goalentity = NUM_FOR_EDICT(world);
	}
}

// If an item is picked up, all bots heading for that item should re-evaluate their goals
// FIXME: Called during items.qc[{pickup functions}]
void UpdateGoalEntity(gedict_t* item) {
	gedict_t* plr;
	int item_entity = NUM_FOR_EDICT(item);

	for (plr = world; plr = find_plr(plr); ) {
		if (plr->s.v.goalentity == item_entity) {
			float goal_refresh_time_ = g_globalvars.time + random();
			if (plr->fb.goal_refresh_time > goal_refresh_time_) {
				plr->fb.goal_refresh_time = goal_refresh_time_;
			}
		}
	}
}

// Evaluates a goal 
static void EvalGoal(gedict_t* goal_entity) {
	float goal_desire = goal_entity && goal_entity->fb.desire ? goal_entity->fb.desire (self) : 0;
	
	if (!goal_entity)
		return;

	goal_entity->fb.saved_goal_desire = goal_desire;
	if (goal_desire > 0) {
		// TODO: replace aerowalk dynamic_item (quad) with proper quad spawn
		if (streq(g_globalvars.mapname, "aerowalk")) {
			if (! match_in_progress) {
				if (streq(goal_entity->s.v.classname, "dynamic_item")) {
					goal_entity->fb.saved_goal_desire = 0;
					return;
				}
			}
		}

		// TODO: there are two doors open? :)
		if (streq(g_globalvars.mapname, "povdmm4")) {
			if (!door_open) {
				if (streq(goal_entity->s.v.classname, "item_armor2")) {
					goal_entity->fb.saved_goal_desire = 0;
					return;
				}
			}
		}

		// If one person on a team is sitting waiting for an item, assume that they
		//   are going to take it 
		if (g_globalvars.time < goal_entity->fb.touchPlayerTime) {
			if (goal_entity->s.v.nextthink > 0) {
				if ( SameTeam(goal_entity, self) ) {
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
			if (touch_marker_->fb.zones[0].task & DM6_DOOR) {
				if (dm6_door->s.v.takedamage) {
					if (enemy_ == look_object_) {
						if (!self->invincible_time) {
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

// FIXME: parameters
void EvalGoal2() {
	if (goal_entity == NULL)
		return;

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
	int items_ = self->s.v.items;

	self->fb.goal_refresh_time = g_globalvars.time + 2 + random();
	prediction_error_ = self->fb.prediction_error;
	best_score = 0;
	best_goal = NULL;
	enemy_ = &g_edicts[self->s.v.enemy];
	enemy_touch_marker = enemy_->fb.touch_marker;
	enemy_desire = enemy_repel = 0;

	BotEvadeLogic();

	if (enemy_touch_marker) {
		virtual_enemy = enemy_;
		enemy_desire = enemy_ && enemy_->fb.desire ? enemy_->fb.desire(self) : 0;
		if (enemy_desire > 0) {
			from_marker = touch_marker_;
			enemy_touch_marker->fb.zone_marker();
			enemy_touch_marker->fb.sub_arrival_time();
			enemy_->fb.saved_respawn_time = 0;
			enemy_->fb.saved_goal_time = traveltime;
			//G_bprint (2, "Enemy %s/%d, traveltime %f, lookahead_time_ %f, zone_time %f\n", enemy_ ? enemy_->s.v.classname : "?", enemy_touch_marker->fb.index, traveltime, lookahead_time_, zone_time);
			if (traveltime < lookahead_time_) {
				goal_score = enemy_desire * (lookahead_time_ - traveltime) / (traveltime + 5);
				if (goal_score > best_score) {
					best_score = goal_score;
					best_goal = enemy_;
					enemy_->fb.saved_goal_desire = enemy_desire;
				}
			}
		}
		else if (enemy_->s.v.enemy == NUM_FOR_EDICT(self)) {
			enemy_repel = enemy_desire;
		}
	}
	else  {
		virtual_enemy = dropper;
	}

	//G_bprint (2, "After enemy evaluation: best_goal %s, best_score %f\n", best_goal ? best_goal->s.v.classname : "(none)", best_score);
	for (i = 0; i < sizeof(m->fb.goals) / sizeof(m->fb.goals[0]); ++i) {
		EvalGoal(touch_marker_->fb.goals[i].next_marker->fb.virtual_goal);
	}
	//G_bprint (2, "After evaling goals: best_goal %s, best_score %f\n", best_goal ? best_goal->s.v.classname : "(none)", best_score);

	for (goal_entity = world; goal_entity = ez_find(goal_entity, "dynamic_item"); ) {
		if (goal_entity->fb.touch_marker) {
			EvalGoal(goal_entity);
		}
	}

	if (teamplay && !isRA()) {
		search_entity = HelpTeammate();
		if (search_entity && random() < 0.25) {
			best_goal = search_entity;
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

		for (goal_entity = world; goal_entity = ez_find(goal_entity, "dynamic_item"); ) {
			if (goal_entity->fb.touch_marker) {
				EvalGoal2();
			}
		}
		self->s.v.goalentity = NUM_FOR_EDICT(best_goal2);
		self->fb.goal_respawn_time = g_globalvars.time + best_goal2->fb.saved_respawn_time;
	}
	else  {
		self->s.v.goalentity = NUM_FOR_EDICT(world);
	}

	G_bprint (2, "UpdateGoal() => %s\n", g_edicts[self->s.v.goalentity].s.v.classname);
}

