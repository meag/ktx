// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

#define MAX_MARKER_COUNT 400

void Calc_G_time_3_path();
void Calc_G_time_4_path();
void Calc_G_time_5_path();
void Calc_G_time_6_path();
void Calc_G_time_8_path();
void Calc_G_time_10_path();
void Calc_G_time_7();
void Calc_G_time_9();
void Calc_G_time_11();
void Calc_G_time_12();
void Calc_G_time_13();
void Calc_G_time_14();
void Calc_G_time_2();

typedef struct frogbots_s {
	gedict_t* markers[MAX_MARKER_COUNT];
	int marker_count;
} frogbots_t;

frogbots_t frogbots;

// This calculates water columns (if marker is in water, can the player go straight up to get air?)
void InitialiseMarkerRoutes() {
	self = dropper;
	for (m = first_marker; m && m != world; m = m->fb.marker_link) {
		vec3_t point;

		VectorAdd(m->s.v.absmin, m->s.v.view_ofs, m_pos);
		m->fb.touch_marker = m;

		VectorCopy(m_pos, point);
		point[2] += 4;
		content = trap_pointcontents(PASSVEC3(point));
		if (content >= CONTENT_LAVA) {
			if (content <= CONTENT_WATER) {
				CheckWaterColumn(m, m_pos, testplace);
				if (testplace[2] - m_pos[2] > 0) {
					setsize(m, m->s.v.mins[0], m->s.v.mins[1], m->s.v.mins[2], m->s.v.maxs[0] + testplace[0] - m_pos[0], m->s.v.maxs[1] + testplace[1] - m_pos[1], m->s.v.maxs[2] + testplace[2] - m_pos[2]);
				}
				count_ = count_ - 1;
			}
		}
	}

	m = dropper;
	Calc_G_time_2();
}

void TravelTime() {
	if (m_D & ROCKET_JUMP) {
		traveltime = 100000;
		return;
	}
	if (m_D & JUMP_LEDGE) {
		m_D = m_D | NO_DODGE;
	}
	if (streq(m->s.v.classname, "trigger_teleport")) {
		m_P->fb.near_teleport = m;
		traveltime = 0;
	}
	else  {
		VectorAdd(m_P->s.v.absmin, m_P->s.v.view_ofs, m_P_pos);
		if ((m->fb.T & T_WATER) || (m_P->fb.T & T_WATER)) {
			m_D = m_D | WATER_PATH;
			traveltime = ((VectorDistance(m_P_pos, m_pos) / sv_maxwaterspeed));
		}
		else  {
			traveltime = ((VectorDistance(m_P_pos, m_pos) / sv_maxspeed));
		}
	}
}

void Calc_G_time_2() {
	for (m = first_marker; m && m != world; m = m->fb.marker_link) {
		int i = 0;

		VectorAdd(m->s.v.absmin, m->s.v.view_ofs, m_pos);

		for (i = 0; i < sizeof(m->fb.paths) / sizeof(m->fb.paths[0]); ++i) {
			m_P = m->fb.paths[i].next_marker;
			if (m_P && m_P->fb.fl_marker) {
				m_D = m->fb.paths[i].flags;
				TravelTime();
				m->fb.paths[i].time = traveltime;
				m->fb.paths[i].flags = m_D;
			}
			else  {
				m->fb.paths[i].next_marker = world;
			}
		}

		for (i = 0; i < sizeof(m->fb.goals) / sizeof(m->fb.goals[0]); ++i) {
			if (! m->fb.goals[i].next_marker) {
				m->fb.goals[i].time = 1000000;
				m->fb.goals[i].next_marker = dropper;
			}
		}

		for (i = 0; i < sizeof(m->fb.zones) / sizeof(m->fb.zones[0]); ++i) {
			if (! m->fb.zones[i].marker) {
				m->fb.zones[i].time = m->fb.zones[i].reverse_time = m->fb.zones[i].from_time = 1000000;
				m->fb.zones[i].marker = m->fb.zones[i].reverse_marker = dropper;
			}

			m->fb.zones[i].sight_from_time = 1000000;
		}

		m->fb.Z1_height_equal_time = 1000000;
		m->fb.Z8_height_equal_time = 1000000;

		for (i = 0; i < sizeof(m->fb.subzones) / sizeof(m->fb.subzones[0]); ++i) {
			if (m->fb.S_ != i) {
				m->fb.subzones[i].time = 1000000;
			}
		}
	}

	Calc_G_time_3_path();
	//m = first_marker;
	//dropper->s.v.use = (func_t) Calc_G_time_3_path;
	//dropper->s.v.think = (func_t) Calc_G_time_path;
	//set_load();
	return;
}

