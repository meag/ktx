// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

static float unstick_time = 0;
static qbool no_bots_stuck = 0;

int NumberOfClients (void)
{
	int count = 0;
	gedict_t* plr = NULL;

	for (plr = world; plr = find_plr (plr); ) {
		if (plr->ct == ctPlayer)
			++count;
	}

	return count;
}

static void AirControl (gedict_t* self, vec3_t hor_velocity, float hor_speed)
{
	vec3_t temp;
	float max_accel_forward = sv_accelerate * g_globalvars.frametime * sv_maxspeed;
	vec3_t velocity_hor_angle = { 0, vectoyaw(hor_velocity) + (self->fb.turning_speed * g_globalvars.frametime), 0 };
	vec3_t desired_accel;
	vec3_t dir_forward;
	float accel_forward = 0;
	float velocity_forward = 0;

	trap_makevectors(velocity_hor_angle);
	VectorScale(g_globalvars.v_forward, hor_speed, temp);
	VectorSubtract(temp, hor_velocity, desired_accel);
	normalize(desired_accel, dir_forward);
	accel_forward = min(vlen(desired_accel), max_accel_forward);
	velocity_forward = DotProduct(self->s.v.velocity, dir_forward);
	if ((velocity_forward + accel_forward) > 30) {
		accel_forward = 30 - velocity_forward;
		if (accel_forward < 0) {
			accel_forward = 0;
		}
	}
	// TODO: this is wrong, set the command instead
	//VectorMA(self->s.v.velocity, accel_forward, dir_forward, self->s.v.velocity);
	VectorMA(self->fb.dir_move_, accel_forward, dir_forward, self->fb.dir_move_);
}

//Sets self.obstruction_normal to be horizontal normal direction into wall obstruction encountered
// during quake physics (ie. between PlayerPreThink and PlayerPostThink)
static void obstruction(gedict_t* self) {
	vec3_t delta_velocity = { 0 };

	VectorSubtract(self->fb.oldvelocity, self->s.v.velocity, delta_velocity);
	if (abs(delta_velocity[0]) < 0.1 && abs(delta_velocity[1]) < 0.1) {
		VectorClear(self->fb.obstruction_normal);
		return;
	}

	if (!((int)self->s.v.flags & FL_ONGROUND)) {
		if ((!self->s.v.waterlevel) || ((int)self->s.v.flags & FL_WATERJUMP)) {
			VectorClear(self->fb.obstruction_normal);
			return;
		}
	}

	if (abs(delta_velocity[2]) < 0.1) {
		vec3_t hor_velocity;

		VectorCopy(self->s.v.velocity, hor_velocity);
		hor_velocity[2] = 0;
		if (hor_velocity[0] || hor_velocity[1] || hor_velocity[2]) {
			if (self->ct == ctPlayer && !self->isBot) {
				// FIXME: this is doing nothing now?
				vec3_t hor_direction;

				normalize(hor_velocity, hor_direction);
				VectorScale(hor_direction, DotProduct(hor_direction, self->fb.oldvelocity), hor_velocity);
				//self->s.v.velocity[0] = hor_velocity[0];
				//self->s.v.velocity[1] = hor_velocity[1];
			}
			VectorSubtract(self->fb.oldvelocity, hor_velocity, self->fb.velocity_normal);
			self->fb.velocity_normal[2] = 0;
			normalize(self->fb.velocity_normal, self->fb.obstruction_normal);
			return;
		}

		VectorCopy(self->fb.oldvelocity, hor_velocity);
		hor_velocity[2] = 0;
		if (hor_velocity[0] || hor_velocity[1] || hor_velocity[2]) {
			VectorMA(self->fb.oldvelocity, -DotProduct(self->fb.obstruction_normal, self->fb.oldvelocity), self->fb.obstruction_normal, self->fb.velocity_normal);
			self->fb.velocity_normal[2] = 0;
			normalize(self->fb.velocity_normal, self->fb.obstruction_normal);
			return;
		}
	}

	VectorClear(self->fb.obstruction_normal);
}

void FrogbotPrePhysics1(void) {
	// Set all players to non-solid so we can avoid hazards
	if (IsHazardFrame()) {
		for (self = world; self = find_plr(self); ) {
			self->fb.oldsolid = self->s.v.solid;
			self->s.v.solid = SOLID_NOT;
		}
	}

	// 
	for (self = world; self = find_plr (self); ) {
		if (self->isBot && self->s.v.takedamage) {
			VectorCopy(self->s.v.velocity, self->fb.oldvelocity);
			if (IsHazardFrame()) {
				AvoidHazards(self);
			}
		}
	}

	// Re-instate client entity types
	if (IsHazardFrame()) {
		for (self = world; self = find_plr(self); ) {
			self->s.v.solid = self->fb.oldsolid;
		}
	}
}

