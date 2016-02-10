// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void SetGoal(int goal, gedict_t* marker)
{
	marker->fb.goals[goal - 1].next_marker = marker;
	marker->fb.G_ = goal - 1;
}

void Set_sub_arrival_time(gedict_t* marker) {
	marker->fb.S_ = S_index;
	if (S_index <= 16) {
		if (S_index <= 8) {
			if (S_index <= 4) {
				if (S_index <= 2) {
					if (S_index == 1) {
						marker->fb.sub_arrival_time = S1_time_;
						marker->fb.sub_path_marker = S1_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S2_time_;
						marker->fb.sub_path_marker = S2_path_marker;
					}
				}
				else  {
					if (S_index == 3) {
						marker->fb.sub_arrival_time = S3_time_;
						marker->fb.sub_path_marker = S3_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S4_time_;
						marker->fb.sub_path_marker = S4_path_marker;
					}
				}
			}
			else  {
				if (S_index <= 6) {
					if (S_index == 5) {
						marker->fb.sub_arrival_time = S5_time_;
						marker->fb.sub_path_marker = S5_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S6_time_;
						marker->fb.sub_path_marker = S6_path_marker;
					}
				}
				else  {
					if (S_index == 7) {
						marker->fb.sub_arrival_time = S7_time_;
						marker->fb.sub_path_marker = S7_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S8_time_;
						marker->fb.sub_path_marker = S8_path_marker;
					}
				}
			}
		}
		else  {
			if (S_index <= 12) {
				if (S_index <= 10) {
					if (S_index == 9) {
						marker->fb.sub_arrival_time = S9_time_;
						marker->fb.sub_path_marker = S9_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S10_time_;
						marker->fb.sub_path_marker = S10_path_marker;
					}
				}
				else  {
					if (S_index == 11) {
						marker->fb.sub_arrival_time = S11_time_;
						marker->fb.sub_path_marker = S11_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S12_time_;
						marker->fb.sub_path_marker = S12_path_marker;
					}
				}
			}
			else  {
				if (S_index <= 14) {
					if (S_index == 13) {
						marker->fb.sub_arrival_time = S13_time_;
						marker->fb.sub_path_marker = S13_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S14_time_;
						marker->fb.sub_path_marker = S14_path_marker;
					}
				}
				else  {
					if (S_index == 15) {
						marker->fb.sub_arrival_time = S15_time_;
						marker->fb.sub_path_marker = S15_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S16_time_;
						marker->fb.sub_path_marker = S16_path_marker;
					}
				}
			}
		}
	}
	else if (S_index <= 32) {
		if (S_index <= 24) {
			if (S_index <= 20) {
				if (S_index <= 18) {
					if (S_index == 17) {
						marker->fb.sub_arrival_time = S17_time_;
						marker->fb.sub_path_marker = S17_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S18_time_;
						marker->fb.sub_path_marker = S18_path_marker;
					}
				}
				else  {
					if (S_index == 19) {
						marker->fb.sub_arrival_time = S19_time_;
						marker->fb.sub_path_marker = S19_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S20_time_;
						marker->fb.sub_path_marker = S20_path_marker;
					}
				}
			}
			else  {
				if (S_index <= 22) {
					if (S_index == 21) {
						marker->fb.sub_arrival_time = S21_time_;
						marker->fb.sub_path_marker = S21_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S22_time_;
						marker->fb.sub_path_marker = S22_path_marker;
					}
				}
				else  {
					if (S_index == 23) {
						marker->fb.sub_arrival_time = S23_time_;
						marker->fb.sub_path_marker = S23_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S24_time_;
						marker->fb.sub_path_marker = S24_path_marker;
					}
				}
			}
		}
		else  {
			if (S_index <= 28) {
				if (S_index <= 26) {
					if (S_index == 25) {
						marker->fb.sub_arrival_time = S25_time_;
						marker->fb.sub_path_marker = S25_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S26_time_;
						marker->fb.sub_path_marker = S26_path_marker;
					}
				}
				else  {
					if (S_index == 27) {
						marker->fb.sub_arrival_time = S27_time_;
						marker->fb.sub_path_marker = S27_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S28_time_;
						marker->fb.sub_path_marker = S28_path_marker;
					}
				}
			}
			else  {
				if (S_index <= 30) {
					if (S_index == 29) {
						marker->fb.sub_arrival_time = S29_time_;
						marker->fb.sub_path_marker = S29_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S30_time_;
						marker->fb.sub_path_marker = S30_path_marker;
					}
				}
				else  {
					if (S_index == 31) {
						marker->fb.sub_arrival_time = S31_time_;
						marker->fb.sub_path_marker = S31_path_marker;
					}
					else  {
						marker->fb.sub_arrival_time = S32_time_;
						marker->fb.sub_path_marker = S32_path_marker;
					}
				}
			}
		}
	}
}