typedef qbool (*fb_path_calc_func_t)(gedict_t* m, gedict_t* m_P, float P_time);

qbool Calc_G_time_3_path_apply(gedict_t* m, gedict_t* m_P /* next */, float P_time /* traveltime */) {
	qbool no_change = (qbool) true;

	if (!m || m == world)
		return no_change;
	if (!m_P || m_P == world)
		return no_change;

	if (m->fb.Z_ == m_P->fb.Z_) {
		int i = 0;

		for (i = 0; i < sizeof(m->fb.subzones) / sizeof(m->fb.subzones[0]); ++i) {
			if (m->fb.subzones[i].time > (P_time + m_P->fb.subzones[i].time)) {
				no_change = (qbool) false;
				m->fb.subzones[i].time = P_time + m_P->fb.subzones[i].time;
				m->fb.subzones[i].next_marker = m_P;
			}
		}
	}

	return no_change;
}

void PathCalculation(fb_path_calc_func_t func) {
	// Keep going until no changes detected (hmm...)
	int no_change = FALSE;
	while (!no_change) {
		gedict_t* m;

		no_change = TRUE;
		for (m = first_marker; m && m != world; m = m->fb.marker_link) {
			int i = 0;

			for (i = 0; i < sizeof(from_marker->fb.paths) / sizeof(from_marker->fb.paths[0]); ++i) {
				no_change &= func(m, m->fb.paths[i].next_marker, m->fb.paths[i].time);
			}
		}
	}
}

void Calc_G_time_3_path() {
	PathCalculation(Calc_G_time_3_path_apply);
	Com_Printf("Time[3] complete\n");

	Calc_G_time_4_path();
}

qbool Calc_G_time_4_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = (qbool) true;
	int i = 0;

	if (!m || m == world)
		return no_change;
	if (!m_P || m_P == world)
		return no_change;

	for (i = 0; i < sizeof(m->fb.goals) / sizeof(m->fb.goals[0]); ++i) {
		if (m->fb.goals[i].time > (P_time + m_P->fb.goals[i].time)) {
			no_change = (qbool) false;
			m->fb.goals[i].next_marker = m_P->fb.goals[i].next_marker;
			m->fb.goals[i].time = P_time + m_P->fb.goals[i].time;
		}
	}

	return no_change;
}

void Calc_G_time_4_path() {
	PathCalculation(Calc_G_time_4_path_apply);
	Com_Printf("Time[4] complete\n");

	Calc_G_time_5_path();
}

void ZoneTimeAdjust(gedict_t* m, gedict_t* m_P, int x) {
	if (m->fb.zones[x].time > (P_time + m_P->fb.zones[x].time)) { 
		no_change = (qbool) false; 
		m->fb.zones[x].marker = m_P->fb.zones[x].marker; 
		m->fb.zones[x].time = P_time + m_P->fb.zones[x].time; 
		m->fb.zones[x].next = m_P; 
		{ 
			if (m->fb.Z_ == m_P->fb.Z_) { 
				m->fb.zones[x].next_zone = m_P->fb.zones[x].next_zone; 
			} 
			else  { 
				m->fb.zones[x].next_zone = m_P; 
			} 
		} 
		m->fb.zones[x].task |= m_D; 
	}
}

qbool Calc_G_time_5_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = (qbool) true;
	int i = 0;

	if (!m || m == world)
		return no_change;
	if (!m_P || m_P == world)
		return no_change;

	for (i = 0; i < sizeof(m->fb.zones) / sizeof(m->fb.zones[0]); ++i) {
		ZoneTimeAdjust(m, m_P, i);
	}

	return no_change;
}

void Calc_G_time_5_path() {
	PathCalculation(Calc_G_time_5_path_apply);
	Com_Printf("Time[5] complete\n");

	Calc_G_time_6_path();
}

