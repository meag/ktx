
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

void SetDirectionMove (gedict_t* self, vec3_t dir_move, const char* explanation)
{
	if (self->fb.linked_marker && self->fb.linked_marker->fb.index == 8 &&
		self->fb.touch_marker && self->fb.touch_marker->fb.index == 78)
		G_bprint (2, "Setting direction: %s [%f %f %f]\n", explanation, PASSVEC3(dir_move));
	normalize (dir_move, self->fb.dir_move_);
}

void NewVelocityForArrow(gedict_t* self, vec3_t dir_move, const char* explanation) {
	SetDirectionMove (self, dir_move, explanation);
	self->fb.arrow_time = g_globalvars.time + ARROW_TIME_INCREASE;
}

