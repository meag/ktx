// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void POVDMM4LookDoor(gedict_t* self);
void AMPHI2BotInLava(void);
qbool DM6FireAtDoor (gedict_t* self);

static gedict_t* dodge_missile = NULL;

// FIXME: Move to bot.skill
#define CHANCE_EVADE_DUEL 0.08
#define CHANCE_EVADE_NONDUEL 0.1

static void BotSetDesiredAngles (gedict_t* self, vec3_t rel_pos)
{
	vectoangles(rel_pos, self->fb.desired_angle);
	if (self->fb.desired_angle[0] > 180) {
		self->fb.desired_angle[0] = 360 - self->fb.desired_angle[0];
	}
	else {
		self->fb.desired_angle[0] = 0 - self->fb.desired_angle[0];
	}

	// FIXME: why? :(
	/*
	self->fb.desired_angle[0] = (pr1_rint(self->fb.desired_angle[0] / 1.40625));
	self->fb.desired_angle[1] = (pr1_rint(self->fb.desired_angle[1] / 1.40625));
	VectorScale(self->fb.desired_angle, 1.40625, self->fb.desired_angle);
	*/

	if (self->fb.state & HURT_SELF) {
		self->fb.desired_angle[0] = 180;
	}
}

static void BotSetMouseParameters (gedict_t* self)
{
	return;

	VectorSubtract(self->fb.desired_angle, self->s.v.v_angle, self->fb.angle_error);
	self->fb.angle_error[0] -= (1 - self->fb.skill.fast_aim) * (self->fb.pitchspeed * self->fb.skill.firing_reflex);
	self->fb.angle_error[1] -= (1 - self->fb.skill.fast_aim) * (self->fb.yawspeed * self->fb.skill.firing_reflex);
	if (self->fb.angle_error[1] >= 180) {
		self->fb.angle_error[1] = self->fb.angle_error[1] - 360;
	}
	else if (self->fb.angle_error[1] < -180) {
		self->fb.angle_error[1] = self->fb.angle_error[1] + 360;
	}

	self->fb.track_pitchspeed += self->fb.skill.fast_aim * self->fb.angle_error[0] / self->fb.skill.firing_reflex;
	self->fb.track_yawspeed += self->fb.skill.fast_aim * self->fb.angle_error[1] / self->fb.skill.firing_reflex;
	self->fb.pitchaccel = (1 - self->fb.skill.fast_aim) * self->fb.angle_error[0] / self->fb.skill.firing_reflex;
	self->fb.yawaccel = (1 - self->fb.skill.fast_aim) * self->fb.angle_error[1] / self->fb.skill.firing_reflex;

	if (self->fb.pitchaccel > 0) {
		self->fb.pitchaccel = self->fb.pitchaccel + 5400;
	}
	else if (self->fb.pitchaccel < 0) {
		self->fb.pitchaccel = self->fb.pitchaccel - 5400;
	}
	if (self->fb.yawaccel > 0) {
		self->fb.yawaccel = self->fb.yawaccel + 5400;
	}
	else if (self->fb.yawaccel < 0) {
		self->fb.yawaccel = self->fb.yawaccel - 5400;
	}
}

// Sets a client's last marker
void SetMarker(gedict_t* client, gedict_t* marker) {
	client->fb.touch_distance = 0;
	client->fb.touch_marker = marker;
	client->fb.Z_ = marker ? marker->fb.Z_ : 0;
	client->fb.touch_marker_time = g_globalvars.time + 5;
	EnterZone(client->fb.Z_, client->s.v.team, client->fb.is_strong);
}

void SetNextThinkTime(gedict_t* ent) {
	ent->fb.frogbot_nextthink = ent->fb.frogbot_nextthink + 0.15 + (0.015 * random());
	if (ent->fb.frogbot_nextthink <= g_globalvars.time) {
		ent->fb.frogbot_nextthink = g_globalvars.time + 0.16;
	}
}

static void AvoidLookObjectsMissile(gedict_t* self) {
	gedict_t* rocket;

	dodge_missile = NULL;
	if (self->fb.look_object && self->fb.look_object->ct == ctPlayer) {
		for (rocket = world; rocket = ez_find (rocket, "rocket"); ) {
			if (rocket->s.v.owner == EDICT_TO_PROG (self->fb.look_object)) {
				dodge_missile = rocket;
				break;
			}
		}
	}
}

