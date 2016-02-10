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

void ResetEnemy() {
	test_enemy = first_client;
	while (test_enemy) {
		if (test_enemy->s.v.enemy == NUM_FOR_EDICT(self)) {
			test_enemy->s.v.enemy = NUM_FOR_EDICT(world);
			if (test_enemy->fb.look_object->fb.client_) {
				ClearLookObject(test_enemy);
				test_enemy->fb.look_object = world;
			}
			if (test_enemy->s.v.goalentity == NUM_FOR_EDICT(self)) {
				test_enemy->fb.goal_refresh_time = 0;
			}
		}
		test_enemy = test_enemy->fb.next;
	}
	self->s.v.enemy = NUM_FOR_EDICT(world);
}

void CheckCombatEnemy() {
	if (targ->fb.look_object->fb.firepower < attacker->fb.firepower) {
		if (attacker != targ) {
			targ->fb.look_object = attacker;
			VectorCopy(attacker->s.v.origin, targ->fb.predict_origin);
			if (attacker->fb.realteam != targ->fb.realteam) {
				if (targ->s.v.goalentity == targ->s.v.enemy) {
					targ->fb.goal_refresh_time = 0;
				}
				targ->fb.enemy_time = g_globalvars.time + 1;
				targ->s.v.enemy = NUM_FOR_EDICT(attacker);
			}
			else  {
				targ->fb.enemy_time = g_globalvars.time + 2.5;
				//targ->fb.friend = attacker;	// doesn't appear to ever be read
			}
		}
	}
}

void check_sound() {
	if (enemy_->fb.client_) {
		self_sound = self;
		self = first_client;
		while (self) {
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
			self = self->fb.next;
		}
		self = self_sound;
	}
}
/*
void sound(float chan, char* samp, float vol, float atten) {
	trap_sound(g_globalvars.msg_entity, chan, samp, vol, atten);
	enemy_ = &g_edicts[g_globalvars.msg_entity];
	check_sound();
}*/

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
		if (realteam_ != test_enemy->fb.realteam) {
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