void BotDetectTrapped(gedict_t* self) {
	// This tries to detect stuck bots, and fixes the situation by either jumping or committing suicide
	vec3_t point = { self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] - 24 };
	int content1 = trap_pointcontents(point[0], point[1], point[2]);
	if (content1 == CONTENT_EMPTY) {
		self->fb.oldwaterlevel = 0;
		self->fb.oldwatertype = CONTENT_EMPTY;
	}
	else if (content1 == CONTENT_SOLID) {
		unstick_time = unstick_time + g_globalvars.frametime;
		if (unstick_time <= NumberOfClients()) {
			no_bots_stuck = false;
			self->fb.jumping = true;
		}
		else  {
			self->fb.botchose = 1;
			self->fb.next_impulse = CLIENTKILL;
		}
	}
	else {
		int content2 = trap_pointcontents(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 4);
		if (content2 == CONTENT_EMPTY) {
			self->fb.oldwaterlevel = 1;
			self->fb.oldwatertype = content1;
		}
		else  {
			int content3 = trap_pointcontents(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 22);
			if (content3 == CONTENT_EMPTY) {
				self->fb.oldwaterlevel = 2;
				self->fb.oldwatertype = content2;
			}
			else  {
				self->fb.oldwaterlevel = 3;
				self->fb.oldwatertype = content3;
			}
		}
	}
}

void FrogbotPrePhysics2() {
	no_bots_stuck = true;

	for (self = world; self = find_plr (self); ) {
		if (self->isBot) {
			BotDetectTrapped(self);

			if (self->s.v.takedamage) {
				PlayerPreThink_apply();

				if ((int)self->s.v.flags & FL_ONGROUND) {
					if (self->s.v.velocity[2] < 0) {
						self->fb.oldvelocity[2] = self->s.v.velocity[2] = 0;
					}
					if (self->fb.fl_ontrain) {
						self->fb.fl_ontrain = false;
					}
					else {
						self->s.v.flags = self->s.v.flags - FL_ONGROUND;
					}
				}
				else  {
					self->jump_flag = self->s.v.velocity[2];
					self->fb.fl_ontrain = false;
				}

				// FIXME: Should standard logic deal with this now?
				/*
				if ((self->fb.oldwaterlevel >= 2) || ((int)self->s.v.flags & FL_WATERJUMP)) {
					self->s.v.velocity[2] += (800 * g_globalvars.frametime);
				}*/

				VectorCopy(self->s.v.origin, self->s.v.oldorigin);

				self->s.v.waterlevel = self->s.v.watertype = 0;
			}
			else if (!intermission_running && self->s.v.deadflag >= DEAD_DEAD) {
				PlayerDeathThink();
			}
		}
	}
	if (no_bots_stuck) {
		unstick_time = 0;
	}
}

void FrogbotPostPhysics(void) {
	//gedict_t* old_self = self;

	//for (self = world; self = find_plr (self); ) {
		if (self->isBot) {
			self->s.v.waterlevel = self->fb.oldwaterlevel;
			self->s.v.watertype = self->fb.oldwatertype;
			obstruction(self);
			if (self->fb.obstruction_normal[0] || self->fb.obstruction_normal[1] || self->fb.obstruction_normal[2]) {
				vec3_t temp, originDiff;
				vec3_t new_velocity;
				float yaw = 0;
				float dist = 0;
				int oldflags = 0;

				VectorAdd(self->s.v.velocity, self->fb.velocity_normal, new_velocity);
				yaw = vectoyaw(self->fb.obstruction_normal);

				//dist = ((new_velocity * frametime) - (self.origin - self.oldorigin)) * self.obstruction_normal;
				VectorScale(new_velocity, g_globalvars.frametime, temp);
				VectorSubtract(self->s.v.origin, self->s.v.oldorigin, originDiff);
				VectorSubtract(temp, originDiff, temp);
				dist = DotProduct(temp, self->fb.obstruction_normal);

				oldflags = self->s.v.flags;
				self->s.v.flags = (int) self->s.v.flags | FL_ONGROUND_PARTIALGROUND;
				if (walkmove(self, yaw, dist)) {
					//VectorCopy(new_velocity, self->s.v.velocity);
					//self->s.v.velocity[2] = 0;
					VectorCopy(new_velocity, self->fb.dir_move_);
					self->fb.dir_move_[2] = 0;
					VectorClear(self->fb.obstruction_normal);
				}
				else  {
					self->s.v.flags = oldflags;
				}
			}
		}
	//}

//	self = old_self;
}
