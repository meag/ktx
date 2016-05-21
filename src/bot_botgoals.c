// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

//static float best_score;

#define BACKPACK_CLASSNAME "backpack"
#define FROGBOT_CHANCE_HELP_TEAMMATE 0.25

void POVDMM4DontWalkThroughDoor (gedict_t* goal_entity);
qbool DM6DoorLogic (gedict_t* self, gedict_t* goal_entity);

// Called by BotPlayerDeathEvent
void ResetGoalEntity(gedict_t* self) {
	if (self->s.v.goalentity) {
		gedict_t* ent = &g_edicts[self->s.v.goalentity];
		ent->fb.teamflag -= (ent->fb.teamflag & self->fb.teamflag);
		self->s.v.goalentity = NUM_FOR_EDICT(world);
	}
}

// If an item is picked up, all bots heading for that item should re-evaluate their goals
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

static qbool GoalLeaveForTeammate (gedict_t* self, gedict_t* goal_entity) {
	if (g_globalvars.time < goal_entity->fb.touchPlayerTime) {
		if (goal_entity->s.v.nextthink > g_globalvars.time && SameTeam(goal_entity, self) ) {
			if (goal_entity->fb.touchPlayer != self) {
				goal_entity->fb.saved_goal_desire = 0;
				return true;
			}
		}
	}

	return false;
}

