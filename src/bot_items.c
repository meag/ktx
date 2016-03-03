
#include "g_local.h"
#include "fb_globals.h"

// Goal functions

static float goal_health0(gedict_t* self) {
	return self->fb.desire_health0;
}

static float goal_health2(gedict_t* self) {
	return self->fb.desire_health2;
}

static float goal_NULL(gedict_t* self) {
	return 0;
}

static float goal_armor1(gedict_t* self) {
	return self->fb.desire_armor1;
}

static float goal_armor2(gedict_t* self) {
	if (self->fb.desire_armor2) {
		return (self->fb.desire_armor2 + virtual_enemy->fb.desire_armor2);	
	}
	else {
		qbool low_armor = (qbool) (self->fb.total_armor <= 100 && self->s.v.health >= 50);
		qbool has_rl = (qbool) (((int)self->s.v.items & IT_ROCKET_LAUNCHER) && self->s.v.ammo_rockets);
		qbool has_quad = (qbool) (self->super_damage_finished <= g_globalvars.time);

		if (low_armor && has_rl && has_quad) {
			return virtual_enemy->fb.desire_armor2;
		}
		return 0;
	}
}

static float goal_armorInv(gedict_t* self) {
	if (self->fb.desire_armorInv) {
		return (self->fb.desire_armorInv + virtual_enemy->fb.desire_armorInv);
	}
	else {
		qbool has_rl = (qbool) (((int)self->s.v.items & IT_ROCKET_LAUNCHER) && (self->s.v.ammo_rockets));
		qbool has_quad = (qbool) (self->super_damage_finished <= g_globalvars.time);
		qbool ok_health = (qbool) (self->s.v.health >= 50);
		
		if (has_rl && has_quad && ok_health) {
			return virtual_enemy->fb.desire_armorInv;
		}
		return 0;
	}
}

static float goal_supershotgun1(gedict_t* self) {
	return (self->fb.desire_supershotgun + (virtual_enemy->fb.desire_supershotgun * 0.5));
}

static float goal_supershotgun2(gedict_t* self) {
	if ((int)self->s.v.items & IT_SUPER_SHOTGUN) {
		return 0;
	}
	return goal_supershotgun1();
}

static float goal_nailgun1(gedict_t* self) {
	return (self->fb.desire_nailgun + (virtual_enemy->fb.desire_nailgun * 0.5));
}

static float goal_nailgun2(gedict_t* self) {
	if ((int)self->s.v.items & IT_NAILGUN) {
		return 0;
	}
	return goal_nailgun1();
}

static float goal_supernailgun1(gedict_t* self) {
	return (self->fb.desire_supernailgun + (virtual_enemy->fb.desire_supernailgun * 0.5));
}

static float goal_supernailgun2(gedict_t* self) {
	if ((int)self->s.v.items & IT_SUPER_NAILGUN) {
		return 0;
	}
	return goal_supernailgun1();
}

static float goal_grenadelauncher1(gedict_t* self) {
	return (self->fb.desire_grenadelauncher + virtual_enemy->fb.desire_grenadelauncher);
}

static float goal_grenadelauncher2(gedict_t* self) {
	if ((int)self->s.v.items & IT_GRENADE_LAUNCHER) {
		return 0;
	}
	return goal_grenadelauncher1();
}

static float goal_rocketlauncher1(gedict_t* self) {
	return (self->fb.desire_rocketlauncher + virtual_enemy->fb.desire_rocketlauncher);
}

static float goal_rocketlauncher2(gedict_t* self) {
	if ((int)self->s.v.items & IT_ROCKET_LAUNCHER) {
		return 0;
	}
	return goal_rocketlauncher1();
}

static float goal_lightning1(gedict_t* self) {
	return (self->fb.desire_lightning + (virtual_enemy->fb.desire_lightning * 0.5));
}

// dmm3: if we have it then ignore
static float goal_lightning2(gedict_t* self) {
	if ((int)self->s.v.items & IT_LIGHTNING) {
		return 0;
	}
	return goal_lightning1(self);
}

static float goal_shells(gedict_t* self) {
	if (self->s.v.ammo_shells < 100) {
		return self->fb.desire_shells;
	}
	return 0;
}

static float goal_spikes(gedict_t* self) {
	if (self->s.v.ammo_nails < 200) {
		return self->fb.desire_nails;
	}
	return 0;
}

static float goal_rockets(gedict_t* self) {
	if (self->s.v.ammo_rockets < 100) {
		return (self->fb.desire_rockets + virtual_enemy->fb.desire_rockets);
	}
	return goal_desire;
}

static float goal_cells(gedict_t* self) {
	if (self->s.v.ammo_cells < 100) {
		return (self->fb.desire_cells + (virtual_enemy->fb.desire_cells * 0.5));
	}
	return 0;
}

static float goal_artifact_invulnerability(gedict_t* self) {
	return 200 + self->fb.total_damage;
}

static float goal_artifact_invisibility(gedict_t* self) {
	return 200 + self->fb.total_damage;
}

