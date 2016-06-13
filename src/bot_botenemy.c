// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

// Removes the look object for the given player
void ClearLookObject(gedict_t* player) {
	player->fb.track_pitchspeed = player->fb.track_yawspeed = player->fb.pitchaccel = player->fb.yawaccel = 0;
	player->fb.look_object = NULL;
}

// Sets the look object for the player
void LookEnemy(gedict_t* player, gedict_t* enemy) {
	look_object_ = player->fb.look_object = enemy;
	VectorCopy(player->s.v.origin, player->fb.predict_origin);
	self->fb.old_linked_marker = NULL;
}

void ResetEnemy(gedict_t* self) {
	gedict_t* test_enemy = NULL;

	// Find all players who consider the current player their enemy, and clear it
	for (test_enemy = world; test_enemy = find_plr(test_enemy); ) {
		if (test_enemy->s.v.enemy == NUM_FOR_EDICT(self)) {
			test_enemy->s.v.enemy = NUM_FOR_EDICT(world);

			// Clear look object as well
			if (test_enemy->fb.look_object && test_enemy->fb.look_object->ct == ctPlayer) {
				ClearLookObject(test_enemy);
				test_enemy->fb.look_object = NULL;
			}

			// Clear goal entity (if we were hunting them down)
			if (test_enemy->s.v.goalentity == NUM_FOR_EDICT(self)) {
				test_enemy->fb.goal_refresh_time = 0;
			}
		}
	}

	self->s.v.enemy = NUM_FOR_EDICT(world);
}

void CheckCombatEnemy(gedict_t* attacker, gedict_t* targ) {
	// if object we're looking at has less firepower than us...
	if (targ->fb.look_object && targ->fb.look_object->fb.firepower < attacker->fb.firepower) {
		if (attacker != targ) {
			// look at the attacker
			targ->fb.look_object = attacker;
			VectorCopy(attacker->s.v.origin, targ->fb.predict_origin);

			if (! SameTeam(attacker, targ)) {
				if (targ->s.v.goalentity == targ->s.v.enemy) {
					targ->fb.goal_refresh_time = 0;
				}
				targ->fb.enemy_time = g_globalvars.time + 1;
				targ->s.v.enemy = NUM_FOR_EDICT(attacker);
			}
			else {
				targ->fb.enemy_time = g_globalvars.time + 2.5;
			}
		}
	}
}

// TODO: length of sound is also important?  Can hear direction someone is jumping...
void BotsSoundMade(gedict_t* entity) {
	if (entity && entity->ct == ctPlayer) {
		gedict_t* plr;

		// Find all bots which has this entity as enemy
		for (plr = world; plr = find_plr (plr); ) {
			if (plr->isBot && !(plr->fb.state & NOTARGET_ENEMY)) {
				if (NUM_FOR_EDICT(entity) == plr->s.v.enemy && entity != plr->fb.look_object) {
					vec3_t temp;
					VectorSubtract(entity->s.v.origin, plr->s.v.origin, temp);

					// Did the bot hear it?
					if (VectorLength(temp) < 1000) {
						if (Visible_360 (plr, entity)) {
							// Look directly at the player
							plr->fb.look_object = entity;
						}
						else if (isDuel()) {
							if (entity->fb.touch_marker && Visible_360 (plr, entity->fb.touch_marker)) {
								plr->fb.look_object = entity->fb.touch_marker;
							}
							else {
								// TODO: Look at the closest possible point
								//   (think of picking up mega on DM4, humans would look at mega exit)
								//   (find route from noise -> player, look at first visible marker?)
							}
							
						}
					}
				}
			}
		}
	}
}

// Evaluates a potential enemy (globals: path_normal)
static void BestEnemy_apply(gedict_t* test_enemy, float* best_score, gedict_t** enemy_, float* predict_dist) {
	float enemy_score;

	path_normal = true;

	look_marker = NULL;
	from_marker->fb.sight_from_marker();

	if (look_marker != NULL) {
		look_marker->fb.zone_marker();
		look_marker->fb.sub_arrival_time();
		enemy_score = traveltime + random();
	}
	else {
		SightMarker(from_marker);
		enemy_score = look_traveltime + random();
	}

	if (enemy_score < *best_score) {
		vec3_t marker_view;
		vec3_t to_marker_view;

		VectorAdd(look_marker->s.v.absmin, look_marker->s.v.view_ofs, marker_view);
		VectorAdd(to_marker->s.v.absmin, to_marker->s.v.view_ofs, to_marker_view);

		*best_score = enemy_score;
		*enemy_ = test_enemy;
		*predict_dist = VectorDistance(marker_view, to_marker_view);
	}
}

// Selects best enemy from all players.
//   Evaluates based on time from respective last-touched markers 
//   FIXME: evaluates in either direction (ease vs risk?  fix)
void BestEnemy(gedict_t* self) {
	float best_score = 1000000;
	gedict_t* enemy_ = NULL;
	float predict_dist = 600;
	gedict_t* test_enemy;

	for (test_enemy = world; test_enemy = find_plr(test_enemy); ) {
		if ( ! SameTeam(self, test_enemy) ) {
			from_marker = test_enemy->fb.touch_marker;
			if (from_marker) {
				to_marker = self->fb.touch_marker;
				BestEnemy_apply(test_enemy, &best_score, &enemy_, &predict_dist);
				to_marker = from_marker;
				from_marker = self->fb.touch_marker;
				BestEnemy_apply(test_enemy, &best_score, &enemy_, &predict_dist);
			}
		}
	}

	self->fb.enemy_time = g_globalvars.time + 1;
	self->s.v.enemy = (enemy_ == NULL ? 0 : NUM_FOR_EDICT(enemy_));
	self->fb.enemy_dist = predict_dist;
}