void Z1(gedict_t* marker) {
	Z1_S_index = Z1_S_index + 1;
	S_index = Z1_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z1_ = marker->fb.Z1_rev = marker->fb.Z1_next_zone = marker->fb.Z1_next = marker;
	marker->fb.Z_ = 1;
	marker->fb.zone_marker = Z1_marker;
	marker->fb.next_zone_marker = Z1_next_zone_marker;
	marker->fb.path_marker = Z1_path_marker;
	marker->fb.sight_from_marker = Z1_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z1_higher_sight_from_marker;
	marker->fb.sight_from_time = Z1_sight_from_time;
	if (Z1_tail) {
		Z1_tail->fb.Z_next = marker;
		Z1_tail = marker;
	}
	else  {
		Z1_head = Z1_tail = marker;
		Z1_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z1_head;
	}
	marker->fb.Z_head = Z1_head;
}

void Z2(gedict_t* marker) {
	Z2_S_index = Z2_S_index + 1;
	S_index = Z2_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z2_ = marker->fb.Z2_rev = marker->fb.Z2_next_zone = marker->fb.Z2_next = marker;
	marker->fb.Z_ = 2;
	marker->fb.zone_marker = Z2_marker;
	marker->fb.next_zone_marker = Z2_next_zone_marker;
	marker->fb.path_marker = Z2_path_marker;
	marker->fb.sight_from_marker = Z2_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z2_higher_sight_from_marker;
	marker->fb.sight_from_time = Z2_sight_from_time;
	if (Z2_tail) {
		Z2_tail->fb.Z_next = marker;
		Z2_tail = marker;
	}
	else  {
		Z2_head = Z2_tail = marker;
		Z2_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z2_head;
	}
	marker->fb.Z_head = Z2_head;
}

void Z3(gedict_t* marker) {
	Z3_S_index = Z3_S_index + 1;
	S_index = Z3_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z3_ = marker->fb.Z3_rev = marker->fb.Z3_next_zone = marker->fb.Z3_next = marker;
	marker->fb.Z_ = 3;
	marker->fb.zone_marker = Z3_marker;
	marker->fb.next_zone_marker = Z3_next_zone_marker;
	marker->fb.path_marker = Z3_path_marker;
	marker->fb.sight_from_marker = Z3_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z3_higher_sight_from_marker;
	marker->fb.sight_from_time = Z3_sight_from_time;
	if (Z3_tail) {
		Z3_tail->fb.Z_next = marker;
		Z3_tail = marker;
	}
	else  {
		Z3_head = Z3_tail = marker;
		Z3_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z3_head;
	}
	marker->fb.Z_head = Z3_head;
}

void Z4(gedict_t* marker) {
	Z4_S_index = Z4_S_index + 1;
	S_index = Z4_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z4_ = marker->fb.Z4_rev = marker->fb.Z4_next_zone = marker->fb.Z4_next = marker;
	marker->fb.Z_ = 4;
	marker->fb.zone_marker = Z4_marker;
	marker->fb.next_zone_marker = Z4_next_zone_marker;
	marker->fb.path_marker = Z4_path_marker;
	marker->fb.sight_from_marker = Z4_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z4_higher_sight_from_marker;
	marker->fb.sight_from_time = Z4_sight_from_time;
	if (Z4_tail) {
		Z4_tail->fb.Z_next = marker;
		Z4_tail = marker;
	}
	else  {
		Z4_head = Z4_tail = marker;
		Z4_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z4_head;
	}
	marker->fb.Z_head = Z4_head;
}