// Evaluates a goal 
static void EvalGoal(gedict_t* self, gedict_t* goal_entity) {
	float goal_desire = goal_entity && goal_entity->fb.desire ? goal_entity->fb.desire (self) : 0;
	float goal_time = 0.0f;
	
	if (!goal_entity)
		return;

	goal_entity->fb.saved_goal_desire = goal_desire;
	if (goal_desire > 0) {
		// TODO: there are two doors open? :)
		if (streq(g_globalvars.mapname, "povdmm4")) {
			POVDMM4DontWalkThroughDoor (goal_entity);
		}

		// If one person on a team is sitting waiting for an item to respawn
		if (GoalLeaveForTeammate (self, goal_entity)) {
			return;
		}

		// Calculate travel time to the goal
		from_marker = self->fb.touch_marker;
		to_marker = goal_entity->fb.touch_marker;
		to_marker->fb.zone_marker();
		to_marker->fb.sub_arrival_time();
		goal_time = traveltime;

		if (self->fb.goal_enemy_repel) {
			// Time for our enemy to get there
			from_marker = enemy_touch_marker;
			to_marker->fb.zone_marker();
			to_marker->fb.sub_arrival_time();
			// If enemy will get there much faster than we will...
			if (traveltime <= (goal_time - 1.25)) {
				goal_desire += self->fb.goal_enemy_repel;
				goal_entity->fb.saved_goal_desire = goal_desire;
				if (goal_desire <= 0) {
					return;
				}
			}
		}

		if (DM6DoorLogic (self, goal_entity)) {
			return;
		}

		goal_entity->fb.saved_respawn_time += (goal_time * self->fb.skill.prediction_error * random()) - g_globalvars.time;
		if (goal_time < goal_entity->fb.saved_respawn_time) {
			goal_time = goal_entity->fb.saved_respawn_time;
		}
		goal_entity->fb.saved_goal_time = goal_time;
		if (self->fb.bot_evade) {
			if (self->fb.goal_enemy_repel) {
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
			if (goal_score > self->fb.best_goal_score) {
				self->fb.best_goal_score = goal_score;
				self->fb.best_goal = goal_entity;
			}
		}
	}
}

// FIXME: parameters
static void EvalGoal2(gedict_t* goal_entity, gedict_t* best_goal_marker) {
	float goal_desire = 0.0f;
	float traveltime2 = 0.0f;

	if (goal_entity == NULL)
		return;

	goal_desire = goal_entity->fb.saved_goal_desire;
	if (goal_desire > 0) {
		float goal_time2 = goal_entity->fb.saved_goal_time;
		if (goal_time2 <= 5) {
			gedict_t* goal_marker2 = goal_entity->fb.touch_marker;
			from_marker = goal_marker2;
			best_goal_marker->fb.zone_marker();
			best_goal_marker->fb.sub_arrival_time();
			traveltime2 = max(best_respawn_time, goal_time2 + traveltime);

			if (self->fb.bot_evade && self->fb.goal_enemy_repel) {
				if ((enemy_time_squared + traveltime2 * traveltime2) >= self->fb.best_goal->fb.saved_enemy_time_squared) {
					traveltime2 = 1000000;
				}
			}

			if (traveltime2 < lookahead_time_) {
				goal_score2 = (goal_desire * (lookahead_time_ - goal_time2) / (goal_time2 + 5)) + (self->fb.best_goal->fb.saved_goal_desire * (lookahead_time_ - traveltime2) / (traveltime2 + 5));
				if (goal_score2 > self->fb.best_score2) {
					self->fb.best_score2 = goal_score2;
					self->fb.best_goal2 = goal_entity;
				}
			}
			from_marker = best_goal_marker;
			goal_marker2->fb.zone_marker();
			goal_marker2->fb.sub_arrival_time();
			traveltime2 = self->fb.best_goal_time + traveltime;
			if (traveltime2 < goal_entity->fb.saved_respawn_time) {
				traveltime2 = goal_entity->fb.saved_respawn_time;
			}
			if (self->fb.bot_evade && self->fb.goal_enemy_repel) {
				if ((enemy_time_squared + traveltime2 * traveltime2) >= goal_entity->fb.saved_enemy_time_squared) {
					return;
				}
			}
			if (traveltime2 < lookahead_time_) {
				goal_score2 = self->fb.best_goal_score + (goal_desire * (lookahead_time_ - traveltime2) / (traveltime2 + 5));
				if (goal_score2 > self->fb.best_score2) {
					self->fb.best_score2 = goal_score2;
					self->fb.best_goal2 = self->fb.best_goal;
				}
			}
		}
	}
}

static void EnemyGoalLogic (gedict_t* self)
{
	float best_goal_desire = self->fb.best_goal->fb.saved_goal_desire;
	float best_goal_time = self->fb.best_goal->fb.saved_goal_time;
	gedict_t* best_goal_marker = self->fb.best_goal->fb.touch_marker;
	float best_respawn_time = self->fb.best_goal->fb.saved_respawn_time;

	self->fb.best_goal->fb.saved_goal_desire = 0;
	if (self->fb.goal_enemy_desire > 0) {
		float goal_time2 = enemy_->fb.saved_goal_time;
		if (goal_time2 <= 5) {
			// Work out time to enemy marker
			gedict_t* goal_marker2 = enemy_->fb.touch_marker;
			float traveltime2 = 0.0f;
			from_marker = goal_marker2;
			best_goal_marker->fb.zone_marker();
			best_goal_marker->fb.sub_arrival_time();
			traveltime2 = max(goal_time2 + traveltime, best_respawn_time);

			if (traveltime2 < lookahead_time_) {
				goal_score2 = (self->fb.goal_enemy_desire * (lookahead_time_ - goal_time2) / (goal_time2 + 5)) + (best_goal_desire * (lookahead_time_ - traveltime2) / (traveltime2 + 5));
				if (goal_score2 > self->fb.best_score2) {
					self->fb.best_score2 = goal_score2;
					self->fb.best_goal2 = enemy_;
				}
			}

			// Work out time to best goal marker
			from_marker = best_goal_marker;
			goal_marker2->fb.zone_marker();
			goal_marker2->fb.sub_arrival_time();
			traveltime2 = max(best_goal_time + traveltime, enemy_->fb.saved_respawn_time);

			if (traveltime2 < lookahead_time_) {
				goal_score2 = self->fb.best_goal_score + (self->fb.goal_enemy_desire * (lookahead_time_ - traveltime2) / (traveltime2 + 5));
				if (goal_score2 > self->fb.best_score2) {
					self->fb.best_score2 = goal_score2;
					self->fb.best_goal2 = self->fb.best_goal;
				}
			}
		}
	}
}

void UpdateGoal() {
	int i = 0;
	int items_ = self->s.v.items;

	self->fb.goal_refresh_time = g_globalvars.time + 2 + random();
	prediction_error_ = self->fb.skill.prediction_error;
	enemy_ = &g_edicts[self->s.v.enemy];
	enemy_touch_marker = enemy_->fb.touch_marker;
	
	self->fb.best_goal_score = 0;
	self->fb.best_goal = NULL;
	self->fb.goal_enemy_repel = self->fb.goal_enemy_desire = 0;

	BotEvadeLogic(self);

	if (enemy_touch_marker) {
		virtual_enemy = enemy_;
		self->fb.goal_enemy_desire = enemy_ && enemy_->fb.desire ? enemy_->fb.desire(self) : 0;
		if (self->fb.goal_enemy_desire > 0) {
			// Time from here to the enemy's last marker
			from_marker = touch_marker_;
			enemy_touch_marker->fb.zone_marker();
			enemy_touch_marker->fb.sub_arrival_time();
			enemy_->fb.saved_respawn_time = 0;
			enemy_->fb.saved_goal_time = traveltime;

			if (traveltime < lookahead_time_) {
				goal_score = self->fb.goal_enemy_desire * (lookahead_time_ - traveltime) / (traveltime + 5);
				if (goal_score > self->fb.best_goal_score) {
					self->fb.best_goal_score = goal_score;
					self->fb.best_goal = enemy_;
					enemy_->fb.saved_goal_desire = self->fb.goal_enemy_desire;
				}
			}
		}
		else if (enemy_->s.v.enemy == NUM_FOR_EDICT(self)) {
			// our enemy is after us...
			self->fb.goal_enemy_repel = self->fb.goal_enemy_desire;
		}
	}
	else  {
		virtual_enemy = dropper;
	}

	if (self->fb.debug)
		G_bprint (2, "After enemy-eval: best_goal %s, best_score %f\n", self->fb.best_goal ? self->fb.best_goal->s.v.classname : "(none)", self->fb.best_goal_score);
	for (i = 0; i < NUMBER_GOALS; ++i) {
		EvalGoal(self, touch_marker_->fb.goals[i].next_marker->fb.virtual_goal);
	}
	if (self->fb.debug)
		G_bprint (2, "After goal-eval1: best_goal %s, best_score %f\n", self->fb.best_goal ? self->fb.best_goal->s.v.classname : "(none)", self->fb.best_goal_score);

	for (goal_entity = world; goal_entity = ez_find(goal_entity, BACKPACK_CLASSNAME); ) {
		if (goal_entity->fb.touch_marker) {
			EvalGoal(self, goal_entity);
		}
	}
	if (self->fb.debug)
		G_bprint (2, "After backpacks:  best_goal %s, best_score %f\n", self->fb.best_goal ? self->fb.best_goal->s.v.classname : "(none)", self->fb.best_goal_score);

	if (teamplay && !isRA()) {
		search_entity = HelpTeammate();
		if (search_entity && random() < FROGBOT_CHANCE_HELP_TEAMMATE) {
			self->fb.best_goal = search_entity;
		}
	}

	if (self->fb.best_goal) {
		self->fb.best_goal2 = self->fb.best_goal;
		self->fb.best_score2 = self->fb.best_goal_score;

		EnemyGoalLogic (self);
		if (self->fb.debug)
			G_bprint (2, "After enemy-logic:  best_goal %s, best_score %f\n", self->fb.best_goal2 ? self->fb.best_goal2->s.v.classname : "(none)", self->fb.best_score2);

		for (i = 0; i < NUMBER_GOALS; ++i) {
			if (touch_marker_->fb.goals[i].next_marker) {
				EvalGoal2 (touch_marker_->fb.goals[i].next_marker->fb.virtual_goal, self->fb.best_goal->fb.touch_marker);
			}
		}
		if (self->fb.debug)
			G_bprint (2, "After EvalGoal2:  best_goal %s, best_score %f\n", self->fb.best_goal2 ? self->fb.best_goal2->s.v.classname : "(none)", self->fb.best_score2);

		for (goal_entity = world; goal_entity = ez_find(goal_entity, BACKPACK_CLASSNAME); ) {
			if (goal_entity->fb.touch_marker) {
				EvalGoal2 (goal_entity, self->fb.best_goal->fb.touch_marker);
			}
		}
		if (self->fb.debug)
			G_bprint (2, "After EvalGoal2-BP:  best_goal %s, best_score %f\n", self->fb.best_goal2 ? self->fb.best_goal2->s.v.classname : "(none)", self->fb.best_score2);

		self->s.v.goalentity = NUM_FOR_EDICT(self->fb.best_goal2);
		self->fb.goal_respawn_time = g_globalvars.time + self->fb.best_goal2->fb.saved_respawn_time;
	}
	else {
		self->s.v.goalentity = NUM_FOR_EDICT(world);
	}

	if (self->fb.debug)
		G_bprint (2, "UpdateGoal() => %s\n", g_edicts[self->s.v.goalentity].s.v.classname);

	self->fb.debug = false;
}

