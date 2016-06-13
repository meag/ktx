// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void SetSkill();

// TODO: Exchange standard attributes for different bot characters/profiles
void SetAttribs(gedict_t* self) {
	float smartness = 10;
	int skill_ = self->fb.skill.skill_level;
	G_bprint (2, "skill &cf00%d&r\n", self->fb.skill.skill_level);
	if (skill_ > 10) {
		self->fb.skill.fast_aim = (skill_ - 10) * 0.1;
		skill_ = 10;
	}
	else  {
		self->fb.skill.fast_aim = 0;
	}
	self->fb.skill.firing_reflex = 0.5 - (skill_ * 0.04);
	self->fb.skill.accuracy = 45 - (skill_ * 2.25);
	self->fb.skill.stop_turn_speed = 135 + (smartness * 40.5);
	self->fb.skill.dodge_amount = smartness * 0.1;
	self->fb.skill.look_anywhere = smartness * 0.1;
	self->fb.skill.lookahead_time = 5 + (smartness * 2.5);
	self->fb.skill.prediction_error = 1 - (smartness * 0.1);
}

void CalculatePhysicsVariables() {
	sv_accelerate = cvar("sv_accelerate");
	sv_maxspeed = cvar("sv_maxspeed");
	sv_maxstrafespeed = sv_maxspeed;
	sv_maxwaterspeed = sv_maxspeed * 0.7;
	half_sv_maxspeed = sv_maxspeed * 0.5;
	inv_sv_maxspeed = 1 / sv_maxspeed;
}

void InitParameters() {
	char buffer[1024] = { 0 };

	dropper = spawn();
	setsize(dropper, PASSVEC3( VEC_HULL_MIN ), PASSVEC3( VEC_HULL_MAX ));
	dropper->fb.desire = goal_NULL;
	dropper->fb.virtual_goal = dropper;
	
	self = dropper;
	NewItems();
	InitBodyQue();

	//initialize(); // TODO: RA initialization?

	sv_accelerate = cvar("sv_accelerate");

	CalculatePhysicsVariables();
}

void SetSkill() {
	bprint_fb(2, va("%s changed to %s\n", redtext("botskill"), dig3(cvar(FB_CVAR_SKILL))));
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