void Z5(gedict_t* marker) {
	Z5_S_index = Z5_S_index + 1;
	S_index = Z5_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z5_ = marker->fb.Z5_rev = marker->fb.Z5_next_zone = marker->fb.Z5_next = marker;
	marker->fb.Z_ = 5;
	marker->fb.zone_marker = Z5_marker;
	marker->fb.next_zone_marker = Z5_next_zone_marker;
	marker->fb.path_marker = Z5_path_marker;
	marker->fb.sight_from_marker = Z5_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z5_higher_sight_from_marker;
	marker->fb.sight_from_time = Z5_sight_from_time;
	if (Z5_tail) {
		Z5_tail->fb.Z_next = marker;
		Z5_tail = marker;
	}
	else  {
		Z5_head = Z5_tail = marker;
		Z5_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z5_head;
	}
	marker->fb.Z_head = Z5_head;
}

void Z6(gedict_t* marker) {
	Z6_S_index = Z6_S_index + 1;
	S_index = Z6_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z6_ = marker->fb.Z6_rev = marker->fb.Z6_next_zone = marker->fb.Z6_next = marker;
	marker->fb.Z_ = 6;
	marker->fb.zone_marker = Z6_marker;
	marker->fb.next_zone_marker = Z6_next_zone_marker;
	marker->fb.path_marker = Z6_path_marker;
	marker->fb.sight_from_marker = Z6_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z6_higher_sight_from_marker;
	marker->fb.sight_from_time = Z6_sight_from_time;
	if (Z6_tail) {
		Z6_tail->fb.Z_next = marker;
		Z6_tail = marker;
	}
	else  {
		Z6_head = Z6_tail = marker;
		Z6_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z6_head;
	}
	marker->fb.Z_head = Z6_head;
}

void Z7(gedict_t* marker) {
	Z7_S_index = Z7_S_index + 1;
	S_index = Z7_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z7_ = marker->fb.Z7_rev = marker->fb.Z7_next_zone = marker->fb.Z7_next = marker;
	marker->fb.Z_ = 7;
	marker->fb.zone_marker = Z7_marker;
	marker->fb.next_zone_marker = Z7_next_zone_marker;
	marker->fb.path_marker = Z7_path_marker;
	marker->fb.sight_from_marker = Z7_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z7_higher_sight_from_marker;
	marker->fb.sight_from_time = Z7_sight_from_time;
	if (Z7_tail) {
		Z7_tail->fb.Z_next = marker;
		Z7_tail = marker;
	}
	else  {
		Z7_head = Z7_tail = marker;
		Z7_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z7_head;
	}
	marker->fb.Z_head = Z7_head;
}

void Z8(gedict_t* marker) {
	Z8_S_index = Z8_S_index + 1;
	S_index = Z8_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z8_ = marker->fb.Z8_rev = marker->fb.Z8_next_zone = marker->fb.Z8_next = marker;
	marker->fb.Z_ = 8;
	marker->fb.zone_marker = Z8_marker;
	marker->fb.next_zone_marker = Z8_next_zone_marker;
	marker->fb.path_marker = Z8_path_marker;
	marker->fb.sight_from_marker = Z8_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z8_higher_sight_from_marker;
	marker->fb.sight_from_time = Z8_sight_from_time;
	if (Z8_tail) {
		Z8_tail->fb.Z_next = marker;
		Z8_tail = marker;
	}
	else  {
		Z8_head = Z8_tail = marker;
		Z8_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z8_head;
	}
	marker->fb.Z_head = Z8_head;
}

void Z9(gedict_t* marker) {
	Z9_S_index = Z9_S_index + 1;
	S_index = Z9_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z9_ = marker->fb.Z9_rev = marker->fb.Z9_next_zone = marker->fb.Z9_next = marker;
	marker->fb.Z_ = 9;
	marker->fb.zone_marker = Z9_marker;
	marker->fb.next_zone_marker = Z9_next_zone_marker;
	marker->fb.path_marker = Z9_path_marker;
	marker->fb.sight_from_marker = Z9_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z9_higher_sight_from_marker;
	marker->fb.sight_from_time = Z9_sight_from_time;
	if (Z9_tail) {
		Z9_tail->fb.Z_next = marker;
		Z9_tail = marker;
	}
	else  {
		Z9_head = Z9_tail = marker;
		Z9_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z9_head;
	}
	marker->fb.Z_head = Z9_head;
}

