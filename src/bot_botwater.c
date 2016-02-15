// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

float WaterCombat() {
	if (self->s.v.waterlevel < 2) {
		return TRUE;
	}
	return ((trap_pointcontents(enemy_->s.v.origin[0], enemy_->s.v.origin[1], enemy_->s.v.origin[2]) == CONTENT_WATER) && (enemy_->s.v.origin[2] < origin_[2] - 32));
}

float JumpInWater() {
	return (trap_pointcontents(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] - 64) == CONTENT_WATER);
}

void BotWaterJumpFix() {
	if (self->isBot) {
		self->fb.tread_water_count = self->fb.tread_water_count + 1;
		if (self->fb.tread_water_count > 60) {
			self->fb.tread_water_count = 0;
			self->fb.old_linked_marker = world;
			self->fb.linked_marker = LocateMarker(self->s.v.origin);
			self->fb.path_state = 0;
			self->fb.linked_marker_time = g_globalvars.time + 5;
		}
	}
}

float BotCanReachMarker() {
	vec3_t spot1,
	       spot2;
	VectorCopy(self->fb.linked_marker->s.v.origin, spot2);
	VectorAdd(self->s.v.origin, self->s.v.view_ofs, spot1);
	traceline(spot1[0], spot1[1], spot1[2], spot2[0], spot2[1], spot2[2], TRUE, self);
	return (g_globalvars.trace_fraction == 1);
}

float BotSwimDown() {
	return (self->fb.linked_marker->s.v.origin[2] < self->s.v.origin[2]);
}

float BotSwimUp() {
	return (self->fb.linked_marker->s.v.origin[2] >= self->s.v.origin[2]);
}

float BotGoUpForAir() {
	if (g_globalvars.time > (self->air_finished - 2)) {
		traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 64, TRUE, self);
		if (g_globalvars.trace_fraction == 1) {
			return (self->fb.swim_arrow = UP);
		}
		else  {
			vec3_t temp;

			VectorCopy(self->s.v.velocity, new_velocity);
			VectorNormalize(dir_move);
			VectorCopy(new_velocity, temp);
			VectorNormalize(temp);
			VectorAdd(dir_move, temp, dir_move);
			dir_move[2] = 0;
			NewVelocityForArrow();
		}
		if (g_globalvars.time > self->air_finished) {
			traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 32, TRUE, self);
			if (g_globalvars.trace_fraction != 1) {
				return (self->fb.swim_arrow = UP);
			}
		}
	}
	return FALSE;
}

void FrogTreadWater() {
	if (self->isBot) {
		self->fb.tread_water_count = self->fb.tread_water_count + 1;
		if (self->fb.tread_water_count > 75) {
			self->fb.old_linked_marker = world;
			self->fb.linked_marker = LocateMarker(self->s.v.origin);
			self->fb.path_state = 0;
			self->fb.linked_marker_time = g_globalvars.time + 5;
		}
	}
}

void SwimAwayFromWall() {
	if (DotProduct(self->fb.obstruction_normal, rel_pos) > 0.5) {
		VectorScale(dir_move, -1, dir_move);
	}
	traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0] + g_globalvars.v_right[0] * 20, self->s.v.origin[1] + g_globalvars.v_right[1] * 20, self->s.v.origin[2] + g_globalvars.v_right[2] * 20, TRUE, self);
	if (g_globalvars.trace_fraction != 1) {
		vec3_t temp;

		VectorNormalize(dir_move);
		VectorScale(g_globalvars.v_right, random() * -32, temp);
		VectorAdd(temp, dir_move, temp);
		normalize(temp, dir_move);
	}
	traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0] + g_globalvars.v_right[0] * (-20), self->s.v.origin[1] + g_globalvars.v_right[1] * (-20), self->s.v.origin[2] + g_globalvars.v_right[2] * (-20), TRUE, self);
	if (g_globalvars.trace_fraction != 1) {
		vec3_t temp;

		VectorNormalize(dir_move);
		VectorScale(g_globalvars.v_right, random() * 32, temp);
		VectorAdd(temp, dir_move, temp);
		normalize(temp, dir_move);
	}
	NewVelocityForArrow();
}

void FrogWaterMove() {
	if (self->isBot) {
		if (self->s.v.waterlevel > 2) {
			if (g_globalvars.time < self->fb.frogwatermove_time) {
				return;
			}
			self->fb.frogwatermove_time = self->fb.frogbot_nextthink + 0.1;
			if (self->fb.obstruction_normal[0] || self->fb.obstruction_normal[1] || self->fb.obstruction_normal[2]) {
				SwimAwayFromWall();
			}
			if (BotGoUpForAir()) {
				VelocityForArrow();
				return;
			}
			if (BotCanReachMarker()) {
				if (BotSwimDown()) {
					self->fb.swim_arrow = DOWN;
				}
				else if (BotSwimUp()) {
					self->fb.swim_arrow = UP;
				}
			}
			else  {
				traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 32, TRUE, self);
				if (g_globalvars.trace_fraction == 1) {
					self->fb.swim_arrow = UP;
				}
				else  {
					traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + -32, TRUE, self);
					if (g_globalvars.trace_fraction == 1) {
						self->fb.swim_arrow = DOWN;
					}
					else  {
						self->fb.swim_arrow = UP;
					}
				}
			}
			VelocityForArrow();
		}
	}
}

float BotShouldDischarge() {
	float n;
	gedict_t* p;
	if (self->s.v.waterlevel != 3) {
		return FALSE;
	}
	if (!((int)self->s.v.items & IT_LIGHTNING)) {
		return FALSE;
	}
	if (self->s.v.ammo_cells < 25) {
		return FALSE;
	}
	if (self->fb.enemy_dist > 600) {
		return FALSE;
	}
	if (look_object_ != enemy_) {
		return FALSE;
	}
	if (self->invincible_time > g_globalvars.time) {
		if (trap_pointcontents(PASSVEC3(enemy_->s.v.origin)) == CONTENT_WATER) {
			return TRUE;
		}
	}
	if (((int)self->s.v.items & IT_ROCKET_LIGHTNING) && (self->s.v.ammo_rockets > 25) && (self->s.v.ammo_cells > 25)) {
		return FALSE;
	}
	if (((int)self->s.v.items & IT_NAILGUN_ROCKET) && (self->s.v.ammo_rockets > 25) && (self->s.v.ammo_nails > 25)) {
		return FALSE;
	}
	n = 0;

	for (p = world; p = trap_findradius(p, self->s.v.origin, 1000); ) {
		if (trap_pointcontents(PASSVEC3(p->s.v.origin)) == CONTENT_WATER) {
			if (p->ct == ctPlayer) {
				if (p->s.v.takedamage) {
					if (IsVisible(p)) {
						if (! SameTeam(p, self)) {
							n = n + 1;
						}
						else  {
							n = n - 1;
						}
					}
				}
			}
		}
	}
	if (n >= 2) {
		if (random() < 0.003) {
			return TRUE;
		}
	}
	return FALSE;
}

