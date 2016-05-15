
#include "g_local.h"
#include "fb_globals.h"

// Goal functions

void check_marker (void);

static float goal_health0(gedict_t* self) {
	return self->fb.desire_health0;
}

static float goal_health2(gedict_t* self) {
	return self->fb.desire_health2;
}

float goal_NULL(gedict_t* self) {
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
	return goal_supershotgun1(self);
}

static float goal_nailgun1(gedict_t* self) {
	return (self->fb.desire_nailgun + (virtual_enemy->fb.desire_nailgun * 0.5));
}

static float goal_nailgun2(gedict_t* self) {
	if ((int)self->s.v.items & IT_NAILGUN) {
		return 0;
	}
	return goal_nailgun1(self);
}

static float goal_supernailgun1(gedict_t* self) {
	return (self->fb.desire_supernailgun + (virtual_enemy->fb.desire_supernailgun * 0.5));
}

static float goal_supernailgun2(gedict_t* self) {
	if ((int)self->s.v.items & IT_SUPER_NAILGUN) {
		return 0;
	}
	return goal_supernailgun1(self);
}

static float goal_grenadelauncher1(gedict_t* self) {
	return (self->fb.desire_grenadelauncher + virtual_enemy->fb.desire_grenadelauncher);
}

static float goal_grenadelauncher2(gedict_t* self) {
	if ((int)self->s.v.items & IT_GRENADE_LAUNCHER) {
		return 0;
	}
	return goal_grenadelauncher1(self);
}

static float goal_rocketlauncher1(gedict_t* self) {
	return (self->fb.desire_rocketlauncher + virtual_enemy->fb.desire_rocketlauncher);
}