void Z10(gedict_t* marker) {
	Z10_S_index = Z10_S_index + 1;
	S_index = Z10_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z10_ = marker->fb.Z10_rev = marker->fb.Z10_next_zone = marker->fb.Z10_next = marker;
	marker->fb.Z_ = 10;
	marker->fb.zone_marker = Z10_marker;
	marker->fb.next_zone_marker = Z10_next_zone_marker;
	marker->fb.path_marker = Z10_path_marker;
	marker->fb.sight_from_marker = Z10_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z10_higher_sight_from_marker;
	marker->fb.sight_from_time = Z10_sight_from_time;
	if (Z10_tail) {
		Z10_tail->fb.Z_next = marker;
		Z10_tail = marker;
	}
	else  {
		Z10_head = Z10_tail = marker;
		Z10_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z10_head;
	}
	marker->fb.Z_head = Z10_head;
}

void Z11(gedict_t* marker) {
	Z11_S_index = Z11_S_index + 1;
	S_index = Z11_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z11_ = marker->fb.Z11_rev = marker->fb.Z11_next_zone = marker->fb.Z11_next = marker;
	marker->fb.Z_ = 11;
	marker->fb.zone_marker = Z11_marker;
	marker->fb.next_zone_marker = Z11_next_zone_marker;
	marker->fb.path_marker = Z11_path_marker;
	marker->fb.sight_from_marker = Z11_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z11_higher_sight_from_marker;
	marker->fb.sight_from_time = Z11_sight_from_time;
	if (Z11_tail) {
		Z11_tail->fb.Z_next = marker;
		Z11_tail = marker;
	}
	else  {
		Z11_head = Z11_tail = marker;
		Z11_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z11_head;
	}
	marker->fb.Z_head = Z11_head;
}

void Z12(gedict_t* marker) {
	Z12_S_index = Z12_S_index + 1;
	S_index = Z12_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z12_ = marker->fb.Z12_rev = marker->fb.Z12_next_zone = marker->fb.Z12_next = marker;
	marker->fb.Z_ = 12;
	marker->fb.zone_marker = Z12_marker;
	marker->fb.next_zone_marker = Z12_next_zone_marker;
	marker->fb.path_marker = Z12_path_marker;
	marker->fb.sight_from_marker = Z12_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z12_higher_sight_from_marker;
	marker->fb.sight_from_time = Z12_sight_from_time;
	if (Z12_tail) {
		Z12_tail->fb.Z_next = marker;
		Z12_tail = marker;
	}
	else  {
		Z12_head = Z12_tail = marker;
		Z12_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z12_head;
	}
	marker->fb.Z_head = Z12_head;
}

void Z13(gedict_t* marker) {
	Z13_S_index = Z13_S_index + 1;
	S_index = Z13_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z13_ = marker->fb.Z13_rev = marker->fb.Z13_next_zone = marker->fb.Z13_next = marker;
	marker->fb.Z_ = 13;
	marker->fb.zone_marker = Z13_marker;
	marker->fb.next_zone_marker = Z13_next_zone_marker;
	marker->fb.path_marker = Z13_path_marker;
	marker->fb.sight_from_marker = Z13_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z13_higher_sight_from_marker;
	marker->fb.sight_from_time = Z13_sight_from_time;
	if (Z13_tail) {
		Z13_tail->fb.Z_next = marker;
		Z13_tail = marker;
	}
	else  {
		Z13_head = Z13_tail = marker;
		Z13_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z13_head;
	}
	marker->fb.Z_head = Z13_head;
}

void Z14(gedict_t* marker) {
	Z14_S_index = Z14_S_index + 1;
	S_index = Z14_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z14_ = marker->fb.Z14_rev = marker->fb.Z14_next_zone = marker->fb.Z14_next = marker;
	marker->fb.Z_ = 14;
	marker->fb.zone_marker = Z14_marker;
	marker->fb.next_zone_marker = Z14_next_zone_marker;
	marker->fb.path_marker = Z14_path_marker;
	marker->fb.sight_from_marker = Z14_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z14_higher_sight_from_marker;
	marker->fb.sight_from_time = Z14_sight_from_time;
	if (Z14_tail) {
		Z14_tail->fb.Z_next = marker;
		Z14_tail = marker;
	}
	else  {
		Z14_head = Z14_tail = marker;
		Z14_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z14_head;
	}
	marker->fb.Z_head = Z14_head;
}

