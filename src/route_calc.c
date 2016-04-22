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
				m->fb.paths[i].next_marker = 0;
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

		for (i = 0; i < sizeof(m->fb.subzones) / sizeof(m->fb.subzones[0]); ++i) {
			if (m->fb.S_ != i) {
				m->fb.subzones[i].time = 1000000;
			}
		}
	}

	Calc_G_time_3_path();
	return;
}

typedef qbool (*fb_path_calc_func_t)(gedict_t* m, gedict_t* m_P, float P_time);

qbool Calc_G_time_3_path_apply(gedict_t* m, gedict_t* m_P /* next */, float P_time /* traveltime */) {
	qbool no_change = TRUE;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	if (m->fb.Z_ == m_P->fb.Z_) {
		int i = 0;

		for (i = 0; i < sizeof(m->fb.subzones) / sizeof(m->fb.subzones[0]); ++i) {
			if (m->fb.subzones[i].time > (P_time + m_P->fb.subzones[i].time)) {
				no_change = FALSE;
				m->fb.subzones[i].time = P_time + m_P->fb.subzones[i].time;
				m->fb.subzones[i].next_marker = m_P;
			}
		}
	}

	return no_change;
}

void PathCalculation(fb_path_calc_func_t func) {
	// Keep going until no changes detected (hmm...)
	qbool no_change = FALSE;
	while (!no_change) {
		gedict_t* m;

		no_change = TRUE;
		for (m = first_marker; m && m != world; m = m->fb.marker_link) {
			int i = 0;

			for (i = 0; i < sizeof(from_marker->fb.paths) / sizeof(from_marker->fb.paths[0]); ++i) {
				no_change = no_change & func(m, m->fb.paths[i].next_marker, m->fb.paths[i].time);
			}
		}
	}
}

void Calc_G_time_3_path() {
	PathCalculation(Calc_G_time_3_path_apply);

	Calc_G_time_4_path();
}

qbool Calc_G_time_4_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = TRUE;
	int i = 0;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	for (i = 0; i < sizeof(m->fb.goals) / sizeof(m->fb.goals[0]); ++i) {
		if (m->fb.goals[i].time > (P_time + m_P->fb.goals[i].time)) {
			no_change = FALSE;
			m->fb.goals[i].next_marker = m_P->fb.goals[i].next_marker;
			m->fb.goals[i].time = P_time + m_P->fb.goals[i].time;
		}
	}

	return no_change;
}

void Calc_G_time_4_path() {
	PathCalculation(Calc_G_time_4_path_apply);

	Calc_G_time_5_path();
}

qbool ZoneTimeAdjust(gedict_t* m, gedict_t* m_P, int x) {
	if (m->fb.zones[x].time > (P_time + m_P->fb.zones[x].time)) { 
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
		return FALSE;
	}

	return TRUE;
}

qbool Calc_G_time_5_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = TRUE;
	int i = 0;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	for (i = 0; i < sizeof(m->fb.zones) / sizeof(m->fb.zones[0]); ++i) {
		no_change = (qbool) (no_change & ZoneTimeAdjust(m, m_P, i));
	}

	return no_change;
}

void Calc_G_time_5_path() {
	PathCalculation(Calc_G_time_5_path_apply);

	Calc_G_time_6_path();
}

qbool ZoneFromTimeAdjust(gedict_t* m, gedict_t* m_P, int x) {
	if (m_P->fb.zones[x].from_time > (m->fb.zones[x].from_time + P_time)) { 
		m_P->fb.zones[x].from_time = m->fb.zones[x].from_time + P_time; 
		return FALSE;
	}

	return TRUE;
}

qbool Calc_G_time_6_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = TRUE;
	int i = 0;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	for (i = 0; i < NUMBER_ZONES; ++i) {
		no_change = (qbool) (no_change & ZoneFromTimeAdjust(m, m_P, i));
	}

	return no_change;
}

void Calc_G_time_6_path() {
	PathCalculation(Calc_G_time_6_path_apply);

	Calc_G_time_7();
}

int CheckReversible(gedict_t* m, gedict_t* from_marker) {
	gedict_t* next_marker = from_marker->fb.zones[m->fb.Z_].next_zone;
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

	Calc_G_time_8_path();
}

qbool ZoneReverseTimeAdjust(gedict_t* m, gedict_t* m_P, int x) {
	if (m->fb.zones[x].reverse_time > (P_time + m_P->fb.zones[x].reverse_time)) { 
		m->fb.zones[x].reverse_marker = m_P->fb.zones[x].reverse_marker; 
		m->fb.zones[x].reverse_time = P_time + m_P->fb.zones[x].reverse_time; 
		m->fb.zones[x].reverse_next = m_P; 

		return FALSE;
	}

	return TRUE;
}

qbool Calc_G_time_8_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = TRUE;
	int i = 0;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	if (m_D & REVERSIBLE) {
		for (i = 0; i < sizeof(m->fb.zones) / sizeof(m->fb.zones[0]); ++i) {
			no_change = (qbool) (no_change & ZoneReverseTimeAdjust(m, m_P, i));
		}
	}

	return (qbool) no_change;
}

void Calc_G_time_8_path() {
	PathCalculation(Calc_G_time_8_path_apply);

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

qbool ZoneMinSightFromTimeCalc(gedict_t* m, gedict_t* m_P, int x) {
	if (m->fb.zones[x].sight_from_time < (m_P->fb.zones[x].sight_from_time - P_time)) {
		m->fb.zones[x].sight_from_time = m_P->fb.zones[x].sight_from_time - P_time; 
		return FALSE;
	}

	return TRUE;
}

qbool Calc_G_time_10_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = TRUE;
	int i = 0;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	for (i = 0; i < sizeof(m->fb.zones) / sizeof(m->fb.zones[0]); ++i) {
		no_change = (qbool) (no_change & ZoneMinSightFromTimeCalc(m, m_P, i));
	}

	return no_change;
}

void Calc_G_time_10_path() {
	PathCalculation(Calc_G_time_10_path_apply);

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
			if (middle_marker != dropper && middle_marker != m) {
				runaway_dest = middle_marker;
				runaway_time = path_normal ? zone_time : zone_time + 5;
				
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
}

