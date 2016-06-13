// Converted from .qc on 05/02/2016

// After markers have been created and zone/subzone set, this calculates travel time
//   between different markers & goals etc.
// Single entry point InitialiseMarkerRoutes()

// TODO: 5/6/10 have near-identical body, can be ForAllZones(func)

#include "g_local.h"
#include "fb_globals.h"

typedef qbool (*fb_path_calc_func_t)(gedict_t* m, gedict_t* m_P, float P_time, int m_D);

static void Calc_G_time_12 (void);

// 
static float runaway_score = 0;
static float min_traveltime = 0;
static float runaway_time = 0;

// FIXME: Globals
extern gedict_t* first_marker;
extern float P_time;
extern gedict_t* dropper;
extern gedict_t* zone_stack_head;

static void TravelTimeForPath (gedict_t* m, int i)
{
	gedict_t* m_P = m->fb.paths[i].next_marker;
	vec3_t m_pos;

	VectorAdd(m->s.v.absmin, m->s.v.view_ofs, m_pos);

	if (m->fb.paths[i].flags & ROCKET_JUMP) {
		m->fb.paths[i].time = 100000;
		return;
	}

	if (m->fb.paths[i].flags & JUMP_LEDGE) {
		m->fb.paths[i].flags |= NO_DODGE;
	}

	// Distance irrelevant if teleporting
	if (streq(m->s.v.classname, "trigger_teleport")) {
		m_P->fb.near_teleport = m;
		m->fb.paths[i].time = 0;
	}
	else {
		vec3_t m_P_pos;

		VectorAdd(m_P->s.v.absmin, m_P->s.v.view_ofs, m_P_pos);
		if ((m->fb.T & T_WATER) || (m_P->fb.T & T_WATER)) {
			m->fb.paths[i].flags |= WATER_PATH;
			m->fb.paths[i].time = (VectorDistance(m_P_pos, m_pos) / sv_maxwaterspeed);
		}
		else {
			if (m->fb.index == 26 && m_P->fb.index == 120) {
				Com_Printf ("1 => 2 = [%f %f %f] => [%f %f %f] = %f / %f = %f seconds\n", PASSVEC3(m_pos), PASSVEC3(m_P_pos), VectorDistance (m_P_pos, m_pos), sv_maxspeed, VectorDistance (m_P_pos, m_pos) / sv_maxspeed);
			}

			m->fb.paths[i].time = (VectorDistance(m_P_pos, m_pos) / sv_maxspeed);
		}
	}
}
/*
static float TravelTime(gedict_t* m, gedict_t* m_P, int* m_D) {
	vec3_t m_pos;

	VectorAdd(m->s.v.absmin, m->s.v.view_ofs, m_pos);

	if (*m_D & ROCKET_JUMP) {
		return 100000;
	}
	if (*m_D & JUMP_LEDGE) {
		*m_D |= NO_DODGE;
	}
	if (streq(m->s.v.classname, "trigger_teleport")) {
		m_P->fb.near_teleport = m;
		return 0;
	}
	else {
		vec3_t m_P_pos;

		VectorAdd(m_P->s.v.absmin, m_P->s.v.view_ofs, m_P_pos);
		if ((m->fb.T & T_WATER) || (m_P->fb.T & T_WATER)) {
			*m_D |= WATER_PATH;
			return ((VectorDistance(m_P_pos, m_pos) / sv_maxwaterspeed));
		}
		else {
			if (m->fb.index == 1 && m_P->fb.index == 2) {
				Com_Printf ("1 => 2 = [%f %f %f] => [%f %f %f] = %f / %f = %f seconds\n", PASSVEC3(m_pos), PASSVEC3(m_P_pos), VectorDistance (m_P_pos, m_pos), sv_maxspeed, VectorDistance (m_P_pos, m_pos) / sv_maxspeed);
			}

			return ((VectorDistance(m_P_pos, m_pos) / sv_maxspeed));
		}
	}
}
*/

// Was: Calc_G_time_3_path_apply
static qbool IdentifyFastestSubzoneRoute(gedict_t* m, gedict_t* m_P /* next */, float P_time /* traveltime */, int m_D) {
	qbool no_change = true;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	if (m->fb.Z_ == m_P->fb.Z_) {
		int i = 0;

		for (i = 0; i < NUMBER_SUBZONES; ++i) {
			if (m->fb.subzones[i].time > (P_time + m_P->fb.subzones[i].time)) {
				no_change = false;
				m->fb.subzones[i].time = P_time + m_P->fb.subzones[i].time;
				m->fb.subzones[i].next_marker = m_P;
			}
		}
	}

	return no_change;
}