void Z15(gedict_t* marker) {
	Z15_S_index = Z15_S_index + 1;
	S_index = Z15_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z15_ = marker->fb.Z15_rev = marker->fb.Z15_next_zone = marker->fb.Z15_next = marker;
	marker->fb.Z_ = 15;
	marker->fb.zone_marker = Z15_marker;
	marker->fb.next_zone_marker = Z15_next_zone_marker;
	marker->fb.path_marker = Z15_path_marker;
	marker->fb.sight_from_marker = Z15_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z15_higher_sight_from_marker;
	marker->fb.sight_from_time = Z15_sight_from_time;
	if (Z15_tail) {
		Z15_tail->fb.Z_next = marker;
		Z15_tail = marker;
	}
	else  {
		Z15_head = Z15_tail = marker;
		Z15_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z15_head;
	}
	marker->fb.Z_head = Z15_head;
}

void Z16(gedict_t* marker) {
	Z16_S_index = Z16_S_index + 1;
	S_index = Z16_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z16_ = marker->fb.Z16_rev = marker->fb.Z16_next_zone = marker->fb.Z16_next = marker;
	marker->fb.Z_ = 16;
	marker->fb.zone_marker = Z16_marker;
	marker->fb.next_zone_marker = Z16_next_zone_marker;
	marker->fb.path_marker = Z16_path_marker;
	marker->fb.sight_from_marker = Z16_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z16_higher_sight_from_marker;
	marker->fb.sight_from_time = Z16_sight_from_time;
	if (Z16_tail) {
		Z16_tail->fb.Z_next = marker;
		Z16_tail = marker;
	}
	else  {
		Z16_head = Z16_tail = marker;
		Z16_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z16_head;
	}
	marker->fb.Z_head = Z16_head;
}

void Z17(gedict_t* marker) {
	Z17_S_index = Z17_S_index + 1;
	S_index = Z17_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z17_ = marker->fb.Z17_rev = marker->fb.Z17_next_zone = marker->fb.Z17_next = marker;
	marker->fb.Z_ = 17;
	marker->fb.zone_marker = Z17_marker;
	marker->fb.next_zone_marker = Z17_next_zone_marker;
	marker->fb.path_marker = Z17_path_marker;
	marker->fb.sight_from_marker = Z17_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z17_higher_sight_from_marker;
	marker->fb.sight_from_time = Z17_sight_from_time;
	if (Z17_tail) {
		Z17_tail->fb.Z_next = marker;
		Z17_tail = marker;
	}
	else  {
		Z17_head = Z17_tail = marker;
		Z17_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z17_head;
	}
	marker->fb.Z_head = Z17_head;
}

void Z18(gedict_t* marker) {
	Z18_S_index = Z18_S_index + 1;
	S_index = Z18_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z18_ = marker->fb.Z18_rev = marker->fb.Z18_next_zone = marker->fb.Z18_next = marker;
	marker->fb.Z_ = 18;
	marker->fb.zone_marker = Z18_marker;
	marker->fb.next_zone_marker = Z18_next_zone_marker;
	marker->fb.path_marker = Z18_path_marker;
	marker->fb.sight_from_marker = Z18_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z18_higher_sight_from_marker;
	marker->fb.sight_from_time = Z18_sight_from_time;
	if (Z18_tail) {
		Z18_tail->fb.Z_next = marker;
		Z18_tail = marker;
	}
	else  {
		Z18_head = Z18_tail = marker;
		Z18_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z18_head;
	}
	marker->fb.Z_head = Z18_head;
}

void Z19(gedict_t* marker) {
	Z19_S_index = Z19_S_index + 1;
	S_index = Z19_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z19_ = marker->fb.Z19_rev = marker->fb.Z19_next_zone = marker->fb.Z19_next = marker;
	marker->fb.Z_ = 19;
	marker->fb.zone_marker = Z19_marker;
	marker->fb.next_zone_marker = Z19_next_zone_marker;
	marker->fb.path_marker = Z19_path_marker;
	marker->fb.sight_from_marker = Z19_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z19_higher_sight_from_marker;
	marker->fb.sight_from_time = Z19_sight_from_time;
	if (Z19_tail) {
		Z19_tail->fb.Z_next = marker;
		Z19_tail = marker;
	}
	else  {
		Z19_head = Z19_tail = marker;
		Z19_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z19_head;
	}
	marker->fb.Z_head = Z19_head;
}

