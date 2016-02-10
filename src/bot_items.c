
#include "g_local.h"
#include "fb_globals.h"

// Goal functions (TODO)
//   These should all be converted to float goal_xxx(gedict_t* self)

void goal_health0() {
	goal_desire = self->fb.desire_health0;
}

void goal_health2() {
	goal_desire = self->fb.desire_health2;
}

void goal_NULL() {
	goal_desire = 0;
}

void goal_armor1() {
	goal_desire = self->fb.desire_armor1;
}

void goal_armor2() {
	if (self->fb.desire_armor2) {
		goal_desire = (self->fb.desire_armor2 + virtual_enemy->fb.desire_armor2);	
	}
	else {
		qbool low_armor = (qbool) (self->fb.total_armor <= 100 && self->s.v.health >= 50);
		qbool has_rl = (qbool) ((items_ & IT_ROCKET_LAUNCHER) && self->s.v.ammo_rockets);
		qbool has_quad = (qbool) (self->fb.super_damage_finished <= g_globalvars.time);

		if (low_armor && has_rl && has_quad) {
			goal_desire = virtual_enemy->fb.desire_armor2;
			return;
		}
		goal_desire = 0;
	}
}

void goal_armorInv() {
	if (self->fb.desire_armorInv) {
		goal_desire = (self->fb.desire_armorInv + virtual_enemy->fb.desire_armorInv);
	}
	else {
		qbool has_rl = (qbool) ((items_ & IT_ROCKET_LAUNCHER) && (self->s.v.ammo_rockets));
		qbool has_quad = (qbool) (self->fb.super_damage_finished <= g_globalvars.time);
		qbool ok_health = (qbool) (self->s.v.health >= 50);
		
		if (has_rl && has_quad && ok_health) {
			goal_desire = virtual_enemy->fb.desire_armorInv;
		}
		else {
			goal_desire = 0;
		}
	}
}

void goal_supershotgun1() {
	goal_desire = (self->fb.desire_supershotgun + (virtual_enemy->fb.desire_supershotgun * 0.5));
}

void goal_supershotgun2() {
	if (items_ & IT_SUPER_SHOTGUN) {
		goal_desire = 0;
	}
	else {
		goal_supershotgun1();
	}
}

void goal_nailgun1() {
	goal_desire = (self->fb.desire_nailgun + (virtual_enemy->fb.desire_nailgun * 0.5));
}

void goal_nailgun2() {
	if (items_ & IT_NAILGUN) {
		goal_desire = 0;
	}
	else {
		goal_nailgun1();
	}
}

void goal_supernailgun1() {
	goal_desire = (self->fb.desire_supernailgun + (virtual_enemy->fb.desire_supernailgun * 0.5));
}

void goal_supernailgun2() {
	if (items_ & IT_SUPER_NAILGUN) {
		goal_desire = 0;
	}
	else {
		goal_supernailgun1();
	}
}

void goal_grenadelauncher1() {
	goal_desire = (self->fb.desire_grenadelauncher + virtual_enemy->fb.desire_grenadelauncher);
}

void goal_grenadelauncher2() {
	if (items_ & IT_GRENADE_LAUNCHER) {
		goal_desire = 0;
	}
	else {
		goal_grenadelauncher1();
	}
}

void goal_rocketlauncher1() {
	goal_desire = (self->fb.desire_rocketlauncher + virtual_enemy->fb.desire_rocketlauncher);
}

void goal_rocketlauncher2() {
	if (items_ & IT_ROCKET_LAUNCHER) {
		goal_desire = 0;
	}
	else {
		goal_rocketlauncher1();
	}
}

void goal_lightning1() {
	goal_desire = (self->fb.desire_lightning + (virtual_enemy->fb.desire_lightning * 0.5));
}

// dmm3: if we have it then ignore
void goal_lightning2() {
	if (items_ & IT_LIGHTNING) {
		goal_desire = 0;
	}
	else {
		goal_lightning1();
	}
}

void goal_shells() {
	if (self->s.v.ammo_shells < 100) {
		goal_desire = self->fb.desire_shells;
	}
	else {
		goal_desire = 0;
	}
}

void goal_spikes() {
	if (self->s.v.ammo_nails < 200) {
		goal_desire = self->fb.desire_nails;
	}
	else {
		goal_desire = 0;
	}
}

void goal_rockets() {
	if (self->s.v.ammo_rockets < 100) {
		goal_desire = (self->fb.desire_rockets + virtual_enemy->fb.desire_rockets);
	}
	else {
		goal_desire = 0;
	}
}

void goal_cells() {
	if (self->s.v.ammo_cells < 100) {
		goal_desire = (self->fb.desire_cells + (virtual_enemy->fb.desire_cells * 0.5));
	}
	else {
		goal_desire = 0;
	}
}

void goal_artifact_invulnerability() {
	goal_desire = 200 + self->fb.total_damage;
}

void goal_artifact_invisibility() {
	goal_desire = 200 + self->fb.total_damage;
}

void goal_artifact_super_damage() {
	goal_desire = 200 + self->fb.total_damage;
}

// Pickup functions (TODO)

qbool pickup_health0() {
	return (qbool) (self->s.v.health < 100);
}

qbool pickup_health2() {
	return (qbool) (self->s.v.health < 250);
}

qbool pickup_armor1() {
	return (qbool) (self->fb.total_armor < 30);
}

qbool pickup_armor2() {
	return (qbool) (self->fb.total_armor < 90);
}

qbool pickup_armorInv() {
	return (qbool) (self->fb.total_armor < 160);
}

qbool pickup_supershotgun2() {
	return (qbool) !((int)self->s.v.items & IT_SUPER_SHOTGUN);
}

qbool pickup_true() {
	return (qbool) true;
}

qbool pickup_nailgun2() {
	return (qbool) !((int)self->s.v.items & IT_NAILGUN);
}

qbool pickup_supernailgun2() {
	return (qbool) !((int)self->s.v.items & IT_SUPER_NAILGUN);
}

qbool pickup_grenadelauncher2() {
	return (qbool) !((int)self->s.v.items & IT_GRENADE_LAUNCHER);
}

qbool pickup_rocketlauncher2() {
	return (qbool) !((int)self->s.v.items & IT_ROCKET_LAUNCHER);
}

qbool pickup_lightning2() {
	return (qbool) !((int)self->s.v.items & IT_LIGHTNING);
}

qbool pickup_shells() {
	return (qbool) (self->s.v.ammo_shells < 100);
}

qbool pickup_spikes() {
	return (qbool) (self->s.v.ammo_nails < 200);
}

qbool pickup_rockets() {
	return (qbool) (self->s.v.ammo_rockets < 100);
}

qbool pickup_cells() {
	return (qbool) (self->s.v.ammo_cells < 100);
}

