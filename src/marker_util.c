// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void SUB_regen ();

void BecomeMarker(gedict_t* marker) {
	marker->fb.fl_marker = true;
	marker->fb.marker_link = first_marker;
	first_marker = marker;
	//dropper->fb.marker_link = first_marker;
}

// FIXME: Never called
void RemoveMarker(gedict_t* marker) {
	gedict_t* e;
	marker->fb.fl_marker = false;
	e = dropper;
	while (e->fb.marker_link != marker) {
		e = e->fb.marker_link;
	}
	e->fb.marker_link = marker->fb.marker_link;
}

void spawn_marker(vec3_t org) {
	marker_ = spawn();
	marker_->s.v.classname = "marker";
	marker_->s.v.flags = FL_ITEM;
	BecomeMarker(marker_);
	marker_->s.v.origin[0] = pr1_rint(org[0]);
	marker_->s.v.origin[1] = pr1_rint(org[1]);
	marker_->s.v.origin[2] = pr1_rint(org[2]);
	marker_->s.v.solid = SOLID_TRIGGER;
	marker_->s.v.touch = (func_t) marker_touch;
	setmodel( marker_, "progs/w_g_key.mdl" );
	VectorSet(marker_->s.v.view_ofs, 80, 80, 24);
	setsize(marker_, -65, -65, -24, 65, 65, 32);
}

// FIXME: Arguments/globals
void check_marker(void) {
	vec3_t temp;

	if (random() < 0.25) {
		self->fb.touchPlayer = other;
		self->fb.touchPlayerTime = g_globalvars.time + 2.5;
	}

	VectorAdd(self->s.v.absmin, self->s.v.view_ofs, temp);
	VectorSubtract(temp, other->s.v.origin, temp);
	distance = vlen(temp);
	if (distance < other->fb.touch_distance && self->s.v.absmin[2] - 20 < other->s.v.absmin[2]) {
		if (other->fb.Z_ != self->fb.Z_) {
			LeaveZone(other->fb.Z_, other->s.v.team, other->fb.is_strong);
			EnterZone(self->fb.Z_, self->s.v.team, self->fb.is_strong);
		}
		other->fb.touch_distance = distance;

		//if (other->fb.touch_marker != self) {
			//G_bprint (2, "Set touch_marker: %d\n", self->fb.index);
		//}

		other->fb.touch_marker = self;
		other->fb.Z_ = self->fb.Z_;
		other->fb.touch_marker_time = g_globalvars.time + 5;
	}
}

void marker_touch() {
	if (marker_time) {
		if (other->ct == ctPlayer) {
			check_marker();
		}
	}
}

void adjust_view_ofs_z(gedict_t* ent) {
	gedict_t* fallspot_self = ent;
	ent = dropper;
	VectorSet(
		testplace, 
		fallspot_self->s.v.absmin[0] + fallspot_self->s.v.view_ofs[0], 
		fallspot_self->s.v.absmin[1] + fallspot_self->s.v.view_ofs[1], 
		fallspot_self->s.v.absmin[2] + fallspot_self->s.v.view_ofs[2] + 1);
	VectorCopy(testplace, ent->s.v.origin);
	ent->s.v.flags = FL_ONGROUND_PARTIALGROUND;
	if (walkmove(ent, 0, 0)) {
		if (ent->s.v.origin[2] <= testplace[2]) {
			if (droptofloor(ent)) {
				if (ent->s.v.origin[2] > testplace[2] - 56) {
					fallspot_self->s.v.view_ofs[2] = ent->s.v.origin[2] - fallspot_self->s.v.absmin[2];
				}
			}
		}
	}
}

gedict_t* LocateMarker(vec3_t org) {
	gedict_t* marker_ = NULL;
	float shortest_distance = 1000000;
	gedict_t* closest_marker = NULL;
	float distance = 0;

	for (marker_ = world; marker_ = trap_findradius(marker_, org, 1000); ) {
		if (marker_->fb.fl_marker) {
			vec3_t marker_pos;

			VectorAdd(marker_->s.v.absmin, marker_->s.v.view_ofs, marker_pos);
			distance = VectorDistance(marker_pos, org);
			traceline(org[0], org[1], org[2], marker_pos[0], marker_pos[1], marker_pos[2], TRUE, dropper);
			if (g_globalvars.trace_fraction != 1) {
				distance = distance + 1000;
			}
			if (distance < shortest_distance) {
				self->fb.near_teleport = NULL;
				shortest_distance = distance;
				closest_marker = marker_;
			}
			else if (streq(marker_->s.v.classname, "trigger_teleport")) {
				self->fb.near_teleport = marker_;
			}
		}
	}

	return closest_marker;
}

// If the current item is a goal but has been taken and waiting to respawn, set a virtual goal
void AssignVirtualGoal_apply(gedict_t* marker_) {
	int goal_number = marker_->fb.G_;
	if (goal_number) {
		gedict_t* test_goal = marker_;

		if (test_goal->s.v.nextthink) {
			int i = 0;
			for (i = 0; i < NUMBER_PATHS; ++i) {
				test_goal = marker_->fb.paths[i].next_marker;
				if (test_goal && ((test_goal->fb.G_ != goal_number) || (test_goal->s.v.nextthink > g_globalvars.time)))
					break;
			}

			if (i >= NUMBER_PATHS) {
				// Waiting to respawn...
				if ((marker_->s.v.nextthink > g_globalvars.time) && (marker_->s.v.think == (func_t) SUB_regen)) {
					test_goal = marker_;
				}
				else {
					test_goal = dropper;
				}
			}
		}
		marker_->fb.virtual_goal = test_goal;
	}
}

// Assign virtual goal to all markers in an items zone
void AssignVirtualGoal(gedict_t* item) {
	gedict_t* marker_;

	for (marker_ = zone_head[item->fb.Z_]; marker_; marker_ = marker_->fb.Z_next) {
		AssignVirtualGoal_apply(marker_);
	}
}

float ExistsPath(gedict_t* from_marker, gedict_t* to_marker) {
	int i = 0;

	if (from_marker == NULL || to_marker == NULL)
		return FALSE;

	for (i = 0; i < NUMBER_PATHS; ++i) {
		if (from_marker->fb.paths[i].next_marker == to_marker) {
			new_path_state = from_marker->fb.paths[i].flags;
			return TRUE;
		}
	}

	return FALSE;
}

void CheckWaterColumn(gedict_t* m, vec3_t m_pos, vec3_t testplace) {
	m->fb.T |= T_WATER;
	VectorCopy(m_pos, testplace);
	while (TRUE) {
		vec3_t point;

		VectorCopy(testplace, self->s.v.origin);
		self->s.v.origin[2] += 16;
		self->s.v.flags = FL_ONGROUND_PARTIALGROUND;
		if (!walkmove(self, 0, 0)) {
			m->fb.T |= T_NO_AIR;
			return;
		}

		VectorCopy(testplace, point);
		point[2] += 32;
		if (trap_pointcontents(point[0], point[1], point[2]) == CONTENT_EMPTY) {
			do {
				testplace[2] = testplace[2] - 1;

				VectorCopy(testplace, point);
				point[2] += 32;
			} while (trap_pointcontents(point[0], point[1], point[2]) == CONTENT_EMPTY);
			return;
		}
		testplace[2] = testplace[2] + 16;
	}
}

