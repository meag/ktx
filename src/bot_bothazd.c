// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

// A lot of this is the bot cheating?..
void AvoidEdge();

// FIXME: called in triggers.qc[TeleportTouch]
void HazardTeleport() {
	gedict_t* plr;

	if (self->fb.arrow_time < g_globalvars.time + 0.5) {
		self->fb.arrow_time = g_globalvars.time + 0.5;
	}

	for (plr = world; plr = find_plr(plr); ) {
		if (test_enemy != other) {
			if (test_enemy->fb.linked_marker == self) {
				test_enemy->fb.old_linked_marker = NULL;
				test_enemy->fb.linked_marker = LocateMarker(test_enemy->s.v.origin);
				test_enemy->fb.path_state = 0;
				test_enemy->fb.linked_marker_time = g_globalvars.time + 5;
			}
		}
	}
}

void ExplodeAlert(vec3_t org) {
	for (grenade_marker = world; grenade_marker = trap_findradius(grenade_marker, org, 256); ) {
		if (grenade_marker->fb.fl_marker) {
			traceline(org[0], org[1], org[2], grenade_marker->s.v.absmin[0] + grenade_marker->s.v.view_ofs[0], grenade_marker->s.v.absmin[1] + grenade_marker->s.v.view_ofs[1], grenade_marker->s.v.absmin[2] + grenade_marker->s.v.view_ofs[2], TRUE, grenade_marker);
			if (g_globalvars.trace_fraction == 1) {
				grenade_marker->fb.arrow_time = nextthink_;
				test_enemy = first_teleport;
				while (test_enemy) {
					if (test_enemy->s.v.enemy == NUM_FOR_EDICT(grenade_marker)) {
						if (test_enemy->fb.arrow_time < nextthink_) {
							test_enemy->fb.arrow_time = nextthink_;
						}
					}
					test_enemy = test_enemy->fb.next;
				}
			}
		}
	}
}

// FIXME: called from weapons.qc[W_Attack]
void GrenadeAlert() {
	nextthink_ = self->s.v.nextthink = g_globalvars.time + 0.05;
	self->s.v.think = (func_t) GrenadeAlert;
	if (self->fb.frogbot_nextthink <= g_globalvars.time) {
		self->s.v.think = (func_t) GrenadeExplode;
		self->s.v.nextthink = g_globalvars.time;
	}
	ExplodeAlert(self->s.v.origin);
}

// FIXME: called from weapons.qc[W_Attack]
void RocketAlert() {
	nextthink_ = self->s.v.nextthink = g_globalvars.time + 0.5;
	if (nextthink_ >= self->fb.frogbot_nextthink) {
		self->s.v.think = (func_t) Missile_Remove;
	}
	VectorCopy(self->s.v.origin, src);
	traceline(src[0], src[1], src[2], src[0] + self->s.v.velocity[0] * 600, src[1] + self->s.v.velocity[1] * 600, src[2] + self->s.v.velocity[2] * 600, TRUE, &g_edicts[self->s.v.owner]);
	ExplodeAlert(g_globalvars.trace_endpos);
}

void NewVelocityForArrow() {
	float best_arrow = BestArrowForDirection();
	if (self->fb.arrow != best_arrow) {
		VectorCopy(dir_move, self->fb.dir_move_);
		self->fb.arrow = best_arrow;
		self->fb.arrow_time = g_globalvars.time + 0.15;
		VectorCopy(oldvelocity_, self->s.v.velocity);
		VelocityForArrow();
	}
}

