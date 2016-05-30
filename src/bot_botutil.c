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

// TODO: the height offset is fixed here, leading to the bot not targetting the player when they are behind barrier on povdmm4
//       improve with VISIBILITY_LOW | NORMAL | HIGH?  
// Test if one player is visible to another.  takes into account other entities & ring
static qbool VisibilityTest (gedict_t* self, gedict_t* visible_object, float min_dot_product)
{
	vec3_t temp;

	if (visible_object->s.v.takedamage) {
		// Can only see invisible objects when they're attacking
		if (g_globalvars.time < visible_object->invisible_finished && g_globalvars.time >= visible_object->attack_finished) {
			return false;
		}

		// If we can draw straight line between the two...
		traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 32, visible_object->s.v.origin[0], visible_object->s.v.origin[1], visible_object->s.v.origin[2] + 32, true, self);
		if (g_globalvars.trace_fraction == 1) {
			if (min_dot_product == 0) {
				return true;
			}

			// Check it's sufficiently in front of the player
			trap_makevectors(self->s.v.v_angle);
			VectorSubtract(visible_object->s.v.origin, self->s.v.origin, temp);
			VectorNormalize(temp);

			return DotProduct (g_globalvars.v_forward, temp) >= min_dot_product;
		}
	}
	return false;
}

qbool Visible_360(gedict_t* self, gedict_t* visible_object) {
	return (self->fb.enemy_visible = VisibilityTest (self, visible_object, 0.0f));
}

qbool Visible_infront(gedict_t* self, gedict_t* visible_object) {
	// FIXME: Effective FOV should be in fb.skill
	return (self->fb.enemy_visible = VisibilityTest (self, visible_object, 0.7071067f));
}
