// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

gedict_t* AddToList(gedict_t* first_in_list, gedict_t* ent) {
	ent->fb.next = first_in_list;
	if (first_in_list) {
		ent->fb.previous = first_in_list->fb.previous;
		if (first_in_list->fb.previous) {
			first_in_list->fb.previous->fb.next = ent;
		}
		first_in_list->fb.previous = ent;
	}
	return ent;
}

gedict_t* RemoveFromList(gedict_t* first_in_list, gedict_t* ent) {
	if (first_in_list == ent) {
		first_in_list = first_in_list->fb.next;
	}
	if (ent->fb.previous) {
		ent->fb.previous->fb.next = ent->fb.next;
	}
	if (ent->fb.next) {
		ent->fb.next->fb.previous = ent->fb.previous;
	}
	return first_in_list;
}
/*
void Add_takedamage(gedict_t* ent) {
	ent->s.v.takedamage = DAMAGE_YES;
	first_takedamage = AddToList(first_takedamage, ent);
	takedamage_exists = TRUE;
}

void UpdateRadiusDamage() {
	if (takedamage_exists) {
		if (numberofclients) {
			return;
		}
		test_enemy = first_takedamage;
		while (test_enemy->fb.next) {
			test_enemy = test_enemy->fb.next;
		}
		test_enemy->fb.next = first_client;
		first_client->fb.previous = test_enemy;
	}
	else  {
		first_takedamage = first_client;
	}
}

void Add_client() {
	if (self->ct == ctPlayer) {
		return;
	}
	first_client = AddToList(first_client, self);
	UpdateRadiusDamage();
	numberofclients = numberofclients + 1;
	if (teamplay) {
		team_index = self->s.v.team - 1;
		self->fb.teamflag = 1;
		while (team_index) {
			team_index = team_index - 1;
			self->fb.teamflag = self->fb.teamflag * 2;
		}
	}
	self->fb.movetarget = NULL;
	self->s.v.goalentity = self->s.v.enemy = NUM_FOR_EDICT(world);
}
*/

gedict_t* EntityAt(gedict_t* start_entity, float index_) {
	while (index_ > 0) {
		start_entity = nextent(start_entity);
		index_ = index_ - 1;
	}
	return start_entity;
}

void InitFrogbots1() {
	prephysics = spawn();
	prephysics->s.v.think = (func_t) FrogbotPrePhysics2;
	postphysics = frogbot_spawned = spawn();
	postphysics->s.v.think = (func_t) FrogbotPostPhysics;
}

void InitFrogbots2() {
	total_entity_count = 1;
	for (self = world; self = nextent(self); ) {
		++total_entity_count;
	}

	for (self = first_item; self; self = self->fb.next) {
		self->s.v.movetype = MOVETYPE_NONE;
		self->s.v.origin[2] = self->s.v.origin[2] + 6;
		droptofloor(self);
		setsize(self, self->s.v.mins[0] - 49, self->s.v.mins[1] - 49, self->s.v.mins[2], self->s.v.maxs[0] + 49, self->s.v.maxs[1] + 49, self->s.v.maxs[2]);
		adjust_view_ofs_z(self);

		VectorCopy(self->s.v.absmin, self->fb.virtual_mins);
		self->fb.virtual_mins[0] += 32;
		self->fb.virtual_mins[1] += 32;
		self->fb.virtual_mins[2] -= 33;

		VectorCopy(self->fb.virtual_mins, self->fb.virtual_maxs);
		self->fb.virtual_mins[0] += 96;
		self->fb.virtual_mins[1] += 96;
		self->fb.virtual_mins[1] += 114;
	}

	LoadMap();
}