static qbool FastestGoalCheck (gedict_t* m, gedict_t* m_P, int i, float P_time)
{
	if (m->fb.goals[i].time > (P_time + m_P->fb.goals[i].time)) {
		m->fb.goals[i].next_marker = m_P->fb.goals[i].next_marker;
		m->fb.goals[i].time = P_time + m_P->fb.goals[i].time;
		return false;
	}

	return true;
}

// Calc_G_time_4_path_apply
static qbool IdentifyFastestGoalRoute(gedict_t* m, gedict_t* m_P, float P_time, int m_D) {
	qbool no_change = true;
	int i = 0;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	for (i = 0; i < NUMBER_GOALS; ++i) {
		no_change &= FastestGoalCheck (m, m_P, i, P_time);
	}

	return no_change;
}

// Next two functions: Calc_G_time_5_path_apply
static qbool IdentifyFastestZoneRoute(gedict_t* m, gedict_t* m_P, int x, float P_time, int m_D) {
	if (m->fb.zones[x].time > (P_time + m_P->fb.zones[x].time)) { 
		m->fb.zones[x].marker = m_P->fb.zones[x].marker; 
		m->fb.zones[x].time = P_time + m_P->fb.zones[x].time; 
		m->fb.zones[x].next = m_P; 
		m->fb.zones[x].next_zone = (m->fb.Z_ == m_P->fb.Z_ ? m_P->fb.zones[x].next_zone : m_P); 
		if (x == 0)
			m->fb.zones[x].task |= m_D; 
		return false;
	}

	return true;
}

static qbool Calc_G_time_5_path_apply(gedict_t* m, gedict_t* m_P, float P_time, int m_D) {
	qbool no_change = true;
	int i = 0;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	for (i = 0; i < NUMBER_ZONES; ++i) {
		no_change &= IdentifyFastestZoneRoute(m, m_P, i, P_time, m_D);
	}

	return no_change;
}

static qbool ZoneFromTimeAdjust(gedict_t* m, gedict_t* m_P, int x, float P_time, int m_D) {
	if (m_P->fb.zones[x].from_time > (m->fb.zones[x].from_time + P_time)) { 
		m_P->fb.zones[x].from_time = m->fb.zones[x].from_time + P_time; 
		return false;
	}

	return true;
}

// was: Calc_G_time_6_path_apply
static qbool Calc_G_time_6_path_apply(gedict_t* m, gedict_t* m_P, float P_time, int m_D) {
	qbool no_change = true;
	int i = 0;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	for (i = 0; i < NUMBER_ZONES; ++i) {
		no_change &= ZoneFromTimeAdjust (m, m_P, i, P_time, m_D);
	}

	return no_change;
}

static int CheckReversible(gedict_t* m, gedict_t* from_marker) {
	gedict_t* next_marker = from_marker->fb.zones[m->fb.Z_].next_zone;
	if (next_marker != NULL && next_marker->fb.Z_ == m->fb.Z_) {
		return REVERSIBLE;
	}
	else  {
		return 0;
	}
}

// if path from m1->m2 and m2->m1, set the reversible flag
static void Calc_G_time_7(void) {
	gedict_t* m, *from_marker;

	for (m = first_marker; m; m = m->fb.marker_link) {
		int i = 0;

		for (i = 0; i < NUMBER_PATHS; ++i) {
			from_marker = m->fb.paths[i].next_marker;
			if (from_marker && from_marker != world) {
				m->fb.paths[i].flags |= CheckReversible(m, from_marker);
			}
		}
	}
}

static qbool ZoneReverseTimeAdjust(gedict_t* m, gedict_t* m_P, int x, int m_D) {
	if (m->fb.zones[x].reverse_time > (P_time + m_P->fb.zones[x].reverse_time)) { 
		m->fb.zones[x].reverse_marker = m_P->fb.zones[x].reverse_marker; 
		m->fb.zones[x].reverse_time = P_time + m_P->fb.zones[x].reverse_time; 
		m->fb.zones[x].reverse_next = m_P; 

		return false;
	}

	return true;
}