static void LookingAtEnemyLogic(gedict_t* self) {
	//UpdateEnemy
	if (Visible_360(self, self->fb.look_object)) {
		if (self->fb.look_object == &g_edicts[self->s.v.enemy]) {
			self->fb.enemy_dist = VectorDistance (self->fb.look_object->s.v.origin, self->s.v.origin);
		}
		else {
			if (g_globalvars.time >= self->fb.enemy_time) {
				ClearLookObject(self);
			}
		}
	}
	else  {
		ClearLookObject(self);
	}
}

static void NewlyPickedEnemyLogic() {
	gedict_t* goalentity_ = &g_edicts[self->s.v.goalentity];

	if (goalentity_ == &g_edicts[self->s.v.enemy]) {
		if (Visible_360(self, &g_edicts[self->s.v.enemy])) {
			LookEnemy(self, &g_edicts[self->s.v.enemy]);
		}
		else  {
			if (g_globalvars.time >= self->fb.enemy_time) {
				BestEnemy(self);
				if (&g_edicts[self->s.v.enemy] != goalentity_) {
					self->fb.goal_refresh_time = 0;
				}
			}
		}
	}
	else {
		if (Visible_infront(self, &g_edicts[self->s.v.enemy])) {
			LookEnemy(self, &g_edicts[self->s.v.enemy]);
		}
		else {
			if (g_globalvars.time >= self->fb.enemy_time) {
				BestEnemy(self);
			}
		}
	}
}

void TargetEnemyLogic() {
	dodge_missile = NULL;

	if (!(self->fb.state & NOTARGET_ENEMY)) {
		if (self->fb.look_object && self->fb.look_object->ct == ctPlayer) {
			// Interesting - they only avoid missiles from objects they can see / are locked onto?
			AvoidLookObjectsMissile(self);

			LookingAtEnemyLogic(self);
		}
		else if (&g_edicts[self->s.v.enemy]) {
			NewlyPickedEnemyLogic();
		}
		else  {
			BestEnemy(self);
		}
	}
}

static void BotDodgeMovement(gedict_t* self, vec3_t dir_move, float dodge_factor) {
	if (dodge_factor) {
		if (dodge_factor < 0) {
			++dodge_factor;
		}
		else  {
			--dodge_factor;
		}
		trap_makevectors(self->s.v.v_angle);
		VectorMA(dir_move, random() * self->fb.skill.dodge_amount * dodge_factor, g_globalvars.v_right, dir_move);
	}
}

static void BotOnGroundMovement(gedict_t* self, vec3_t dir_move) {
	float dodge_factor = 0;

	if ((int)self->s.v.flags & FL_ONGROUND) {
		if (!(self->fb.path_state & NO_DODGE)) {
			vec3_t temp;

			// Dodge a rocket our enemy is firing at us
			if (dodge_missile) {
				if (PROG_TO_EDICT(dodge_missile->s.v.owner)->ct == ctPlayer) {
					vec3_t rel_pos;

					VectorSubtract(self->s.v.origin, dodge_missile->s.v.origin, rel_pos);
					if (DotProduct(rel_pos, dodge_missile->fb.missile_forward) > 0.7071067) {
						normalize(rel_pos, temp);
						dodge_factor = DotProduct(temp, dodge_missile->fb.missile_right);
					}
				}
				else  {
					dodge_missile = NULL;
				}
			}

			// Not dodging a missile, dodge away from the player instead
			if (self->fb.look_object && self->fb.look_object->ct == ctPlayer) {
				if (!dodge_factor) {
					vec3_t rel_pos;

					VectorSubtract (self->s.v.origin, self->fb.look_object->s.v.origin, rel_pos);
					trap_makevectors(self->fb.look_object->s.v.v_angle);
					if (DotProduct(rel_pos, g_globalvars.v_forward) > 0) {
						normalize(rel_pos, temp);
						dodge_factor = DotProduct(temp, g_globalvars.v_right);
					}
				}
			}

			BotDodgeMovement(self, dir_move, dodge_factor);
		}
	}

	// If we're not in water, cannot have vertical direction (think of markers heading up stairs)
	if (self->s.v.waterlevel <= 1) {
		dir_move[2] = 0;
	}
}

