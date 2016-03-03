// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

float BestArrowForDirection() {
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

	trap_makevectors(self->s.v.v_angle);

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

// Test if one player is visible to another.  takes into account bodies & ring
qbool Visible_360(gedict_t* self, gedict_t* visible_object) {
	// If the object is already dead
	if (visible_object->s.v.takedamage) {
		// can't see eyes unless it's attacking
		if (g_globalvars.time < visible_object->invisible_finished) {
			if (g_globalvars.time >= visible_object->attack_finished) {
				return (qbool) false;
			}
		}

		traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 32, visible_object->s.v.origin[0], visible_object->s.v.origin[1], visible_object->s.v.origin[2] + 32, TRUE, self);
		if (g_globalvars.trace_fraction == 1) {
			return (qbool) true;
		}
	}
	return (qbool) false;
}

void Visible_infront() {
	if (visible_object->s.v.takedamage) {
		if (g_globalvars.time < visible_object->invisible_finished) {
			if (g_globalvars.time >= visible_object->attack_finished) {
				enemy_visible = FALSE;
				return;
			}
		}
		traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 32, visible_object->s.v.origin[0], visible_object->s.v.origin[1], visible_object->s.v.origin[2] + 32, TRUE, self);
		if (g_globalvars.trace_fraction == 1) {
			vec3_t temp;
			trap_makevectors(self->s.v.v_angle);
			VectorSubtract(visible_object->s.v.origin, self->s.v.origin, temp);
			VectorNormalize(temp);

			if (DotProduct(g_globalvars.v_forward, temp) > 0.7071067) {
				enemy_visible = TRUE;
				return;
			}
		}
	}
	enemy_visible = FALSE;
}

void PredictSpot() {
	fallspot_self = self;
	self = dropper;
	VectorCopy(testplace, self->s.v.origin);
	self->s.v.flags = FL_ONGROUND_PARTIALGROUND;
	if (walkmove(self, 0, 0)) {
		if (!(droptofloor(self))) {
			predict_spot = FALSE;
			self = fallspot_self;
			testplace[2] = testplace[2] - 400 * (rel_time * rel_time) - 38;
			return;
		}
		if (self->s.v.origin[2] < fallheight) {
			predict_spot = FALSE;
			self = fallspot_self;
			testplace[2] = testplace[2] - 400 * (rel_time * rel_time) - 38;
			return;
		}
		predict_spot = TRUE;
		self = fallspot_self;
		return;
	}
	predict_spot = FALSE;
	self = fallspot_self;
	VectorCopy(enemy_->s.v.origin, testplace);
}

void FallSpotGround() {
	fallspot_self = self;
	self = dropper;
	VectorCopy(testplace, self->s.v.origin);
	self->s.v.flags = FL_ONGROUND_PARTIALGROUND;
	if (walkmove(self, 0, 0)) {
		if (!JumpInWater()) {
			if (!(droptofloor(self))) {
				VectorCopy(testplace, self->s.v.origin);
				self->s.v.origin[2] -= 256;
				if (!(droptofloor(self))) {
					fall = FALL_DEATH;
					self = fallspot_self;
					return;
				}
			}
		}
		content = trap_pointcontents(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] - 24);
		if (content == CONTENT_LAVA) {
			fall = FALL_DEATH;
		}
		else if (self->s.v.origin[2] < fallheight) {
			fall = FALL_LAND;
		}
		else  {
			fall = FALL_FALSE;
		}
		self = fallspot_self;
		return;
	}
	else  {
		fall = FALL_BLOCKED;
		self = fallspot_self;
	}
}

void FallSpotAir() {
	fallspot_self = self;
	self = dropper;
	VectorCopy(testplace, self->s.v.origin);
	self->s.v.flags = FL_ONGROUND_PARTIALGROUND;
	if (walkmove(self, 0, 0)) {
		if (!JumpInWater()) {
			if (self->s.v.origin[2] > testplace[2]) {
				fall = FALL_BLOCKED;
				self = fallspot_self;
				return;
			}
		}
	}
	else  {
		fall = FALL_BLOCKED;
		self = fallspot_self;
		return;
	}
	if (!JumpInWater()) {
		if (!(droptofloor(self))) {
			VectorCopy(testplace, self->s.v.origin);
			self->s.v.origin[2] -= 256;
			
			if (!(droptofloor(self))) {
				fall = FALL_DEATH;
				self = fallspot_self;
				return;
			}
		}
	}
	content = trap_pointcontents(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] - 24);
	if (content == CONTENT_LAVA) {
		fall = FALL_DEATH;
	}
	else if (self->s.v.origin[2] < fallheight) {
		fall = FALL_LAND;
	}
	else  {
		fall = FALL_FALSE;
	}
	self = fallspot_self;
}

