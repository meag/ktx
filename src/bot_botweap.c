// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void DM6SelectWeaponToOpenDoor (gedict_t* self);

static qbool WaterCombat(void) {
	if (self->s.v.waterlevel < 2) {
		return TRUE;
	}
	return ((trap_pointcontents(enemy_->s.v.origin[0], enemy_->s.v.origin[1], enemy_->s.v.origin[2]) == CONTENT_WATER) && (enemy_->s.v.origin[2] < origin_[2] - 32));
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
		if (ar_time > g_globalvars.time) {
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

void CheckNewWeapon(int desired_weapon) {
	int weapons[] = { 
		IT_AXE, IT_SHOTGUN, IT_SUPER_SHOTGUN, IT_NAILGUN, IT_SUPER_NAILGUN, IT_GRENADE_LAUNCHER, IT_ROCKET_LAUNCHER, IT_LIGHTNING
	};

	if (self->s.v.weapon != desired_weapon) {
		int i = 0;
		for (i = 0; i < sizeof(weapons) / sizeof(weapons[0]); ++i) {
			if (weapons[i] == desired_weapon) {
				self->fb.botchose = true;
				self->fb.next_impulse = i + 1;
			}
		}
	}
}

static qbool ShotForLuck(vec3_t object) {
	trap_makevectors(self->s.v.v_angle);
	traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], object[0], object[1], object[2], TRUE, self);
	return (g_globalvars.trace_fraction == 1);
}