static void BotMoveTowardsLinkedMarker(gedict_t* self, vec3_t dir_move) {
	vec3_t temp;
	gedict_t* goalentity_ = &g_edicts[self->s.v.goalentity];

	VectorAdd(self->fb.linked_marker->s.v.absmin, self->fb.linked_marker->s.v.view_ofs, temp);
	VectorSubtract(temp, self->s.v.origin, temp);
	normalize(temp, dir_move);
	if (self->fb.linked_marker == self->fb.touch_marker) {
		if (goalentity_ == self->fb.touch_marker) {
			if (WaitingToRespawn(self->fb.touch_marker)) {
				VectorClear(dir_move);
			}
		}
		else {
			VectorClear(dir_move);
		}
	}
}

// Called when the bot has a touch marker set
static void BotTouchMarkerLogic() {
	TargetEnemyLogic();

	if (g_globalvars.time >= self->fb.goal_refresh_time) {
		UpdateGoal();
	}

	if (g_globalvars.time >= self->fb.linked_marker_time) {
		self->fb.old_linked_marker = NULL;
	}

	if (self->fb.old_linked_marker != self->fb.touch_marker) {
		frogbot_marker_touch();
	}

	if (g_globalvars.time < self->fb.arrow_time) {
		if (g_globalvars.time < self->fb.arrow_time2) {
			if (random() < 0.5) {
				self->fb.old_linked_marker = self->fb.linked_marker = self->fb.touch_marker;
				self->fb.path_state = 0;
				self->fb.linked_marker_time = g_globalvars.time + 0.3;
			}
		}
	}
	else {
		vec3_t dir_move;

		BotMoveTowardsLinkedMarker(self, dir_move);
		BotOnGroundMovement(self, dir_move);

		self->fb.arrow = BestArrowForDirection(self, dir_move);
		VectorCopy(dir_move, self->fb.dir_move_);
	}

	SelectWeapon();
}

// Called when a human player touches a marker
static void HumanTouchMarkerLogic() {
	if (g_globalvars.time >= self->fb.enemy_time) {
		BestEnemy(self);
	}
}

static void PeriodicAllClientLogic() {
	SetNextThinkTime(self);

	if (g_globalvars.time >= self->fb.weapon_refresh_time) {
		UpdateWeapons();
	}

	// If we haven't touched a marker in a while, find closest marker
	if (g_globalvars.time >= self->fb.touch_marker_time) {
		SetMarker(self, LocateMarker(self->s.v.origin));
	}

	if (self->fb.touch_marker) {
		if (self->fb.state & AWARE_SURROUNDINGS) {
			gedict_t* goalentity_ = &g_edicts[self->s.v.goalentity];

			if (self->isBot) {
				BotTouchMarkerLogic();
			}
			else  {
				HumanTouchMarkerLogic();
			}
		}
		else {
			self->fb.goal_refresh_time = 0;
			if (self->isBot) {
				self->fb.old_linked_marker = NULL;
				self->fb.state |= AWARE_SURROUNDINGS;
			}
			else {
				self->fb.state |= AWARE_SURROUNDINGS;
			}
		}
	}
}

static void BotStopFiring() {
	qbool continuous = (qbool) ((int)self->s.v.weapon & IT_CONTINUOUS);

	if (!(continuous || self->fb.rocketjumping)) {
		self->fb.firing = false;
	}
}

// FIXME: Magic numbers (400 = 0.5 * sv_gravity)
static qbool PredictSpot(gedict_t* self, gedict_t* enemy_, vec3_t testplace, float rel_time, float fallheight) {
	VectorCopy(testplace, dropper->s.v.origin);
	dropper->s.v.flags = FL_ONGROUND_PARTIALGROUND;

	if (walkmove(dropper, 0, 0)) {
		if (! droptofloor(dropper)) {
			testplace[2] = testplace[2] - 400 * (rel_time * rel_time) - 38;
			return false;
		}

		if (dropper->s.v.origin[2] < fallheight) {
			testplace[2] = testplace[2] - 400 * (rel_time * rel_time) - 38;
			return false;
		}
		return true;
	}

	VectorCopy(enemy_->s.v.origin, testplace);
	return false;
}

