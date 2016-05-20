// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

static int marker_index = 0;

void AddToQue(gedict_t* ent) {
	markers[marker_index] = ent;
	ent->fb.index = marker_index++;

	ent->fb.zone_marker = Z_marker_error;
	ent->fb.sub_arrival_time = S_time_error;
	ent->fb.sub_path_marker = S_path_marker_error;
	ent->fb.path_marker = Z_path_marker_error;
	ent->fb.next_zone_marker = Z_next_zone_marker_error;
	ent->fb.sight_from_marker = Z_sight_from_marker_error;
	ent->fb.higher_sight_from_marker = Z_higher_sight_from_marker_error;
	ent->fb.sight_from_time = Z_sight_from_time_error;
}

void N(float x, float y, float z) {
	vec3_t org = { x, y, z };
	spawn_marker(org);
	self = marker_;
	AddToQue(marker_);
}

void AllMarkersLoaded() {
	self = NULL;
	m_zone = zone_stack_head;
	path_normal = TRUE;

	InitialiseMarkerRoutes();
}

void SetMarkerPathFlags(int marker_number, int path_index, int flags) {
	--marker_number;

	if (marker_number < 0 || marker_number >= NUMBER_MARKERS) {
		return;
	}

	markers[marker_number]->fb.paths[path_index].flags = flags;
}

void SetMarkerPath(int source_marker, int path_index, int next_marker) {
	--source_marker;
	--next_marker;

	if (source_marker < 0 || source_marker >= NUMBER_MARKERS) {
		return;
	}
	if (next_marker < 0 || next_marker >= NUMBER_MARKERS) {
		return;
	}

	if (source_marker == 32)
		Com_Printf ("SetMarkerPath(%d, %d, %d) = %d\n", source_marker, path_index, next_marker, markers[next_marker] == 0 ? -1 : markers[next_marker]->fb.index);
	markers[source_marker]->fb.paths[path_index].next_marker = markers[next_marker];
}

void SetMarkerViewOffset (int marker, float zOffset)
{
	--marker;

	if (marker < 0 || marker >= NUMBER_MARKERS || markers[marker] == 0) {
		return;
	}

	markers[marker]->s.v.view_ofs[2] = zOffset;
}
