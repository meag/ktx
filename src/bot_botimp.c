// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void SetSkill();
void LoadTemp1();
void AddBot();

void SetAttribs() {
	float smartness;
	int skill_ = self->fb.bot_skill;

	smartness = 10;
	bprint_fb(2, "skill &cf00");
	bprint_g(2, skill_);
	bprint_fb(2, "&r\n");
	if (skill_ > 10) {
		self->fb.fast_aim = (skill_ - 10) * 0.1;
		skill_ = 10;
	}
	else  {
		self->fb.fast_aim = 0;
	}
	self->fb.firing_reflex = 0.5 - (skill_ * 0.04);
	self->fb.accuracy = 45 - (skill_ * 2.25);
	self->fb.stop_turn_speed = 135 + (smartness * 40.5);
	self->fb.dodge_amount = smartness * 0.1;
	self->fb.look_anywhere = smartness * 0.1;
	self->fb.lookahead_time = 5 + (smartness * 2.5);
	self->fb.prediction_error = 1 - (smartness * 0.1);
}

void CalculatePhysicsVariables() {
	sv_maxspeed = cvar("sv_maxspeed");
	sv_maxstrafespeed = sv_maxspeed;
	sv_maxwaterspeed = sv_maxspeed * 0.7;
	half_sv_maxspeed = sv_maxspeed * 0.5;
	inv_sv_maxspeed = 1 / sv_maxspeed;
}

void SetGame() {
	//game_rl_pref = gamemode & GAME_RL_PREF;
	//game_lg_pref = gamemode & GAME_LG_PREF;
	game_disable_autosteams = (!(gamemode & GAME_ENABLE_AUTOSTEAMS));
}

void InitParameters() {
	char buffer[1024] = { 0 };

	first_ent = nextent(world);
	test_enemy = first_ent;
	while (test_enemy) {
		maxplayers = maxplayers + 1;
		test_enemy = nextent(test_enemy);
	}
	dropper = spawn();
	setsize(dropper, PASSVEC3( VEC_HULL_MIN ), PASSVEC3( VEC_HULL_MAX ));
	dropper->fb.desire = goal_NULL;
	dropper->fb.virtual_goal = dropper;
	self = dropper;
	NewItems();
	InitBodyQue();

	deathmatch = cvar("deathmatch");
	if (maxplayers > 24) {
		maxplayers = 24;
	}
	//nextmap = g_globalvars.mapname;
	gamemode = cvar("samelevel");
	SetGame();
	teamplay = cvar("teamplay");
	//initialize(); // TODO: RA initialization?
	if (teamplay == 1) {
		healthplay = TEAM_TOTAL_HEALTH_PROTECT;
	}
	else if (teamplay == 2) {
		healthplay = TEAM_FRAG_PENALTY;
	}
	else if (teamplay == 3) {
		healthplay = TEAM_HEALTH_PROTECT;
	}
	else if (teamplay == 4) {
		healthplay = TEAM_HEALTH_PROTECT;
		armorplay = TRUE;
	}
	else if (teamplay == 5) {
		healthplay = TEAM_TOTAL_HEALTH_PROTECT;
		armorplay = TRUE;
	}
	else  {
		teamplay = 0;
		cvar_set("teamplay", "0");
	}

	next_teamplay = teamplay;
	sv_accelerate = cvar("sv_accelerate");
	sv_friction = cvar("sv_friction");
	numberofbots = -1;
	if ((deathmatch < 1) || (deathmatch > MAX_DEATHMATCH)) {
		deathmatch = 1;
		cvar_set("deathmatch", "1");
	}
	next_deathmatch = deathmatch;
	use_ammo = (deathmatch != 4);

	available_weapons = deathmatch <= 3 ? IT_AXE_SHOTGUN : (deathmatch == 4 ? IT_ALL_BUT_GRENADE : IT_ALL);
	leave = (deathmatch != 1);

	CalculatePhysicsVariables();
}

void SetSkill() {
	bprint_fb(2, va("%s changed to %s\n", "botskill", dig3(cvar("k_bot_skill"))));
}

void localload(float frogbot_pos) {
	char buffer[1024] = { 0 };
	
	str = va("%f", frogbot_pos * 2 + frogbot_load_stage);
	//registered = atof(infokey(world, str, buffer, sizeof(buffer)));
}

void CheckParameters() {
	check_parm_time = floor(g_globalvars.time) + 1;
	/*
	if (sv_accelerate > 10) {
		sv_maxfriction = 10;
	}
	else  {
		sv_maxfriction = sv_accelerate;
	}
	if (sv_friction > sv_maxfriction) {
		cvar_fset("sv_friction", sv_maxfriction);
	}
	if (sv_friction != cvar("sv_friction")) {
		sv_friction = cvar("sv_friction");
		if (sv_friction < 0) {
			sv_friction = 0;
		}
		cvar_fset("sv_friction", sv_friction);
	}
	*/
}

float BotExists() {
	bot = frogbot_spawned;
	while (bot != postphysics) {
		if (bot != self) {
			if (bot->fb.botnumber == self->fb.botnumber) {
				return TRUE;
			}
		}
		bot = nextent(bot);
	}
	return FALSE;
}

void HCFrogbot(float color, float numb) {
	self->fb.color_ = color;
	self->fb.botnumber = numb;
}

