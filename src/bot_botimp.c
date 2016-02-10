// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void SetSkill();
void LoadTemp1();
void localsave(float frogbot_pos, float index_, float value);
void AddBot();

void SetAttribs() {
	float smartness;
	smartness = 10;
	skill_ = self->fb.bot_skill;
	if (game_show_rules) {
		bprint_fb(2, "skill �");
		bprint_g(2, skill_);
		bprint_fb(2, "�\\");
	}
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
	if (sv_maxspeed >= 346.666666) {
		if (sv_maxspeed > 400) {
			cvar_set("sv_maxspeed", "400");
			sv_maxspeed = 400;
		}
		sv_maxstrafespeed = 346.666666;
	}
	else  {
		if (sv_maxspeed < 320) {
			cvar_set("sv_maxspeed", "320");
			sv_maxspeed = 320;
		}
		sv_maxstrafespeed = sv_maxspeed;
	}
	sv_maxwaterspeed = sv_maxspeed * 0.7;
	half_sv_maxspeed = sv_maxspeed * 0.5;
	inv_sv_maxspeed = 1 / sv_maxspeed;
}

void SetGame() {
	game_disable_powerups = (!(gamemode & GAME_ENABLE_POWERUPS));
	game_enable_runes = gamemode & GAME_ENABLE_RUNES;
	game_not_rune_rj = (!(gamemode & GAME_RUNE_RJ));
	game_rl_pref = gamemode & GAME_RL_PREF;
	game_lg_pref = gamemode & GAME_LG_PREF;
	game_not_match = (!(gamemode & GAME_MATCH));
	game_lava_cheat = gamemode & GAME_LAVA_CHEAT;
	game_show_rules = (!(gamemode & GAME_HIDE_RULES));
	game_disable_autoreport = (!(gamemode & GAME_ENABLE_AUTOREPORT));
	game_disable_dropweap = (!(gamemode & GAME_ENABLE_DROPWEAP));
	game_disable_autosteams = (!(gamemode & GAME_ENABLE_AUTOSTEAMS));
	game_disable_botchat = (!(gamemode & GAME_ENABLE_BOTCHAT));
	game_qizmo = false;
	//game_qizmo = (stof(infokey(world, "proxy")));
	//a_gamemode();
}

void InitParameters() {
	float ds;
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

	infokey(world, "temp1", buffer, sizeof(buffer));
	temp1 = atoi(buffer);

	infokey(world, "humanDamage", buffer, sizeof(buffer));
	humanDamageMultiplier = atof(buffer);
	if (humanDamageMultiplier) {
		humanDamageMultiplier = humanDamageMultiplier / 100;
		humanDamageMultiplier = humanDamageMultiplier - 1;
		if (humanDamageMultiplier <= -1 || humanDamageMultiplier >= 4) {
			humanDamageMultiplier = 0;
		}
	}
	else  {
		humanDamageMultiplier = 0;
	}
	lg_mode = atof(infokey(world, "lgmode", buffer, sizeof(buffer)));
	povTraining = atof(infokey(world, "povdmm4tmode", buffer, sizeof(buffer)));
	if (strneq(g_globalvars.mapname, "povdmm4")) {
		povTraining = 0;
	}
	deathmatch = cvar("deathmatch");
	if (maxplayers > 24) {
		maxplayers = 24;
	}
	//nextmap = g_globalvars.mapname;
	pre_game = TRUE;
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
	if (!temp1) {
		ds = atof(infokey(world, "d_skill", buffer, sizeof(buffer)));
		if (ds) {
			skill = ds;
		}
		else  {
			skill = 10;
		}
		localcmd("\\");
		localsave(0, 1, 0);
		localsave(1, 1, 1);
	}
	else  {
		LoadTemp1();
	}
	SetSkill();
	sv_accelerate = cvar("sv_accelerate");
	sv_friction = cvar("sv_friction");
	numberofbots = -1;
	if ((deathmatch < 1) || (deathmatch > MAX_DEATHMATCH)) {
		deathmatch = 1;
		cvar_set("deathmatch", "1");
	}
	next_deathmatch = deathmatch;
	if (game_not_match) {
		GameSettings();
		max_dead_time = 1000000;
	}
	else  {
		max_dead_time = 5;
	}
	if (deathmatch <= 3) {
		available_weapons = IT_AXE_SHOTGUN;
	}
	else  {
		if (deathmatch == 4) {
			available_weapons = IT_ALL_BUT_GRENADE;
		}
		else  {
			available_weapons = IT_ALL;
		}
	}
	if (deathmatch != 4) {
		quad_factor = 4;
	}
	else  {
		quad_factor = 8;
	}
	leave = (deathmatch != 1);

	CalculatePhysicsVariables();
}

