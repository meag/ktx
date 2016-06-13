// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

// FIXME: fb.skill
#define ATTACK_RESPAWN_TIME 3

// FIXME: globals, this is just setting
extern gedict_t* look_object_;
extern float risk_factor;
extern float risk;

void DM6SelectWeaponToOpenDoor (gedict_t* self);

static qbool WaterCombat(gedict_t* self) {
	gedict_t* enemy_ = &g_edicts[self->s.v.enemy];

	if (self->s.v.waterlevel < 2) {
		return true;
	}
	return ((trap_pointcontents(enemy_->s.v.origin[0], enemy_->s.v.origin[1], enemy_->s.v.origin[2]) == CONTENT_WATER) && (enemy_->s.v.origin[2] < self->s.v.origin[2] - 32));
}

static qbool RocketSafe(void) {
	float splash_damage = 80 - (0.25 * self->fb.enemy_dist);
	if (splash_damage <= 0 || (teamplay != 1 && teamplay != 5)) {
		return true;
	}

	if (self->super_damage_finished > g_globalvars.time) {
		splash_damage = splash_damage * (deathmatch != 4 ? 4 : 8);
		if (self->ctf_flag & ITEM_RUNE_MASK) {
			if (self->ctf_flag & CTF_RUNE_STR) {
				splash_damage = splash_damage * 2;
			}
			else if (self->ctf_flag & CTF_RUNE_RES) {
				splash_damage = splash_damage * 0.5;
			}
		}
	}

	return (self->fb.total_damage > splash_damage);
}

