// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

typedef void (*fb_spawn_func_t)(gedict_t* ent);

typedef struct fb_mapping_s {
	char* name;
	fb_void_func_t func;
} fb_mapping_t;

typedef struct fb_spawn_s {
	char* name;
	fb_spawn_func_t func;
} fb_spawn_t;

static fb_mapping_t maps[] = {
	{ "dm6", map_dm6 },
	{ "dm4", map_dm4 },
	{ "e1m2", map_e1m2 },
	{ "aerowalk", map_aerowalk },
	{ "ztndm3", map_ztndm3 },
	{ "dm3", map_dm3 },
	{ "spinev2", map_spinev2 },
	{ "pkeg1", map_pkeg1 },
	{ "ultrav", map_ultrav },
	{ "frobodm2", map_frobodm2 },
	{ "amphi2", map_amphi2 },
	{ "povdmm4", map_povdmm4 },
	{ "ukooldm2", map_ukooldm2 },
	{ "ztndm4", map_ztndm4 },
	{ "ztndm5", map_ztndm5 },
};

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

static fb_spawn_t itemSpawnFunctions[] = {
	{ "item_health", fb_spawn_health },
	{ "item_armor1", fb_spawn_armor },
	{ "item_armor2", fb_spawn_armor },
	{ "item_armorInv", fb_spawn_armor },
	{ "weapon_supershotgun", fb_spawn_ssg },
	{ "weapon_nailgun", fb_spawn_ng },
	{ "weapon_supernailgun", fb_spawn_sng },
	{ "weapon_grenadelauncher", fb_spawn_gl },
	{ "weapon_rocketlauncher", fb_spawn_rl },
	{ "weapon_lightning", fb_spawn_lg },
	{ "item_shells", fb_spawn_shells },
	{ "item_spikes", fb_spawn_spikes },
	{ "item_rockets", fb_spawn_rockets },
	{ "item_cells", fb_spawn_cells },
	{ "item_weapon", fb_spawn_weapon },
	{ "item_artifact_invulnerability", fb_spawn_pent },
	{ "item_artifact_envirosuit", fb_spawn_biosuit },
	{ "item_artifact_invisibility", fb_spawn_ring },
	{ "item_artifact_super_damage", fb_spawn_quad }
};

static void fb_spawn_button(gedict_t* ent) {
	AddToQue(ent);

	if (ent->s.v.health) {
		Add_takedamage(ent);
	}
	else {
		BecomeMarker(ent);
		adjust_view_ofs_z(ent);
	}
}

static void fb_spawn_changelever(gedict_t* ent) {
	AddToQue(ent);
}

static void fb_spawn_spawnpoint(gedict_t* ent) {
	AddToQue(ent);
	BecomeMarker(ent);
	adjust_view_ofs_z(ent);
}

// fixme: also in doors.c
#define SECRET_OPEN_ONCE 1	// stays open

static void fb_spawn_door(gedict_t* ent) {
	AddToQue(ent);

	if (ent->fb.wait < 0) {
		// TODO: ?
		return;
	}
	else if ((int)ent->s.v.spawnflags & SECRET_OPEN_ONCE) {
		// TODO: ?
		return;
	}
	else {
		if (ent->s.v.health) {
			Add_takedamage(ent);
		}

		adjust_view_ofs_z(ent);
		BecomeMarker(ent);
	}
}

static void fb_spawn_simple(gedict_t* ent) {
	AddToQue(ent);

	if (ent->fb.wait < 0) {
		// Remove... so ignore?
		return;
	}
	else {
		if (ent->s.v.health) {
			Add_takedamage(ent);
		}
	}
}

static fb_spawn_t stdSpawnFunctions[] = {
	{ "func_button", fb_spawn_button },
	{ "trigger_changelevel", fb_spawn_simple },
	{ "info_player_deathmatch", fb_spawn_spawnpoint },
	{ "door", fb_spawn_door },
	{ "trigger_multiple", fb_spawn_simple },
	{ "trigger_once", fb_spawn_simple },
	{ "trigger_secret", fb_spawn_simple },
	{ "trigger_counter", fb_spawn_simple },
	{ "info_teleport_destination", fb_spawn_simple },
	{ "trigger_teleport", fb_spawn_simple },
	{ "trigger_setskill", fb_spawn_simple },
	{ "trigger_onlyregistered", fb_spawn_simple },
	{ "trigger_hurt", fb_spawn_simple },
	{ "trigger_push", fb_spawn_simple }
};

void InvalidMap() {
	G_sprint(self, 2, g_globalvars.mapname);
	G_sprint(self, 2, "Map is unsupported for bots.\n");
	G_sprint(self, 2, "Valid maps for bots are:\n");
	G_sprint(self, 2, "dm3, dm4, dm6, e1m2, frobodm2, aerowalk, spinev2, pkeg1, ultrav, ztndm3, amphi2, povdmm4\n");
}

void CreateItemMarkers() {
	// Old frogbot method was to call during item spawns, we just 
	//    catch up afterwards once we know the map is valid
	gedict_t* item;

	for (item = world; item; item = nextent(item)) {
		int i = 0;
		qbool found = false;

		// check for item spawn
		for (i = 0; i < sizeof(itemSpawnFunctions) / sizeof(itemSpawnFunctions[0]); ++i) {
			if (streq(itemSpawnFunctions[i].name, item->s.v.classname)) {
				itemSpawnFunctions[i].func(item);
				found = true;
				break;
			}
		}

		// check for std spawn (world items like buttons etc)
		if (! found) {
			for (i = 0; i < sizeof(stdSpawnFunctions) / sizeof(stdSpawnFunctions[0]); ++i) {
				if (streq(stdSpawnFunctions[i].name, item->s.v.classname)) {
					stdSpawnFunctions[i].func(item);
					break;
				}
			}
		}
	}
}

void LoadMap() {
	int i = 0;

	load_frogbots = invalid_map = TRUE;

	for (i = 0; i < sizeof(maps) / sizeof(maps[0]); ++i) {
		if (streq(g_globalvars.mapname, maps[i].name)) {
			CreateItemMarkers();
			maps[i].func();
			return;
		}
	}

	StartItems();
}