static qbool Calc_G_time_8_path_apply(gedict_t* m, gedict_t* m_P, float P_time, int m_D) {
	qbool no_change = true;
	int i = 0;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	if (m_D & REVERSIBLE) {
		for (i = 0; i < NUMBER_ZONES; ++i) {
			no_change &= ZoneReverseTimeAdjust(m, m_P, i, m_D);
		}
	}

	return (qbool) no_change;
}

static void ZoneSightFromMarkerCalculate(gedict_t* m, gedict_t* m_2, int x) {
	if (m->fb.zones[x].sight_from_time > m_2->fb.zones[x].from_time) { 
		m->fb.zones[x].sight_from_time = m_2->fb.zones[x].from_time; 
		m->fb.zones[x].sight_from = m_2; 
	} 
	else if (!m_2->fb.zones[x].from_time) { 
		m->fb.zones[x].sight_from = NULL; 
	}
}

static void ZoneHigherSightFromMarkerCalculate(gedict_t* m, gedict_t* m_2, int x) {
	if (m->fb.zones[x].higher_sight_from_time > m_2->fb.zones[x].from_time) { 
		m->fb.zones[x].higher_sight_from_time = m_2->fb.zones[x].from_time; 
		m->fb.zones[x].higher_sight_from = m_2; 
	} 
	else if (!m_2->fb.zones[x].from_time) { 
		m->fb.zones[x].higher_sight_from = NULL; 
	}
}

static void Calc_G_time_9_apply(gedict_t* m, gedict_t* m_2, vec3_t m_pos, vec3_t m2_pos) {
	int i = 0; 

	for (i = 0; i < NUMBER_ZONES; ++i) {
		ZoneSightFromMarkerCalculate(m, m_2, i);
	}

	if (m2_pos[2] - m_pos[2] >= 40) {
		if (VectorDistance(m2_pos, m_pos) <= 1000) {
			for (i = 0; i < NUMBER_ZONES; ++i) {
				ZoneHigherSightFromMarkerCalculate(m, m_2, i);
			}
		}
	}
}

static void Calc_G_time_9(void) {
	gedict_t* m;
	gedict_t* m2;
	vec3_t m_pos, m2_pos;

	for (m = first_marker; m && m != world; m = m->fb.marker_link) {
		for (m2 = first_marker; m2 && m2 != world; m2 = m2->fb.marker_link) {
			VectorAdd(m->s.v.absmin, m->s.v.view_ofs, m_pos);
			m_pos[2] += 32;
		
			VectorAdd(m2->s.v.absmin, m2->s.v.view_ofs, m2_pos);
			m2_pos[2] += 32;

			traceline(m_pos[0], m_pos[1], m_pos[2], m2_pos[0], m2_pos[1], m2_pos[2], true, world);
			if (g_globalvars.trace_fraction == 1) {
				if (strneq(m2->s.v.classname, "trigger_teleport")) {
					Calc_G_time_9_apply(m, m2, m_pos, m2_pos);
				}
			}
		}
	}
}

static qbool ZoneMinSightFromTimeCalc(gedict_t* m, gedict_t* m_P, int x, int m_D) {
	if (m->fb.zones[x].sight_from_time < (m_P->fb.zones[x].sight_from_time - P_time)) {
		m->fb.zones[x].sight_from_time = m_P->fb.zones[x].sight_from_time - P_time; 
		return false;
	}

	return true;
}

static qbool Calc_G_time_10_path_apply(gedict_t* m, gedict_t* m_P, float P_time, int m_D) {
	qbool no_change = true;
	int i = 0;

	if (!m || m == world || !m_P || m_P == world) {
		return no_change;
	}

	for (i = 0; i < NUMBER_ZONES; ++i) {
		no_change &= ZoneMinSightFromTimeCalc(m, m_P, i, m_D);
	}

	return no_change;
}