static void PredictEnemyLocationInFuture(gedict_t* enemy, float rel_time) {
	vec3_t testplace;
	float fallheight = enemy->s.v.origin[2] - 56 + enemy->s.v.velocity[2] * rel_time;

	enemy->fb.oldsolid = enemy->s.v.solid;
	enemy->s.v.solid = SOLID_NOT;
	VectorMA(enemy->s.v.origin, rel_time, enemy->s.v.velocity, testplace);
	testplace[2] += 36;

	if (PredictSpot(self, enemy, testplace, rel_time, fallheight)) {
		VectorCopy(dropper->s.v.origin, self->fb.predict_origin);
	}
	else {
		vec3_t dir_forward;

		VectorSubtract(self->fb.predict_origin, enemy->s.v.origin, dir_forward);
		dir_forward[2] = 0;
		if ((vlen(dir_forward) > half_sv_maxspeed) || (DotProduct(dir_forward, enemy->s.v.velocity) <= 0)) {
			VectorCopy(testplace, self->fb.predict_origin);
		}
	}
	enemy->s.v.solid = enemy->fb.oldsolid;
}

// This is when firing at buttons/doors etc
static void BotsFireAtWorldLogic(gedict_t* self, vec3_t rel_pos, float* rel_dist) {
	VectorAdd(self->fb.look_object->s.v.absmin, self->fb.look_object->s.v.view_ofs, rel_pos);
	VectorSubtract(rel_pos, self->s.v.origin, rel_pos);
	*rel_dist = vlen(rel_pos);

	if (DM6FireAtDoor (self))
		return;

	if (*rel_dist < 160) {
		vec3_t rel_pos2 = { rel_pos[0], rel_pos[1], 0 };
		VectorNormalize(rel_pos2);
		VectorScale(rel_pos2, 160, rel_pos2);
		rel_pos[0] = rel_pos2[0];
		rel_pos[1] = rel_pos2[1];
		*rel_dist = 160;
	}
}

// When firing at another player
static void BotsFireAtPlayerLogic(gedict_t* self, vec3_t rel_pos, float* rel_dist) {
	float rel_time = 0;

	VectorSubtract(self->fb.look_object->s.v.origin, self->s.v.origin, rel_pos);
	*rel_dist = vlen(rel_pos);

	// If (not a hitscan weapon)
	if ((int)self->s.v.weapon & IT_VELOCITY) {
		if ((int)self->s.v.weapon & IT_GRENADE_LAUNCHER) {
			rel_time = *rel_dist / 600;
		}
		else {
			rel_time = *rel_dist / 1000;
			if ((int)self->ctf_flag & CTF_RUNE_HST) {
				if ((int)self->s.v.weapon & IT_EITHER_NAILGUN) {
					rel_time *= 0.5;
				}
			}
		}

		rel_time = min(rel_time, 0.5);

		if (self->s.v.enemy) {
			PredictEnemyLocationInFuture(&g_edicts[self->s.v.enemy], rel_time);

			if (self->fb.look_object == &g_edicts[self->s.v.enemy]) {
				VectorSubtract(self->fb.predict_origin, self->s.v.origin, rel_pos);
			}
		}
	}
}