void Z20(gedict_t* marker) {
	Z20_S_index = Z20_S_index + 1;
	S_index = Z20_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z20_ = marker->fb.Z20_rev = marker->fb.Z20_next_zone = marker->fb.Z20_next = marker;
	marker->fb.Z_ = 20;
	marker->fb.zone_marker = Z20_marker;
	marker->fb.next_zone_marker = Z20_next_zone_marker;
	marker->fb.path_marker = Z20_path_marker;
	marker->fb.sight_from_marker = Z20_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z20_higher_sight_from_marker;
	marker->fb.sight_from_time = Z20_sight_from_time;
	if (Z20_tail) {
		Z20_tail->fb.Z_next = marker;
		Z20_tail = marker;
	}
	else  {
		Z20_head = Z20_tail = marker;
		Z20_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z20_head;
	}
	marker->fb.Z_head = Z20_head;
}

void Z21(gedict_t* marker) {
	Z21_S_index = Z21_S_index + 1;
	S_index = Z21_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z21_ = marker->fb.Z21_rev = marker->fb.Z21_next_zone = marker->fb.Z21_next = marker;
	marker->fb.Z_ = 21;
	marker->fb.zone_marker = Z21_marker;
	marker->fb.next_zone_marker = Z21_next_zone_marker;
	marker->fb.path_marker = Z21_path_marker;
	marker->fb.sight_from_marker = Z21_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z21_higher_sight_from_marker;
	marker->fb.sight_from_time = Z21_sight_from_time;
	if (Z21_tail) {
		Z21_tail->fb.Z_next = marker;
		Z21_tail = marker;
	}
	else  {
		Z21_head = Z21_tail = marker;
		Z21_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z21_head;
	}
	marker->fb.Z_head = Z21_head;
}

void Z22(gedict_t* marker) {
	Z22_S_index = Z22_S_index + 1;
	S_index = Z22_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z22_ = marker->fb.Z22_rev = marker->fb.Z22_next_zone = marker->fb.Z22_next = marker;
	marker->fb.Z_ = 22;
	marker->fb.zone_marker = Z22_marker;
	marker->fb.next_zone_marker = Z22_next_zone_marker;
	marker->fb.path_marker = Z22_path_marker;
	marker->fb.sight_from_marker = Z22_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z22_higher_sight_from_marker;
	marker->fb.sight_from_time = Z22_sight_from_time;
	if (Z22_tail) {
		Z22_tail->fb.Z_next = marker;
		Z22_tail = marker;
	}
	else  {
		Z22_head = Z22_tail = marker;
		Z22_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z22_head;
	}
	marker->fb.Z_head = Z22_head;
}

void Z23(gedict_t* marker) {
	Z23_S_index = Z23_S_index + 1;
	S_index = Z23_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z23_ = marker->fb.Z23_rev = marker->fb.Z23_next_zone = marker->fb.Z23_next = marker;
	marker->fb.Z_ = 23;
	marker->fb.zone_marker = Z23_marker;
	marker->fb.next_zone_marker = Z23_next_zone_marker;
	marker->fb.path_marker = Z23_path_marker;
	marker->fb.sight_from_marker = Z23_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z23_higher_sight_from_marker;
	marker->fb.sight_from_time = Z23_sight_from_time;
	if (Z23_tail) {
		Z23_tail->fb.Z_next = marker;
		Z23_tail = marker;
	}
	else  {
		Z23_head = Z23_tail = marker;
		Z23_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z23_head;
	}
	marker->fb.Z_head = Z23_head;
}

void Z24(gedict_t* marker) {
	Z24_S_index = Z24_S_index + 1;
	S_index = Z24_S_index;
	Set_sub_arrival_time(marker);
	marker->fb.Z24_ = marker->fb.Z24_rev = marker->fb.Z24_next_zone = marker->fb.Z24_next = marker;
	marker->fb.Z_ = 24;
	marker->fb.zone_marker = Z24_marker;
	marker->fb.next_zone_marker = Z24_next_zone_marker;
	marker->fb.path_marker = Z24_path_marker;
	marker->fb.sight_from_marker = Z24_sight_from_marker;
	marker->fb.higher_sight_from_marker = Z24_higher_sight_from_marker;
	marker->fb.sight_from_time = Z24_sight_from_time;
	if (Z24_tail) {
		Z24_tail->fb.Z_next = marker;
		Z24_tail = marker;
	}
	else  {
		Z24_head = Z24_tail = marker;
		Z24_head->fb.zone_stack_next = zone_stack_head;
		zone_stack_head = Z24_head;
	}
	marker->fb.Z_head = Z24_head;
}

