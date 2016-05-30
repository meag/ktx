// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

#define BOT_DROWN_SAFETY_TIME           2  // Time before air running out that the bot starts searching for air

static qbool BotCanReachMarker(gedict_t* self) {
	vec3_t spot1,
	       spot2;
	VectorCopy(self->fb.linked_marker->s.v.origin, spot2);
	VectorAdd(self->s.v.origin, self->s.v.view_ofs, spot1);
	traceline(spot1[0], spot1[1], spot1[2], spot2[0], spot2[1], spot2[2], true, self);
	return (g_globalvars.trace_fraction == 1);
}

static qbool BotSwimDown(gedict_t* self) {
	return (self->fb.linked_marker->s.v.origin[2] < self->s.v.origin[2]);
}

static qbool BotSwimUp(gedict_t* self) {
	return (self->fb.linked_marker->s.v.origin[2] >= self->s.v.origin[2]);
}

static qbool BotGoUpForAir(gedict_t* self, vec3_t dir_move) {
	vec3_t temp;

	if (g_globalvars.time > (self->air_finished - BOT_DROWN_SAFETY_TIME)) {
		traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 64, true, self);
		if (g_globalvars.trace_fraction == 1) {
			return (self->fb.swim_arrow = UP);
		}

		VectorCopy(self->s.v.velocity, new_velocity);
		VectorNormalize(dir_move);
		VectorCopy(new_velocity, temp);
		VectorNormalize(temp);
		VectorAdd(dir_move, temp, dir_move);
		dir_move[2] = 0;
		NewVelocityForArrow(self, dir_move);

		// Drowning...
		if (g_globalvars.time > self->air_finished) {
			traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 32, true, self);
			if (g_globalvars.trace_fraction != 1) {
				return (self->fb.swim_arrow = UP);
			}
		}
	}
	return false;
}

static void SwimAwayFromWall(vec3_t dir_move) {
	if (DotProduct(self->fb.obstruction_normal, rel_pos) > 0.5) {
		VectorScale(dir_move, -1, dir_move);
	}
	traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0] + g_globalvars.v_right[0] * 20, self->s.v.origin[1] + g_globalvars.v_right[1] * 20, self->s.v.origin[2] + g_globalvars.v_right[2] * 20, true, self);
	if (g_globalvars.trace_fraction != 1) {
		vec3_t temp;

		VectorNormalize(dir_move);
		VectorScale(g_globalvars.v_right, random() * -32, temp);
		VectorAdd(temp, dir_move, temp);
		normalize(temp, dir_move);
	}
	traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0] + g_globalvars.v_right[0] * (-20), self->s.v.origin[1] + g_globalvars.v_right[1] * (-20), self->s.v.origin[2] + g_globalvars.v_right[2] * (-20), true, self);
	if (g_globalvars.trace_fraction != 1) {
		vec3_t temp;

		VectorNormalize(dir_move);
		VectorScale(g_globalvars.v_right, random() * 32, temp);
		VectorAdd(temp, dir_move, temp);
		normalize(temp, dir_move);
	}
	NewVelocityForArrow(self, dir_move);
}

void BotWaterMove(gedict_t* self) {
	vec3_t dir_move;
	VectorCopy (self->fb.dir_move_, dir_move);
	
	self->fb.swim_arrow = 0;

	if (self->s.v.waterlevel <= 2 || g_globalvars.time < self->fb.frogwatermove_time) {
		return;
	}

	self->fb.frogwatermove_time = self->fb.frogbot_nextthink + 0.1;
	if (self->fb.obstruction_normal[0] || self->fb.obstruction_normal[1] || self->fb.obstruction_normal[2]) {
		SwimAwayFromWall(dir_move);
	}
	if (BotGoUpForAir (self, dir_move)) {
		VelocityForArrow(self);
		return;
	}

	if (BotCanReachMarker(self)) {
		if (BotSwimDown(self)) {
			self->fb.swim_arrow = DOWN;
		}
		else if (BotSwimUp(self)) {
			self->fb.swim_arrow = UP;
		}
	}
	else  {
		traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 32, true, self);
		if (g_globalvars.trace_fraction == 1) {
			self->fb.swim_arrow = UP;
		}
		else  {
			traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + -32, true, self);
			if (g_globalvars.trace_fraction == 1) {
				self->fb.swim_arrow = DOWN;
			}
			else {
				self->fb.swim_arrow = UP;
			}
		}
	}

	VelocityForArrow(self);
}

// botweap.qc
// FIXME: should still discharge if < 25 cells and would kill enemy...
qbool BotShouldDischarge (void)
{
	float n;
	gedict_t* p;
	if (self->s.v.waterlevel != 3) {
		return false;
	}
	if (!((int)self->s.v.items & IT_LIGHTNING)) {
		return false;
	}
	if (self->s.v.ammo_cells < 25) {
		return false;
	}
	if (self->fb.enemy_dist > 600) {
		return false;
	}
	if (look_object_ != enemy_) {
		return false;
	}
	if (self->invincible_time > g_globalvars.time) {
		if (trap_pointcontents (PASSVEC3 (enemy_->s.v.origin)) == CONTENT_WATER) {
			return true;
		}
	}
	if (((int)self->s.v.items & IT_ROCKET_LIGHTNING) && (self->s.v.ammo_rockets > 25) && (self->s.v.ammo_cells > 25)) {
		return false;
	}
	if (((int)self->s.v.items & IT_NAILGUN_ROCKET) && (self->s.v.ammo_rockets > 25) && (self->s.v.ammo_nails > 25)) {
		return false;
	}
	n = 0;

	for (p = world; p = trap_findradius (p, self->s.v.origin, 1000); ) {
		if (trap_pointcontents (PASSVEC3 (p->s.v.origin)) == CONTENT_WATER) {
			if (p->ct == ctPlayer) {
				if (p->s.v.takedamage) {
					if (IsVisible (p)) {
						if (!SameTeam (p, self)) {
							n = n + 1;
						}
						else {
							n = n - 1;
						}
					}
				}
			}
		}
	}
	if (n >= 2) {
		return (random () < 0.003);
	}
	return false;
}

// client.qc
void BotWaterJumpFix() {
	if (self->isBot) {
		self->fb.tread_water_count = self->fb.tread_water_count + 1;
		if (self->fb.tread_water_count > 60) {
			self->fb.tread_water_count = 0;
			self->fb.old_linked_marker = NULL;
			self->fb.linked_marker = LocateMarker(self->s.v.origin);
			self->fb.path_state = 0;
			self->fb.linked_marker_time = g_globalvars.time + 5;
		}
	}
}