static float goal_artifact_super_damage(gedict_t* self) {
	return 200 + self->fb.total_damage;
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


// Item creation functions 
static void StartItem(gedict_t* ent) {
	AddToQue(ent);
	VectorSet(ent->s.v.view_ofs, 80, 80, 24);
	first_item = AddToList(first_item, ent);
	//ent->s.v.solid = SOLID_TRIGGER;
	//ent->s.v.flags = FL_ITEM;
	BecomeMarker(ent);
}

static void fb_spawn_health(gedict_t* ent) {
	if ((int)ent->s.v.spawnflags & H_MEGA) {
		ent->fb.desire = goal_health2;
		ent->fb.pickup = pickup_health2;
	}
	else {
		ent->fb.desire = goal_health0;
		ent->fb.pickup = pickup_health0;
	}

	StartItem(ent);
}

static void fb_spawn_armor(gedict_t* ent) {
	if (streq(ent->s.v.classname, "item_armor1")) {
		ent->fb.desire = goal_armor1;
		ent->fb.pickup = pickup_armor1;
	}
	else if (streq(ent->s.v.classname, "item_armor2")) {
		ent->fb.desire = goal_armor2;
		ent->fb.pickup = pickup_armor2;
	}
	else if (streq(ent->s.v.classname, "item_armorInv")) {
		ent->fb.desire = goal_armorInv;
		ent->fb.pickup = pickup_armorInv;
	}

	StartItem(ent);
}

static void fb_spawn_ssg(gedict_t* ent) {
	SetGoalForMarker(21, ent);

	available_weapons |= IT_SUPER_SHOTGUN;
	if (deathmatch == 1) {
		ent->fb.desire = goal_supershotgun1;
		ent->fb.pickup = pickup_true;
	}
	else {
		ent->fb.desire = goal_supershotgun2;
		ent->fb.pickup = pickup_supershotgun2;
	}

	StartItem(ent);
}

static void fb_spawn_ng(gedict_t* ent) {
	SetGoalForMarker(22, ent);

	available_weapons |= IT_NAILGUN;
	if (deathmatch == 1) {
		ent->fb.desire = goal_nailgun1;
		ent->fb.pickup = pickup_true;
	}
	else {
		ent->fb.desire = goal_nailgun2;
		ent->fb.pickup = pickup_nailgun2;
	}

	StartItem(ent);
}

static void fb_spawn_sng(gedict_t* ent) {
	SetGoalForMarker(20, ent);

	available_weapons |= IT_SUPER_NAILGUN;
	if (deathmatch == 1) {
		ent->fb.desire = goal_supernailgun1;
		ent->fb.pickup = pickup_true;
	}
	else {
		ent->fb.desire = goal_supernailgun2;
		ent->fb.pickup = pickup_supernailgun2;
	}

	StartItem(ent);
}

static void fb_spawn_gl(gedict_t* ent) {
	// no goal set for GL

	available_weapons |= IT_GRENADE_LAUNCHER;
	if (deathmatch == 1) {
		ent->fb.desire = goal_grenadelauncher1;
		ent->fb.pickup = pickup_true;
	}
	else {
		ent->fb.desire = goal_grenadelauncher2;
		ent->fb.pickup = pickup_grenadelauncher2;
	}

	StartItem(ent);
}

static void fb_spawn_rl(gedict_t* ent) {
	// no goal set for RL

	available_weapons |= IT_ROCKET_LAUNCHER;
	if (deathmatch == 1) {
		ent->fb.desire = goal_rocketlauncher1;
		ent->fb.pickup = pickup_true;
	}
	else {
		ent->fb.desire = goal_rocketlauncher2;
		ent->fb.pickup = pickup_rocketlauncher2;
	}

	StartItem(ent);
}

static void fb_spawn_lg(gedict_t* ent) {
	// no goal set for LG

	available_weapons |= IT_LIGHTNING;
 	if (deathmatch == 1) {
		ent->fb.desire = goal_lightning1;
		ent->fb.pickup = pickup_true;
	}
	else {
		ent->fb.desire = goal_lightning2;
		ent->fb.pickup = pickup_lightning2;
	}

	StartItem(ent);
}

static void fb_spawn_shells(gedict_t* ent) {
	SetGoalForMarker(24, ent);

	ent->fb.desire = goal_shells;
	ent->fb.pickup = pickup_shells;

	StartItem(ent);
}

static void fb_spawn_spikes(gedict_t* ent) {
	SetGoalForMarker(23, ent);

	ent->fb.desire = goal_spikes;
	ent->fb.pickup = pickup_spikes;

	StartItem(ent);
}

static void fb_spawn_rockets(gedict_t* ent) {
	// no goal for rockets

	ent->fb.desire = goal_rockets;
	ent->fb.pickup = pickup_rockets;

	StartItem(ent);
}

static void fb_spawn_cells(gedict_t* ent) {
	SetGoalForMarker(19, ent);

	ent->fb.desire = goal_cells;
	ent->fb.pickup = pickup_cells;

	StartItem(ent);
}

static void fb_spawn_weapon(gedict_t* ent) {
	if ((int)ent->s.v.spawnflags & WEAPON_SHOTGUN) {
		fb_spawn_shells(ent);
	}
	else if ((int)ent->s.v.spawnflags & WEAPON_SPIKES) {
		fb_spawn_spikes(ent);
	}
	else if ((int)ent->s.v.spawnflags & WEAPON_ROCKET) {
		fb_spawn_rockets(ent);
	}
}

static void fb_spawn_pent(gedict_t* ent) {
	ent->fb.desire = goal_artifact_invulnerability;
	ent->fb.pickup = pickup_true;

	StartItem(ent);
}

static void fb_spawn_biosuit(gedict_t* ent) {
	ent->fb.desire = goal_NULL;
	ent->fb.pickup = pickup_true;

	StartItem(ent);
}

static void fb_spawn_ring(gedict_t* ent) {
	ent->fb.desire = goal_artifact_invisibility;
	ent->fb.pickup = pickup_true;

	StartItem(ent);
}

static void fb_spawn_quad(gedict_t* ent) {
	ent->fb.desire = goal_artifact_super_damage;
	ent->fb.pickup = pickup_true;
	
	StartItem(ent);
}