#define ZONE_FROM_TIME_ADJUST(m, m_P, x) \
	if (m_P->fb.Z ## x ## _from_time > (m->fb.Z ## x ## _from_time + P_time)) { \
		no_change = (qbool) false; \
		m_P->fb.Z ## x ## _from_time = m->fb.Z ## x ## _from_time + P_time; \
	}

void ZoneFromTimeAdjust(gedict_t* m, gedict_t* m_P, int x) {
	if (m_P->fb.zones[x].from_time > (m->fb.zones[x].from_time + P_time)) { 
		no_change = (qbool) false; 
		m_P->fb.zones[x].from_time = m->fb.zones[x].from_time + P_time; 
	}
}

qbool Calc_G_time_6_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = (qbool) true;
	int i = 0;

	if (!m || m == world)
		return no_change;
	if (!m_P || m_P == world)
		return no_change;

	for (i = 0; i < NUMBER_ZONES; ++i) {
		ZoneFromTimeAdjust(m, m_P, i);
	}

	return no_change;
}

void Calc_G_time_6_path() {
	PathCalculation(Calc_G_time_6_path_apply);
	Com_Printf("Time[6] complete\n");

	Calc_G_time_7();
}

int CheckReversible(gedict_t* m, gedict_t* from_marker) {
	gedict_t* next_marker = from_marker->fb.zones[m->fb.Z_].next_zone;// m->fb.next_zone_marker();
	if (next_marker->fb.Z_ == m->fb.Z_) {
		return REVERSIBLE;
	}
	else  {
		return 0;
	}
}

// if path from m1->m2 and m2->m1, set the reversible flag
void Calc_G_time_7() {
	gedict_t* m, *from_marker;

	for (m = first_marker; m; m = m->fb.marker_link) {
		int i = 0;

		for (i = 0; i < sizeof(m->fb.paths) / sizeof(m->fb.paths[0]); ++i) {
			from_marker = m->fb.paths[i].next_marker;
			if (from_marker && from_marker != world) {
				m->fb.paths[i].flags |= CheckReversible(m, from_marker);
			}
		}
	}

	Com_Printf("Time[7] complete\n");

	Calc_G_time_8_path();
}

#define ZONE_REVERSE_TIME_ADJUST(m, m_P, x) \
	if (m->fb.Z ## x ## _time_rev > (P_time + m_P->fb.Z ## x ## _time_rev)) { \
		no_change = (qbool) false; \
		m->fb.Z ## x ## _rev = m_P->fb.Z ## x ## _rev; \
		m->fb.Z ## x ## _time_rev = P_time + m_P->fb.Z ## x ## _time_rev; \
		m->fb.Z ## x ## _next_rev = m_P; \
	}

void ZoneReverseTimeAdjust(gedict_t* m, gedict_t* m_P, int x) {
	if (m->fb.zones[x].reverse_time > (P_time + m_P->fb.zones[x].reverse_time)) { 
		no_change = (qbool) false; 
		m->fb.zones[x].reverse_marker = m_P->fb.zones[x].reverse_marker; 
		m->fb.zones[x].reverse_time = P_time + m_P->fb.zones[x].reverse_time; 
		m->fb.zones[x].reverse_next = m_P; 
	}
}

qbool Calc_G_time_8_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = (qbool) true;
	int i = 0;

	if (!m || m == world)
		return no_change;
	if (!m_P || m_P == world)
		return no_change;

	if (m_D & REVERSIBLE) {
		for (i = 0; i < sizeof(m->fb.zones) / sizeof(m->fb.zones[0]); ++i) {
			ZoneReverseTimeAdjust(m, m_P, i);
		}
	}

	return (qbool) no_change;
}

void Calc_G_time_8_path() {
	PathCalculation(Calc_G_time_8_path_apply);
	Com_Printf("Time[8] complete\n");

	Calc_G_time_9();
}

void ZoneSightFromMarkerCalculate(gedict_t* m, gedict_t* m_2, int x) {
	if (m->fb.zones[x].sight_from_time > m_2->fb.zones[x].from_time) { 
		m->fb.zones[x].sight_from_time = m_2->fb.zones[x].from_time; 
		m->fb.zones[x].sight_from = m_2; 
	} 
	else if (!m_2->fb.zones[x].from_time) { 
		m->fb.zones[x].sight_from = world; 
	}
}