void SetFireButton(gedict_t* self) {
	/*
	if (match_in_progress != 2) {
		if (! match_in_progress) {
			if ((g_globalvars.time + random()) < enemy_->attack_finished) {
				self->fb.firing = false;
				return;
			}
		}
		else  {
			self->fb.firing = false;
			return;
		}
	}*/
	if (self->fb.firing) {
		if (look_object_ == enemy_) {
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

		if (enemy_) {
			if (enemy_->fb.touch_marker) {
				traceline(origin_[0], origin_[1], origin_[2] + 16, origin_[0] + rel_pos[0], origin_[1] + rel_pos[1], origin_[2] + rel_pos[2] + 16, FALSE, self);
				if (g_globalvars.trace_fraction == 1) {
					if (self->s.v.weapon != IT_ROCKET_LAUNCHER && look_object_ != enemy_) {
						return;
					}
				}
				else if (PROG_TO_EDICT(g_globalvars.trace_ent) != look_object_) {
					gedict_t* traced = PROG_TO_EDICT(g_globalvars.trace_ent);
					if (traced->ct == ctPlayer) {
						if (!SameTeam(traced, self)) {
							if (!((int)self->s.v.flags & FL_WATERJUMP)) {
								self->s.v.enemy = NUM_FOR_EDICT( traced );
								enemy_ = traced;
								LookEnemy(self, enemy_);
							}
						}
						return;
					}
					else {
						if (look_object_ == enemy_) {
							if (!self->s.v.waterlevel) {
								if (self->fb.allowedMakeNoise) {
									if ((int)self->s.v.flags & FL_ONGROUND) {
										traceline(origin_[0], origin_[1], origin_[2] + 32, origin_[0] + rel_pos[0], origin_[1] + rel_pos[1], origin_[2] + rel_pos[2] + 32 , FALSE, self);
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
				if ((int)self->s.v.items & IT_QUAD) {
					if (teamplay != 1 && teamplay != 5) {
						if (!((int)self->s.v.items & IT_INVULNERABILITY)) {
							if ((self->s.v.weapon == IT_ROCKET_LAUNCHER) || (self->s.v.weapon == IT_GRENADE_LAUNCHER)) {
								if (self->fb.look_object == enemy_) {
									if (self->fb.enemy_dist <= 250) {
										// Enemy is too close for explosion, fire something else instead.
										int items_ = (int) self->s.v.items;
										int desired_weapon = 0;
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
										CheckNewWeapon( desired_weapon );
										if (self->s.v.weapon == desired_weapon) {
											self->fb.firing = true;
										}
									}
								}
							}
						}
					}
				}

				if (self->s.v.weapon == IT_ROCKET_LAUNCHER) {
					hit_radius = 160;
					VectorCopy(origin_, rocket_origin);
					rocket_origin[2] += 16;
					trap_makevectors(self->s.v.v_angle);
					traceline(rocket_origin[0], rocket_origin[1], rocket_origin[2], rocket_origin[0] + (g_globalvars.v_forward[0] * 600), rocket_origin[1] + (g_globalvars.v_forward[1] * 600), rocket_origin[2] + (g_globalvars.v_forward[2] * 600), FALSE, self);
					VectorCopy(g_globalvars.trace_endpos, rocket_endpos);
					risk_strength = g_globalvars.trace_fraction;

					for (test_enemy = world; test_enemy = find_plr (test_enemy); ) {
						if (test_enemy->s.v.takedamage) {
							if (test_enemy == enemy_) {
								predict_dist = 1000000;
								if (look_object_->ct == ctPlayer) {
									if (look_object_ == enemy_) {
										VectorCopy(self->fb.predict_origin, testplace);
										predict_dist = VectorDistance(testplace, rocket_endpos);
									}
								}
								else if (look_object_) {
									if (self->fb.allowedMakeNoise) {
										if (self->fb.predict_shoot) {
											VectorAdd(look_object_->s.v.absmin, look_object_->s.v.view_ofs, testplace);
											from_marker = enemy_->fb.touch_marker;
											path_normal = TRUE;
											look_object_->fb.zone_marker();
											look_object_->fb.sub_arrival_time();
											predict_dist = (traveltime * sv_maxspeed) + VectorDistance(testplace, rocket_endpos);
										}
									}
								}
							}
							else  {
								VectorCopy(test_enemy->s.v.origin, testplace);
								predict_dist = VectorDistance(testplace, rocket_endpos);
							}
							if (predict_dist <= (hit_radius / (1 - risk))) {
								traceline(rocket_endpos[0], rocket_endpos[1], rocket_endpos[2], testplace[0], testplace[1], testplace[2], TRUE, self);
								if (g_globalvars.trace_fraction == 1) {
									if ( ! SameTeam(test_enemy, self)) {
										risk_factor = risk_factor / risk_strength;
										if (self->fb.look_object == enemy_) {
											self->fb.firing = true;
										}
										else if (predict_dist <= (80 / (1.2 - risk))) {
											self->fb.firing = true;
										}
										else  {
											if ((int)self->s.v.items & IT_ROCKET_LAUNCHER) {
												if (!self->fb.lines) {
													if (self->fb.look_object) {
														float dist_sfl = ((int)self->s.v.items & IT_QUAD) ? 300.0f : 250.0f;

														VectorAdd(self->fb.look_object->s.v.absmin, self->fb.look_object->s.v.view_ofs, testplace);
														VectorSubtract(testplace, self->s.v.origin, rel_pos);
														rel_dist = vlen(rel_pos);
														if (self->s.v.ammo_rockets > 3) {
															if (!visible_teammate(self)) {
																if (!self->fb.rocketjumping) {
																	if (self->fb.allowedMakeNoise) {
																		if (rel_dist > dist_sfl) {
																			if (ShotForLuck(testplace)) {
																				if (RocketSafe()) {
																					traceline(
																						origin_[0], origin_[1], origin_[2] + 16, 
																						self->s.v.origin[0] + rel_pos[0], 
																						self->s.v.origin[1] + rel_pos[1], 
																						self->s.v.origin[2] + rel_pos[2] - 22, 
																						TRUE, self);
																					if (g_globalvars.trace_fraction == 1) {
																						rel_pos[2] = rel_pos[2] - 38;
																					}
																					self->fb.state |= SHOT_FOR_LUCK;
																					self->fb.botchose = 1;
																					self->fb.next_impulse = 7;
																					self->fb.firing = true;
																				}
																				else  {
																					self->fb.state = self->fb.state - (self->fb.state & SHOT_FOR_LUCK);
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
											if ((int)self->s.v.items & IT_GRENADE_LAUNCHER) {
												if (!self->fb.lines) {
													if (enemy_) {
														if (!self->fb.rocketjumping) {
															if (self->fb.allowedMakeNoise) {
																if (self->s.v.ammo_rockets > 3) {
																	if (!visible_teammate(self)) {
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
												}
											}
										}
									}
									else  {
										if (test_enemy != self) {
											return;
										}
										else  {
											risk_factor = risk_factor * risk_strength;
										}
									}
								}
							}
						}
					}
					return;
				}

				VectorSubtract(self->fb.desired_angle, self->s.v.v_angle, self->fb.angle_error);
				if (self->fb.angle_error[1] >= 180) {
					self->fb.angle_error[1] -= 360;
				}
				else if (self->fb.angle_error[1] < -180) {
					self->fb.angle_error[1] += 360;
				}
				if (self->fb.angle_error[0] < 0) {
					self->fb.angle_error[0] = 0 - self->fb.angle_error[0];
				}
				if (self->fb.angle_error[1] < 0) {
					self->fb.angle_error[1] = 0 - self->fb.angle_error[1];
				}
				min_angle_error = (1 + risk) * risk_factor * (self->fb.skill.accuracy + (1440 / rel_dist));
				if (self->fb.angle_error[0] > min_angle_error) {
					return;
				}
				if (self->fb.angle_error[1] > min_angle_error) {
					return;
				}
				self->fb.firing = true;
			}
		}
	}
}

static int DesiredWeapon() {
	int items_ = self->s.v.items;
	qbool shaft_available = false;
	qbool avoid_rockets = false;

	if ((int)self->s.v.items & IT_QUAD) {
		if (teamplay != 1 && teamplay != 5) {
			search_entity = identify_teammate_(self);
			if (!search_entity->invincible_time) {
				if (VisibleEntity(search_entity)) {
					if (enemy_visible) {
						if (VectorDistance(search_entity->s.v.origin, enemy_->s.v.origin) < 150) {
							if (self->s.v.ammo_shells) {
								return IT_SHOTGUN;
							}
						}
					}
				}
			}
		}
	}

	if (game_rl_pref || fb_lg_disabled()) {
		if (items_ & IT_ROCKET_LAUNCHER) {
			if (self->s.v.ammo_rockets) {
				if (RocketSafe()) {
					return IT_ROCKET_LAUNCHER;
				}
				avoid_rockets = TRUE;
			}
		}
	}

	shaft_available = false;
	if (game_lg_pref && !fb_lg_disabled()) {
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
						if (look_object_ == enemy_) {
							vec3_t diff;
							VectorSubtract(look_object_->s.v.origin, origin_, diff);
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
					avoid_rockets = TRUE;
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
						if (WaterCombat()) {
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

void SelectWeapon() {
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

void DelayUpdateWeapons() {
	weapon_refresh_time_ = g_globalvars.time + 1;
	if (self->fb.weapon_refresh_time > weapon_refresh_time_) {
		self->fb.weapon_refresh_time = weapon_refresh_time_;
	}
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

