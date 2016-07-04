
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

	normalize (dir_move, self->fb.dir_move_);
	self->fb.arrow_time = g_globalvars.time + ARROW_TIME_INCREASE;
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