void NameZone(float zoneNumber, char* name) {
	gedict_t* m;
	m = first_marker;
	while (m) {
		if (m->fb.Z_ == zoneNumber) {
			m->s.v.netname = name;
		}
		m = m->fb.marker_link;
	}
	m = zone_stack_head;
}

void AdjustZone(gedict_t* zoneHead, float teamNumber, float strong, float adjust) {
	if (!zoneHead) {
		return;
	}
	zoneHead->fb.total_players = zoneHead->fb.total_players + adjust;
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
	}
}

void LeaveZone(float zoneNumber, float teamNumber, float strong) {
	if (zoneNumber == 1) {
		AdjustZone(Z1_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 2) {
		AdjustZone(Z2_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 3) {
		AdjustZone(Z3_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 4) {
		AdjustZone(Z4_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 5) {
		AdjustZone(Z5_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 6) {
		AdjustZone(Z6_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 7) {
		AdjustZone(Z7_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 8) {
		AdjustZone(Z8_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 9) {
		AdjustZone(Z9_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 10) {
		AdjustZone(Z10_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 11) {
		AdjustZone(Z11_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 12) {
		AdjustZone(Z12_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 13) {
		AdjustZone(Z13_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 14) {
		AdjustZone(Z12_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 15) {
		AdjustZone(Z13_head, teamNumber, strong, -1);
	}
	else if (zoneNumber == 16) {
		AdjustZone(Z12_head, teamNumber, strong, -1);
	}
}

void EnterZone(float zoneNumber, float teamNumber, float strong) {
	if (zoneNumber == 1) {
		AdjustZone(Z1_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 2) {
		AdjustZone(Z2_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 3) {
		AdjustZone(Z3_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 4) {
		AdjustZone(Z4_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 5) {
		AdjustZone(Z5_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 6) {
		AdjustZone(Z6_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 7) {
		AdjustZone(Z7_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 8) {
		AdjustZone(Z8_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 9) {
		AdjustZone(Z9_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 10) {
		AdjustZone(Z10_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 11) {
		AdjustZone(Z11_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 12) {
		AdjustZone(Z12_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 13) {
		AdjustZone(Z13_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 14) {
		AdjustZone(Z13_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 15) {
		AdjustZone(Z13_head, teamNumber, strong, 1);
	}
	else if (zoneNumber == 16) {
		AdjustZone(Z13_head, teamNumber, strong, 1);
	}
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

void DebugZone(float i) {
	if (i == 1) {
		DebugZoneByEnt(Z1_head);
	}
	if (i == 2) {
		DebugZoneByEnt(Z2_head);
	}
	if (i == 3) {
		DebugZoneByEnt(Z3_head);
	}
	if (i == 4) {
		DebugZoneByEnt(Z4_head);
	}
	if (i == 5) {
		DebugZoneByEnt(Z5_head);
	}
	if (i == 6) {
		DebugZoneByEnt(Z6_head);
	}
	if (i == 7) {
		DebugZoneByEnt(Z7_head);
	}
	if (i == 8) {
		DebugZoneByEnt(Z8_head);
	}
	if (i == 9) {
		DebugZoneByEnt(Z9_head);
	}
	if (i == 10) {
		DebugZoneByEnt(Z10_head);
	}
	if (i == 11) {
		DebugZoneByEnt(Z11_head);
	}
	if (i == 12) {
		DebugZoneByEnt(Z12_head);
	}
	if (i == 13) {
		DebugZoneByEnt(Z13_head);
	}
	if (i == 14) {
		DebugZoneByEnt(Z14_head);
	}
	if (i == 15) {
		DebugZoneByEnt(Z15_head);
	}
	if (i == 16) {
		DebugZoneByEnt(Z16_head);
	}
}

void DebugZones() {
	bprint_fb(2, "Zones:\\");
	for (i = 1;i < 16;++i) {
		DebugZone(i);
	}
}

