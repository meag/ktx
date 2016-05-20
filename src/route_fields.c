// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

#define SUBZONEFUNCTIONS(name) \
	S1_ ## name, \
	S2_ ## name, \
	S3_ ## name, \
	S4_ ## name, \
	S5_ ## name, \
	S6_ ## name, \
	S7_ ## name, \
	S8_ ## name, \
	S9_ ## name, \
	S10_ ## name, \
	S11_ ## name, \
	S12_ ## name, \
	S13_ ## name, \
	S14_ ## name, \
	S15_ ## name, \
	S16_ ## name, \
	S17_ ## name, \
	S18_ ## name, \
	S19_ ## name, \
	S20_ ## name, \
	S21_ ## name, \
	S22_ ## name, \
	S23_ ## name, \
	S24_ ## name, \
	S25_ ## name, \
	S26_ ## name, \
	S27_ ## name, \
	S28_ ## name, \
	S29_ ## name, \
	S30_ ## name, \
	S31_ ## name, \
	S32_ ## name

#define ZONEFUNCTIONS(name) \
	Z1_ ## name, \
	Z2_ ## name, \
	Z3_ ## name, \
	Z4_ ## name, \
	Z5_ ## name, \
	Z6_ ## name, \
	Z7_ ## name, \
	Z8_ ## name, \
	Z9_ ## name, \
	Z10_ ## name, \
	Z11_ ## name, \
	Z12_ ## name, \
	Z13_ ## name, \
	Z14_ ## name, \
	Z15_ ## name, \
	Z16_ ## name, \
	Z17_ ## name, \
	Z18_ ## name, \
	Z19_ ## name, \
	Z20_ ## name, \
	Z21_ ## name, \
	Z22_ ## name, \
	Z23_ ## name, \
	Z24_ ## name

void SetGoalForMarker(int goal, gedict_t* marker) {
	if (goal <= 0 || goal > NUMBER_GOALS)
		return;

	marker->fb.goals[goal - 1].next_marker = marker;
	marker->fb.G_ = goal;
}

void SetGoal(int goal, int marker_number) {
	gedict_t* marker = NULL;

	--marker_number;

	if (marker_number < 0 || marker_number >= NUMBER_MARKERS)
		return;

	marker = markers[marker_number];
	SetGoalForMarker (goal, marker);
}

void Set_sub_arrival_time(gedict_t* marker, int index) {
	fb_void_func_t sub_arrival_time_functions[] = { SUBZONEFUNCTIONS (time_) };
	fb_void_func_t sub_path_marker_functions[] = { SUBZONEFUNCTIONS (path_marker) };

	marker->fb.S_ = index;

	if (index < sizeof(sub_arrival_time_functions) / sizeof(sub_arrival_time_functions[0])) {
		marker->fb.sub_arrival_time = sub_arrival_time_functions[index];
		marker->fb.sub_path_marker = sub_path_marker_functions[index];
	}
}

void SetZone(int zone, int marker_number) {
	gedict_t* marker;
	fb_zone_t* z;
	fb_void_func_t zone_marker_functions[] = { ZONEFUNCTIONS(marker) };
	fb_void_func_t zone_next_zone_marker_functions[] = { ZONEFUNCTIONS(next_zone_marker) };
	fb_void_func_t zone_path_marker_functions[] = { ZONEFUNCTIONS(path_marker) };
	fb_void_func_t zone_sight_from_marker_functions[] = { ZONEFUNCTIONS(sight_from_marker) };
	fb_void_func_t zone_higher_sight_from_marker_functions[] = { ZONEFUNCTIONS(higher_sight_from_marker) };
	fb_void_func_t zone_sight_from_time_functions[] = { ZONEFUNCTIONS(sight_from_time) };

	// old frogbot code was 1-based
	--zone;
	--marker_number;

	if (zone < 0 || zone >= NUMBER_ZONES)
		return;
	if (marker_number < 0 || marker_number >= NUMBER_MARKERS)
		return;

	marker = markers[marker_number];
	z = &marker->fb.zones[zone];

	Set_sub_arrival_time(marker, subzone_indexes[zone]++);
	z->marker = z->reverse_marker = z->next_zone = z->next = z->reverse_next = marker;
	marker->fb.Z_ = zone;
	marker->fb.zone_marker = zone_marker_functions[zone];
	marker->fb.next_zone_marker = zone_next_zone_marker_functions[zone];
	marker->fb.path_marker = zone_path_marker_functions[zone];
	marker->fb.sight_from_marker = zone_sight_from_marker_functions[zone];
	marker->fb.higher_sight_from_marker = zone_higher_sight_from_marker_functions[zone];
	marker->fb.sight_from_time = zone_sight_from_time_functions[zone];
	if (zone_tail[zone]) {
		zone_tail[zone]->fb.Z_next = marker;
		zone_tail[zone] = marker;
	}
	else  {
		zone_head[zone] = zone_tail[zone] = marker;
		zone_head[zone]->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = zone_head[zone];
	}
	marker->fb.Z_head = zone_head[zone];
}

