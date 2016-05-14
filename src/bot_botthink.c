// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void POVDMM4LookDoor(void);
void AMPHI2BotInLava(void);

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

static void AvoidLookObjectsMissile() {
	// TODO: this is why bot is good at avoiding grenades unless you spam them round corner...  need to set movetarget when missile is fired...
	/*
	if (look_object_->fb.movetarget->s.v.owner == NUM_FOR_EDICT(look_object_)) {
		dodge_missile = look_object_->fb.movetarget;
	}*/
}

static void LookingAtEnemyLogic() {
	//UpdateEnemy
	if (Visible_360(self, look_object_)) {
		if (look_object_ == enemy_) {
			self->fb.enemy_dist = VectorDistance(look_object_->s.v.origin, origin_);
		}
		else  {
			if (g_globalvars.time >= self->fb.enemy_time) {
				ClearLookObject(self);
				look_object_ = NULL;
			}
		}
	}
	else  {
		ClearLookObject(self);
		look_object_ = NULL;
	}
}

static void NewlyPickedEnemyLogic() {
	gedict_t* goalentity_ = &g_edicts[self->s.v.goalentity];

	//G_bprint (2, "No look object, enemy is %s, goal is %s\n", enemy_->s.v.classname, goalentity_->s.v.classname);

	visible_object = enemy_;
	if (goalentity_ == enemy_) {
		if (Visible_360(self, enemy_)) {
			LookEnemy(self, enemy_);
		}
		else  {
			if (g_globalvars.time >= self->fb.enemy_time) {
				BestEnemy(self);
				if (enemy_ != goalentity_) {
					self->fb.goal_refresh_time = 0;
				}
			}
		}
	}
	else  {
		Visible_infront();
		if (enemy_visible) {
			LookEnemy(self, enemy_);
		}
		else  {
			if (g_globalvars.time >= self->fb.enemy_time) {
				BestEnemy(self);
			}
		}
	}
}

void TargetEnemyLogic() {
	if (!(self->fb.state & NOTARGET_ENEMY)) {
		if (look_object_ && look_object_->ct == ctPlayer) {
			// Interesting - they only avoid missiles from objects they can see / are locked onto?
			AvoidLookObjectsMissile();

			LookingAtEnemyLogic();
		}
		else if (enemy_) {
			NewlyPickedEnemyLogic();
		}
		else  {
			BestEnemy(self);
		}
	}
}

static void BotDodgeMovement(vec3_t dir_move) {
	if (dodge_factor) {
		if (dodge_factor < 0) {
			dodge_factor = dodge_factor + 1;
		}
		else  {
			dodge_factor = dodge_factor - 1;
		}
		trap_makevectors(self->s.v.v_angle);
		VectorMA(dir_move, random() * self->fb.dodge_amount * dodge_factor, g_globalvars.v_right, dir_move);
	}
}