char* SetTeamNetName() {
	float playersOnThisTeam,
	      playersOnOtherTeams,
	      frogbotsOnThisTeam;
	char* attemptedName;
	playersOnThisTeam = 0;
	frogbotsOnThisTeam = 0;
	playersOnOtherTeams = 0;
	search_entity = first_client;
	while (search_entity) {
		if (!search_entity->isBot) {
			if (search_entity->s.v.team == self->s.v.team) {
				playersOnThisTeam = playersOnThisTeam + 1;
			}
			else  {
				playersOnOtherTeams = playersOnOtherTeams + 1;
			}
		}
		else if (search_entity->s.v.team == self->s.v.team) {
			frogbotsOnThisTeam = frogbotsOnThisTeam + 1;
		}
		search_entity = search_entity->fb.next;
	}
	if (playersOnOtherTeams > 0 && playersOnThisTeam == 0) {
		attemptedName = EnemyTeamName();
	}
	else if (playersOnThisTeam > 0 && playersOnOtherTeams == 0) {
		attemptedName = FriendTeamName();
	}
	else  {
		return SetNetName();
	}
	if (attemptedName) {
		return attemptedName;
	}
	return SetNetName();
}

char* EnemyTeamName() {
	char* names[] = {
		"Timber",
		"Kane",
		"Rix",
		"Batch",
		"Nikodemus",
		"Paralyzer",
		"Sujoy",
		"Gollum",
		"sCary",
		"Xenon",
		"Thresh",
		"Frick",
		"B2",
		"Reptile",
		"Unholy",
		"Spice"
	};

	return names[(int)bound(0, self->fb.botnumber - 1, sizeof(names) / sizeof(names[0]) - 1)];
}

char* FriendTeamName() {
	char* names[] = {
		"Mr Justice",
		"Paz",
		"Jon",
		"Gaz",
		"Jakey",
		"Tele",
		"Thurg",
		"Kool",
		"Zaphod",
		"Dreamer",
		"Mandrixx",
		"Skill5",
		"Gunner",
		"DanJ",
		"Vid",
		"Soul99"
	};

	return names[(int)bound(0, self->fb.botnumber - 1, sizeof(names) / sizeof(names[0]) - 1)];
}

char* SetNetName() {
	char* names[] = {
		"//boff",
		"//watkins",
		"//kaylee",
		"//inara",
		"//fragGod",
		"//dizzy",
		"//daisy",
		"//chuckie",

		"//grue",
		"//machine",
		"//lassie",
		"//blue",
		"//dave",
		"//pressure",
		"//junked",
		"//overlord"
	};

	return names[(int)bound(0, self->fb.botnumber - 1, sizeof(names) / sizeof(names[0]) - 1)];
}

void ToggleGameMode(int value, char* s) {
	new_gamemode = cvar("samelevel");
	if (new_gamemode & value) {
		new_gamemode = new_gamemode - value;
		bprint_fb(2, s);
		bprint_fb(2, " disabled after restart\\");
	}
	else  {
		new_gamemode = new_gamemode | value;
		bprint_fb(2, s);
		bprint_fb(2, " enabled after restart\\");
	}
	cvar_fset("samelevel", new_gamemode);
}

void ToggleGameModeNow(int value, char* s) {
	int new_gamemode = cvar("k_bots_settings");
	if (gamemode & value) {
		gamemode = gamemode - value;
		new_gamemode = new_gamemode - (new_gamemode & value);
		bprint_fb(2, s);
		bprint_fb(2, " disabled\\");
	}
	else  {
		gamemode = gamemode | value;
		new_gamemode = new_gamemode | value;
		bprint_fb(2, s);
		bprint_fb(2, " enabled\\");
	}
	cvar_fset("k_bots_settings", new_gamemode);
	SetGame();
}

void print_boolean(int value, char* s) {
	G_sprint(self, 2, s);
	if (gamemode & value) {
		G_sprint(self, 2, "enabled\\");
	}
	else  {
		G_sprint(self, 2, "disabled\\");
	}
}

void PrintRules() {
	/*
	sprint_fb(self, 2, "skill....... ");
	sprint_ftos(self, 2, skill);
	sprint_fb(self, 2, "\\deathmatch.. ");
	sprint_ftos(self, 2, deathmatch);
	sprint_fb(self, 2, "\\teamplay.... ");
	sprint_ftos(self, 2, teamplay);
	sprint_fb(self, 2, "\\");
	a_rulesfix();
	print_boolean(GAME_ENABLE_POWERUPS, "��������.... ");
	print_boolean(GAME_ENABLE_RUNES, "����........ ");
	print_boolean(GAME_RUNE_RJ, "�������..... ");
	print_boolean(GAME_MATCH, "�����....... ");
	print_boolean(GAME_RL_PREF, "�������..... ");
	print_boolean(GAME_LG_PREF, "�������..... ");
	print_boolean(GAME_HIDE_RULES, "����........ ");
	print_boolean(GAME_ENABLE_AUTOREPORT, "����������.. ");
	print_boolean(GAME_ENABLE_DROPWEAP, "��������.... ");
	print_boolean(GAME_ENABLE_AUTOSTEAMS, "����������.. ");
	a_boolean();
	sprint_fb(self, 2, "\\");
	if (sv_accelerate != 10) {
		sprint_fb(self, 2, "sv_accelerate is ");
		sprint_ftos(self, 2, sv_accelerate);
		sprint_fb(self, 2, "\\");
	}
	if (sv_friction != 4) {
		sprint_fb(self, 2, "sv_friction is ");
		sprint_ftos(self, 2, sv_friction);
		sprint_fb(self, 2, "\\");
	}*/
}