void TestTopBlock() {
	float xDelta[4] = { -16,  16, -16,  16 };
	float yDelta[4] = { -16, -16,  16,  16 };
	int i = 0;
	for (i = 0; i < 4; ++i)
	{
		traceline(last_clear_point[0] + xDelta[i], last_clear_point[1] + yDelta[i], last_clear_point[2] + 32, testplace[0] + xDelta[i], testplace[1] - yDelta[i], testplace[2] + 32, TRUE, self);
		if (g_globalvars.trace_fraction != 1) {
			if (g_globalvars.trace_plane_normal[2] <= 0) {
				if (first_trace_fraction > g_globalvars.trace_fraction) {
					first_trace_fraction = g_globalvars.trace_fraction;
					VectorCopy(g_globalvars.trace_plane_normal, first_trace_plane_normal);
				}
			}
		}
	}
}

void TestBottomBlock() {
	float xDelta[4] = { -16,  16, -16,  16 };
	float yDelta[4] = { -16, -16,  16,  16 };
	int i = 0;
	for (i = 0; i < 4; ++i)
	{
		traceline(last_clear_point[0] + xDelta[i], last_clear_point[1] + yDelta[i], last_clear_point[2] - 24, testplace[0] + xDelta[i], testplace[1] - yDelta[i], testplace[2] - 24, TRUE, self);
		if (g_globalvars.trace_fraction != 1) {
			if (g_globalvars.trace_plane_normal[2] >= 0) {
				if (first_trace_fraction > g_globalvars.trace_fraction) {
					first_trace_fraction = g_globalvars.trace_fraction;
					VectorCopy(g_globalvars.trace_plane_normal, first_trace_plane_normal);
				}
			}
		}
	}
}

void CanJumpOver() {
	int i = 0;

	tries = 0;
	VectorCopy(jump_origin, last_clear_point);
	VectorCopy(jump_velocity, last_clear_velocity);
	VectorCopy(last_clear_velocity, last_clear_hor_velocity);
	last_clear_hor_velocity[2] = 0;
	last_clear_hor_speed = vlen(last_clear_hor_velocity);
	last_clear_velocity[2] = jump_velocity[2] - (12800 / last_clear_hor_speed);
	while ((tries < 20) && (last_clear_point[2] >= fallheight)) {
		// testplace = last_clear_point + (last_clear_velocity * (32 / last_clear_hor_speed));
		for (i = 0; i < 3; ++i)
			testplace[i] = last_clear_point[i] + last_clear_velocity[i] * (32 / last_clear_hor_speed);

		FallSpotAir();
		if (fall == FALL_BLOCKED) {
			first_trace_fraction = 1;
			TestTopBlock();
			TestBottomBlock();
			if (first_trace_fraction != 1) {
				//testplace = last_clear_point + (last_clear_velocity * (first_trace_fraction * 32 / last_clear_hor_speed));
				for (i = 0; i < 3; ++i)
					testplace[i] = last_clear_point[i] + (last_clear_velocity[i] * (first_trace_fraction * 32 / last_clear_hor_speed));

				//last_clear_velocity = last_clear_velocity - (first_trace_plane_normal * (first_trace_plane_normal * last_clear_velocity));
				for (i = 0; i < 3; ++i)
					last_clear_velocity[i] = last_clear_velocity[i] - (first_trace_plane_normal[i] * DotProduct(first_trace_plane_normal, last_clear_velocity));
				VectorCopy(last_clear_velocity, last_clear_hor_velocity);
				last_clear_hor_velocity[2] = 0;
				last_clear_hor_speed = vlen(last_clear_hor_velocity);
				for (i = 0; i < 3; ++i)
					testplace[i] = testplace[i] + (last_clear_velocity[i] * (32 / last_clear_hor_speed) * (1 - first_trace_fraction));
			}
			FallSpotAir();
		}
		if (fall == FALL_BLOCKED) {
			do_jump = FALSE;
			return;
		}
		else  {
			if (fall > current_fallspot) {
				last_clear_velocity[2] = last_clear_velocity[2] - (25600 / last_clear_hor_speed);
				VectorCopy(testplace, last_clear_point);
			}
			else  {
				do_jump = TRUE;
				if ((int)self->s.v.flags & FL_ONGROUND) {
					test_enemy = first_client;
					while (test_enemy) {
						test_enemy->s.v.solid = test_enemy->fb.oldsolid;
						test_enemy = test_enemy->fb.next;
					}

					for (test_enemy = world; test_enemy = trap_findradius(test_enemy, testplace, 84); ) {
						if (test_enemy->fb.T & UNREACHABLE) {
							test_enemy = world;
							do_jump = FALSE;
						}
					}
					test_enemy = first_client;
					while (test_enemy) {
						test_enemy->s.v.solid = SOLID_NOT;
						test_enemy = test_enemy->fb.next;
					}
				}
				return;
			}
		}
		if (turning_speed) {
			vectoangles(last_clear_velocity, last_clear_angle);
			last_clear_angle[0] = 0 - last_clear_angle[0];
			last_clear_angle[1] = last_clear_angle[1] + (turning_speed * 32 / last_clear_hor_speed);
			trap_makevectors(last_clear_angle);
			VectorScale(g_globalvars.v_forward, vlen(last_clear_velocity), last_clear_velocity);
		}
		tries = tries + 1;
	}
	do_jump = FALSE;
}

