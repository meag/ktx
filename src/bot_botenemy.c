// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void ClearLookObject(gedict_t* client) {
	client->fb.track_pitchspeed = client->fb.track_yawspeed = client->fb.pitchaccel = client->fb.yawaccel = 0;
	client->fb.look_object = world;
}

void LookEnemy() {
	look_object_ = self->fb.look_object = enemy_;
	VectorCopy(enemy_->s.v.origin, self->fb.predict_origin);
	self->fb.old_linked_marker = world;
}

// TODO: Call this when any player dies (see .qc)
void ResetEnemy() {
	// Find all players who consider the current player their enemy, and clear it
	for (test_enemy = find_plr(world); test_enemy; test_enemy = find_plr(test_enemy) ) {
		if (test_enemy->s.v.enemy == NUM_FOR_EDICT(self)) {
			test_enemy->s.v.enemy = NUM_FOR_EDICT(world);

			// Clear look object as well
			if (test_enemy->fb.look_object->ct == ctPlayer) {
				ClearLookObject(test_enemy);
				test_enemy->fb.look_object = world;
			}

			// Clear goal entity (if we were hunting them down)
			if (test_enemy->s.v.goalentity == NUM_FOR_EDICT(self)) {
				test_enemy->fb.goal_refresh_time = 0;
			}
		}
	}

	self->s.v.enemy = NUM_FOR_EDICT(world);
}

void CheckCombatEnemy() {
	// if object we're looking at has less firepower than us...
	if (targ->fb.look_object->fb.firepower < attacker->fb.firepower) {
		// ?  targ would have to be looking 
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
			else  {
				targ->fb.enemy_time = g_globalvars.time + 2.5;
			}
		}
	}
}

void check_sound() {
	if (enemy_->ct == ctPlayer) {
		self_sound = self;
		for (self = find_plr(world); self; self = find_plr(self)) {
			if (!(self->fb.state & NOTARGET_ENEMY)) {
				if (NUM_FOR_EDICT(enemy_) == self->s.v.enemy) {
					if (enemy_ != self->fb.look_object) {
						vec3_t temp;
						VectorSubtract(enemy_->s.v.origin, self->s.v.origin, temp);
						if (VectorLength(temp) < 1000) {
							visible_object = enemy_;
							Visible_360();
							if (enemy_visible) {
								self->fb.look_object = enemy_;
							}
						}
					}
				}
			}
		}
		self = self_sound;
	}
}

void BestEnemy_apply() {
	from_marker->fb.sight_from_marker();
	if (look_marker) {
		look_marker->fb.zone_marker();
		look_marker->fb.sub_arrival_time();
		enemy_score = traveltime + random();
	}
	else  {
		SightMarker();
		enemy_score = look_traveltime + random();
	}
	if (enemy_score < best_score) {
		vec3_t marker_view;
		vec3_t to_marker_view;

		VectorAdd(look_marker->s.v.absmin, look_marker->s.v.view_ofs, marker_view);
		VectorAdd(to_marker->s.v.absmin, to_marker->s.v.view_ofs, to_marker_view);

		best_score = enemy_score;
		enemy_ = test_enemy;
		predict_dist = VectorDistance(marker_view, to_marker_view);
	}
}

void BestEnemy() {
	best_score = 1000000;
	enemy_ = world;
	predict_dist = 600;
	path_normal = TRUE;
	test_enemy = first_client;
	while (test_enemy) {
		if ( ! SameTeam(self, test_enemy) ) {
			from_marker = test_enemy->fb.touch_marker;
			if (from_marker) {
				to_marker = touch_marker_;
				BestEnemy_apply();
				to_marker = from_marker;
				from_marker = touch_marker_;
				BestEnemy_apply();
			}
		}
		test_enemy = test_enemy->fb.next;
	}
	self->fb.enemy_time = g_globalvars.time + 1;
	self->s.v.enemy = NUM_FOR_EDICT(enemy_);
	self->fb.enemy_dist = predict_dist;
}

