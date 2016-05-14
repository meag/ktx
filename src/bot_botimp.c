// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void SetSkill();
void LoadTemp1();
void AddBot();

void SetAttribs(gedict_t* self) {
	float smartness;
	int skill_ = self->fb.bot_skill;

	smartness = 10;
	G_bprint (2, "skill &cf00%d&r\n", self->fb.bot_skill);
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

void InitParameters() {
	char buffer[1024] = { 0 };

	first_ent = nextent(world);

	// FIXME: ?
	for (test_enemy = world; test_enemy = nextent (test_enemy); ) {
		maxplayers = maxplayers + 1;
	}
	if (maxplayers > 24) {
		maxplayers = 24;
	}

	dropper = spawn();
	setsize(dropper, PASSVEC3( VEC_HULL_MIN ), PASSVEC3( VEC_HULL_MAX ));
	dropper->fb.desire = goal_NULL;
	dropper->fb.virtual_goal = dropper;
	self = dropper;
	NewItems();
	InitBodyQue();

	gamemode = cvar(FB_CVAR_GAMEMODE);			// FIXME
	game_disable_autosteams = (!(gamemode & GAME_ENABLE_AUTOSTEAMS));

	//initialize(); // TODO: RA initialization?

	if (teamplay == 4) {
		armorplay = TRUE;
	}
	else if (teamplay == 5) {
		armorplay = TRUE;
	}
	else  {
		teamplay = 0;
		cvar_set("teamplay", "0");
	}

	sv_accelerate = cvar("sv_accelerate");
	sv_friction = cvar("sv_friction");
	use_ammo = (deathmatch != 4);

	available_weapons = deathmatch <= 3 ? IT_AXE_SHOTGUN : (deathmatch == 4 ? IT_ALL_BUT_GRENADE : IT_ALL);
	leave = (deathmatch != 1);

	CalculatePhysicsVariables();
}

void SetSkill() {
	bprint_fb(2, va("%s changed to %s\n", redtext("botskill"), dig3(cvar(FB_CVAR_SKILL))));
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

char* SetTeamNetName() {
	float playersOnThisTeam,
	      playersOnOtherTeams,
	      frogbotsOnThisTeam;
	char* attemptedName;
	gedict_t* search_entity;

	playersOnThisTeam = 0;
	frogbotsOnThisTeam = 0;
	playersOnOtherTeams = 0;

	for (search_entity = world; search_entity = find_plr (search_entity); ) {
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
		"::Timber",
		"::Kane",
		"::Rix",
		"::Batch",
		"::Nikodemus",
		"::Paralyzer",
		"::Sujoy",
		"::Gollum",
		"::sCary",
		"::Xenon",
		"::Thresh",
		"::Frick",
		"::B2",
		"::Reptile",
		"::Unholy",
		"::Spice"
	};

	return names[(int)bound(0, self->fb.botnumber - 1, sizeof(names) / sizeof(names[0]) - 1)];
}

char* FriendTeamName() {
	char* names[] = {
		"Mr Justice",
		"Parrais",
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

// FIXME
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
		"//machina",
		"//gudgie",
		"//scoosh",
		"//frazzle",
		"//pressure",
		"//junked",
		"//overload"
	};

	return names[(int)bound(0, self->fb.botnumber - 1, sizeof(names) / sizeof(names[0]) - 1)];
}

void ToggleGameMode(int value, char* s) {
	new_gamemode = cvar(FB_CVAR_GAMEMODE);
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
	cvar_fset(FB_CVAR_GAMEMODE, new_gamemode);
}

void ToggleGameModeNow(int value, char* s) {
	int new_gamemode = cvar(FB_CVAR_GAMEMODE);
	if (gamemode & value) {
		gamemode &= ~value;
		new_gamemode &= ~value;
		bprint_fb(2, s);
		bprint_fb(2, " disabled%s\\");
	}
	else  {
		gamemode |= value;
		new_gamemode |= value;
		bprint_fb(2, s);
		bprint_fb(2, " enabled\\");
	}
	cvar_fset(FB_CVAR_GAMEMODE, new_gamemode);
	//SetGame();
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
