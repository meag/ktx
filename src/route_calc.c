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

		if (!m->fb.Z1_) {
			m->fb.Z1_time = m->fb.Z1_time_rev = m->fb.Z1_from_time = 1000000;
			m->fb.Z1_ = m->fb.Z1_rev = dropper;
		}
		if (!m->fb.Z2_) {
			m->fb.Z2_time = m->fb.Z2_time_rev = m->fb.Z2_from_time = 1000000;
			m->fb.Z2_ = m->fb.Z2_rev = dropper;
		}
		if (!m->fb.Z3_) {
			m->fb.Z3_time = m->fb.Z3_time_rev = m->fb.Z3_from_time = 1000000;
			m->fb.Z3_ = m->fb.Z3_rev = dropper;
		}
		if (!m->fb.Z4_) {
			m->fb.Z4_time = m->fb.Z4_time_rev = m->fb.Z4_from_time = 1000000;
			m->fb.Z4_ = m->fb.Z4_rev = dropper;
		}
		if (!m->fb.Z5_) {
			m->fb.Z5_time = m->fb.Z5_time_rev = m->fb.Z5_from_time = 1000000;
			m->fb.Z5_ = m->fb.Z5_rev = dropper;
		}
		if (!m->fb.Z6_) {
			m->fb.Z6_time = m->fb.Z6_time_rev = m->fb.Z6_from_time = 1000000;
			m->fb.Z6_ = m->fb.Z6_rev = dropper;
		}
		if (!m->fb.Z7_) {
			m->fb.Z7_time = m->fb.Z7_time_rev = m->fb.Z7_from_time = 1000000;
			m->fb.Z7_ = m->fb.Z7_rev = dropper;
		}
		if (!m->fb.Z8_) {
			m->fb.Z8_time = m->fb.Z8_time_rev = m->fb.Z8_from_time = 1000000;
			m->fb.Z8_ = m->fb.Z8_rev = dropper;
		}
		if (!m->fb.Z9_) {
			m->fb.Z9_time = m->fb.Z9_time_rev = m->fb.Z9_from_time = 1000000;
			m->fb.Z9_ = m->fb.Z9_rev = dropper;
		}
		if (!m->fb.Z10_) {
			m->fb.Z10_time = m->fb.Z10_time_rev = m->fb.Z10_from_time = 1000000;
			m->fb.Z10_ = m->fb.Z10_rev = dropper;
		}
		if (!m->fb.Z11_) {
			m->fb.Z11_time = m->fb.Z11_time_rev = m->fb.Z11_from_time = 1000000;
			m->fb.Z11_ = m->fb.Z11_rev = dropper;
		}
		if (!m->fb.Z12_) {
			m->fb.Z12_time = m->fb.Z12_time_rev = m->fb.Z12_from_time = 1000000;
			m->fb.Z12_ = m->fb.Z12_rev = dropper;
		}
		if (!m->fb.Z13_) {
			m->fb.Z13_time = m->fb.Z13_time_rev = m->fb.Z13_from_time = 1000000;
			m->fb.Z13_ = m->fb.Z13_rev = dropper;
		}
		if (!m->fb.Z14_) {
			m->fb.Z14_time = m->fb.Z14_time_rev = m->fb.Z14_from_time = 1000000;
			m->fb.Z14_ = m->fb.Z14_rev = dropper;
		}
		if (!m->fb.Z15_) {
			m->fb.Z15_time = m->fb.Z15_time_rev = m->fb.Z15_from_time = 1000000;
			m->fb.Z15_ = m->fb.Z15_rev = dropper;
		}
		if (!m->fb.Z16_) {
			m->fb.Z16_time = m->fb.Z16_time_rev = m->fb.Z16_from_time = 1000000;
			m->fb.Z16_ = m->fb.Z16_rev = dropper;
		}
		if (!m->fb.Z17_) {
			m->fb.Z17_time = m->fb.Z17_time_rev = m->fb.Z17_from_time = 1000000;
			m->fb.Z17_ = m->fb.Z17_rev = dropper;
		}
		if (!m->fb.Z18_) {
			m->fb.Z18_time = m->fb.Z18_time_rev = m->fb.Z18_from_time = 1000000;
			m->fb.Z18_ = m->fb.Z18_rev = dropper;
		}
		if (!m->fb.Z19_) {
			m->fb.Z19_time = m->fb.Z19_time_rev = m->fb.Z19_from_time = 1000000;
			m->fb.Z19_ = m->fb.Z19_rev = dropper;
		}
		if (!m->fb.Z20_) {
			m->fb.Z20_time = m->fb.Z20_time_rev = m->fb.Z20_from_time = 1000000;
			m->fb.Z20_ = m->fb.Z20_rev = dropper;
		}
		if (!m->fb.Z21_) {
			m->fb.Z21_time = m->fb.Z21_time_rev = m->fb.Z21_from_time = 1000000;
			m->fb.Z21_ = m->fb.Z21_rev = dropper;
		}
		if (!m->fb.Z22_) {
			m->fb.Z22_time = m->fb.Z22_time_rev = m->fb.Z22_from_time = 1000000;
			m->fb.Z22_ = m->fb.Z22_rev = dropper;
		}
		if (!m->fb.Z23_) {
			m->fb.Z23_time = m->fb.Z23_time_rev = m->fb.Z23_from_time = 1000000;
			m->fb.Z23_ = m->fb.Z23_rev = dropper;
		}
		if (!m->fb.Z24_) {
			m->fb.Z24_time = m->fb.Z24_time_rev = m->fb.Z24_from_time = 1000000;
			m->fb.Z24_ = m->fb.Z24_rev = dropper;
		}
		m->fb.Z1_sight_from_time_ = m->fb.Z2_sight_from_time_ = m->fb.Z3_sight_from_time_ = m->fb.Z4_sight_from_time_ = m->fb.Z5_sight_from_time_ = m->fb.Z6_sight_from_time_ = m->fb.Z7_sight_from_time_ = m->fb.Z8_sight_from_time_ = m->fb.Z9_sight_from_time_ = m->fb.Z10_sight_from_time_ = m->fb.Z11_sight_from_time_ = m->fb.Z12_sight_from_time_ = m->fb.Z13_sight_from_time_ = m->fb.Z14_sight_from_time_ = m->fb.Z15_sight_from_time_ = m->fb.Z16_sight_from_time_ = m->fb.Z17_sight_from_time_ = m->fb.Z18_sight_from_time_ = m->fb.Z19_sight_from_time_ = m->fb.Z20_sight_from_time_ = m->fb.Z21_sight_from_time_ = m->fb.Z22_sight_from_time_ = m->fb.Z23_sight_from_time_ = m->fb.Z24_sight_from_time_ = m->fb.Z1_higher_sight_from_time_ = m->fb.Z2_higher_sight_from_time_ = m->fb.Z3_higher_sight_from_time_ = m->fb.Z4_higher_sight_from_time_ = m->fb.Z5_higher_sight_from_time_ = m->fb.Z6_higher_sight_from_time_ = m->fb.Z7_higher_sight_from_time_ = m->fb.Z8_higher_sight_from_time_ = m->fb.Z9_higher_sight_from_time_ = m->fb.Z10_higher_sight_from_time_ = m->fb.Z11_higher_sight_from_time_ = m->fb.Z12_higher_sight_from_time_ = m->fb.Z13_higher_sight_from_time_ = m->fb.Z14_higher_sight_from_time_ = m->fb.Z15_higher_sight_from_time_ = m->fb.Z16_higher_sight_from_time_ = m->fb.Z17_higher_sight_from_time_ = m->fb.Z18_higher_sight_from_time_ = m->fb.Z19_higher_sight_from_time_ = m->fb.Z20_higher_sight_from_time_ = m->fb.Z21_higher_sight_from_time_ = m->fb.Z22_higher_sight_from_time_ = m->fb.Z23_higher_sight_from_time_ = m->fb.Z24_higher_sight_from_time_ = 1000000;
		m->fb.Z1_height_equal_time = 1000000;
		m->fb.Z8_height_equal_time = 1000000;
		if (m->fb.S_ != 1) {
			m->fb.S1_time = 1000000;
		}
		if (m->fb.S_ != 2) {
			m->fb.S2_time = 1000000;
		}
		if (m->fb.S_ != 3) {
			m->fb.S3_time = 1000000;
		}
		if (m->fb.S_ != 4) {
			m->fb.S4_time = 1000000;
		}
		if (m->fb.S_ != 5) {
			m->fb.S5_time = 1000000;
		}
		if (m->fb.S_ != 6) {
			m->fb.S6_time = 1000000;
		}
		if (m->fb.S_ != 7) {
			m->fb.S7_time = 1000000;
		}
		if (m->fb.S_ != 8) {
			m->fb.S8_time = 1000000;
		}
		if (m->fb.S_ != 9) {
			m->fb.S9_time = 1000000;
		}
		if (m->fb.S_ != 10) {
			m->fb.S10_time = 1000000;
		}
		if (m->fb.S_ != 11) {
			m->fb.S11_time = 1000000;
		}
		if (m->fb.S_ != 12) {
			m->fb.S12_time = 1000000;
		}
		if (m->fb.S_ != 13) {
			m->fb.S13_time = 1000000;
		}
		if (m->fb.S_ != 14) {
			m->fb.S14_time = 1000000;
		}
		if (m->fb.S_ != 15) {
			m->fb.S15_time = 1000000;
		}
		if (m->fb.S_ != 16) {
			m->fb.S16_time = 1000000;
		}
		if (m->fb.S_ != 17) {
			m->fb.S17_time = 1000000;
		}
		if (m->fb.S_ != 18) {
			m->fb.S18_time = 1000000;
		}
		if (m->fb.S_ != 19) {
			m->fb.S19_time = 1000000;
		}
		if (m->fb.S_ != 20) {
			m->fb.S20_time = 1000000;
		}
		if (m->fb.S_ != 21) {
			m->fb.S21_time = 1000000;
		}
		if (m->fb.S_ != 22) {
			m->fb.S22_time = 1000000;
		}
		if (m->fb.S_ != 23) {
			m->fb.S23_time = 1000000;
		}
		if (m->fb.S_ != 24) {
			m->fb.S24_time = 1000000;
		}
		if (m->fb.S_ != 25) {
			m->fb.S25_time = 1000000;
		}
		if (m->fb.S_ != 26) {
			m->fb.S26_time = 1000000;
		}
		if (m->fb.S_ != 27) {
			m->fb.S27_time = 1000000;
		}
		if (m->fb.S_ != 28) {
			m->fb.S28_time = 1000000;
		}
		if (m->fb.S_ != 29) {
			m->fb.S29_time = 1000000;
		}
		if (m->fb.S_ != 30) {
			m->fb.S30_time = 1000000;
		}
		if (m->fb.S_ != 31) {
			m->fb.S31_time = 1000000;
		}
		if (m->fb.S_ != 32) {
			m->fb.S32_time = 1000000;
		}
	}

	Calc_G_time_3_path();
	//m = first_marker;
	//dropper->s.v.use = (func_t) Calc_G_time_3_path;
	//dropper->s.v.think = (func_t) Calc_G_time_path;
	//set_load();
	return;
}