static void BotOnGroundMovement(gedict_t* self, vec3_t dir_move) {
	if ((int)self->s.v.flags & FL_ONGROUND) {
		if (!(self->fb.path_state & NO_DODGE)) {
			vec3_t temp;

			dodge_factor = 0;
			if (dodge_missile) {
				if (g_edicts[dodge_missile->s.v.owner].ct == ctPlayer) {
					VectorSubtract(origin_, dodge_missile->s.v.origin, rel_pos);
					if (DotProduct(rel_pos, dodge_missile->fb.missile_forward) > 0.7071067) {
						normalize(rel_pos, temp);
						dodge_factor = DotProduct(temp, dodge_missile->fb.missile_right);
					}
				}
				else  {
					dodge_missile = NULL;
				}
			}

			if (look_object_ && look_object_->ct == ctPlayer) {
				if (!dodge_factor) {
					VectorSubtract(origin_, look_object_->s.v.origin, rel_pos);
					trap_makevectors(look_object_->s.v.v_angle);
					if (DotProduct(rel_pos, g_globalvars.v_forward) > 0) {
						normalize(rel_pos, temp);
						dodge_factor = DotProduct(temp, g_globalvars.v_right);
					}
				}
			}

			BotDodgeMovement(dir_move);
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

	VectorAdd(linked_marker_->s.v.absmin, linked_marker_->s.v.view_ofs, temp);
	VectorSubtract(temp, origin_, temp);
	normalize(temp, dir_move);
	if (linked_marker_ == touch_marker_) {
		if (goalentity_ == touch_marker_) {
			if (touch_marker_->s.v.nextthink) {
				VectorClear(dir_move);
			}
		}
		else {
			VectorClear(dir_move);
		}
	}
}

// Called when the bot touches a marker
static void BotTouchMarkerLogic() {
	TargetEnemyLogic();

	if (g_globalvars.time >= self->fb.goal_refresh_time) {
		UpdateGoal();
	}

	if (g_globalvars.time >= self->fb.linked_marker_time) {
		self->fb.old_linked_marker = NULL;
	}

	if (self->fb.old_linked_marker != touch_marker_) {
		frogbot_marker_touch();
	}

	if (g_globalvars.time < self->fb.arrow_time) {
		if (g_globalvars.time < self->fb.arrow_time2) {
			if (random() < 0.5) {
				linked_marker_ = self->fb.old_linked_marker = self->fb.linked_marker = touch_marker_;
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
	gedict_t* goalentity_ = &g_edicts[self->s.v.goalentity];

	enemy_ = &g_edicts[self->s.v.enemy];
	touch_marker_ = self->fb.touch_marker;
	lookahead_time_ = self->fb.lookahead_time;
	VectorCopy(self->s.v.origin, origin_);
	goalentity_ = &g_edicts[self->s.v.goalentity];
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

			enemy_ = &g_edicts[self->s.v.enemy];
			look_object_ = self->fb.look_object;
			touch_marker_ = self->fb.touch_marker;
			lookahead_time_ = self->fb.lookahead_time;
			linked_marker_ = self->fb.linked_marker;

			VectorCopy(self->s.v.origin, origin_);
			goalentity_ = &g_edicts[self->s.v.goalentity];
			if (self->isBot) {
				BotTouchMarkerLogic();
			}
			else  {
				HumanTouchMarkerLogic();
			}
		}
		else  {
			self->fb.goal_refresh_time = 0;
			if (self->isBot) {
				self->fb.old_linked_marker = NULL;
				self->fb.state |= AWARE_SURROUNDINGS;
			}
			else if (markers_loaded) {
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

static qbool PredictSpot(gedict_t* self, gedict_t* enemy_, vec3_t testplace, float rel_time) {
	gedict_t* fallspot_self = self;
	self = dropper;
	VectorCopy(testplace, self->s.v.origin);
	self->s.v.flags = FL_ONGROUND_PARTIALGROUND;
	if (walkmove(self, 0, 0)) {
		if (!(droptofloor(self))) {
			self = fallspot_self;
			testplace[2] = testplace[2] - 400 * (rel_time * rel_time) - 38;
			return FALSE;
		}
		if (self->s.v.origin[2] < fallheight) {
			self = fallspot_self;
			testplace[2] = testplace[2] - 400 * (rel_time * rel_time) - 38;
			return FALSE;
		}
		self = fallspot_self;
		return TRUE;
	}
	self = fallspot_self;
	VectorCopy(enemy_->s.v.origin, testplace);
	return FALSE;
}

static void PredictEnemyLocationInFuture(gedict_t* enemy, float rel_time) {
	vec3_t testplace;
	qbool predict_spot;

	enemy_->fb.oldsolid = enemy_->s.v.solid;
	enemy_->s.v.solid = SOLID_NOT;
	fallheight = enemy_->s.v.origin[2] - 56 + enemy_->s.v.velocity[2] * rel_time;
	VectorMA(enemy_->s.v.origin, rel_time, enemy_->s.v.velocity, testplace);
	testplace[2] += 36;

	predict_spot = PredictSpot(self, enemy_, testplace, rel_time);

	if (predict_spot) {
		VectorCopy(dropper->s.v.origin, self->fb.predict_origin);
	}
	else  {
		VectorSubtract(self->fb.predict_origin, enemy_->s.v.origin, dir_forward);
		dir_forward[2] = 0;
		if ((vlen(dir_forward) > half_sv_maxspeed) || (DotProduct(dir_forward, enemy_->s.v.velocity) <= 0)) {
			VectorCopy(testplace, self->fb.predict_origin);
		}
	}
	enemy_->s.v.solid = enemy_->fb.oldsolid;
}

// This is when firing at buttons/doors etc
static void BotsFireAtWorldLogic(vec3_t rel_pos, float* rel_dist) {
	VectorAdd(look_object_->s.v.absmin, look_object_->s.v.view_ofs, rel_pos);
	VectorSubtract(rel_pos, origin_, rel_pos);
	*rel_dist = vlen(rel_pos);

	if (self->fb.path_state & DM6_DOOR) {
		if (dm6_door->s.v.takedamage) {
			rel_pos[2] = rel_pos[2] - 38;
		}
		else  {
			self->fb.path_state = self->fb.path_state - DM6_DOOR;
			self->fb.state = self->fb.state & NOT_NOTARGET_ENEMY;
		}
	}
	else if (*rel_dist < 160) {
		rel_pos2[0] = rel_pos[0];
		rel_pos2[1] = rel_pos[1];
		VectorNormalize(rel_pos2);
		VectorScale(rel_pos2, 160, rel_pos2);
		rel_pos[0] = rel_pos2[0];
		rel_pos[1] = rel_pos2[1];
		*rel_dist = 160;
	}
}

// When firing at another player
static void BotsFireAtPlayerLogic(vec3_t rel_pos, float* rel_dist) {
	VectorSubtract(look_object_->s.v.origin, origin_, rel_pos);
	*rel_dist = vlen(rel_pos);

	// If (not a hitscan weapon)
	if ((int)self->s.v.weapon & IT_VELOCITY) {
		if ((int)self->s.v.weapon & IT_GRENADE_LAUNCHER) {
			rel_time = *rel_dist / 600;
		}
		else  {
			rel_time = *rel_dist / 1000;
			if ((int)self->ctf_flag & CTF_RUNE_HST) {
				if ((int)self->s.v.weapon & IT_EITHER_NAILGUN) {
					rel_time = rel_time * 0.5;
				}
			}
		}

		rel_time = min(rel_time, 0.5);

		if (enemy_) {
			PredictEnemyLocationInFuture(enemy_, rel_time);

			if (look_object_ == enemy_) {
				VectorSubtract(self->fb.predict_origin, self->s.v.origin, rel_pos);
			}
		}
	}
}

static void BotsFireLogic() {
	if (g_globalvars.time >= self->fb.fire_nextthink) {
		self->fb.fire_nextthink = self->fb.fire_nextthink + (self->fb.firing_reflex * (0.95 + (0.1 * random())));
		if (self->fb.fire_nextthink <= g_globalvars.time) {
			self->fb.fire_nextthink = g_globalvars.time + (self->fb.firing_reflex * (0.95 + (0.1 * random())));
		}

		look_object_ = self->fb.look_object;
		enemy_ = &g_edicts[self->s.v.enemy];

		// a_attackfix()
		if (!self->fb.rocketjumping && enemy_ == world && !(self->state & SHOT_FOR_LUCK)) {
			self->fb.firing = false;
		}

		if (look_object_) {
			VectorCopy(self->s.v.origin, origin_);
			if (look_object_->ct == ctPlayer) {
				BotsFireAtPlayerLogic(rel_pos, &rel_dist);
				//G_bprint (2, "Firing @ %s @ rel(%f %f %f)\n", look_object_->s.v.netname, PASSVEC3 (rel_pos));
			}
			else {
				BotsFireAtWorldLogic(rel_pos, &rel_dist);
				//G_bprint (2, "Firing @ world @ rel(%f %f %f)\n", PASSVEC3 (rel_pos));
			}

			// Aim lower over longer distances?  (FIXME)
			if (self->s.v.weapon == IT_ROCKET_LAUNCHER && rel_dist > 96) {
				traceline(origin_[0], origin_[1], origin_[2] + 16, origin_[0] + rel_pos[0], origin_[1] + rel_pos[1], origin_[2] + rel_pos[2] - 22, TRUE, self);
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
				VectorSubtract(look_object_->s.v.velocity, self->s.v.velocity, vdiff);

				self->fb.track_pitchspeed = DotProduct(vdiff, pitch_tangent);
				self->fb.track_yawspeed = DotProduct(vdiff, yaw_tangent);
			}

			vectoangles(rel_pos, self->fb.desired_angle);
			if (self->fb.desired_angle[0] > 180) {
				self->fb.desired_angle[0] = 360 - self->fb.desired_angle[0];
			}
			else {
				self->fb.desired_angle[0] = 0 - self->fb.desired_angle[0];
			}

			self->fb.desired_angle[0] = (pr1_rint(self->fb.desired_angle[0] / 1.40625));
			self->fb.desired_angle[1] = (pr1_rint(self->fb.desired_angle[1] / 1.40625));
			VectorScale(self->fb.desired_angle, 1.40625, self->fb.desired_angle);
			if (self->fb.state & HURT_SELF) {
				self->fb.desired_angle[0] = 180;
			}
			VectorSubtract(self->fb.desired_angle, self->s.v.v_angle, self->fb.angle_error);
			self->fb.angle_error[0] -= (1 - self->fb.fast_aim) * (self->fb.pitchspeed * self->fb.firing_reflex);
			self->fb.angle_error[1] -= (1 - self->fb.fast_aim) * (self->fb.yawspeed * self->fb.firing_reflex);
			if (self->fb.angle_error[1] >= 180) {
				self->fb.angle_error[1] = self->fb.angle_error[1] - 360;
			}
			else if (self->fb.angle_error[1] < -180) {
				self->fb.angle_error[1] = self->fb.angle_error[1] + 360;
			}

			self->fb.track_pitchspeed += self->fb.fast_aim * self->fb.angle_error[0] / self->fb.firing_reflex;
			self->fb.track_yawspeed += self->fb.fast_aim * self->fb.angle_error[1] / self->fb.firing_reflex;
			self->fb.pitchaccel = (1 - self->fb.fast_aim) * self->fb.angle_error[0] / self->fb.firing_reflex;
			self->fb.yawaccel = (1 - self->fb.fast_aim) * self->fb.angle_error[1] / self->fb.firing_reflex;

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

			if (!self->fb.rocketjumping) {
				SetFireButton(self);
			}
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
		self->fb.willRocketJumpThisTic = able_rj();

		BotStopFiring();

		AttackRespawns();

		// FIXME: This is called for every bot, rather than for every frame (or ideally, when the doors open/close)
		if (streq(g_globalvars.mapname, "povdmm4")) {
			POVDMM4LookDoor();
		}

		BotsFireLogic();
	}
}

void BotEvadeLogic() {
	self->fb.bot_evade = FALSE;
	if (deathmatch <= 3 && !isRA()) {
		if (numberofclients == 2) {
			if (random() < 0.08) {
				if ((self->s.v.origin[2] + 18) > (enemy_->s.v.absmin[2] + enemy_->s.v.view_ofs[2])) {
					if ((int)self->s.v.items & IT_ROCKET_LAUNCHER) {
						if (self->s.v.ammo_rockets > 4) {
							if (!self->s.v.waterlevel) {
								self->fb.bot_evade = (qbool) (self->s.v.health > 70) && (self->s.v.armorvalue > 100) && !enemy_visible;
							}
						}
					}
				}
			}
		}
		else if (numberofclients > 2) {
			if (random() < 0.1) {
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
}
