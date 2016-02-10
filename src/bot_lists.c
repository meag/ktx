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

void CheckTwoTeams() {
	gedict_t* client;
	first_team = second_team = 0;
	client = first_client;
	first_team = first_client->fb.realteam;
	while (client) {
		new_team = client->fb.realteam;
		if (new_team != first_team) {
			if (second_team) {
				if (new_team != second_team) {
					two_teams = FALSE;
					return;
				}
			}
			else  {
				second_team = new_team;
			}
		}
		client = client->fb.next;
	}
	if (second_team) {
		two_teams = TRUE;
	}
	else  {
		two_teams = FALSE;
	}
}

void Add_client() {
	if (self->fb.client_) {
		return;
	}
	self->fb.client_ = TRUE;
	first_client = AddToList(first_client, self);
	UpdateRadiusDamage();
	numberofclients = numberofclients + 1;
	if (teamplay) {
		self->fb.realteam = self->s.v.team;
		team_index = self->s.v.team - 1;
		self->fb.teamflag = 1;
		while (team_index) {
			team_index = team_index - 1;
			self->fb.teamflag = self->fb.teamflag * 2;
		}
	}
	else  {
		current_team = current_team + 1;
		self->fb.realteam = current_team;
	}
	CheckTwoTeams();
	self->fb.movetarget = world;
	self->s.v.goalentity = self->s.v.enemy = NUM_FOR_EDICT(world);
}

void RemoveFromAllLists() {
	if (self->fb.client_) {
		self->fb.client_ = FALSE;
		self->fb.frogbot = FALSE;
		first_client = RemoveFromList(first_client, self);
		UpdateRadiusDamage();
		numberofclients = numberofclients - 1;
		CheckTwoTeams();
	}
}

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
	while (frogbot_number < maxplayers) {
		new_bot = spawn();
		new_bot->s.v.touch = (func_t) thud_touch;
		if (!first_frogbot) {
			first_frogbot = new_bot;
		}
		new_bot->fb.score_pos = frogbot_number;
		frogbot_number = frogbot_number + 1;
		new_bot->s.v.colormap = frogbot_number;
	}
	postphysics = frogbot_spawned = spawn();
	postphysics->s.v.think = (func_t) FrogbotPostPhysics;
}

void InitFrogbots2() {
	test_enemy = ez_find(world, "path_corner");
	while (test_enemy) {
		if (!test_enemy->fb.admin_code) {
			trap_remove(NUM_FOR_EDICT(test_enemy));
		}
		test_enemy = ez_find(test_enemy, "path_corner");
	}
	total_entity_count = 1;
	self = world;
	while (self = nextent(self)) {
		total_entity_count = total_entity_count + 1;
	}
	self = first_item;
	while (self) {
		self->s.v.movetype = MOVETYPE_NONE;
		self->s.v.origin[2] = self->s.v.origin[2] + 6;
		droptofloor(self);
		setsize(self, self->s.v.mins[0] - 49, self->s.v.mins[1] - 49, self->s.v.mins[2], self->s.v.maxs[0] + 49, self->s.v.maxs[1] + 49, self->s.v.maxs[2]);
		adjust_view_ofs_z();

		VectorCopy(self->s.v.absmin, self->fb.virtual_mins);
		self->fb.virtual_mins[0] += 32;
		self->fb.virtual_mins[1] += 32;
		self->fb.virtual_mins[2] -= 33;
		
		VectorCopy(self->fb.virtual_mins, self->fb.virtual_maxs);
		self->fb.virtual_mins[0] += 96;
		self->fb.virtual_mins[1] += 96;
		self->fb.virtual_mins[1] += 114;
		self = self->fb.next;
	}
	SpawnRunes(false);
	LoadMap();
}

void UpdateFrags(gedict_t* e) {
	if (fraglimit) {
		if (e->s.v.frags >= fraglimit) {
			NextLevel();
		}
	}
	if (e->fb.player) {
		return;
	}
	WriteByte(MSG_ALL, MSG_UPDATEFRAGS);
	WriteByte(MSG_ALL, e->fb.score_pos);
	WriteShort(MSG_ALL, e->s.v.frags);
}

void ClearName(float to, gedict_t* client) {
	WriteByte(to, SVC_UPDATEUSERINFO);
	WriteByte(to, client->fb.score_pos);
	WriteLong(to, 0);
	WriteByte(to, 92);
	WriteByte(to, _n);
	WriteByte(to, _a);
	WriteByte(to, _m);
	WriteByte(to, _e);
	WriteByte(to, 92);
	WriteByte(to, 0);
}