#define SUBZONE_TIME_ADJUST(m, x) \
	if (m->fb.S ## x ## _time > (P_time + m_P->fb.S ## x ## _time)) { \
		no_change = (qbool) false; \
		m->fb.S ## x ## _time = P_time + m_P->fb.S ## x ## _time; \
		m->fb.S ## x ## _next = m_P; \
	}

typedef qbool (*fb_path_calc_func_t)(gedict_t* m, gedict_t* m_P, float P_time);

qbool Calc_G_time_3_path_apply(gedict_t* m, gedict_t* m_P /* next */, float P_time /* traveltime */) {
	qbool no_change = (qbool) true;

	if (!m || m == world)
		return no_change;
	if (!m_P || m_P == world)
		return no_change;

	if (m->fb.Z_ == m_P->fb.Z_) {
		SUBZONE_TIME_ADJUST(m, 1);
		SUBZONE_TIME_ADJUST(m, 2);
		SUBZONE_TIME_ADJUST(m, 3);
		SUBZONE_TIME_ADJUST(m, 4);
		SUBZONE_TIME_ADJUST(m, 5);
		SUBZONE_TIME_ADJUST(m, 6);
		SUBZONE_TIME_ADJUST(m, 7);
		SUBZONE_TIME_ADJUST(m, 8);
		SUBZONE_TIME_ADJUST(m, 9);
		SUBZONE_TIME_ADJUST(m, 10);
		SUBZONE_TIME_ADJUST(m, 11);
		SUBZONE_TIME_ADJUST(m, 12);
		SUBZONE_TIME_ADJUST(m, 13);
		SUBZONE_TIME_ADJUST(m, 14);
		SUBZONE_TIME_ADJUST(m, 15);
		SUBZONE_TIME_ADJUST(m, 16);
		SUBZONE_TIME_ADJUST(m, 17);
		SUBZONE_TIME_ADJUST(m, 18);
		SUBZONE_TIME_ADJUST(m, 19);
		SUBZONE_TIME_ADJUST(m, 20);
		SUBZONE_TIME_ADJUST(m, 21);
		SUBZONE_TIME_ADJUST(m, 22);
		SUBZONE_TIME_ADJUST(m, 23);
		SUBZONE_TIME_ADJUST(m, 24);
		SUBZONE_TIME_ADJUST(m, 25);
		SUBZONE_TIME_ADJUST(m, 26);
		SUBZONE_TIME_ADJUST(m, 27);
		SUBZONE_TIME_ADJUST(m, 28);
		SUBZONE_TIME_ADJUST(m, 29);
		SUBZONE_TIME_ADJUST(m, 30);
		SUBZONE_TIME_ADJUST(m, 31);
		SUBZONE_TIME_ADJUST(m, 32);
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

#define ZONE_TIME_ADJUST(m, m_P, x) \
	if (m->fb.Z ## x ## _time > (P_time + m_P->fb.Z ## x ## _time)) { \
		no_change = (qbool) false; \
		m->fb.Z ## x ## _ = m_P->fb.Z ## x ## _; \
		m->fb.Z ## x ## _time = P_time + m_P->fb.Z ## x ## _time; \
		m->fb.Z ## x ## _next = m_P; \
		{ \
			if (m->fb.Z_ == m_P->fb.Z_) { \
				m->fb.Z ## x ## _next_zone = m_P->fb.Z ## x ## _next_zone; \
			} \
			else  { \
				m->fb.Z ## x ## _next_zone = m_P; \
			} \
		} \
		m->fb.zoneTask[( x - 1 )] |= m_D; \
	}

qbool Calc_G_time_5_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = (qbool) true;

	if (!m || m == world)
		return no_change;
	if (!m_P || m_P == world)
		return no_change;

	ZONE_TIME_ADJUST(m, m_P, 1);
	ZONE_TIME_ADJUST(m, m_P, 2);
	ZONE_TIME_ADJUST(m, m_P, 3);
	ZONE_TIME_ADJUST(m, m_P, 4);
	ZONE_TIME_ADJUST(m, m_P, 5);
	ZONE_TIME_ADJUST(m, m_P, 6);
	ZONE_TIME_ADJUST(m, m_P, 7);
	ZONE_TIME_ADJUST(m, m_P, 8);
	ZONE_TIME_ADJUST(m, m_P, 9);
	ZONE_TIME_ADJUST(m, m_P, 10);
	ZONE_TIME_ADJUST(m, m_P, 11);
	ZONE_TIME_ADJUST(m, m_P, 12);
	ZONE_TIME_ADJUST(m, m_P, 13);
	ZONE_TIME_ADJUST(m, m_P, 14);
	ZONE_TIME_ADJUST(m, m_P, 15);
	ZONE_TIME_ADJUST(m, m_P, 16);
	ZONE_TIME_ADJUST(m, m_P, 17);
	ZONE_TIME_ADJUST(m, m_P, 18);
	ZONE_TIME_ADJUST(m, m_P, 19);
	ZONE_TIME_ADJUST(m, m_P, 20);
	ZONE_TIME_ADJUST(m, m_P, 21);
	ZONE_TIME_ADJUST(m, m_P, 22);
	ZONE_TIME_ADJUST(m, m_P, 23);
	ZONE_TIME_ADJUST(m, m_P, 24);

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

qbool Calc_G_time_6_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = (qbool) true;

	if (!m || m == world)
		return no_change;
	if (!m_P || m_P == world)
		return no_change;

	ZONE_FROM_TIME_ADJUST(m, m_P, 1);
	ZONE_FROM_TIME_ADJUST(m, m_P, 2);
	ZONE_FROM_TIME_ADJUST(m, m_P, 3);
	ZONE_FROM_TIME_ADJUST(m, m_P, 4);
	ZONE_FROM_TIME_ADJUST(m, m_P, 5);
	ZONE_FROM_TIME_ADJUST(m, m_P, 6);
	ZONE_FROM_TIME_ADJUST(m, m_P, 7);
	ZONE_FROM_TIME_ADJUST(m, m_P, 8);
	ZONE_FROM_TIME_ADJUST(m, m_P, 9);
	ZONE_FROM_TIME_ADJUST(m, m_P, 10);
	ZONE_FROM_TIME_ADJUST(m, m_P, 11);
	ZONE_FROM_TIME_ADJUST(m, m_P, 12);
	ZONE_FROM_TIME_ADJUST(m, m_P, 13);
	ZONE_FROM_TIME_ADJUST(m, m_P, 14);
	ZONE_FROM_TIME_ADJUST(m, m_P, 15);
	ZONE_FROM_TIME_ADJUST(m, m_P, 16);
	ZONE_FROM_TIME_ADJUST(m, m_P, 17);
	ZONE_FROM_TIME_ADJUST(m, m_P, 18);
	ZONE_FROM_TIME_ADJUST(m, m_P, 19);
	ZONE_FROM_TIME_ADJUST(m, m_P, 20);
	ZONE_FROM_TIME_ADJUST(m, m_P, 21);
	ZONE_FROM_TIME_ADJUST(m, m_P, 22);
	ZONE_FROM_TIME_ADJUST(m, m_P, 23);
	ZONE_FROM_TIME_ADJUST(m, m_P, 24);

	return no_change;
}

void Calc_G_time_6_path() {
	PathCalculation(Calc_G_time_6_path_apply);
	Com_Printf("Time[6] complete\n");

	Calc_G_time_7();
}

int CheckReversible() {
	m->fb.next_zone_marker();
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
				m->fb.paths[i].flags |= CheckReversible();
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

qbool Calc_G_time_8_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = (qbool) true;

	if (!m || m == world)
		return no_change;
	if (!m_P || m_P == world)
		return no_change;

	if (m_D & REVERSIBLE) {
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 1)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 2)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 3)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 4)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 5)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 6)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 7)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 8)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 9)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 10)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 11)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 12)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 13)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 14)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 15)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 16)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 17)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 18)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 19)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 20)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 21)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 22)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 23)
		ZONE_REVERSE_TIME_ADJUST(m, m_P, 24)
	}

	return (qbool) no_change;
}