static void Calc_G_time_11_apply(gedict_t* m, gedict_t* next_marker) {
	int i = 0;
	int found = -1;

	min_traveltime = min_traveltime + 1.25;
	runaway_score = runaway_score * 0.125;

	for (i = 0; i < NUMBER_PATHS; ++i) {
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

	if (found < 0)
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

static void Calc_G_time_11(void) {
	gedict_t* m_zone;
	gedict_t* m;

	for (m = first_marker; m && m != world; m = m->fb.marker_link) {
		for (m_zone = zone_stack_head; m_zone && m_zone != world; m_zone = m_zone->fb.zone_stack_next) {
			from_marker = m;
			m_zone->fb.zone_marker();
			if (middle_marker != dropper && middle_marker != m) {
				gedict_t* runaway_dest = middle_marker;
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
							Calc_G_time_11_apply(m, next_marker);
						}
					}
				} while (next_marker != runaway_dest);
			}

		}
	}

	m_zone = zone_stack_head;
	if (path_normal) {
		path_normal = false;
	}
	else  {
		path_normal = true;
		Calc_G_time_12();
	}
}

static void Calc_G_time_12(void) {
	gedict_t* m;
	gedict_t* runaway_dest;

	for (m = first_marker; m && m != world; m = m->fb.marker_link) {
		vec3_t m_pos;

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
						traceline(m_pos[0], m_pos[1], m_pos[2], next_marker->s.v.absmin[0] + next_marker->s.v.view_ofs[0], next_marker->s.v.absmin[1] + next_marker->s.v.view_ofs[1], next_marker->s.v.absmin[2] + next_marker->s.v.view_ofs[2] + 32, true, world);
						if (g_globalvars.trace_fraction != 1) {
							from_marker = m;
							next_marker->fb.sub_arrival_time();
							if (traveltime >= min_traveltime) {
								if (strneq(next_marker->s.v.classname, "trigger_teleport")) {
									Calc_G_time_11_apply(m, next_marker);
								}
							}
						}
					} while (next_marker != runaway_dest);
				}
			}
		}
	}
}

// Repeatedly calls func(x, y) until func returns true for every path in the system
static void PathCalculation(fb_path_calc_func_t func) {
	int i = 0, j = 0;
	qbool no_change = false;

	while (!no_change) {
		gedict_t* m;

		no_change = true;
		for (m = first_marker; m && m != world; m = m->fb.marker_link) {
			for (i = 0; i < NUMBER_PATHS; ++i) {
				no_change &= func(m, m->fb.paths[i].next_marker, m->fb.paths[i].time, m->fb.paths[i].flags);
			}
		}
	}
}