void SetColorName(float to, gedict_t* client) {
	char* s1;
	WriteByte(to, SVC_UPDATEPING);
	WriteByte(to, client->fb.score_pos);
	WriteShort(to, 999);
	WriteByte(to, SVC_UPDATEPACKETLOSS);
	WriteByte(to, client->fb.score_pos);
	WriteByte(to, (30 + (random() * 20)));
	WriteByte(to, SVC_UPDATETIME);
	WriteByte(to, client->fb.score_pos);
	WriteLong(to, g_globalvars.time);
	clientshirt = floor(client->fb.color_ / 16);
	clientpants = client->fb.color_ - (clientshirt * 16);
	WriteByte(to, SVC_UPDATEUSERINFO);
	WriteByte(to, client->fb.score_pos);
	WriteLong(to, 0);
	WriteByte(to, 92);
	WriteByte(to, _b);
	WriteByte(to, _o);
	WriteByte(to, _t);
	WriteByte(to, _t);
	WriteByte(to, _o);
	WriteByte(to, _m);
	WriteByte(to, _c);
	WriteByte(to, _o);
	WriteByte(to, _l);
	WriteByte(to, _o);
	WriteByte(to, _r);
	WriteByte(to, 92);
	if (clientpants > 9) {
		WriteByte(to, 49);
		WriteByte(to, 38 + clientpants);
	}
	else  {
		WriteByte(to, 48 + clientpants);
	}
	WriteByte(to, 92);
	WriteByte(to, _t);
	WriteByte(to, _o);
	WriteByte(to, _p);
	WriteByte(to, _c);
	WriteByte(to, _o);
	WriteByte(to, _l);
	WriteByte(to, _o);
	WriteByte(to, _r);
	WriteByte(to, 92);
	if (clientshirt > 9) {
		WriteByte(to, 49);
		WriteByte(to, 38 + clientshirt);
	}
	else  {
		WriteByte(to, 48 + clientshirt);
	}
	WriteByte(to, 92);
	WriteByte(to, _n);
	WriteByte(to, _a);
	WriteByte(to, _m);
	WriteByte(to, _e);
	WriteByte(to, 92);
	WriteString(to, client->s.v.netname);
	if (teamplay) {
		WriteByte(to, SVC_SETINFO);
		WriteByte(to, client->fb.score_pos);
		WriteString(to, "team");
		s1 = GetTeamName(client->s.v.team);
		WriteString(to, s1);
	}
}

void SetScoreboard() {
	g_globalvars.msg_entity = NUM_FOR_EDICT(self);
	msg_level = PRINT_HIGH;
	score_count = 0;
	test_enemy = &g_edicts[other->s.v.enemy];
	while (score_count < SPAWN_SIZE) {
		if (test_enemy->fb.frogbot) {
			WriteByte(MSG_ONE, SVC_UPDATEPING);
			WriteByte(MSG_ONE, test_enemy->fb.score_pos);
			WriteShort(MSG_ONE, (300 + (random() * 40)));
			WriteByte(MSG_ONE, SVC_UPDATEPACKETLOSS);
			WriteByte(MSG_ONE, test_enemy->fb.score_pos);
			WriteByte(MSG_ONE, (30 + (random() * 20)));
			WriteByte(MSG_ONE, SVC_UPDATETIME);
			WriteByte(MSG_ONE, test_enemy->fb.score_pos);
			WriteLong(MSG_ONE, g_globalvars.time);
			WriteByte(MSG_ONE, MSG_UPDATEFRAGS);
			WriteByte(MSG_ONE, test_enemy->fb.score_pos);
			WriteShort(MSG_ONE, test_enemy->s.v.frags);
			SetColorName(MSG_ONE, test_enemy);
		}
		test_enemy = nextent(test_enemy);
		score_count = score_count + 1;
	}
	other->s.v.enemy = NUM_FOR_EDICT(test_enemy);
}

void SetPlayerInfo() {
	WriteByte(MSG_BROADCAST, SVC_PLAYERINFO);
	WriteByte(MSG_BROADCAST, self->fb.score_pos);
	if (self->s.v.modelindex == modelindex_player) {
		if (self->s.v.effects) {
			WriteShort(MSG_BROADCAST, 4254);
		}
		else  {
			WriteShort(MSG_BROADCAST, 4126);
		}
	}
	else  {
		if (self->s.v.effects) {
			WriteShort(MSG_BROADCAST, 4286);
		}
		else  {
			WriteShort(MSG_BROADCAST, 4158);
		}
	}
	WriteCoord(MSG_BROADCAST, self->s.v.origin[0]);
	WriteCoord(MSG_BROADCAST, self->s.v.origin[1]);
	WriteCoord(MSG_BROADCAST, self->s.v.origin[2]);
	WriteByte(MSG_BROADCAST, self->s.v.frame);
	WriteByte(MSG_BROADCAST, 129);
	WriteShort(MSG_BROADCAST, self->s.v.v_angle[0] * 182.0444444);
	WriteShort(MSG_BROADCAST, self->s.v.v_angle[1] * 182.0444444);
	WriteByte(MSG_BROADCAST, real_frametime);
	WriteShort(MSG_BROADCAST, self->s.v.velocity[0]);
	WriteShort(MSG_BROADCAST, self->s.v.velocity[1]);
	WriteShort(MSG_BROADCAST, self->s.v.velocity[2]);
	if (self->s.v.modelindex != modelindex_player) {
		WriteByte(MSG_BROADCAST, self->s.v.modelindex);
	}
	if (self->s.v.effects) {
		WriteByte(MSG_BROADCAST, self->s.v.effects);
	}
}