void SaveTemp1() {
	localcmd("localinfo temp1 %f\n", skill+1);
}

void SetSkill() {
	bprint_fb(2, "\\skill\\ changed to \\");
	bprint_ftos(2, skill);
	bprint_fb(2, "\\\\");
	first_ent->fb.bot_skill = skill;
	SaveTemp1();
}

void LoadTemp1() {
	skill = temp1 - 1;
}

void localsave(float frogbot_pos, float index_, float value) {
	localcmd("localinfo %f %f\n", frogbot_pos * 2 + index_, value);
}

void localload(float frogbot_pos) {
	char buffer[1024] = { 0 };
	
	str = va("%f", frogbot_pos * 2 + frogbot_load_stage);
	registered = atof(infokey(world, str, buffer, sizeof(buffer)));
}

void SaveFrogbot() {
	localsave(numberofbots, 1, 2 + self->fb.color_ + self->fb.teamcolor * 256 + self->fb.bot_skill * 65536);
	localsave(numberofbots, 2, self->fb.botnumber);
	localsave(numberofbots + 1, 1, 1);
}

void LoadFrogbot() {
	self = postphysics;
	if (frogbot_load_stage) {
		if (numberofbots == -1) {
			first_ent->fb.admin_code = self->fb.admin_code = registered;
			numberofbots = 0;
			if (invalid_map) {
				load_frogbots = FALSE;
				return;
			}
		}
		else  {
			if (frogbot_load_stage == 1) {
				if (registered == 1) {
					load_frogbots = FALSE;
					return;
				}
				registered = registered - 2;
				self->fb.color_ = (int)registered & 255;
				self->fb.teamcolor = (int)floor(registered / 256) & 255;
				self->fb.bot_skill = (int)floor(registered / 65536) & 255;
			}
			else if (frogbot_load_stage == 2) {
				self->fb.botnumber = registered;
			}
			self->fb.old_bot = TRUE;
			if (frogbot_load_stage <= NUMBER_LOAD_STAGE) {
				frogbot_load_stage = frogbot_load_stage + 1;
				localload(numberofbots + 1);
				return;
			}
			AddBot();
		}
	}
	frogbot_load_stage = 1;
	localload(numberofbots + 1);
}