static void BotsFireLogic(void) {
	vec3_t rel_pos;

	if (g_globalvars.time < self->fb.fire_nextthink)
		return;

	self->fb.fire_nextthink = self->fb.fire_nextthink + (self->fb.skill.firing_reflex * (0.95 + (0.1 * random())));
	if (self->fb.fire_nextthink <= g_globalvars.time) {
		self->fb.fire_nextthink = g_globalvars.time + (self->fb.skill.firing_reflex * (0.95 + (0.1 * random())));
	}

	// a_attackfix()
	if (!self->fb.rocketjumping && self->s.v.enemy == 0 && !(self->state & SHOT_FOR_LUCK)) {
		self->fb.firing = false;
	}

	if (self->fb.look_object) {
		vec3_t pitch_tangent = { 0 };
		vec3_t yaw_tangent = { 0 };
		float mouse_vel = 0;
		float rel_dist = 0;
		vec3_t rel_dir = { 0 };
		vec3_t rel_hor_dir = { 0 };
		float hor_component = 0;

		if (self->fb.look_object->ct == ctPlayer) {
			BotsFireAtPlayerLogic(self, rel_pos, &rel_dist);
			//G_bprint (2, "Firing @ %s @ rel(%f %f %f)\n", look_object_->s.v.netname, PASSVEC3 (rel_pos));
		}
		else {
			BotsFireAtWorldLogic(self, rel_pos, &rel_dist);
			//G_bprint (2, "Firing @ world @ rel(%f %f %f)\n", PASSVEC3 (rel_pos));
		}

		// Aim lower over longer distances?  (FIXME)
		if (self->s.v.weapon == IT_ROCKET_LAUNCHER && rel_dist > 96) {
			traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 16, self->s.v.origin[0] + rel_pos[0], self->s.v.origin[1] + rel_pos[1], self->s.v.origin[2] + rel_pos[2] - 22, true, self);
			if (g_globalvars.trace_fraction == 1) {
				rel_pos[2] = rel_pos[2] - 38;
			}
		}

		normalize(rel_pos, rel_dir);
		VectorCopy(rel_pos, rel_hor_dir);
		rel_hor_dir[2] = 0;
		normalize(rel_hor_dir, rel_hor_dir);

		hor_component = DotProduct(rel_dir, rel_hor_dir);
		mouse_vel = 57.29578 / rel_dist;

		// rel_hor_dir and '0 0 1' are an orthogonal axis
		// hor_component is the rel_hor_dir (horizontal) component of rel_dir
		// rel_dir_z is the '0 0 1' (vertical) component of rel_dir
		VectorScale(rel_hor_dir, rel_dir[2], pitch_tangent);
		pitch_tangent[2] = 0 - hor_component;

		// pitch_tangent is the tangent normal vector to pitch angular velocity
		VectorScale(pitch_tangent, mouse_vel, pitch_tangent);

		// pitch_tangent has been scaled according to view object distance
		yaw_tangent[0] = 0 - rel_hor_dir[1];
		yaw_tangent[1] = rel_hor_dir[0];
		yaw_tangent[2] = 0;

		// yaw_tangent is the tangent normal vector to yaw angular velocity
		VectorScale(yaw_tangent, mouse_vel, yaw_tangent);

		// yaw_tangent has been scaled according to view object distance
		{
			vec3_t vdiff;
			VectorSubtract(self->fb.look_object->s.v.velocity, self->s.v.velocity, vdiff);

			self->fb.track_pitchspeed = DotProduct(vdiff, pitch_tangent);
			self->fb.track_yawspeed = DotProduct(vdiff, yaw_tangent);
		}

		BotSetDesiredAngles (self, rel_pos);

		BotSetMouseParameters (self);

		if (!self->fb.rocketjumping) {
			SetFireButton(self, rel_pos, rel_dist);
		}
	}
}

void ThinkTime(gedict_t* self) {
	self->fb.jumping = false;

	// Logic that gets called for every player
	if (g_globalvars.time >= self->fb.frogbot_nextthink) {
		PeriodicAllClientLogic();

		if (self->isBot) {
			CheckCombatJump();
			AMPHI2BotInLava();
		}
	}

	// Logic that gets called every frame for every frogbot
	if (self->isBot) {
		self->fb.willRocketJumpThisTic = able_rj(self);

		BotStopFiring();

		AttackRespawns();

		if (streq(g_globalvars.mapname, "povdmm4")) {
			POVDMM4LookDoor(self);
		}

		BotsFireLogic();
	}
}

void BotEvadeLogic(gedict_t* self) {
	gedict_t* enemy_ = &g_edicts[self->s.v.enemy];

	self->fb.bot_evade = false;
	if (deathmatch <= 3 && !isRA()) {
		if (isDuel() && random() < CHANCE_EVADE_DUEL) {
			if ((self->s.v.origin[2] + 18) > (enemy_->s.v.absmin[2] + enemy_->s.v.view_ofs[2])) {
				if ((int)self->s.v.items & IT_ROCKET_LAUNCHER && self->s.v.ammo_rockets > 4) {
					if (!self->s.v.waterlevel) {
						self->fb.bot_evade = (qbool) (self->s.v.health > 70) && (self->s.v.armorvalue > 100) && !self->fb.enemy_visible;
					}
				}
			}
		}
		else if (! isDuel() && random() < CHANCE_EVADE_NONDUEL) {
			if ((self->s.v.origin[2] + 18) > (enemy_->s.v.absmin[2] + enemy_->s.v.view_ofs[2])) {
				if (((int)self->s.v.items & IT_ROCKET_LAUNCHER) || ((int)self->s.v.items & IT_LIGHTNING)) {
					if ((self->s.v.ammo_cells >= 20) || (self->s.v.ammo_rockets > 3)) {
						if (!self->s.v.waterlevel) {
							if ((self->s.v.health > 70) && (self->s.v.armorvalue > 90)) {
								self->fb.bot_evade = (qbool) (!((int)self->s.v.items & (IT_INVULNERABILITY | IT_INVISIBILITY | IT_QUAD)));
							}
						}
					}
				}
			}
		}
	}
}