void SetMarkerFlag(int marker_number, int flags) {
	--marker_number;
	
	if (marker_number < 0 || marker_number >= NUMBER_MARKERS)
		return;

	markers[marker_number]->fb.T |= flags;
}

void NameZone(float zoneNumber, char* name) {
	gedict_t* m;

	for (m = first_marker; m; m = m->fb.marker_link) {
		if (m->fb.Z_ == zoneNumber) {
			m->s.v.netname = name;
		}
	}
}

void AdjustZone(gedict_t* zoneHead, float teamNumber, float strong, float adjust) {
	if (!zoneHead) {
		return;
	}
	zoneHead->fb.total_players += adjust;
	/*
	if (teamNumber == 1) {
		zoneHead->fb.team1_zone_players = zoneHead->fb.team1_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team1_zone_strong_players = zoneHead->fb.team1_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 2) {
		zoneHead->fb.team2_zone_players = zoneHead->fb.team2_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team2_zone_strong_players = zoneHead->fb.team2_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 3) {
		zoneHead->fb.team3_zone_players = zoneHead->fb.team3_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team3_zone_strong_players = zoneHead->fb.team3_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 4) {
		zoneHead->fb.team4_zone_players = zoneHead->fb.team4_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team4_zone_strong_players = zoneHead->fb.team4_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 5) {
		zoneHead->fb.team5_zone_players = zoneHead->fb.team5_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team5_zone_strong_players = zoneHead->fb.team5_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 6) {
		zoneHead->fb.team6_zone_players = zoneHead->fb.team6_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team6_zone_strong_players = zoneHead->fb.team6_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 7) {
		zoneHead->fb.team7_zone_players = zoneHead->fb.team7_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team7_zone_strong_players = zoneHead->fb.team7_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 8) {
		zoneHead->fb.team8_zone_players = zoneHead->fb.team8_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team8_zone_strong_players = zoneHead->fb.team8_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 9) {
		zoneHead->fb.team9_zone_players = zoneHead->fb.team9_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team9_zone_strong_players = zoneHead->fb.team9_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 10) {
		zoneHead->fb.team10_zone_players = zoneHead->fb.team10_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team10_zone_strong_players = zoneHead->fb.team10_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 11) {
		zoneHead->fb.team11_zone_players = zoneHead->fb.team11_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team11_zone_strong_players = zoneHead->fb.team11_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 12) {
		zoneHead->fb.team12_zone_players = zoneHead->fb.team12_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team12_zone_strong_players = zoneHead->fb.team12_zone_strong_players + adjust;
		}
	}
	else if (teamNumber == 13) {
		zoneHead->fb.team13_zone_players = zoneHead->fb.team13_zone_players + adjust;
		if (strong) {
			zoneHead->fb.team13_zone_strong_players = zoneHead->fb.team13_zone_strong_players + adjust;
		}
	}*/
}

void LeaveZone(int zoneNumber, float teamNumber, float strong) {
	AdjustZone(zone_head[zoneNumber], teamNumber, strong, -1);
}

void EnterZone(int zoneNumber, float teamNumber, float strong) {
	AdjustZone(zone_head[zoneNumber], teamNumber, strong, 1);
}

void DebugZoneByEnt(gedict_t* zone) {
	if (!zone) {
		return;
	}
	if (zone->fb.total_players == 0) {
		return;
	}
	bprint_fb(2, "  ");
	bprint_fb(2, zone->s.v.netname);
	bprint_fb(2, ": ");
	bprint_ftos(2, zone->fb.total_players);
	bprint_fb(2, "\\");
}

void DebugZone(int i) {
	DebugZoneByEnt(zone_head[i]);
}

void DebugZones() {
	int i = 0;

	bprint_fb(2, "Zones:\\");
	for (i = 1; i < NUMBER_ZONES; ++i) {
		DebugZone(i);
	}
}