#define ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, x) \
	if (m->fb.Z ## x ## _higher_sight_from_time_ > m_2->fb.Z ## x ## _from_time) { \
		m->fb.Z ## x ## _higher_sight_from_time_ = m_2->fb.Z ## x ## _from_time; \
		m->fb.Z ## x ## _higher_sight_from = m_2; \
	} \
	else if (!m_2->fb.Z ## x ## _from_time) { \
		m->fb.Z ## x ## _higher_sight_from = world; \
	}

void ZoneHigherSightFromMarkerCalculate(gedict_t* m, gedict_t* m_2, int x) {
	if (m->fb.zones[x].higher_sight_from_time > m_2->fb.zones[x].from_time) { 
		m->fb.zones[x].higher_sight_from_time = m_2->fb.zones[x].from_time; 
		m->fb.zones[x].higher_sight_from = m_2; 
	} 
	else if (!m_2->fb.zones[x].from_time) { 
		m->fb.zones[x].higher_sight_from = world; 
	}
}

void Calc_G_time_9_apply(gedict_t* m, gedict_t* m_2, vec3_t m_pos, vec3_t m2_pos) {
	int i = 0; 

	for (i = 0; i < sizeof(m->fb.zones) / sizeof(m->fb.zones[0]); ++i) {
		ZoneSightFromMarkerCalculate(m, m_2, i);
	}

	if (m2_pos[2] - m_pos[2] >= 40) {
		if (VectorDistance(m2_pos, m_pos) <= 1000) {
			for (i = 0; i < sizeof(m->fb.zones) / sizeof(m->fb.zones[0]); ++i) {
				ZoneHigherSightFromMarkerCalculate(m, m_2, i);
			}
		}
	}
}

void Calc_G_time_9() {
	gedict_t* m;
	gedict_t* m2;
	vec3_t m_pos, m2_pos;

	for (m = first_marker; m && m != world; m = m->fb.marker_link) {
		for (m2 = first_marker; m2 && m2 != world; m2 = m2->fb.marker_link) {
			VectorAdd(m->s.v.absmin, m->s.v.view_ofs, m_pos);
			m_pos[2] += 32;
		
			VectorAdd(m2->s.v.absmin, m2->s.v.view_ofs, m2_pos);
			m2_pos[2] += 32;

			traceline(m_pos[0], m_pos[1], m_pos[2], m2_pos[0], m2_pos[1], m2_pos[2], TRUE, world);
			if (g_globalvars.trace_fraction == 1) {
				if (strneq(m2->s.v.classname, "trigger_teleport")) {
					Calc_G_time_9_apply(m, m2, m_pos, m2_pos);
				}
			}
		}
	}

	Calc_G_time_10_path();
}

void ZoneMinSightFromTimeCalc(gedict_t* m, gedict_t* m_P, int x) {
	if (m->fb.zones[x].sight_from_time < (m_P->fb.zones[x].sight_from_time - P_time)) {
		no_change = (qbool) false; 
		m->fb.zones[x].sight_from_time = m_P->fb.zones[x].sight_from_time - P_time; 
	} 
}

qbool Calc_G_time_10_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = (qbool) true;
	int i = 0;

	if (!m || m == world)
		return no_change;
	if (!m_P || m_P == world)
		return no_change;

	for (i = 0; i < sizeof(m->fb.zones) / sizeof(m->fb.zones[0]); ++i) {
		ZoneMinSightFromTimeCalc(m, m_P, i);
	}

	return no_change;
}

void Calc_G_time_10_path() {
	PathCalculation(Calc_G_time_10_path_apply);
	Com_Printf("Time[10] complete\n");

	Calc_G_time_11();
}

void Calc_G_time_11_apply() {
	int i = 0;
	int found = 0;

	min_traveltime = min_traveltime + 1.25;
	runaway_score = runaway_score * 0.125;

	for (i = 0; i < sizeof(m->fb.runaway) / sizeof(m->fb.runaway[0]); ++i) {
		if (m->fb.runaway[i].next_marker == next_marker) {
			if (m->fb.runaway[i].score >= runaway_score) {
				return;
			}
			m->fb.runaway[i].prev_marker = prev_marker;
			m->fb.runaway[i].score = runaway_score;
			m->fb.runaway[i].time = runaway_time;
			found = i;
			break;
		}
	}

	if (found <= 0)
		return;

	// Sort so highest score is first in array (subsequent indexes don't matter)
	for (i = found; i > 0; --i) {
		if (m->fb.runaway[i-1].score < m->fb.runaway[i].score) {
			fb_runaway_route_t temp;

			memcpy(&temp, &m->fb.runaway[i-1], sizeof(temp));
			memcpy(&m->fb.runaway[i-1], &m->fb.runaway[i], sizeof(temp));
			memcpy(&m->fb.runaway[i], &temp, sizeof(temp));
		}
	}
}