static float goal_rocketlauncher2(gedict_t* self) {
	if ((int)self->s.v.items & IT_ROCKET_LAUNCHER) {
		return 0;
	}
	return goal_rocketlauncher1(self);
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

qbool pickup_health0(void) {
	return (qbool) (self->s.v.health < 100);
}

qbool pickup_health2(void) {
	return (qbool) (self->s.v.health < 250);
}

qbool pickup_armor1(void) {
	return (qbool) (self->fb.total_armor < 30);
}

qbool pickup_armor2(void) {
	return (qbool) (self->fb.total_armor < 90);
}

qbool pickup_armorInv(void) {
	return (qbool) (self->fb.total_armor < 160);
}

qbool pickup_supershotgun2(void) {
	return (qbool) !((int)self->s.v.items & IT_SUPER_SHOTGUN);
}

qbool pickup_true(void) {
	return TRUE;
}

qbool pickup_nailgun2(void) {
	return (qbool) !((int)self->s.v.items & IT_NAILGUN);
}

qbool pickup_supernailgun2(void) {
	return (qbool) !((int)self->s.v.items & IT_SUPER_NAILGUN);
}

qbool pickup_grenadelauncher2(void) {
	return (qbool) !((int)self->s.v.items & IT_GRENADE_LAUNCHER);
}

qbool pickup_rocketlauncher2(void) {
	return (qbool) !((int)self->s.v.items & IT_ROCKET_LAUNCHER);
}

qbool pickup_lightning2(void) {
	return (qbool) !((int)self->s.v.items & IT_LIGHTNING);
}

qbool pickup_shells(void) {
	return (qbool) (self->s.v.ammo_shells < 100);
}

qbool pickup_spikes(void) {
	return (qbool) (self->s.v.ammo_nails < 200);
}

qbool pickup_rockets(void) {
	return (qbool) (self->s.v.ammo_rockets < 100);
}

qbool pickup_cells(void) {
	return (qbool) (self->s.v.ammo_cells < 100);
}

// Item creation functions 
static void StartItemFB(gedict_t* ent) {
	AddToQue(ent);
	VectorSet(ent->s.v.view_ofs, 80, 80, 24);
	if (! ent->s.v.touch) {
		ent->s.v.touch = (func_t) marker_touch;
		ent->s.v.nextthink = -1;
	}
	//ent->s.v.solid = SOLID_TRIGGER;
	//ent->s.v.flags = FL_ITEM;
	BecomeMarker(ent);
}

//
// Health
static void fb_health_taken (gedict_t* item, gedict_t* player)
{
	if ((int)item->s.v.spawnflags & H_MEGA)
	{
		// TODO: TeamReport (TookMega)
	}

	AssignVirtualGoal ();
	UpdateTotalDamage (player);
	UpdateGoalEntity (item);
}

static void fb_health_touch (gedict_t* item, gedict_t* player)
{
	if (marker_time) {
		check_marker ();
	}
}

static void fb_health_rot (gedict_t* item, gedict_t* player)
{
	UpdateTotalDamage (player);

	if (player->s.v.health <= 100)
	{
		AssignVirtualGoal ();
	}
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

	ent->fb.item_taken = fb_health_taken;
	ent->fb.item_touch = fb_health_touch;
	ent->fb.item_affect = fb_health_rot;
	StartItemFB(ent);
}

//
// Armor

static void fb_armor_taken (gedict_t* item, gedict_t* player)
{
	UpdateTotalDamage(player);
	UpdateGoalEntity(item);
}

static void fb_armor_touch (gedict_t* item, gedict_t* player)
{
	// allow the bot to hurt themselves to pickup armor
	qbool have_more_armor = item->fb.total_armor >= player->fb.total_armor;
	qbool want_armor = player->s.v.goalentity == NUM_FOR_EDICT (item);
	qbool targetting_player = player->fb.look_object && player->fb.look_object->ct == ctPlayer;

	if (want_armor && have_more_armor && marker_time && !targetting_player && !player->fb.firing) {
		player->fb.state |= HURT_SELF;
		player->fb.linked_marker = item;
		player->fb.path_state = 0;
		player->fb.linked_marker_time = g_globalvars.time + 0.5f;
		player->fb.goal_refresh_time = g_globalvars.time + 2 + random();
	}
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

	ent->fb.item_taken = fb_armor_taken;
	ent->fb.item_touch = fb_armor_touch;
	ent->fb.total_armor = ent->s.v.armortype * ent->s.v.armorvalue;

	StartItemFB(ent);
}

//
// weapons

static void fb_weapon_touch (gedict_t* item, gedict_t* player)
{
	
}

static void fb_weapon_taken (gedict_t* item, gedict_t* player)
{

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

	StartItemFB(ent);
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

	StartItemFB(ent);
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

	StartItemFB(ent);
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

	StartItemFB(ent);
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

	StartItemFB(ent);
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

	StartItemFB(ent);
}

static void fb_spawn_shells(gedict_t* ent) {
	SetGoalForMarker(24, ent);

	ent->fb.desire = goal_shells;
	ent->fb.pickup = pickup_shells;

	StartItemFB(ent);
}

static void fb_spawn_spikes(gedict_t* ent) {
	SetGoalForMarker(23, ent);

	ent->fb.desire = goal_spikes;
	ent->fb.pickup = pickup_spikes;

	StartItemFB(ent);
}

static void fb_spawn_rockets(gedict_t* ent) {
	// no goal for rockets

	ent->fb.desire = goal_rockets;
	ent->fb.pickup = pickup_rockets;

	StartItemFB(ent);
}

static void fb_spawn_cells(gedict_t* ent) {
	SetGoalForMarker(19, ent);

	ent->fb.desire = goal_cells;
	ent->fb.pickup = pickup_cells;

	StartItemFB(ent);
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

	StartItemFB(ent);
}

static void fb_spawn_biosuit(gedict_t* ent) {
	ent->fb.desire = goal_NULL;
	ent->fb.pickup = pickup_true;

	StartItemFB(ent);
}

static void fb_spawn_ring(gedict_t* ent) {
	ent->fb.desire = goal_artifact_invisibility;
	ent->fb.pickup = pickup_true;

	StartItemFB(ent);
}

static void fb_spawn_quad(gedict_t* ent) {
	ent->fb.desire = goal_artifact_super_damage;
	ent->fb.pickup = pickup_true;
	
	StartItemFB(ent);
}

fb_spawn_t itemSpawnFunctions[] = {
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

int ItemSpawnFunctionCount (void)
{
	return sizeof (itemSpawnFunctions) / sizeof (itemSpawnFunctions[0]);
}