// When duelling, try and spawn frag.
void AttackRespawns(void) {
	gedict_t* enemy_ = &g_edicts[self->s.v.enemy];

	if (isRA() || isHoonyMode() || !isDuel()) {
		return;
	}

	if (enemy_ != world && enemy_->s.v.health < 1) {
		if (enemy_->fb.last_death <= g_globalvars.time + ATTACK_RESPAWN_TIME) {
			if (self->fb.skill.skill_level >= 15) {
				if ((int)self->s.v.items & IT_ROCKET_LAUNCHER) {
					if (self->s.v.ammo_rockets > 3) {
						if (!self->fb.rocketjumping) {
							if (random() > 0.15) {
								gedict_t* resp;
								for (resp = world; resp = trap_findradius(resp, self->s.v.origin, 1000); ) {
									if (streq(resp->s.v.classname, "info_player_deathmatch")) {
										vec3_t test;
										VectorCopy(self->s.v.origin, test);
										test[2] += 16;
										if (VectorDistance(resp->s.v.origin, test) > 160) {
											if (VisibleEntity(resp)) {
												float ang1,
												      ang2;
												vec3_t diff;

												self->fb.botchose = 1;
												self->fb.next_impulse = 7;
												self->fb.look_object = look_object_ = resp;
												VectorCopy(resp->s.v.origin, self->fb.predict_origin);
												self->fb.predict_origin[2] += 16;
												self->fb.old_linked_marker = NULL;

												VectorSubtract(resp->s.v.origin, self->s.v.origin, diff);
												ang2 = vectoyaw(diff);
												ang1 = anglemod(self->s.v.angles[1] - ang2);
												if (ang1 < 20 || ang1 > 340) {
													self->fb.firing = true;
												}
												return;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

qbool CheckNewWeapon(int desired_weapon) {
	int weapons[] = { 
		IT_AXE, IT_SHOTGUN, IT_SUPER_SHOTGUN, IT_NAILGUN, IT_SUPER_NAILGUN, IT_GRENADE_LAUNCHER, IT_ROCKET_LAUNCHER, IT_LIGHTNING
	};

	if (self->s.v.weapon != desired_weapon) {
		int i = 0;
		for (i = 0; i < sizeof(weapons) / sizeof(weapons[0]); ++i) {
			if (weapons[i] == desired_weapon) {
				self->fb.botchose = true;
				self->fb.next_impulse = i + 1;
				return true;
			}
		}
	}

	return false;
}

static qbool ShotForLuck(vec3_t object) {
	trap_makevectors(self->s.v.v_angle);
	traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], object[0], object[1], object[2], true, self);
	return (g_globalvars.trace_fraction == 1);
}

// FIXME: 
// Frogbots take into account the distance they've had to snap to look at the player, and won't fire if distance
//   is too high, compared to skill.accuracy
static void SetFireButtonBasedOnAngles (gedict_t* self, float risk, float risk_factor, float rel_dist)
{
	float min_angle_error;
	int i;

	VectorSubtract(self->fb.desired_angle, self->s.v.v_angle, self->fb.angle_error);

	for (i = 0; i < 2; ++i) {
		if (self->fb.angle_error[i] >= 180)
			self->fb.angle_error[i] -= 360;
		else if (self->fb.angle_error[i] < -180)
			self->fb.angle_error[i] += 360;

		self->fb.angle_error[i] = fabs (self->fb.angle_error[i]);
	}

	min_angle_error = (1 + risk) * risk_factor * (self->fb.skill.accuracy + (1440 / rel_dist));
	self->fb.firing |= (self->fb.angle_error[0] <= min_angle_error && self->fb.angle_error[1] <= min_angle_error);
}

// FIXME: should just be avoiding bore anyway?
// FIXME: take strength of player & enemy into account, player might survive quad splashdamage, to enemy weapon
static void AvoidQuadBore (gedict_t* self)
{
	qbool has_quad = (int)self->s.v.items & IT_QUAD;
	qbool has_pent = (int)self->s.v.items & IT_INVULNERABILITY;
	qbool could_explode = (self->s.v.weapon == IT_ROCKET_LAUNCHER) || (self->s.v.weapon == IT_GRENADE_LAUNCHER);
	qbool could_hurt_self = could_explode && !has_pent && teamplay != 1 && teamplay != 5;

	if (!has_quad || !could_hurt_self)
		return;

	if (self->fb.look_object == &g_edicts[self->s.v.enemy] && self->fb.enemy_dist <= 250) {
		// Enemy is too close for explosion, fire something else instead.
		int items_ = (int) self->s.v.items;
		int desired_weapon = IT_AXE;

		if ((items_ & IT_LIGHTNING) && (self->s.v.ammo_cells)) {
			desired_weapon = IT_LIGHTNING;
		}
		else if ((items_ & IT_SUPER_NAILGUN) && (self->s.v.ammo_nails)) {
			desired_weapon = IT_SUPER_NAILGUN;
		}
		else if ((items_ & IT_NAILGUN) && (self->s.v.ammo_nails)) {
			desired_weapon = IT_NAILGUN;
		}
		else if ((items_ & IT_SUPER_SHOTGUN) && (self->s.v.ammo_shells)) {
			desired_weapon = IT_SUPER_SHOTGUN;
		}
		else if (self->s.v.ammo_shells) {
			desired_weapon = IT_SHOTGUN;
		}
		
		self->fb.firing |= CheckNewWeapon( desired_weapon );
	}
}

// FIXME: Interesting... if a marker is the look object then it wouldn't explode on that?
static void SpamRocketShot (gedict_t* self)
{
	qbool has_rl = ((int)self->s.v.items & IT_ROCKET_LAUNCHER) && self->s.v.ammo_rockets > 3;
	qbool safe_to_fire = !visible_teammate (self) && self->fb.allowedMakeNoise;
    
	if (self->fb.rocketjumping)
		return;

	if (!has_rl || !safe_to_fire)
		return;

	if (self->fb.look_object) {
		// dist_sfl = threshold distance before attempting shot for luck
		float dist_sfl = ((int)self->s.v.items & IT_QUAD) ? 300.0f : 250.0f;
		vec3_t testplace;
		vec3_t rel_pos;
		float rel_dist;

		// rel_dist = distance between player and the item they're about to fire at
		VectorAdd(self->fb.look_object->s.v.absmin, self->fb.look_object->s.v.view_ofs, testplace);
		VectorSubtract(testplace, self->s.v.origin, rel_pos);
		rel_dist = vlen(rel_pos);

		if (rel_dist > dist_sfl && ShotForLuck(testplace)) {
			// FIXME: This uses distance to enemy, not to testplace (?)
			if (RocketSafe()) {
				// FIXME: Aim lower?  This looks like copy & paste from BotsFireLogic()
				//        Why self->origin + rel_pos when rel_pos = testplace - origin, why not just testplace? (or did RocketSafe() just overwrite?)
				traceline(
					self->s.v.origin[0],
					self->s.v.origin[1],
					self->s.v.origin[2] + 16,
					self->s.v.origin[0] + rel_pos[0],
					self->s.v.origin[1] + rel_pos[1],
					self->s.v.origin[2] + rel_pos[2] - 22,
					true, self);
				if (g_globalvars.trace_fraction == 1) {
					rel_pos[2] -= 38;
				}
				self->fb.state |= SHOT_FOR_LUCK;
				self->fb.botchose = 1;
				self->fb.next_impulse = 7;
				self->fb.firing = true;
			}
			else {
				self->fb.state &= ~SHOT_FOR_LUCK;
			}
		}
	}
}

static void RocketLauncherShot (gedict_t* self, float risk)
{
	float hit_radius = 160;
	vec3_t rocket_origin;     // where the rocket will be spawned from
	vec3_t rocket_endpos;     // where it will explode
	float risk_strength;
	gedict_t* test_enemy;

	// 
	VectorCopy(self->s.v.origin, rocket_origin);
	rocket_origin[2] += 16;
	trap_makevectors(self->s.v.v_angle);      // FIXME: desired angle?  will be one frame behind
	traceline(rocket_origin[0], rocket_origin[1], rocket_origin[2], rocket_origin[0] + (g_globalvars.v_forward[0] * 600), rocket_origin[1] + (g_globalvars.v_forward[1] * 600), rocket_origin[2] + (g_globalvars.v_forward[2] * 600), false, self);
	VectorCopy(g_globalvars.trace_endpos, rocket_endpos);
	risk_strength = g_globalvars.trace_fraction;

	for (test_enemy = world; test_enemy = find_plr (test_enemy); ) {
		float predict_dist = 1000000;
		vec3_t testplace;

		// Ignore corpses
		if (!test_enemy->s.v.takedamage)
			continue;

		if (test_enemy == &g_edicts[self->s.v.enemy]) {
			if (self->fb.look_object && self->fb.look_object->ct == ctPlayer) {
				if (self->fb.look_object == &g_edicts[self->s.v.enemy]) {
					VectorCopy(self->fb.predict_origin, testplace);
					predict_dist = VectorDistance(testplace, rocket_endpos);
				}
			}
			else if (self->fb.look_object && self->fb.look_object != world) {
				if (self->fb.allowedMakeNoise && self->fb.predict_shoot) {
					VectorAdd(self->fb.look_object->s.v.absmin, self->fb.look_object->s.v.view_ofs, testplace);
					from_marker = g_edicts[self->s.v.enemy].fb.touch_marker;
					path_normal = true;
					look_object_->fb.zone_marker();
					look_object_->fb.sub_arrival_time();
					predict_dist = (traveltime * sv_maxspeed) + VectorDistance(testplace, rocket_endpos);
				}
			}
		}
		else {
			VectorCopy(test_enemy->s.v.origin, testplace);
			predict_dist = VectorDistance(testplace, rocket_endpos);
		}

		if (predict_dist <= (hit_radius / (1 - risk))) {
			// See if the explosion would hurt that player
			traceline(rocket_endpos[0], rocket_endpos[1], rocket_endpos[2], testplace[0], testplace[1], testplace[2], true, self);
			if (g_globalvars.trace_fraction == 1) {
				// Nothing blocking the explosion...
				if ( ! SameTeam(test_enemy, self)) {
					// Enemy
					risk_factor = risk_factor / risk_strength;
					if (self->fb.look_object == &g_edicts[self->s.v.enemy]) {
						self->fb.firing = true;
					}
					else if (predict_dist <= (80 / (1.2 - risk))) {
						self->fb.firing = true;
					}
					else {
						SpamRocketShot (self);

						if ((int)self->s.v.items & IT_GRENADE_LAUNCHER) {
							if (self->s.v.enemy && !self->fb.rocketjumping) {
								if (self->fb.allowedMakeNoise && self->s.v.ammo_rockets > 3 && !visible_teammate(self)) {
									if (self->fb.arrow == BACK) {
										self->fb.botchose = 1;
										self->fb.next_impulse = 6;
										self->fb.firing = true;
									}
								}
							}
						}
					}
				}
				else  {
					if (test_enemy != self) {
						return;
					}
					else {
						risk_factor = risk_factor * risk_strength;
					}
				}
			}
		}
	}
}

void SetFireButton(gedict_t* self, vec3_t rel_pos, float rel_dist) {
	// Only fire in pre-war if enemy attacked us
	if (match_in_progress == 0 && (g_globalvars.time + random()) < g_edicts[self->s.v.enemy].attack_finished) {
		self->fb.firing = false;
		return;
	}

	// Countdown, never fire
	if (match_in_progress == 1) {
		self->fb.firing = false;
		return;
	}

	if (self->fb.firing) {
		if (self->fb.look_object == &g_edicts[self->s.v.enemy]) {
			if (random() < 0.666667) {
				if (!self->fb.next_impulse) {
					return;
				}
			}
		}

		if (!self->fb.rocketjumping) {
			self->fb.firing = false;
		}
	}
	else if (g_globalvars.time < self->attack_finished) {
		return;
	}

	if (self->fb.next_impulse) {
		return;
	}

	if (! SameTeam(look_object_, self)) {
		DM6SelectWeaponToOpenDoor (self);

		// FIXME: what is this referring to?
		if (self->fb.state & HURT_SELF) {
			if (self->s.v.weapon == IT_ROCKET_LAUNCHER && self->fb.real_pitch == 78.75) {
				self->fb.firing = true;
				self->fb.state &= ~HURT_SELF;
			}
			return;
		}

		if (self->s.v.enemy && g_edicts[self->s.v.enemy].fb.touch_marker) {
			traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 16, self->s.v.origin[0] + rel_pos[0], self->s.v.origin[1] + rel_pos[1], self->s.v.origin[2] + rel_pos[2] + 16, false, self);
			if (g_globalvars.trace_fraction == 1) {
				if (self->s.v.weapon != IT_ROCKET_LAUNCHER && self->fb.look_object != &g_edicts[self->s.v.enemy]) {
					return;
				}
			}
			else if (PROG_TO_EDICT(g_globalvars.trace_ent) != look_object_) {
				gedict_t* traced = PROG_TO_EDICT(g_globalvars.trace_ent);
				if (traced->ct == ctPlayer) {
					if (!SameTeam(traced, self)) {
						if (!((int)self->s.v.flags & FL_WATERJUMP)) {
							self->s.v.enemy = NUM_FOR_EDICT( traced );
							LookEnemy(self, traced);
						}
					}
					return;
				}
				else {
					if (self->fb.look_object == &g_edicts[self->s.v.enemy]) {
						if (!self->s.v.waterlevel) {
							if (self->fb.allowedMakeNoise) {
								if ((int)self->s.v.flags & FL_ONGROUND) {
									traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 32, self->s.v.origin[0] + rel_pos[0], self->s.v.origin[1] + rel_pos[1], self->s.v.origin[2] + rel_pos[2] + 32 , false, self);
									self->fb.jumping |= (g_globalvars.trace_fraction == 1);
								}
							}
						}
					}
					return;
				}
			}

			// Deal with bot deciding to discharge elsewhere
			if (self->s.v.weapon == IT_LIGHTNING && self->s.v.waterlevel > 1) {
				return;
			}

			risk_factor = 0.5;
			risk = random();
			risk = risk * risk;

			AvoidQuadBore (self);

			if (self->s.v.weapon == IT_ROCKET_LAUNCHER) {
				RocketLauncherShot (self, risk);
			}
			else {
				SetFireButtonBasedOnAngles (self, risk, risk_factor, rel_dist);
			}
		}
	}
}

static int DesiredWeapon(void) {
	int items_ = self->s.v.items;
	qbool shaft_available = false;
	qbool avoid_rockets = false;

	if ((int)self->s.v.items & IT_QUAD) {
		if (teamplay != 1 && teamplay != 5) {
			gedict_t* search_entity = identify_teammate_(self);
			if (!search_entity->invincible_time) {
				if (VisibleEntity(search_entity)) {
					if (self->fb.enemy_visible) {
						if (VectorDistance(search_entity->s.v.origin, g_edicts[self->s.v.enemy].s.v.origin) < 150) {
							if (self->s.v.ammo_shells) {
								return IT_SHOTGUN;
							}
						}
					}
				}
			}
		}
	}

	// When to always use RL
	if (self->fb.skill.rl_preference >= random() || fb_lg_disabled()) {
		if (items_ & IT_ROCKET_LAUNCHER) {
			if (self->s.v.ammo_rockets) {
				if (RocketSafe()) {
					return IT_ROCKET_LAUNCHER;
				}
				avoid_rockets = true;
			}
		}
	}

	if (self->fb.skill.lg_preference >= random() && !fb_lg_disabled()) {
		if ((self->s.v.waterlevel <= 1) || ((int)self->s.v.items & IT_INVULNERABILITY)) {
			if ((items_ & IT_LIGHTNING) && self->s.v.ammo_cells) {
				if (self->fb.enemy_dist <= 600) {
					return IT_LIGHTNING;
				}
				shaft_available = true;
			}
		}
	}

	if (BotShouldDischarge()) {
		return IT_LIGHTNING;
	}

	if (!fb_lg_disabled()) {
		if (self->s.v.waterlevel <= 1 || ((int)self->s.v.items & IT_INVULNERABILITY)) {
			if (items_ & IT_LIGHTNING) {
				if (self->s.v.ammo_cells) {
					if (self->fb.enemy_dist <= 600) {
						if (self->fb.look_object == &g_edicts[self->s.v.enemy]) {
							vec3_t diff, enemy_angles;

							VectorSubtract(self->fb.look_object->s.v.origin, self->s.v.origin, diff);
							vectoangles(diff, enemy_angles);
							if (enemy_angles[0] < 15) {
								if (enemy_angles[0] > -15) {
									return IT_LIGHTNING;
								}
							}
						}
						else  {
							return IT_LIGHTNING;
						}
						shaft_available = true;
					}
				}
			}
		}
	}

	if (!avoid_rockets) {
		if (items_ & IT_ROCKET_LAUNCHER) {
			if (self->s.v.ammo_rockets) {
				if (RocketSafe()) {
					return IT_ROCKET_LAUNCHER;
				}
				if (!((int)self->s.v.items & IT_INVULNERABILITY)) {
					avoid_rockets = true;
				}
			}
		}
	}
	if (self->fb.state & WAIT) {
		if (items_ & IT_ROCKET_LAUNCHER) {
			if (self->s.v.ammo_rockets) {
				if (RocketSafe()) {
					return IT_ROCKET_LAUNCHER;
				}
			}
		}
		else if (items_ & IT_LIGHTNING) {
			if (self->s.v.ammo_cells) {
				if (shaft_available) {
					return IT_LIGHTNING;
				}
			}
		}
	}
	if (shaft_available) {
		return IT_LIGHTNING;
	}
	if (self->fb.enemy_dist <= 320) {
		if (!avoid_rockets) {
			if (items_ & IT_GRENADE_LAUNCHER) {
				if (self->s.v.ammo_rockets) {
					if (RocketSafe()) {
						if (WaterCombat(self)) {
							return IT_GRENADE_LAUNCHER;
						}
					}
				}
			}
		}
	}
	if (self->fb.enemy_dist <= 600) {
		if (items_ & IT_SUPER_NAILGUN) {
			if (self->s.v.ammo_nails) {
				return IT_SUPER_NAILGUN;
			}
		}
		if (items_ & IT_SUPER_SHOTGUN) {
			if (self->s.v.ammo_shells) {
				return IT_SUPER_SHOTGUN;
			}
		}
		if (items_ & IT_NAILGUN) {
			if (self->s.v.ammo_nails) {
				return IT_NAILGUN;
			}
		}
	}
	if (self->s.v.ammo_shells) {
		return IT_SHOTGUN;
	}
	return IT_AXE;
}

void SelectWeapon(void) {
	if (self->fb.path_state & DM6_DOOR) {
		return;
	}
	if (self->fb.state & HURT_SELF) {
		if ((int)self->s.v.items & IT_ROCKET_LAUNCHER) {
			if (self->s.v.ammo_rockets) {
				if (self->s.v.health >= 50) {
					if (self->super_damage_finished <= g_globalvars.time) {
						if (self->s.v.weapon != IT_ROCKET_LAUNCHER) {
							self->fb.botchose = 1;
							self->fb.next_impulse = 7;
						}
						return;
					}
				}
			}
		}
		self->fb.state = self->fb.state & NOT_HURT_SELF;
	}
	CheckNewWeapon( DesiredWeapon() );
}

void DelayUpdateWeapons(gedict_t* self) {
	self->fb.weapon_refresh_time = min (g_globalvars.time + 1, self->fb.weapon_refresh_time);
}

void UpdateWeapons() {
	int items_ = (int) self->s.v.items;
	float firepower_ = 100.0f;

	self->fb.weapon_refresh_time = 1000000;
	if (deathmatch != 4) {
		int attackbonus = 0;
		firepower_ = 0;
		if (items_ & IT_ROCKET_LAUNCHER) {
			firepower_ = self->s.v.ammo_rockets * 8;
			if (self->s.v.ammo_rockets) {
				attackbonus = 50;
			}
		}
		else if (items_ & IT_GRENADE_LAUNCHER) {
			firepower_ = self->s.v.ammo_rockets * 6;
			if (firepower_ > 50) {
				firepower_ = 50;
			}
		}


		if (items_ & IT_LIGHTNING) {
			firepower_ = firepower_ + self->s.v.ammo_cells;
			if (self->s.v.ammo_cells >= 10) {
				attackbonus = attackbonus + 50;
			}
		}
		if (items_ & IT_EITHER_NAILGUN) {
			firepower_ = firepower_ + (self->s.v.ammo_nails * 0.1);
		}
		if (items_ & IT_SUPER_SHOTGUN) {
			if (self->s.v.ammo_shells >= 50) {
				firepower_ = firepower_ + 20;
			}
			else  {
				firepower_ = firepower_ + self->s.v.ammo_shells * 0.4;
			}
		}
		else  {
			if (self->s.v.ammo_shells >= 25) {
				firepower_ = firepower_ + 10;
			}
			else  {
				firepower_ = firepower_ + self->s.v.ammo_shells * 0.4;
			}
		}
		if (firepower_ > 100) {
			firepower_ = 100;
		}

		self->fb.desire_rockets = max(5, 20 - self->s.v.ammo_rockets);
		self->fb.desire_cells = max(2.5, (50 - self->s.v.ammo_cells) * 0.2);
		self->fb.desire_rocketlauncher = max(100 - firepower_, self->fb.desire_rockets);
		self->fb.desire_lightning = max(self->fb.desire_rocketlauncher, self->fb.desire_cells);

		if (items_ & IT_ROCKET_LAUNCHER) {
			self->fb.desire_rockets = self->fb.desire_grenadelauncher = self->fb.desire_rocketlauncher;
		}
		else {
			self->fb.desire_grenadelauncher = 0;
			if (firepower_ < 50) {
				self->fb.desire_grenadelauncher = 50 - firepower_;
			}

			if (self->fb.desire_grenadelauncher < self->fb.desire_rockets) {
				self->fb.desire_grenadelauncher = self->fb.desire_rockets;
			}
			if (items_ & IT_GRENADE_LAUNCHER) {
				self->fb.desire_rockets = self->fb.desire_grenadelauncher;
			}
		}

		if (items_ & IT_LIGHTNING) {
			self->fb.desire_cells = self->fb.desire_lightning;
		}

		self->fb.desire_nails = self->fb.desire_shells = 0;
		if (firepower_ < 20) {
			self->fb.desire_nails = 2.5 - (self->s.v.ammo_nails * 0.0125);
			if (self->s.v.ammo_shells < 50) {
				self->fb.desire_shells = 2.5 - (self->s.v.ammo_shells * 0.05);
			}
		}

		self->fb.desire_supershotgun = max(0, 20 - firepower_);
		self->fb.desire_nailgun = self->fb.desire_supernailgun = max(self->fb.desire_supershotgun, self->fb.desire_nails);
		self->fb.desire_supershotgun = max(self->fb.desire_supershotgun, self->fb.desire_shells);

		if (items_ & IT_EITHER_NAILGUN) {
			self->fb.desire_nails = self->fb.desire_supernailgun;
		}
		if (items_ & IT_SUPER_SHOTGUN) {
			self->fb.desire_shells = self->fb.desire_supershotgun;
		}

		firepower_ = bound(0, firepower_ + attackbonus, 100);
	}

	if (self->super_damage_finished > g_globalvars.time) {
		firepower_ *= (deathmatch == 4 ? 8 : 4);
	}
	if (self->ctf_flag & CTF_RUNE_STR) {
		firepower_ *= 2;
	}
	self->fb.firepower = firepower_;
}