void Calc_G_time_11() {
	gedict_t* m_zone;
	gedict_t* m;

	for (m = first_marker; m && m != world; m = m->fb.marker_link) {
		for (m_zone = zone_stack_head; m_zone && m_zone != world; m_zone = m_zone->fb.zone_stack_next) {
			from_marker = m;
			m_zone->fb.zone_marker();
			if (middle_marker != dropper) {
				if (middle_marker != m) {
					runaway_dest = middle_marker;
					if (path_normal) {
						runaway_time = zone_time;
					}
					else  {
						runaway_time = zone_time + 5;
					}
					runaway_score = runaway_time;
					next_marker = m;
					min_traveltime = 1.25;
					do {
						from_marker = prev_marker = next_marker;
						runaway_dest->fb.path_marker();
						from_marker = m;
						next_marker->fb.zone_marker();
						next_marker->fb.sub_arrival_time();
						if (traveltime >= min_traveltime) {
							if (strneq(next_marker->s.v.classname, "trigger_teleport")) {
								Calc_G_time_11_apply();
							}
						}
					} while (next_marker != runaway_dest);
				}
			}

		}
	}

	Com_Printf("Time[11] complete\n");
	m_zone = zone_stack_head;
	if (path_normal) {
		path_normal = FALSE;
	}
	else  {
		path_normal = TRUE;
		Calc_G_time_12();
	}
}

void Calc_G_time_12() {
	gedict_t* m;
	gedict_t* runaway_dest;

	for (m = first_marker; m && m != world; m = m->fb.marker_link) {
		middle_marker = m;
		zone_time = 0;
		VectorAdd(m->s.v.absmin, m->s.v.view_ofs, m_pos);
		m_pos[2] += 32;

		for (runaway_dest = m->fb.Z_head; runaway_dest && runaway_dest != world; runaway_dest = runaway_dest->fb.Z_next) {
			if (runaway_dest != m) {
				from_marker = m;
				runaway_dest->fb.sub_arrival_time();
				if (traveltime < 1000000) {
					runaway_score = runaway_time = traveltime;
					next_marker = m;
					min_traveltime = 1.25;
					do
					{
						from_marker = prev_marker = next_marker;
						runaway_dest->fb.sub_path_marker();
						traceline(m_pos[0], m_pos[1], m_pos[2], next_marker->s.v.absmin[0] + next_marker->s.v.view_ofs[0], next_marker->s.v.absmin[1] + next_marker->s.v.view_ofs[1], next_marker->s.v.absmin[2] + next_marker->s.v.view_ofs[2] + 32, TRUE, world);
						if (g_globalvars.trace_fraction != 1) {
							from_marker = m;
							next_marker->fb.sub_arrival_time();
							if (traveltime >= min_traveltime) {
								if (strneq(next_marker->s.v.classname, "trigger_teleport")) {
									Calc_G_time_11_apply();
								}
							}
						}
					} while (next_marker != runaway_dest);
				}
			}
		}
	}

	Com_Printf("Time[12] complete\n");
	//Calc_G_time_13(); // TODO
	Calc_G_time_14();
}

