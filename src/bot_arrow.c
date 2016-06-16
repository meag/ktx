
#include "g_local.h"
#include "fb_globals.h"

#define ARROW_TIME_INCREASE       0.15  // Increase after NewVelocityForArrow

static const char* ArrowName (int arrow)
{
	static char name[64];

	name[0] = 0;
	if (arrow & FORWARD)
		strlcat (name, "Fwd ", sizeof (name));
	if (arrow & BACK)
		strlcat (name, "Back ", sizeof (name));
	if (arrow & LEFT)
		strlcat (name, "Left ", sizeof (name));
	if (arrow & RIGHT)
		strlcat (name, "Rght ", sizeof (name));
	if (arrow & UP)
		strlcat (name, "Up ", sizeof (name));
	if (arrow & DOWN)
		strlcat (name, "Down ", sizeof (name));

	return name;
}

void NewVelocityForArrow(gedict_t* self, vec3_t dir_move) {
	/*float best_arrow = BestArrowForDirection(self, dir_move);

	if (self->fb.arrow != best_arrow) {
		VectorCopy(dir_move, self->fb.dir_move_);
		self->fb.arrow = best_arrow;
		self->fb.arrow_time = g_globalvars.time + ARROW_TIME_INCREASE;
		// FIXME: Not convinced what is going on here
		//VectorCopy(oldvelocity_, self->s.v.velocity);
		//VectorCopy(oldvelocity_, self->fb.real_direction);
		VelocityForArrow(self);
	}*/

	VectorCopy(dir_move, self->fb.dir_move_);
}

int BestArrowForDirection(gedict_t* self, const vec3_t dir_move) {
	vec3_t temp;
	float best_dotproduct = 0;
	float best_arrow = 0;
	float test_forward = 0;
	float test_forward_left = 0;
	float test_forward_right = 0;
	float test_right = 0;
	float test_left = 0;
	float test_back = 0;
	float test_back_left = 0;
	float test_back_right = 0;

	//trap_makevectors(self->s.v.v_angle);
	trap_makevectors (self->fb.desired_angle);

	test_forward = DotProduct(g_globalvars.v_forward, dir_move);
	if (test_forward > best_dotproduct) {
		best_dotproduct = test_forward;
		best_arrow = FORWARD;
	}

	VectorScale(g_globalvars.v_right, 0.875, temp);
	VectorSubtract(g_globalvars.v_forward, temp, temp);
	VectorNormalize(temp);
	test_forward_left = DotProduct(temp, dir_move);
	if (test_forward_left > best_dotproduct) {
		best_dotproduct = test_forward_left;
		best_arrow = FORWARD_LEFT;
	}

	VectorScale(g_globalvars.v_right, 0.875, temp);
	VectorAdd(g_globalvars.v_forward, temp, temp);
	VectorNormalize(temp);
	test_forward_right = DotProduct(temp, dir_move);
	if (test_forward_right > best_dotproduct) {
		best_dotproduct = test_forward_right;
		best_arrow = FORWARD_RIGHT;
	}

	test_right = DotProduct(g_globalvars.v_right, dir_move);
	if (test_right > best_dotproduct) {
		best_dotproduct = test_right;
		best_arrow = RIGHT;
	}
	test_left = 0 - test_right;
	if (test_left > best_dotproduct) {
		best_dotproduct = test_left;
		best_arrow = LEFT;
	}
	test_back = 0 - test_forward;
	if (test_back > best_dotproduct) {
		best_dotproduct = test_back;
		best_arrow = BACK;
	}
	test_back_left = 0 - test_forward_right;
	if (test_back_left > best_dotproduct) {
		best_dotproduct = test_back_left;
		best_arrow = BACK_LEFT;
	}
	test_back_right = 0 - test_forward_left;
	if (test_back_right > best_dotproduct) {
		best_dotproduct = test_back_right;
		best_arrow = BACK_RIGHT;
	}

	return best_arrow;
}

void VelocityForArrow(gedict_t* self) {
	vec3_t dir_forward = { 0 };
	int arrow_ = 0;
	float accel_forward = 0;
	float velocity_forward = 0;
	float current_maxspeed = 0;
	/*
	self->fb.turning_speed = 0;
	if (!self->s.v.waterlevel) {
		if (g_globalvars.time > self->fb.arrow_time2) {
			float hor_speed = 0;
			vec3_t hor_velocity;

			VectorCopy(self->s.v.velocity, hor_velocity);
			hor_velocity[2] = 0;
			hor_speed = vlen(hor_velocity);
			if ((hor_speed > 100) || ((self->fb.path_state & AIR_ACCELERATION))) {
				// hor_normal_vec = right angle to velocity
				vec3_t hor_normal_vec = { 0 - self->s.v.velocity[1], self->s.v.velocity[0], 0 };
				vec3_t rel_pos;

				VectorNormalize(hor_normal_vec);

				// rel_pos = horizontal difference to linked marker
				VectorAdd(self->fb.linked_marker->s.v.absmin, self->fb.linked_marker->s.v.view_ofs, rel_pos);
				VectorSubtract(rel_pos, self->s.v.origin, rel_pos);
				rel_pos[2] = 0;

				// ??? ... not sure why horizontal speed increases turning speed...
				if (DotProduct(rel_pos, rel_pos) != 0) {
					self->fb.turning_speed = 114.59156 * hor_speed * DotProduct(rel_pos, hor_normal_vec) / DotProduct(rel_pos, rel_pos);
				}

				if (fabs(self->fb.turning_speed) > 270) {
					if (self->fb.path_state & AIR_ACCELERATION) {
						if (self->fb.turning_speed > 0) {
							self->fb.turning_speed = 270;
						}
						else  {
							self->fb.turning_speed = -270;
						}
					}
					else {
						self->fb.turning_speed = 0;
					}
				}

				if (!((int)self->s.v.flags & FL_ONGROUND)) {
					AirControl (self, hor_velocity, hor_speed);
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
		else {
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
		//trap_makevectors(self->s.v.v_angle);
		trap_makevectors (self->fb.desired_angle);
		if (arrow_ & FORWARD) {
			if (arrow_ == FORWARD) {
				VectorCopy(g_globalvars.v_forward, dir_forward);
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
	accel_forward = sv_accelerate * g_globalvars.frametime * current_maxspeed;
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
	//VectorMA(self->fb.dir_move_, accel_forward, dir_forward, self->fb.dir_move_);
	//VectorMA(self->fb.dir_move_, accel_forward, dir_forward, self->fb.dir_move_);

	//VectorCopy (dir_forward, self->fb.dir_move_);
	//VectorMA(self->fb.dir_move_, accel_forward, dir_forward, self->fb.dir_move_); // (was this)
	//VectorNormalize (self->fb.dir_move_);

	//G_bprint (2, "Arrow %s => [%f %f %f]\n", ArrowName(self->fb.arrow), PASSVEC3(self->fb.dir_move_));
	*/
}
