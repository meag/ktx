// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void check_marker (gedict_t* self, gedict_t* other);

typedef struct fb_mapping_s {
	char* name;
	fb_void_func_t func;
} fb_mapping_t;

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

void InvalidMap() {
	G_sprint(self, 2, g_globalvars.mapname);
	G_sprint(self, 2, "Map is unsupported for bots.\n");
	G_sprint(self, 2, "Valid maps for bots are:\n");
	G_sprint(self, 2, "dm3, dm4, dm6, e1m2, frobodm2, aerowalk, spinev2, pkeg1, ultrav, ztndm3, amphi2, povdmm4\n");
}

static void fb_spawn_button(gedict_t* ent) {
	AddToQue(ent);

	if (ent->s.v.health) {
		//Add_takedamage(ent);
	}
	else {
		BecomeMarker(ent);
		adjust_view_ofs_z(ent);
	}
}

static void fb_spawn_changelevel(gedict_t* ent) {
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
			//Add_takedamage(ent);
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
			//Add_takedamage(ent);
		}
	}
}

static void fb_spawn_trigger_teleport (gedict_t* ent)
{
	AddToQue (ent);

	//Com_Printf ("fb_trigger_tele([%f %f %f] > [%f %f %f])\n", PASSVEC3 (ent->s.v.mins), PASSVEC3 (ent->s.v.maxs));
	VectorSet (ent->fb.virtual_mins, ent->s.v.mins[0] - 18, ent->s.v.mins[1] - 18, ent->s.v.mins[2] - 34);
	VectorSet (ent->fb.virtual_maxs, ent->s.v.maxs[0] + 18, ent->s.v.maxs[1] + 18, ent->s.v.maxs[2] + 26);
	setsize(ent, ent->s.v.mins[0] - 32, ent->s.v.mins[1] - 32, ent->s.v.mins[2], ent->s.v.maxs[0] + 32, ent->s.v.maxs[1] + 32, ent->s.v.maxs[2]);
	VectorSet (ent->s.v.view_ofs, 0.5 * (ent->s.v.absmax[0] - ent->s.v.absmin[0]), 0.5 * (ent->s.v.absmax[1] - ent->s.v.absmin[1]), 0.5 * (ent->s.v.absmax[2] - ent->s.v.absmin[2]));
	adjust_view_ofs_z(ent);
	BecomeMarker(ent);
}

static void fb_spawn_teleport_destination_touch (void)
{
	if (marker_time && other->ct == ctPlayer) {
		check_marker (self, other);
	}
}

static void fb_spawn_teleport_destination (gedict_t* ent)
{
	AddToQue (ent);

	ent->s.v.solid = SOLID_TRIGGER;
	ent->s.v.touch = (func_t) fb_spawn_teleport_destination_touch;
	ent->s.v.flags = FL_ITEM;
	BecomeMarker (ent);
	setsize (ent, -65, -65, -24, 65, 65, 32);
	VectorSet(ent->s.v.view_ofs, 80, 80, 24);
	ent->fb.pickup = pickup_true;
	adjust_view_ofs_z (ent);
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
	{ "info_teleport_destination", fb_spawn_teleport_destination },
	{ "trigger_teleport", fb_spawn_trigger_teleport },
	{ "trigger_setskill", fb_spawn_simple },
	{ "trigger_onlyregistered", fb_spawn_simple },
	{ "trigger_hurt", fb_spawn_simple },
	{ "trigger_push", fb_spawn_simple }
};

static void CreateItemMarkers() {
	// Old frogbot method was to call during item spawns, we just 
	//    catch up afterwards once we know the map is valid
	gedict_t* item;

	for (item = world; item = nextent(item); ) {
		int i = 0;
		qbool found = false;

		// check for item spawn
		for (i = 0; i < ItemSpawnFunctionCount(); ++i) {
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

// After all markers have been created, re-process items
static void AssignVirtualGoals (void)
{
	for (item = world; item = nextent(item); ) {
		int i = 0;

		for (i = 0; i < ItemSpawnFunctionCount(); ++i) {
			if (streq(itemSpawnFunctions[i].name, item->s.v.classname)) {
				AssignVirtualGoal_apply (item);
				break;
			}
		}
	}
}

qbool LoadBotRoutingFromFile (void);

void LoadMap() {
	// Need to do this anyway, otherwise teleporters will be broken
	CreateItemMarkers();

	// If we have a .bot file, use that
	if (LoadBotRoutingFromFile ()) {
		AssignVirtualGoals ();
		AllMarkersLoaded();
	}
	else {
		// Fall-back to built-in support
		int i = 0;
		for (i = 0; i < sizeof(maps) / sizeof(maps[0]); ++i) {
			if (streq(g_globalvars.mapname, maps[i].name)) {
				maps[i].func();
				AssignVirtualGoals ();
				AllMarkersLoaded();
				return;
			}
		}
	}
}

qbool FrogbotsCheckMapSupport (void)
{
	int i = 0;

	for (i = 0; i < sizeof (maps) / sizeof (maps[0]); ++i) {
		if (streq (g_globalvars.mapname, maps[i].name)) {
			return true;
		}
	}

	G_sprint (self, 2, "Map %s not supported for bots\n", g_globalvars.mapname);
	return false;
}