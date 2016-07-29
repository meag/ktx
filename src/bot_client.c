// Converted from .qc on 07/02/2016

#include "g_local.h"
#include "fb_globals.h"

void PlayerReady ();
void SetMarker (gedict_t* client, gedict_t* marker);

// Called whenever a player dies
void BotPlayerDeathEvent(gedict_t* self) {
	ResetGoalEntity(self);
	ResetEnemy(self);

	self->fb.last_death = g_globalvars.time;
}

// Was: PutClientInServer_apply()
void BotClientEntersEvent(gedict_t* self, gedict_t* spawn_pos) {
	self->fb.oldwaterlevel = self->fb.oldwatertype = 0;
	self->fb.real_pitch = self->s.v.angles[0];
	self->fb.real_yaw = self->s.v.angles[1];
	self->fb.state = 0;

	SetMarker(self, spawn_pos);

	self->fb.arrow = 0;
	ClearLookObject(self);
	VectorSet(self->fb.oldvelocity, 0, 0, 0);
	self->fb.jumping = false;
	self->fb.goal_refresh_time = 0;
	self->fb.allowedMakeNoise = true;

	UpdateTotalDamage (self);
	self->fb.weapon_refresh_time = 0;
}

// Called by client/PlayerDeathThink
void BotDeathThink(void) {
	self->fb.firing = false;
	self->fb.jumping = (qbool) (g_random() >= 0.5); // 50% chance of respawning every frame
}

void BotSetCommand(gedict_t* self) {
	float msec = min ((g_globalvars.time - self->fb.last_cmd_sent) * 1000, 255);
	float mouse_friction = 5400 * g_globalvars.frametime;

	/*
	float pitchspeed_ = self->fb.pitchspeed + self->fb.pitchaccel * g_globalvars.frametime;
	float yawspeed_ = self->fb.yawspeed + self->fb.yawaccel * g_globalvars.frametime;
	float total_pitchspeed = 0;
	float total_yawspeed = 0;
	float new_pitch = 0;
	float new_yaw = 0;

	if (pitchspeed_ > 0) {
		pitchspeed_ = max(pitchspeed_ - mouse_friction, 0);
	}
	else  {
		pitchspeed_ = min(pitchspeed_ + mouse_friction, 0);
	}
	total_pitchspeed = pitchspeed_ + self->fb.track_pitchspeed;
	total_pitchspeed = bound(-450, total_pitchspeed, 450);
	self->fb.pitchspeed = bound (-450, pitchspeed_, 450);
	self->fb.real_pitch = bound(-71.71875, self->fb.real_pitch + total_pitchspeed * g_globalvars.frametime, 78.75);
	*/
	a_rocketjump(self);

	/*
	if (yawspeed_ > 0) {
		yawspeed_ = max(yawspeed_ - mouse_friction, 0);
	}
	else {
		yawspeed_ = min(yawspeed_ + mouse_friction, 0);
	}
	total_yawspeed = yawspeed_ + self->fb.track_yawspeed;
	if (fabs(total_yawspeed) > self->fb.stop_turn_speed) {
		self->fb.arrow = 0;
		self->fb.arrow_time = g_globalvars.time + g_globalvars.frametime;
		total_yawspeed = bound (-540, total_yawspeed, 540);
	}
	self->fb.yawspeed = bound (-540, yawspeed_, 540);

	self->fb.real_yaw += total_yawspeed * g_globalvars.frametime;
	while (self->fb.real_yaw >= 180) {
		self->fb.real_yaw -= 360;
	}
	while (real_yaw_ < -180) {
		self->fb.real_yaw += 360;
	}

	new_pitch = (pr1_rint(self->fb.real_pitch / 1.40625)) * 1.40625;
	new_yaw = (pr1_rint (real_yaw_ / 1.40625)) * 1.40625;
	if (new_pitch < 0) {
		if (new_pitch < -2.8125) {
			new_pitch = new_pitch + 2.8125;
		}
		else {
			new_pitch = 0;
		}
	}

	G_bprint (2, "Desired[%f %f] vs [%f %f]\n", self->fb.desired_angle[0], self->fb.desired_angle[1], new_pitch * -1, new_yaw);

	self->fb.desired_angle[0] = new_pitch * -1;
	self->fb.desired_angle[1] = new_yaw;
	self->fb.desired_angle[2] = 0;*/

	VectorNormalize (self->fb.dir_move_);
	trap_makevectors (self->fb.desired_angle);

	trap_SetBotCMD (
		NUM_FOR_EDICT (self),
		msec,
		self->fb.desired_angle[0],
		self->fb.desired_angle[1],
		0,
		DotProduct (g_globalvars.v_forward, self->fb.dir_move_) * 800,
		DotProduct (g_globalvars.v_right, self->fb.dir_move_) * 800,
		DotProduct (g_globalvars.v_up, self->fb.dir_move_) * 800,
		(self->fb.firing ? 1 : 0) | (self->fb.jumping ? 2 : 0),
		(self->fb.botchose ? self->fb.next_impulse : 0)
	);
	self->fb.next_impulse = 0;
	self->fb.botchose = false;
	self->fb.last_cmd_sent = g_globalvars.time;
}

