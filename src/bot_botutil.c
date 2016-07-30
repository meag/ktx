// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

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
	return (self->fb.enemy_visible = VisibilityTest (self, visible_object, self->fb.skill.visibility));
}