void InitialiseMarkerRoutes(void) {
	gedict_t* m;

	// This calculates water columns (if marker is in water, can the player go straight up to get air?)
	self = dropper;
	for (m = first_marker; m && m != world; m = m->fb.marker_link) {
		vec3_t point, m_pos;
		int content;

		VectorAdd(m->s.v.absmin, m->s.v.view_ofs, m_pos);
		m->fb.touch_marker = m;

		VectorCopy(m_pos, point);
		point[2] += 4;
		content = trap_pointcontents(PASSVEC3(point));
		if (content >= CONTENT_LAVA && content <= CONTENT_WATER) {
			vec3_t testplace;

			CheckWaterColumn(m, m_pos, testplace);
			if (testplace[2] - m_pos[2] > 0) {
				setsize(m, m->s.v.mins[0], m->s.v.mins[1], m->s.v.mins[2], m->s.v.maxs[0] + testplace[0] - m_pos[0], m->s.v.maxs[1] + testplace[1] - m_pos[1], m->s.v.maxs[2] + testplace[2] - m_pos[2]);
			}
		}
	}

	// Calc_G_time_2();
	for (m = first_marker; m && m != world; m = m->fb.marker_link) {
		int i = 0;

		for (i = 0; i < NUMBER_PATHS; ++i) {
			gedict_t* m_P = m->fb.paths[i].next_marker;
			if (m_P && m_P->fb.fl_marker) {
				TravelTimeForPath(m, i);
			}
			else {
				m->fb.paths[i].next_marker = 0;
			}
		}

		for (i = 0; i < NUMBER_GOALS; ++i) {
			if (! m->fb.goals[i].next_marker) {
				m->fb.goals[i].time = 1000000;
				m->fb.goals[i].next_marker = dropper;
			}
		}

		for (i = 0; i < NUMBER_ZONES; ++i) {
			if (! m->fb.zones[i].marker) {
				m->fb.zones[i].time = m->fb.zones[i].reverse_time = m->fb.zones[i].from_time = 1000000;
				m->fb.zones[i].marker = m->fb.zones[i].reverse_marker = dropper;
			}

			m->fb.zones[i].sight_from_time = 1000000;
		}

		for (i = 0; i < NUMBER_SUBZONES; ++i) {
			if (m->fb.S_ != i) {
				m->fb.subzones[i].time = 1000000;
			}
		}
	}

	PathCalculation(IdentifyFastestSubzoneRoute);
	PathCalculation(IdentifyFastestGoalRoute);
	PathCalculation(Calc_G_time_5_path_apply);
	PathCalculation(Calc_G_time_6_path_apply);
	Calc_G_time_7();
	PathCalculation(Calc_G_time_8_path_apply);
	Calc_G_time_9();
	PathCalculation(Calc_G_time_10_path_apply);
	Calc_G_time_11();

	{
		int i = 0, j = 0;
		for (i = 0; i < NUMBER_MARKERS; ++i) {
			/*if (markers[i])*/ {
				// Debug zone times
				/*
				Com_Printf ("M %d %s = %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", markers[i]->fb.index,
					markers[i]->s.v.classname,
					markers[i]->fb.zones[0].time,
					markers[i]->fb.zones[1].time,
					markers[i]->fb.zones[2].time,
					markers[i]->fb.zones[3].time,
					markers[i]->fb.zones[4].time,
					markers[i]->fb.zones[5].time,
					markers[i]->fb.zones[6].time,
					markers[i]->fb.zones[7].time,
					markers[i]->fb.zones[8].time,
					markers[i]->fb.zones[9].time,
					markers[i]->fb.zones[10].time,
					markers[i]->fb.zones[11].time,
					markers[i]->fb.zones[12].time,
					markers[i]->fb.zones[13].time
				);*/
#
				/*
				Com_Printf ("M %d %s = %d %f / %d %f / %d %f / %d %f / %d %f / %d %f / %d %f / %d %f\n",
					markers[i]->fb.index,
					markers[i]->s.v.classname,
					(markers[i]->fb.paths[0].next_marker ? markers[i]->fb.paths[0].next_marker->fb.index : -1),
					markers[i]->fb.paths[0].time,
					(markers[i]->fb.paths[1].next_marker ? markers[i]->fb.paths[1].next_marker->fb.index : -1),
					markers[i]->fb.paths[1].time,
					(markers[i]->fb.paths[2].next_marker ? markers[i]->fb.paths[2].next_marker->fb.index : -1),
					markers[i]->fb.paths[2].time,
					(markers[i]->fb.paths[3].next_marker ? markers[i]->fb.paths[3].next_marker->fb.index : -1),
					markers[i]->fb.paths[3].time,
					(markers[i]->fb.paths[4].next_marker ? markers[i]->fb.paths[4].next_marker->fb.index : -1),
					markers[i]->fb.paths[4].time,
					(markers[i]->fb.paths[5].next_marker ? markers[i]->fb.paths[5].next_marker->fb.index : -1),
					markers[i]->fb.paths[5].time,
					(markers[i]->fb.paths[6].next_marker ? markers[i]->fb.paths[6].next_marker->fb.index : -1),
					markers[i]->fb.paths[6].time,
					(markers[i]->fb.paths[7].next_marker ? markers[i]->fb.paths[7].next_marker->fb.index : -1),
					markers[i]->fb.paths[7].time
				);*/
				/*
				Com_Printf ("M %d %s = %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
					markers[i]->fb.index,
					markers[i]->s.v.classname,
					markers[i]->fb.goals[0].time, markers[i]->fb.goals[1].time, markers[i]->fb.goals[2].time, markers[i]->fb.goals[3].time, markers[i]->fb.goals[4].time,
					markers[i]->fb.goals[5].time, markers[i]->fb.goals[6].time, markers[i]->fb.goals[7].time, markers[i]->fb.goals[8].time, markers[i]->fb.goals[9].time,
					markers[i]->fb.goals[10].time, markers[i]->fb.goals[11].time, markers[i]->fb.goals[12].time, markers[i]->fb.goals[13].time, markers[i]->fb.goals[14].time,
					markers[i]->fb.goals[15].time, markers[i]->fb.goals[16].time, markers[i]->fb.goals[17].time, markers[i]->fb.goals[18].time, markers[i]->fb.goals[19].time,
					markers[i]->fb.goals[20].time, markers[i]->fb.goals[21].time, markers[i]->fb.goals[22].time, markers[i]->fb.goals[23].time
				);*/
			}
		}
/*
		Com_Printf ("-- Goal paths --\n");
		for (i = 0; i < NUMBER_MARKERS; ++i) {
			if (markers[i]) {
				Com_Printf ("M %d %s = %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
					markers[i]->fb.index,
					markers[i]->s.v.classname,
					markers[i]->fb.goals[0].next_marker ? markers[i]->fb.goals[0].next_marker->fb.index : -1,
					markers[i]->fb.goals[1].next_marker ? markers[i]->fb.goals[1].next_marker->fb.index : -1,
					markers[i]->fb.goals[2].next_marker ? markers[i]->fb.goals[2].next_marker->fb.index : -1,
					markers[i]->fb.goals[3].next_marker ? markers[i]->fb.goals[3].next_marker->fb.index : -1,
					markers[i]->fb.goals[4].next_marker ? markers[i]->fb.goals[4].next_marker->fb.index : -1,
					markers[i]->fb.goals[5].next_marker ? markers[i]->fb.goals[5].next_marker->fb.index : -1,
					markers[i]->fb.goals[6].next_marker ? markers[i]->fb.goals[6].next_marker->fb.index : -1,
					markers[i]->fb.goals[7].next_marker ? markers[i]->fb.goals[7].next_marker->fb.index : -1,
					markers[i]->fb.goals[8].next_marker ? markers[i]->fb.goals[8].next_marker->fb.index : -1,
					markers[i]->fb.goals[9].next_marker ? markers[i]->fb.goals[9].next_marker->fb.index : -1,
					markers[i]->fb.goals[10].next_marker ? markers[i]->fb.goals[10].next_marker->fb.index : -1,
					markers[i]->fb.goals[11].next_marker ? markers[i]->fb.goals[11].next_marker->fb.index : -1,
					markers[i]->fb.goals[12].next_marker ? markers[i]->fb.goals[12].next_marker->fb.index : -1,
					markers[i]->fb.goals[13].next_marker ? markers[i]->fb.goals[13].next_marker->fb.index : -1,
					markers[i]->fb.goals[14].next_marker ? markers[i]->fb.goals[14].next_marker->fb.index : -1,
					markers[i]->fb.goals[15].next_marker ? markers[i]->fb.goals[15].next_marker->fb.index : -1,
					markers[i]->fb.goals[16].next_marker ? markers[i]->fb.goals[16].next_marker->fb.index : -1,
					markers[i]->fb.goals[17].next_marker ? markers[i]->fb.goals[17].next_marker->fb.index : -1,
					markers[i]->fb.goals[18].next_marker ? markers[i]->fb.goals[18].next_marker->fb.index : -1,
					markers[i]->fb.goals[19].next_marker ? markers[i]->fb.goals[19].next_marker->fb.index : -1,
					markers[i]->fb.goals[20].next_marker ? markers[i]->fb.goals[20].next_marker->fb.index : -1,
					markers[i]->fb.goals[21].next_marker ? markers[i]->fb.goals[21].next_marker->fb.index : -1,
					markers[i]->fb.goals[22].next_marker ? markers[i]->fb.goals[22].next_marker->fb.index : -1,
					markers[i]->fb.goals[23].next_marker ? markers[i]->fb.goals[23].next_marker->fb.index : -1
				);
			}
		}

		Com_Printf ("-- Zone paths --\n");
		for (i = 0; i < NUMBER_MARKERS; ++i) {
			if (markers[i]) {
				Com_Printf ("M %d %s = %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
					markers[i]->fb.index,
					markers[i]->s.v.classname,
					markers[i]->fb.zones[0].next ? markers[i]->fb.zones[0].next->fb.index : -1,
					markers[i]->fb.zones[1].next ? markers[i]->fb.zones[1].next->fb.index : -1,
					markers[i]->fb.zones[2].next ? markers[i]->fb.zones[2].next->fb.index : -1,
					markers[i]->fb.zones[3].next ? markers[i]->fb.zones[3].next->fb.index : -1,
					markers[i]->fb.zones[4].next ? markers[i]->fb.zones[4].next->fb.index : -1,
					markers[i]->fb.zones[5].next ? markers[i]->fb.zones[5].next->fb.index : -1,
					markers[i]->fb.zones[6].next ? markers[i]->fb.zones[6].next->fb.index : -1,
					markers[i]->fb.zones[7].next ? markers[i]->fb.zones[7].next->fb.index : -1,
					markers[i]->fb.zones[8].next ? markers[i]->fb.zones[8].next->fb.index : -1,
					markers[i]->fb.zones[9].next ? markers[i]->fb.zones[9].next->fb.index : -1,
					markers[i]->fb.zones[10].next ? markers[i]->fb.zones[10].next->fb.index : -1,
					markers[i]->fb.zones[11].next ? markers[i]->fb.zones[11].next->fb.index : -1,
					markers[i]->fb.zones[12].next ? markers[i]->fb.zones[12].next->fb.index : -1,
					markers[i]->fb.zones[13].next ? markers[i]->fb.zones[13].next->fb.index : -1,
					markers[i]->fb.zones[14].next ? markers[i]->fb.zones[14].next->fb.index : -1,
					markers[i]->fb.zones[15].next ? markers[i]->fb.zones[15].next->fb.index : -1,
					markers[i]->fb.zones[16].next ? markers[i]->fb.zones[16].next->fb.index : -1,
					markers[i]->fb.zones[17].next ? markers[i]->fb.zones[17].next->fb.index : -1,
					markers[i]->fb.zones[18].next ? markers[i]->fb.zones[18].next->fb.index : -1,
					markers[i]->fb.zones[19].next ? markers[i]->fb.zones[19].next->fb.index : -1,
					markers[i]->fb.zones[20].next ? markers[i]->fb.zones[20].next->fb.index : -1,
					markers[i]->fb.zones[21].next ? markers[i]->fb.zones[21].next->fb.index : -1,
					markers[i]->fb.zones[22].next ? markers[i]->fb.zones[22].next->fb.index : -1,
					markers[i]->fb.zones[23].next ? markers[i]->fb.zones[23].next->fb.index : -1
				);
			}
		}

		Com_Printf ("-- Zone times --\n");
		for (i = 0; i < NUMBER_MARKERS; ++i) {
			if (markers[i]) {
				Com_Printf ("M %d %s = %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
					markers[i]->fb.index,
					markers[i]->s.v.classname,
					markers[i]->fb.zones[0].marker ? markers[i]->fb.zones[0].time : -1,
					markers[i]->fb.zones[1].marker ? markers[i]->fb.zones[1].time : -1,
					markers[i]->fb.zones[2].marker ? markers[i]->fb.zones[2].time : -1,
					markers[i]->fb.zones[3].marker ? markers[i]->fb.zones[3].time : -1,
					markers[i]->fb.zones[4].marker ? markers[i]->fb.zones[4].time : -1,
					markers[i]->fb.zones[5].marker ? markers[i]->fb.zones[5].time : -1,
					markers[i]->fb.zones[6].marker ? markers[i]->fb.zones[6].time : -1,
					markers[i]->fb.zones[7].marker ? markers[i]->fb.zones[7].time : -1,
					markers[i]->fb.zones[8].marker ? markers[i]->fb.zones[8].time : -1,
					markers[i]->fb.zones[9].marker ? markers[i]->fb.zones[9].time : -1,
					markers[i]->fb.zones[10].marker ? markers[i]->fb.zones[10].time : -1,
					markers[i]->fb.zones[11].marker ? markers[i]->fb.zones[11].time : -1,
					markers[i]->fb.zones[12].marker ? markers[i]->fb.zones[12].time : -1,
					markers[i]->fb.zones[13].marker ? markers[i]->fb.zones[13].time : -1,
					markers[i]->fb.zones[14].marker ? markers[i]->fb.zones[14].time : -1,
					markers[i]->fb.zones[15].marker ? markers[i]->fb.zones[15].time : -1,
					markers[i]->fb.zones[16].marker ? markers[i]->fb.zones[16].time : -1,
					markers[i]->fb.zones[17].marker ? markers[i]->fb.zones[17].time : -1,
					markers[i]->fb.zones[18].marker ? markers[i]->fb.zones[18].time : -1,
					markers[i]->fb.zones[19].marker ? markers[i]->fb.zones[19].time : -1,
					markers[i]->fb.zones[20].marker ? markers[i]->fb.zones[20].time : -1,
					markers[i]->fb.zones[21].marker ? markers[i]->fb.zones[21].time : -1,
					markers[i]->fb.zones[22].marker ? markers[i]->fb.zones[22].time : -1,
					markers[i]->fb.zones[23].marker ? markers[i]->fb.zones[23].time : -1
				);
			}
		}*/
	}

	return;
}