static float goal_client(gedict_t* self) {
	if (g_globalvars.time < self->fb.virtual_enemy->invisible_finished) {
		return 0;     // don't chase enemies with eyes
	}
	else if (g_globalvars.time < self->fb.virtual_enemy->invincible_finished) {
		return 0;     // or with pent
	}
	else if (self->fb.look_object && self->s.v.enemy == NUM_FOR_EDICT(self->fb.look_object)) {
		return ((self->fb.total_damage + 100) * self->fb.firepower - self->fb.virtual_enemy->fb.total_damage * self->fb.virtual_enemy->fb.firepower) * 0.01;
	}
	else if (EnemyDefenceless(self)) {
		return ((self->fb.total_damage + 120) * self->fb.firepower - self->fb.virtual_enemy->fb.total_damage * self->fb.virtual_enemy->fb.firepower) * 0.01;
	}
	else {
		return (self->fb.total_damage * self->fb.firepower - self->fb.virtual_enemy->fb.total_damage * self->fb.virtual_enemy->fb.firepower) * 0.01;
	}
}

static float goal_client6(gedict_t* self) {
	if (! k_matchLess && match_in_progress != 2) {
		return 100;
	}

	if (g_globalvars.time < self->fb.virtual_enemy->invisible_finished || g_globalvars.time < self->fb.virtual_enemy->invincible_finished) {
		return 0;     // or with pent
	}

	return (300 - min(self->fb.virtual_enemy->fb.total_damage, 300));
}

// client.qc
// This is called whenever a client connects (not just bots)
// TODO: any preferences stored against the specific bot to be restored here?
void BotClientConnectedEvent(gedict_t* self) {
	self->fb.desire = (deathmatch <= 3 ? goal_client : goal_client6);
	self->fb.T = BOTFLAG_UNREACHABLE;
	self->fb.skill.skill_level = g_globalvars.parm3;
	self->fb.skill.lookahead_time = 30;
	self->fb.skill.prediction_error = 0;
	self->fb.ammo_used = DelayUpdateWeapons;

	if (self->isBot) {
		PlayerReady ();
	}
}

// client.qc
void BotOutOfWater(gedict_t* self) {
	if (self->s.v.waterlevel == 2) {
		vec3_t start;
		vec3_t end;

		// Tread water
		self->fb.tread_water_count = self->fb.tread_water_count + 1;
		if (self->fb.tread_water_count > 75) {
			self->fb.old_linked_marker = NULL;
			SetLinkedMarker(self, LocateMarker(self->s.v.origin));
			self->fb.path_state = 0;
			self->fb.linked_marker_time = g_globalvars.time + 5;
		}

		trap_makevectors(self->s.v.v_angle);
		VectorCopy(self->s.v.origin, start);
		start[2] += 8;
		g_globalvars.v_forward[2] = 0;
		VectorNormalize (g_globalvars.v_forward);
		VectorScale (g_globalvars.v_forward, 24, g_globalvars.v_forward);
		VectorAdd (start, g_globalvars.v_forward, end);
		trap_traceline(PASSVEC3(start), PASSVEC3(end), true, NUM_FOR_EDICT(self));
		if (g_globalvars.trace_fraction < 1) {
			start[2] = self->s.v.origin[2] + self->s.v.maxs[2];
			VectorAdd (start, g_globalvars.v_forward, end);
			VectorScale (g_globalvars.trace_plane_normal, -50, self->s.v.movedir);
			traceline(start[0], start[1], start[2], end[0], end[1], end[2], true, self);
			if (g_globalvars.trace_fraction == 1) {
				self->s.v.flags = ((int)self->s.v.flags | FL_WATERJUMP) & ~FL_JUMPRELEASED;
				// FIXME
				//self->s.v.velocity[2] = 225;
				self->fb.dir_move_[2] = -225;
				self->s.v.teleport_time = g_globalvars.time + 2;
			}
		}
	}
}

// Called for every player, if bots are enabled
void BotPreThink(gedict_t* self) {
	if (self->isBot) {
		self->fb.firing = self->s.v.button0;
		self->fb.jumping = self->s.v.button2;
	}

	self->fb.touch_distance = 1000000;
}

// This is stored in statistics files
int BotVersionNumber (void)
{
	return 1;
}

qbool FrogbotOptionEnabled (int option)
{
	return ((int)cvar ("k_fb_options")) & option;
}