void AvoidHazards() {
	float hor_speed = 0;

	VectorCopy(self->s.v.velocity, new_velocity);
	linked_marker_ = self->fb.linked_marker;
	if ((int)self->fb.path_state & JUMP_LEDGE) {
		if (g_globalvars.time > self->fb.arrow_time2) {
			VectorAdd(linked_marker_->s.v.absmin, linked_marker_->s.v.view_ofs, rel_pos);
			VectorSubtract(rel_pos, self->s.v.origin, rel_pos);
			if ((int)self->s.v.flags & FL_ONGROUND) {
				try_jump_ledge = TRUE;
				being_blocked = FALSE;
				if (vlen(oldvelocity_) <= 100) {
					VectorCopy(rel_pos, rel_hor_dir);
					rel_hor_dir[2] = 0;
					try_jump_ledge = (vlen(rel_hor_dir) <= 80);
					VectorNormalize(rel_hor_dir);
					being_blocked = (DotProduct(self->fb.obstruction_normal, rel_hor_dir) > 0.5);
				}
				if (try_jump_ledge) {
					if (rel_pos[2] > 18) {
						hor_normal_vec[0] = 0 - rel_pos[1];
						hor_normal_vec[1] = rel_pos[0];
						VectorNormalize(hor_normal_vec);
						jumpspeed = new_velocity[2] + JUMPSPEED;
						if ((jumpspeed * jumpspeed * 0.000625) >= rel_pos[2]) {
							self->fb.jumping = true;
							self->fb.path_state = (int)self->fb.path_state | WAIT_GROUND;
							self->fb.ledge_backup_time = 0;
							return;
						}
					}
				}
				if (being_blocked) {
					if (g_globalvars.time > self->fb.arrow_time) {
						if (self->fb.ledge_backup_time) {
							if (g_globalvars.time >= self->fb.ledge_backup_time) {
								VectorMA(rel_hor_dir, -DotProduct(self->fb.obstruction_normal, rel_hor_dir), self->fb.obstruction_normal, dir_move);
								if (dir_move[0] == 0 && dir_move[1] == 0 && dir_move[2] == 0) {
									VectorScale(self->fb.obstruction_normal, -1, dir_move);
								}
								else if (random() < 0.5) {
									VectorScale(dir_move, -1, dir_move);
								}
								NewVelocityForArrow();
								self->fb.ledge_backup_time = 0;
							}
						}
						else  {
							self->fb.ledge_backup_time = g_globalvars.time + 0.15;
						}
					}
				}
			}
			else if (g_globalvars.time > self->fb.arrow_time) {
				if (new_velocity[2] >= 0) {
					ledge_backup = ((new_velocity[2] * new_velocity[2] * 0.000625) < rel_pos[2]);
				}
				else  {
					ledge_backup = (rel_pos[2] > 0);
				}
				if (ledge_backup) {
					if (self->fb.ledge_backup_time) {
						if (g_globalvars.time >= self->fb.ledge_backup_time) {
							VectorScale(rel_pos, -1, dir_move);
							dir_move[2] = 0;
							NewVelocityForArrow();
							self->fb.ledge_backup_time = 0;
						}
					}
					else  {
						self->fb.ledge_backup_time = g_globalvars.time + 0.15;
					}
				}
			}
		}
		if (g_globalvars.time >= (self->fb.ledge_backup_time + 0.15)) {
			self->fb.ledge_backup_time = 0;
		}
	}
	else if (self->fb.obstruction_normal[0] || self->fb.obstruction_normal[1] || self->fb.obstruction_normal[2]) {
		if (g_globalvars.time > self->fb.arrow_time) {
			if ((int)self->s.v.flags & FL_WATERJUMP) {
				return;
			}
			if (linked_marker_ != self->fb.touch_marker) {
				if (vlen(oldvelocity_) <= 32) {
					VectorMA(new_velocity, -DotProduct(self->fb.obstruction_normal, new_velocity), self->fb.obstruction_normal, dir_move);
					if ((dir_move[0] == 0) && (dir_move[1] == 0)) {
						VectorScale(self->fb.obstruction_normal, -1, dir_move);
						self->fb.path_state = (int)self->fb.path_state | STUCK_PATH;
					}
					else if ((oldvelocity_[0] == 0) && (oldvelocity_[1] == 0)) {
						if (random() < 0.5) {
							VectorScale(dir_move, -1, dir_move);
						}
						self->fb.path_state = (int)self->fb.path_state | STUCK_PATH;
					}
					else  {
						if ((int)self->fb.path_state & STUCK_PATH) {
							vec3_t norm_new_velocity;
							VectorNormalize(dir_move);
							normalize(new_velocity, norm_new_velocity);
							VectorAdd(dir_move, norm_new_velocity, dir_move);
						}
						else  {
							VectorMA(self->fb.dir_move_, -0.5 * DotProduct(self->fb.obstruction_normal, self->fb.dir_move_), self->fb.obstruction_normal, dir_move);
						}
					}
					dir_move[2] = 0;
					NewVelocityForArrow();
				}
			}
		}

		VectorMA(new_velocity, -DotProduct(self->fb.obstruction_normal, new_velocity), self->fb.obstruction_normal, new_velocity);
	}
	VectorCopy(new_velocity, hor_velocity);
	hor_velocity[2] = 0;
	hor_speed = vlen(hor_velocity);
	if (!hor_speed) {
		return;
	}
	VectorCopy(self->s.v.origin, new_origin);
	VectorMA(new_origin, 32 / hor_speed, new_velocity, testplace);
	if (self->s.v.waterlevel) {
		return;
	}
	normalize(hor_velocity, dir_forward);
	fallheight = self->s.v.origin[2] - 38;
	if (linked_marker_) {
		min_second = linked_marker_->s.v.absmin[2] + linked_marker_->s.v.view_ofs[2] - 36;
		if (fallheight > min_second) {
			fallheight = min_second;
		}
	}
	if ((int)self->s.v.flags & FL_ONGROUND) {
		if (new_velocity[2] < 0) {
			new_velocity[2] = 0;
		}
		VectorCopy(self->s.v.origin, last_clear_point);
		VectorMA(last_clear_point, (16 / hor_speed), new_velocity, testplace);
		FallSpotGround();
		if (fall == FALL_BLOCKED) {
			first_trace_fraction = 1;
			TestTopBlock();
			if (first_trace_fraction != 1) {
				VectorMA(last_clear_point, (16 / hor_speed) * first_trace_fraction, new_velocity, testplace);
				VectorMA(new_velocity, -DotProduct(first_trace_plane_normal, new_velocity), first_trace_plane_normal, new_velocity);
				VectorCopy(new_velocity, hor_velocity);
				hor_velocity[2] = 0;
				hor_speed = vlen(hor_velocity);
				VectorMA(testplace, (16 / hor_speed) * (1 - first_trace_fraction), new_velocity, testplace);
			}
			FallSpotGround();
		}
		if (fall >= FALL_LAND) {
			VectorCopy(testplace, jump_origin);
			new_fall = fall;
			VectorCopy(new_origin, testplace);
			FallSpotGround();
			if ((int)self->fb.path_state & DELIBERATE_AIR) {
				if (fall < FALL_LAND) {
					return;
				}
				self->fb.path_state = self->fb.path_state - DELIBERATE_AIR;
			}
			if (new_fall > fall) {
				if (g_globalvars.time > self->fb.arrow_time2) {
					current_fallspot = fall;
					VectorCopy(new_velocity, jump_velocity);
					jump_velocity[2] = jump_velocity[2] - (6400 / hor_speed);
					jump_origin[2] = jump_origin[2] + (jump_velocity[2] * (16 / hor_speed));
					jump_velocity[2] = jump_velocity[2] - (6400 / hor_speed);
					CanJumpOver();
					if (do_jump) {
						self->fb.path_state = (int)self->fb.path_state | DELIBERATE_AIR_WAIT_GROUND;
						if (turning_speed) {
							self->fb.path_state = (int)self->fb.path_state | AIR_ACCELERATION;
						}
						return;
					}
					VectorCopy(new_origin, jump_origin);
					VectorCopy(new_velocity, jump_velocity);
					jump_velocity[2] += JUMPSPEED;
					CanJumpOver();
					if (do_jump) {
						self->fb.jumping = true;
						self->fb.path_state = (int)self->fb.path_state | DELIBERATE_AIR_WAIT_GROUND;
						if (turning_speed) {
							self->fb.path_state = (int)self->fb.path_state | AIR_ACCELERATION;
						}
						return;
					}
				}
				AvoidEdge();
			}
			return;
		}
		VectorMA(testplace, 8 / hor_speed, new_velocity, new_origin);
		VectorMA(testplace, 16 / hor_speed, new_velocity, testplace);
		FallSpotGround();
		if (fall >= FALL_LAND) {
			VectorCopy(testplace, jump_origin);
			new_fall = fall;
			VectorCopy(self->s.v.origin, testplace);
			FallSpotGround();
			if ((int)self->fb.path_state & DELIBERATE_AIR) {
				if (fall < FALL_LAND) {
					return;
				}
				self->fb.path_state = self->fb.path_state - DELIBERATE_AIR;
			}
			if (new_fall > fall) {
				float normal_comp = 0;

				if (g_globalvars.time > self->fb.arrow_time2) {
					current_fallspot = fall;
					VectorCopy(new_velocity, jump_velocity);
					jump_velocity[2] = jump_velocity[2] - (6400 / hor_speed);
					jump_origin[2] = jump_origin[2] + (jump_velocity[2] * (16 / hor_speed));
					jump_velocity[2] = jump_velocity[2] - (6400 / hor_speed);
					CanJumpOver();
					if (do_jump) {
						self->fb.path_state = (int)self->fb.path_state | NO_DODGE;
						return;
					}
					VectorCopy(new_origin, jump_origin);
					VectorCopy(new_velocity, jump_velocity);
					jump_velocity[2] = jump_velocity[2] + JUMPSPEED;
					CanJumpOver();
					if (do_jump) {
						self->fb.path_state = (int)self->fb.path_state | NO_DODGE;
						return;
					}
				}
				traceline(
					self->s.v.origin[0] + (dir_forward[0] * 32), 
					self->s.v.origin[1] + (dir_forward[1] * 32), 
					self->s.v.origin[2] + (dir_forward[2] * 32) - 24.1, 
					self->s.v.origin[0] - (dir_forward[0] * 16), 
					self->s.v.origin[1] - (dir_forward[1] * 16),
					self->s.v.origin[2] - (dir_forward[2] * 16) -24.1, TRUE, world);
				g_globalvars.trace_plane_normal[2] = 0;
				if (g_globalvars.trace_plane_normal[0] == 0 && g_globalvars.trace_plane_normal[1] == 0 && g_globalvars.trace_plane_normal[2] == 0) {
					AvoidEdge();
					return;
				}

				normalize(g_globalvars.trace_plane_normal, edge_normal);
				normal_comp = DotProduct(edge_normal, dir_forward);
				if (normal_comp <= 0) {
					AvoidEdge();
					return;
				}
				VectorMA(dir_forward, -2 * normal_comp, edge_normal, dir_move);
				dir_move[2] = 0;
				NewVelocityForArrow();
				if (normal_comp > 0.5) {
					self->fb.arrow_time2 = self->fb.arrow_time;
				}
			}
		}
	}
	else  {
		FallSpotAir();
		if (fall >= FALL_LAND) {
			new_fall = fall;
			VectorCopy(new_origin, testplace);
			FallSpotAir();
			if ((int)self->fb.path_state & DELIBERATE_AIR) {
				if (fall < FALL_LAND) {
					return;
				}
				self->fb.path_state = self->fb.path_state - DELIBERATE_AIR;
			}
			if (new_fall > fall) {
				VectorMA(new_origin, (16 / hor_speed), new_velocity, testplace);
				FallSpotAir();
				if (new_fall > fall) {
					current_fallspot = fall;
					VectorCopy(new_origin, jump_origin);
					VectorCopy(new_velocity, jump_velocity);
					CanJumpOver();
					if (do_jump) {
						return;
					}
					AvoidEdge();
				}
			}
		}
	}
}

void AvoidEdge() {
	VectorCopy(oldvelocity_, dir_forward);
	dir_forward[2] = 0;
	if (dir_forward[0] || dir_forward[1] || dir_forward[2]) {
		oldvelocity_[0] = oldvelocity_[1] = 0;  // lavacheat always enabled
		VectorScale(dir_forward, -1, dir_move);
		NewVelocityForArrow();
		self->fb.arrow_time2 = self->fb.arrow_time;
	}
}