void CheckParameters() {
	if (!game_not_match || game_arena) {
		real_time = check_parm_time - start_time;
	}
	if ((!game_not_match) && start_time) {
		//DoCountDown();
	}
	if (timelimit) {
		if (!game_not_match) {
			if (!pre_game) {
				if (real_time >= timelimit_user * 60) {
					NextLevel();
				}
			}
		}
		else  {
			if (real_time >= timelimit * 60) {
				NextLevel();
			}
		}
	}
	check_parm_time = floor(g_globalvars.time) + 1;
	if (sv_accelerate != cvar("sv_accelerate")) {
		sv_accelerate = cvar("sv_accelerate");
		if (sv_accelerate < 10) {
			sv_accelerate = 10;
		}
		cvar_fset("sv_accelerate", sv_accelerate);
	}
	if (next_deathmatch != cvar("deathmatch")) {
		next_deathmatch = cvar("deathmatch");
		bprint_fb(2, "deathmatch changed to ");
		bprint_ftos(2, next_deathmatch);
		bprint_fb(2, "\\(enabled after restart)\\");
	}
	if (next_teamplay != cvar("teamplay")) {
		next_teamplay = cvar("teamplay");
		bprint_fb(2, "teamplay changed to ");
		bprint_ftos(2, next_teamplay);
		bprint_fb(2, "\\(enabled after restart)\\");
	}
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
	fraglimit = cvar("fraglimit");
	if ((cvar("sv_gravity") != 800) && (strneq(g_globalvars.mapname, "e1m8"))) {
		cvar_set("sv_gravity", "800");
	}
	if ((cvar("sv_gravity") != 100) && (streq(g_globalvars.mapname, "e1m8"))) {
		cvar_set("sv_gravity", "100");
	}
	if (cvar("sv_mintic") != 0) {
		cvar_set("sv_mintic", "0");
	}
	if (sv_maxspeed != cvar("sv_maxspeed")) {
		NextLevel();
	}
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

float BeenSpawned(gedict_t* client, float value) {
	if (value < 48) {
		spawnbit0_ = 1;
		spawnbit1_ = 0;
		while (value > 0) {
			spawnbit0_ = spawnbit0_ * 2;
			value = value - 1;
		}
		if (spawnbit0_ >= 16777216) {
			spawnbit1_ = spawnbit0_ / 16777216;
			spawnbit0_ = 0;
		}
		if (client->fb.spawnbit0 & spawnbit0_) {
			return TRUE;
		}
		if (client->fb.spawnbit1 & spawnbit1_) {
			return TRUE;
		}
		client->fb.spawnbit0 = client->fb.spawnbit0 | spawnbit0_;
		client->fb.spawnbit1 = client->fb.spawnbit1 | spawnbit1_;
		client->fb.number_spawnbits = client->fb.number_spawnbits + 1;
	}
	return FALSE;
}

void SpawnHardBot() {
	do
 {
		rnd = random();
		rnd = floor(rnd * NUMBERHARDBOTS);
	} while (BeenSpawned(other, rnd + other->fb.number_bots));
	if (rnd == 0) {
		HCFrogbot(45, 1);
	}
	else if (rnd == 1) {
		HCFrogbot(59, 2);
	}
	else if (rnd == 2) {
		HCFrogbot(19, 3);
	}
	else if (rnd == 3) {
		HCFrogbot(196, 4);
	}
	else if (rnd == 4) {
		HCFrogbot(33, 5);
	}
	else if (rnd == 5) {
		HCFrogbot(203, 6);
	}
	else if (rnd == 6) {
		HCFrogbot(43, 7);
	}
	else if (rnd == 7) {
		HCFrogbot(205, 8);
	}
	else if (rnd == 8) {
		HCFrogbot(60, 9);
	}
	else if (rnd == 9) {
		HCFrogbot(160, 10);
	}
	else if (rnd == 10) {
		HCFrogbot(168, 11);
	}
	else if (rnd == 11) {
		HCFrogbot(212, 12);
	}
	else if (rnd == 12) {
		HCFrogbot(70, 13);
	}
	else if (rnd == 13) {
		HCFrogbot(4, 14);
	}
	else if (rnd == 14) {
		HCFrogbot(75, 15);
	}
	else  {
		HCFrogbot(13, 16);
	}
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
		if (!search_entity->fb.frogbot) {
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

float SpawnBot() {
	do {
		if (self->fb.number_spawnbits >= self->fb.number_bots) {
			return FALSE;
		}
		rnd = random();
		rnd = floor(rnd * self->fb.number_bots);
	} while (BeenSpawned(self, rnd));
	g_globalvars.msg_entity = NUM_FOR_EDICT(self);
	stuffcmd(self, "bot %f\n", rnd + 1);
	return TRUE;
}

float AddRandomBot() {
	if (self->fb.number_bots < 0) {
		if (self->fb.number_bots == -1) {
			self->fb.number_bots = 0 - g_globalvars.time;
		}
		else if (self->fb.number_bots != (0 - g_globalvars.time)) {
			self->fb.number_bots = 0;
		}
		if (self->fb.number_bots) {
			g_globalvars.msg_entity = NUM_FOR_EDICT(self);
			stuffcmd(self, "wait;addbot\n");
			return TRUE;
		}
	}
	return (SpawnBot());
}

void AddBot() {
	float t1,
	      t2,
	      color;
	if (self->fb.player) {
		if (load_frogbots) {
			return;
		}
		if (invalid_map) {
			G_sprint(self, 2, "Bots have not been configured for this map\n");
			impulse_ = 0;
			return;
		}
	}

	//trap_AddBot(name, bottomcolor, topcolor, skin)
	if (markers_loaded) {
		char buffer[1024] = { 0 };

		if ((numberofclients >= atof(infokey(world, "maxclients", buffer, sizeof(buffer)))) || (numberofbots == 16)) {
			G_sprint(self, 2, "Server is full.\n");
		}
		else  {
			if (!self->fb.botnumber) {
				if (AddRandomBot()) {
					return;
				}
			}
			else if (BotExists()) {
				CopyBot(self, world);
				if (AddRandomBot()) {
					return;
				}
			}
			other = self;
			numberofbots = numberofbots + 1;
			localcmd("localinfo scratch2 %f\n", numberofbots);
			self = frogbot_spawned = EntityAt(first_frogbot, maxplayers - numberofbots);
			self->s.v.flags = 0;
			self->fb.frogbot = TRUE;
			self->s.v.classname = "frogbot";
			self->fb.stringname = "gamer";
			self->s.v.waterlevel = 0;
			self->s.v.watertype = 0;
			self->s.v.frags = 0;
			self->s.v.deadflag = 0;
			self->fb.arrow = 0;
			self->fb.button0_ = 0;
			self->fb.jump_flag = 0;
			self->s.v.effects = 0;
			self->fb.color_ = other->fb.color_;
			self->fb.teamcolor = other->fb.teamcolor;
			if (game_arena) {
				other->fb.teamcolor = 0;
			}
			self->fb.bot_skill = skill;
			CopyBot(self, other);
			CopyBot(other, world);
			scoreboardsize = scoreboardsize + 1;
			if (!self->fb.botnumber) {
				do {
					SpawnHardBot();
				} while (BotExists());
			}
			if (other->fb.player) {
				SaveFrogbot();
			}
			if (teamplay && !game_disable_autosteams) {
				t1 = 0;
				t2 = 0;
				search_entity = first_client;
				while (search_entity) {
					if (search_entity != self) {
						if (search_entity->fb.realteam == TEAM_COLOR1) {
							t1 = t1 + 1;
						}
						else if (search_entity->fb.realteam == TEAM_COLOR2) {
							t2 = t2 + 1;
						}
					}
					search_entity = search_entity->fb.next;
				}
				if ((t1 < t2) || ((t1 == t2) && (random() < 0.5))) {
					color = TEAM_COLOR1;
				}
				else  {
					color = TEAM_COLOR2;
				}
				self->fb.teamcolor = (color - 1) * 17;
				self->fb.color_ = self->fb.teamcolor;
				self->s.v.team = color;
			}
			else  {
				if (teamplay) {
					self->fb.color_ = self->fb.teamcolor;
				}
				self->s.v.team = (self->fb.color_ & 15) + 1;
			}
			// UpdateFrags(self);
			if (teamplay) {
				self->s.v.netname = SetTeamNetName();
			}
			else  {
				self->s.v.netname = SetNetName();
			}
			//SetColorName(MSG_ALL, self);	// network manipulation not required...
			ClientConnect_apply();
			SetAttribs();
			((fb_void_func_t)g_globalvars.PutClientInServer)();
			self->fb.old_bot = other->fb.old_bot;
			VectorCopy(self->s.v.origin, self->s.v.oldorigin);
			if (!game_not_match && !game_arena) {
				self->fb.ready = 0;
				//PlayerReady();
			}
			self = other;
		}
	}
}

void AddBots() {
	min_second = maxplayers - scoreboardsize + 1;
	if (impulse_ > min_second) {
		impulse_ = min_second;
	}
	while (impulse_ > 0) {
		impulse_ = impulse_ - 1;
		AddBot();
	}
}

void ClearSpawnBits() {
	test_enemy = ez_find(world, "player");
	while (test_enemy) {
		test_enemy->fb.spawnbit0 = test_enemy->fb.spawnbit1 = test_enemy->fb.number_spawnbits = 0;
		test_enemy = ez_find(test_enemy, "player");
	}
}

float RemoveBot(float remove_name) {
	if (numberofbots) {
		if (load_frogbots) {
			return 0;
		}
		if (g_globalvars.time >= frogbot_removetime) {
			frogbot_removetime = g_globalvars.time + 0.1;
			ClearSpawnBits();
			removebot_self = self;
			self = frogbot_spawned;
			scoreboardsize = scoreboardsize - 1;
			if (self->fb.client_) {
				((fb_void_func_t) g_globalvars.ClientDisconnect)();
			}
			self->s.v.nextthink = 0;
			self->s.v.netname = "";
			if (remove_name) {
				//ClearName(MSG_ALL, self);
			}
			localsave(numberofbots, 1, 1);
			numberofbots = numberofbots - 1;
			localcmd("localinfo scratch2 %f\n", numberofbots);
			frogbot_spawned = nextent(frogbot_spawned);
			self = removebot_self;
		}
	}
	return numberofbots;
}

void AutoRemoveBot() {
	if (RemoveBot(TRUE)) {
		self->s.v.nextthink = g_globalvars.time + 0.1;
	}
	else  {
		remove_self();
	}
}

void RemoveAllBots() {
	test_enemy = spawn();
	test_enemy->s.v.think = (func_t) AutoRemoveBot;
	test_enemy->s.v.nextthink = 0.001;
}

void ToggleFlash() {
	if (self->fb.preferences & PREF_FLASH) {
		G_sprint(self, 2, "flashs off\n");
		self->fb.preferences = self->fb.preferences - PREF_FLASH;
	}
	else  {
		G_sprint(self, 2, "flashs on\n");
		self->fb.preferences = self->fb.preferences | PREF_FLASH;
	}
}

void ToggleFramerate() {
	if (self->fb.print_framerate) {
		self->fb.print_framerate = FALSE;
	}
	else  {
		self->fb.print_framerate = TRUE;
	}
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
	new_gamemode = cvar("samelevel");
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
	cvar_fset("samelevel", new_gamemode);
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
	print_boolean(GAME_LAVA_CHEAT, "���������... ");
	print_boolean(GAME_RL_PREF, "�������..... ");
	print_boolean(GAME_LG_PREF, "�������..... ");
	print_boolean(GAME_HIDE_RULES, "����........ ");
	print_boolean(GAME_ENABLE_AUTOREPORT, "����������.. ");
	print_boolean(GAME_ENABLE_DROPWEAP, "��������.... ");
	print_boolean(GAME_ENABLE_AUTOSTEAMS, "����������.. ");
	print_boolean(GAME_ENABLE_BOTCHAT, "�������..... ");
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
/*
void AddBotError() {
	sprint_fb(self, 2, "There is no support of external bots!\n");
	self->s.v.spawnflags = self->s.v.impulse = self->fb.lines = 0;
	self->fb.input_time = g_globalvars.time + 2;
	self->fb.spawnbit0 = self->fb.spawnbit1 = self->fb.number_spawnbits = 0;
	CopyBot(self, world);
}
*/