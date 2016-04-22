// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

float BestArrowForDirection(gedict_t* self, vec3_t dir_move) {
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
				return FALSE;
			}
		}

		traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 32, visible_object->s.v.origin[0], visible_object->s.v.origin[1], visible_object->s.v.origin[2] + 32, TRUE, self);
		if (g_globalvars.trace_fraction == 1) {
			return TRUE;
		}
	}
	return FALSE;
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

void TestTopBlock(void) {
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

void TestBottomBlock(void) {
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