/*
void Check_RA_type2() {
	if (to_marker) {
		if (to_marker != next_marker) {
			to_marker->fb.zone_marker();
			to_marker->fb.sub_arrival_time();
			traveltime3 = traveltime;
			if (traveltime3 < 5) {
				path_normal = FALSE;
				from_marker = next_marker;
				to_marker->fb.zone_marker();
				to_marker->fb.sub_arrival_time();
				traveltime2 = traveltime;
				from_marker = prev_marker;
				to_marker->fb.zone_marker();
				to_marker->fb.sub_arrival_time();
				if (traveltime2 < traveltime) {
					from_marker = m;
					next_marker->fb.zone_marker();
					next_marker->fb.sub_arrival_time();
					traveltime2 = traveltime + traveltime2;
					if (traveltime2 < (traveltime3 + 0.5)) {
						RA_type = THROUGH_NODE;
					}
				}
				path_normal = TRUE;
			}
		}
	}
}*/
/*
void Check_RA_type() {
	RA_type = 0;
	to_marker = m->fb.RA0;
	Check_RA_type2();
	to_marker = m->fb.RA1;
	Check_RA_type2();
	to_marker = m->fb.RA2;
	Check_RA_type2();
	to_marker = m->fb.RA3;
	Check_RA_type2();
	to_marker = m->fb.RA4;
	Check_RA_type2();
	to_marker = m->fb.RA5;
	Check_RA_type2();
	to_marker = m->fb.RA6;
	Check_RA_type2();
	to_marker = m->fb.RA7;
	Check_RA_type2();
}*/

/*
void Calc_G_time_13() {
	self->s.v.nextthink = 0.001;
	count_ = 25;
	while (count_) {
		int i = 0;
		for (i = 0; i < 8; ++i) {
			if (m->fb.runaway[i].next_marker && m->fb.runaway[i].next_marker != world) {
				next_marker = 
			}
		}
		next_marker = m->fb.RA0;
		if (next_marker) {
			prev_marker = m->fb.RA0_prev;
			Check_RA_type();
			m->fb.RA0_type = RA_type;
		}
		next_marker = m->fb.RA1;
		if (next_marker) {
			prev_marker = m->fb.RA1_prev;
			Check_RA_type();
			m->fb.RA1_type = RA_type;
		}
		next_marker = m->fb.RA2;
		if (next_marker) {
			prev_marker = m->fb.RA2_prev;
			Check_RA_type();
			m->fb.RA2_type = RA_type;
		}
		next_marker = m->fb.RA3;
		if (next_marker) {
			prev_marker = m->fb.RA3_prev;
			Check_RA_type();
			m->fb.RA3_type = RA_type;
		}
		next_marker = m->fb.RA4;
		if (next_marker) {
			prev_marker = m->fb.RA4_prev;
			Check_RA_type();
			m->fb.RA4_type = RA_type;
		}
		next_marker = m->fb.RA5;
		if (next_marker) {
			prev_marker = m->fb.RA5_prev;
			Check_RA_type();
			m->fb.RA5_type = RA_type;
		}
		next_marker = m->fb.RA6;
		if (next_marker) {
			prev_marker = m->fb.RA6_prev;
			Check_RA_type();
			m->fb.RA6_type = RA_type;
		}
		next_marker = m->fb.RA7;
		if (next_marker) {
			prev_marker = m->fb.RA7_prev;
			Check_RA_type();
			m->fb.RA7_type = RA_type;
		}
		count_ = count_ - 1;
		m = m->fb.marker_link;
		if (!m) {
			m = first_marker;
			dropper->s.v.think = (func_t) Calc_G_time_14;
			set_load();
			return;
		}
	}
}
*/

void Calc_G_time_14() {

	gedict_t* m;
	gedict_t* m_zone;

	for (m = first_marker; m && m != world; m = m->fb.marker_link) {
		for (m_zone = zone_stack_head; m_zone && m_zone != world; m_zone = m_zone->fb.zone_stack_next) {
			from_marker = m;
			m_zone->fb.zone_marker();
			if (middle_marker != dropper) {
				if (traveltime >= 5) {
					Z1_path_success = TRUE;
					Z8_path_success = TRUE;
					runaway_dest = middle_marker;
					next_marker = m;
					while (next_marker != runaway_dest) {
						from_marker = m;
						next_marker->fb.zone_marker();
						next_marker->fb.sub_arrival_time();
						if (next_marker->fb.zones[0].higher_sight_from_time <= traveltime) {
							Z1_path_success = FALSE;
						}
						if (next_marker->fb.zones[7].higher_sight_from_time <= traveltime) {
							Z8_path_success = FALSE;
						}
						from_marker = next_marker;
						runaway_dest->fb.path_marker();
					}
					if (Z1_path_success) {
						m->fb.Z1_height_equal_time = 0;
					}
					if (Z8_path_success) {
						m->fb.Z8_height_equal_time = 0;
					}
				}
			}
		}
	}
	if (path_normal) {
		path_normal = FALSE;
	}
	else  {
		path_normal = TRUE;
	}
}

