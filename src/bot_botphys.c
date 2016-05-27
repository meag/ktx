// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void obstruction() {
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
		VectorCopy(self->s.v.velocity, hor_velocity);
		hor_velocity[2] = 0;
		if (hor_velocity[0] || hor_velocity[1] || hor_velocity[2]) {
			if (self->ct == ctPlayer && !self->isBot) {
				VectorNormalize(hor_direction);
				VectorScale(hor_direction, DotProduct(hor_direction, self->fb.oldvelocity), hor_velocity);
				self->s.v.velocity[0] = hor_velocity[0];
				self->s.v.velocity[1] = hor_velocity[1];
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

void VelocityForArrow(gedict_t* self) {
	int arrow_ = 0;
	float accel_forward = 0;
	float velocity_forward = 0;

	turning_speed = 0;
	if (!self->s.v.waterlevel) {
		if (g_globalvars.time > self->fb.arrow_time2) {
			float hor_speed = 0;

			VectorCopy(self->s.v.velocity, hor_velocity);
			hor_velocity[2] = 0;
			hor_speed = vlen(hor_velocity);
			if ((hor_speed > 100) || ((self->fb.path_state & AIR_ACCELERATION))) {
				// hor_normal_vec = right angle to velocity
				hor_normal_vec[0] = 0 - self->s.v.velocity[1];
				hor_normal_vec[1] = self->s.v.velocity[0];
				VectorNormalize(hor_normal_vec);

				// rel_pos = horizontal difference to linked marker
				VectorAdd(self->fb.linked_marker->s.v.absmin, self->fb.linked_marker->s.v.view_ofs, rel_pos);
				VectorSubtract(rel_pos, self->s.v.origin, rel_pos);
				rel_pos[2] = 0;

				// ??? ... not sure why horizontal speed increases turning speed...
				if (DotProduct(rel_pos, rel_pos) != 0) {
					turning_speed = 114.59156 * hor_speed * DotProduct(rel_pos, hor_normal_vec) / DotProduct(rel_pos, rel_pos);
				}

				if (fabs(turning_speed) > 270) {
					if (self->fb.path_state & AIR_ACCELERATION) {
						if (turning_speed > 0) {
							turning_speed = 270;
						}
						else  {
							turning_speed = -270;
						}
					}
					else  {
						turning_speed = 0;
					}
				}

				if (!((int)self->s.v.flags & FL_ONGROUND)) {
					vec3_t temp;
					float max_accel_forward = sv_accelerate * g_globalvars.frametime * sv_maxspeed;

					velocity_hor_angle[1] = vectoyaw(hor_velocity) + (turning_speed * g_globalvars.frametime);
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
					//VectorCopy (dir_forward, self->fb.real_direction);
					//VectorMA(self->fb.real_direction, accel_forward, dir_forward, self->fb.real_direction);
					VectorMA(self->fb.dir_move_, accel_forward, dir_forward, self->fb.dir_move_);
					return;
				}
			}
		}
	}

	arrow_ = self->fb.arrow;
	if (!arrow_) {
		if (self->s.v.waterlevel <= 1) {
			return;
		}
		else  {
			if (self->fb.swim_arrow) {
				if (self->fb.swim_arrow == UP) {
					VectorSet(dir_forward, 0, 0, 1);
				}
				else  {
					VectorSet(dir_forward, 0, 0, -1);
				}
				current_maxspeed = 200;
			}
			else  {
				VectorSet(dir_forward, 0, 0, -1);
				current_maxspeed = 42;
			}
		}
	}
	else {
		trap_makevectors(self->s.v.v_angle);
		if (arrow_ & FORWARD) {
			if (arrow_ == FORWARD) {
				VectorCopy(dir_forward, g_globalvars.v_forward);
			}
			else if (arrow_ == FORWARD_LEFT) {
				vec3_t forward, side;

				VectorScale(g_globalvars.v_forward, 0.7525767, forward);
				VectorScale(g_globalvars.v_right, 0.6585046, side);

				VectorSubtract(forward, side, dir_forward);
			}
			else  {
				vec3_t forward, side;

				VectorScale(g_globalvars.v_forward, 0.7525767, forward);
				VectorScale(g_globalvars.v_right, 0.6585046, side);

				VectorAdd(forward, side, dir_forward);
			}
			current_maxspeed = sv_maxspeed;
		}
		else if (arrow_ & BACK) {
			if (arrow_ == BACK) {
				VectorScale(g_globalvars.v_forward, -1, dir_forward);
			}
			else if (arrow_ == BACK_LEFT) {
				vec3_t forward, side;

				VectorScale(g_globalvars.v_forward, -0.7525767, forward);
				VectorScale(g_globalvars.v_right, -0.6585046, side);

				VectorAdd(forward, side, dir_forward);
			}
			else  {
				vec3_t forward, side;

				VectorScale(g_globalvars.v_forward, -0.7525767, forward);
				VectorScale(g_globalvars.v_right, 0.6585046, side);

				VectorAdd(forward, side, dir_forward);
			}
			current_maxspeed = sv_maxspeed;
		}
		else  {
			if (arrow_ == LEFT) {
				VectorScale(g_globalvars.v_right, -1, dir_forward);
			}
			else  {
				VectorCopy(g_globalvars.v_right, dir_forward);
			}
			current_maxspeed = sv_maxstrafespeed;
		}

		if (self->s.v.waterlevel <= 1) {
			dir_forward[2] = 0;
			VectorNormalize(dir_forward);
		}
		else  {
			if (self->fb.swim_arrow) {
				vec3_t temp;
				VectorCopy(dir_forward, temp);

				if (self->fb.swim_arrow == UP) {
					temp[2] += 0.5;
					normalize(temp, dir_forward);
				}
				else  {
					temp[2] -= 0.5;
					normalize(temp, dir_forward);
				}
			}
			current_maxspeed = 0.7 * current_maxspeed;
		}
	}
	accel_forward = sv_accelerate_frametime * current_maxspeed;
	velocity_forward = DotProduct(self->s.v.velocity, dir_forward);
	if (!((int)self->s.v.flags & FL_ONGROUND) && self->s.v.waterlevel <= 1) {
		current_maxspeed = min(current_maxspeed, 30);
	}
	if ((velocity_forward + accel_forward) > (current_maxspeed)) {
		accel_forward = max(0, current_maxspeed - velocity_forward);
	}

	// TODO: this is wrong, set the command instead
	//VectorMA(self->s.v.velocity, accel_forward, dir_forward, self->s.v.velocity);
	//VectorCopy (dir_forward, self->fb.real_direction);
	//VectorMA(self->fb.real_direction, accel_forward, dir_forward, self->fb.real_direction);
	VectorMA(self->fb.dir_move_, accel_forward, dir_forward, self->fb.dir_move_);
}

void FrogbotPrePhysics1(void) {
	// Set all players to non-solid so we can avoid hazards
	if (hazard_time) {
		for (self = world; self = find_plr(self); ) {
			self->fb.oldsolid = self->s.v.solid;
			self->s.v.solid = SOLID_NOT;
		}
	}

	// 
	for (self = world; self = find_plr (self); ) {
		if (self->isBot && self->s.v.takedamage) {
			VectorCopy(self->s.v.velocity, oldvelocity_);
			VelocityForArrow(self);
			if (hazard_time) {
				AvoidHazards();
			}
		}
	}

	// Re-instate client entity types
	if (hazard_time) {
		for (self = world; self = find_plr(self); ) {
			self->s.v.solid = self->fb.oldsolid;
		}
	}
}

void BotDetectTrapped(gedict_t* self) {
	// This tries to detect stuck bots, and fixes the situation by either jumping or committing suicide
	vec3_t point = { self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] - 24 };
	content1 = trap_pointcontents(point[0], point[1], point[2]);
	if (content1 == CONTENT_EMPTY) {
		self->fb.oldwaterlevel = 0;
		self->fb.oldwatertype = CONTENT_EMPTY;
	}
	else if (content1 == CONTENT_SOLID) {
		unstick_time = unstick_time + g_globalvars.frametime;
		if (unstick_time <= numberofclients) {
			no_bots_stuck = FALSE;
			//self->s.v.velocity[2] = JUMPSPEED;
			self->fb.jumping = true;
		}
		else  {
			self->fb.botchose = 1;
			self->fb.next_impulse = CLIENTKILL;
		}
	}
	else {
		VectorSet(point, self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 4);
		content2 = trap_pointcontents(point[0], point[1], point[2]);
		if (content2 == CONTENT_EMPTY) {
			self->fb.oldwaterlevel = 1;
			self->fb.oldwatertype = content1;
		}
		else  {
			VectorSet(point, self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 22);
			content3 = trap_pointcontents(point[0], point[1], point[2]);
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
	g_globalvars.frametime = real_frametime;
	no_bots_stuck = TRUE;

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
						self->fb.fl_ontrain = FALSE;
					}
					else {
						self->s.v.flags = self->s.v.flags - FL_ONGROUND;
					}
				}
				else  {
					self->jump_flag = self->s.v.velocity[2];
					self->fb.fl_ontrain = FALSE;
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

void FrogbotPostPhysics1() {
	g_globalvars.frametime = real_frametime;

	for (self = world; self = find_plr (self); ) {
		if (self->isBot) {
			self->s.v.waterlevel = self->fb.oldwaterlevel;
			self->s.v.watertype = self->fb.oldwatertype;
			obstruction();
			if (self->fb.obstruction_normal[0] || self->fb.obstruction_normal[1] || self->fb.obstruction_normal[2]) {
				vec3_t temp, originDiff;
				float yaw = 0;
				float dist = 0;

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
	}
}

void FrogbotPostPhysics2() {
	/*g_globalvars.frametime = real_frametime;
	for (self = world; self = find_plr (self); ) {
		if (self->s.v.movetype == MOVETYPE_STEP) {
			PlayerPostThink_apply();
		}
	}*/
}

void FrogbotPostPhysics() {
	FrogbotPostPhysics1();
	FrogbotPostPhysics2();
}