void Calc_G_time_8_path() {
	PathCalculation(Calc_G_time_8_path_apply);
	Com_Printf("Time[8] complete\n");

	Calc_G_time_9();
}

#define ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, x) \
	if (m->fb.Z ## x ## _sight_from_time_ > m_2->fb.Z ## x ## _from_time) { \
		m->fb.Z ## x ## _sight_from_time_ = m_2->fb.Z ## x ## _from_time; \
		m->fb.Z ## x ## _sight_from = m_2; \
	} \
	else if (!m_2->fb.Z ## x ## _from_time) { \
		m->fb.Z ## x ## _sight_from = world; \
	}

#define ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, x) \
	if (m->fb.Z ## x ## _higher_sight_from_time_ > m_2->fb.Z ## x ## _from_time) { \
		m->fb.Z ## x ## _higher_sight_from_time_ = m_2->fb.Z ## x ## _from_time; \
		m->fb.Z ## x ## _higher_sight_from = m_2; \
	} \
	else if (!m_2->fb.Z ## x ## _from_time) { \
		m->fb.Z ## x ## _higher_sight_from = world; \
	}

void Calc_G_time_9_apply(gedict_t* m, gedict_t* m_2, vec3_t m_pos, vec3_t m2_pos) {
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 1)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 2)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 3)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 4)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 5)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 6)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 7)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 8)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 9)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 10)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 11)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 12)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 13)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 14)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 15)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 16)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 17)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 18)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 19)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 20)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 21)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 22)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 23)
	ZONE_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 24)

	if (m2_pos[2] - m_pos[2] >= 40) {
		if (VectorDistance(m2_pos, m_pos) <= 1000) {
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 1)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 2)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 3)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 4)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 5)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 6)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 7)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 8)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 9)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 10)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 11)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 12)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 13)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 14)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 15)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 16)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 17)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 18)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 19)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 20)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 21)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 22)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 23)
			ZONE_HIGHER_SIGHT_FROM_MARKER_CALCULATE(m, m_2, 24)
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

#define ZONE_MIN_SIGHT_FROM_TIME_CALC(x) \
	if (m->fb.Z ## x ## _sight_from_time_ < (m_P->fb.Z ## x ## _sight_from_time_ - P_time)) { \
		no_change = (qbool) false; \
		m->fb.Z ## x ## _sight_from_time_ = m_P->fb.Z ## x ## _sight_from_time_ - P_time; \
	} 

qbool Calc_G_time_10_path_apply(gedict_t* m, gedict_t* m_P, float P_time) {
	qbool no_change = (qbool) true;

	if (!m || m == world)
		return no_change;
	if (!m_P || m_P == world)
		return no_change;

	ZONE_MIN_SIGHT_FROM_TIME_CALC(1)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(2)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(3)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(4)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(5)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(6)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(7)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(8)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(9)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(10)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(11)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(12)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(13)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(14)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(15)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(16)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(17)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(18)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(19)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(20)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(21)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(22)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(23)
	ZONE_MIN_SIGHT_FROM_TIME_CALC(24)

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
						if (next_marker->fb.Z1_higher_sight_from_time_ <= traveltime) {
							Z1_path_success = FALSE;
						}
						if (next_marker->fb.Z8_higher_sight_from_time_ <= traveltime) {
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

