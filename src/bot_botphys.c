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
	vec3_t new_velocity;

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

	VectorMA (self->fb.dir_move_, accel_forward, dir_forward, new_velocity);
	SetDirectionMove (self, new_velocity, "AirControl");
}

//Sets self.obstruction_normal to be horizontal normal direction into wall obstruction encountered
// during quake physics (ie. between PlayerPreThink and PlayerPostThink)
static qbool obstruction(gedict_t* self) {
	vec3_t delta_velocity = { 0 };
	qbool onGround = ((int)self->s.v.flags & FL_ONGROUND);
	vec3_t proposed = { 0 };
	vec3_t direction = { 0 };
	float scale = 0.0f;
	float heading = 0.0f;

	return false;

	// This used to compare velocities after physics has been run for all the bots...
	// We can't send the command twice, so instead we use dropper to try and find out if we're up against a wall
	VectorCopy( self->s.v.origin, dropper->s.v.origin );
	dropper->s.v.flags = FL_ONGROUND_PARTIALGROUND;
	heading = vectoyaw( self->fb.dir_move_ );
	if (walkmove (dropper, heading, 16)) {
		VectorClear (self->fb.obstruction_normal);
		return false;
	}

	/*
	normalize (self->fb.dir_move_, direction);
	VectorMA (self->s.v.origin, sv_maxspeed * g_globalvars.frametime * 1.1, direction, proposed);
	traceline (PASSVEC3 (self->s.v.origin), PASSVEC3 (proposed), 0, self);
	if (g_globalvars.trace_fraction == 1.0) {
		VectorClear(self->fb.obstruction_normal);
		return false;
	}*/

	// In mid-air and not swimming?  Nothing we can do about it, wait to hit the ground...
	if (! onGround && ((!self->s.v.waterlevel) || ((int)self->s.v.flags & FL_WATERJUMP))) {
		VectorClear(self->fb.obstruction_normal);
		return false;
	}

	// Store the horizontal normals
	VectorCopy (g_globalvars.trace_plane_normal, self->fb.obstruction_normal);
	self->fb.obstruction_normal[2] = 0;
	normalize (self->fb.dir_move_, self->fb.velocity_normal);
	self->fb.velocity_normal[2] = 0;
	VectorAdd (self->fb.velocity_normal, self->fb.obstruction_normal, self->fb.velocity_normal);
	return true;

	/*
	VectorSubtract (g_globalvars.trace_endpos, self->s.v.origin, direction);
	VectorSubtract(proposed, direction, delta_velocity);
	if (abs(delta_velocity[2]) < 0.1) {
		vec3_t hor_velocity;

		VectorCopy(direction, hor_velocity);
		hor_velocity[2] = 0;
		if (hor_velocity[0] || hor_velocity[1] || hor_velocity[2]) {
			// FIXME: Why would non-bot logic be here?
			if (self->ct == ctPlayer && !self->isBot) {
				vec3_t hor_direction;

				normalize(hor_velocity, hor_direction);
				VectorScale(hor_direction, DotProduct(hor_direction, self->fb.oldvelocity), hor_velocity);
				//self->s.v.velocity[0] = hor_velocity[0];
				//self->s.v.velocity[1] = hor_velocity[1];
			}
			VectorSubtract(self->fb.oldvelocity, hor_velocity, self->fb.velocity_normal);
			self->fb.velocity_normal[2] = 0;
			normalize(self->fb.velocity_normal, self->fb.obstruction_normal);
			return true;
		}

		VectorCopy(self->fb.oldvelocity, hor_velocity);
		hor_velocity[2] = 0;
		if (hor_velocity[0] || hor_velocity[1] || hor_velocity[2]) {
			VectorMA(self->fb.oldvelocity, -DotProduct(self->fb.obstruction_normal, self->fb.oldvelocity), self->fb.obstruction_normal, self->fb.velocity_normal);
			self->fb.velocity_normal[2] = 0;
			normalize(self->fb.velocity_normal, self->fb.obstruction_normal);
			return true;
		}
	}

	VectorClear(self->fb.obstruction_normal);
	return false;*/
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

			// 
			if (obstruction(self)) {
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
					new_velocity[2] = 0;
					SetDirectionMove (self, new_velocity, "PostPhysics");
					VectorClear(self->fb.obstruction_normal);
				}
				else {
					self->s.v.flags = oldflags;
				}
			}
		}
	//}

//	self = old_self;
}
