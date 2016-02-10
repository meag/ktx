// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

float right_direction() {
	vec3_t test_dir,
	       temp;
	float current_dir,
	      wish_dir,
	      right_dir;
	float min_one,
	      min_two;
	normalize(self->fb.oldvelocity, test_dir);
	current_dir = vectoyaw(test_dir);
	VectorSubtract(self->fb.linked_marker->s.v.origin, self->s.v.origin, temp);
	normalize(temp, test_dir);
	wish_dir = vectoyaw(test_dir);
	min_one = fabs(wish_dir - current_dir);
	if (wish_dir >= 180) {
		wish_dir = wish_dir - 360;
	}
	if (current_dir >= 180) {
		current_dir = current_dir - 360;
	}
	min_two = fabs(wish_dir - current_dir);
	right_dir = min_one;
	if (min_two < right_dir) {
		right_dir = min_two;
	}
	return (right_dir <= 90);
}

float able_rj() {
	float health_after;
	if ((int)self->s.v.items & (IT_INVULNERABILITY_QUAD | IT_INVULNERABILITY)) {
		return 1;
	}
	health_after = ceil(self->s.v.armortype * 50);
	if (health_after > self->s.v.armorvalue) {
		health_after = self->s.v.armorvalue;
	}
	health_after = self->s.v.health - ceil(50 - health_after);
	if (self->fb.linked_marker && self->fb.linked_marker->fb.healamount > 0 && self->fb.linked_marker->s.v.nextthink == 0) {
		health_after = health_after + self->fb.linked_marker->fb.healamount;
	}
	if (((health_after > 50) || (teamplay == 1) || (teamplay == 5)) && ((int)self->s.v.items & IT_ROCKET_LAUNCHER) && (self->s.v.ammo_rockets > 2) && ((!game_arena) || (arenastate == A_PLAYING)) && (!beQuiet) && (self->fb.willRocketJumpThisTic)) {
		return 1;
	}
	return 0;
}

float a_evalpath() {
	if (streq(test_marker->s.v.classname, "door")) {
		if (test_marker->fb.state != STATE_BOTTOM) {
			return 1;
		}
	}
	if (description & ROCKET_JUMP) {
		if (self->fb.willRocketJumpThisTic) {
			VectorAdd(touch_marker_->s.v.absmin, touch_marker_->s.v.view_ofs, m_pos);
			VectorAdd(test_marker->s.v.absmin, test_marker->s.v.view_ofs, m_P_pos);
			path_time = ((VectorDistance(m_P_pos, m_pos) / sv_maxspeed));
		}
	}
	return 0;
}

float checkboven() {
	traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 140, TRUE, self);
	return (g_globalvars.trace_fraction == 1);
}

float checkground() {
	trap_makevectors(self->s.v.v_angle);
	g_globalvars.v_forward[2] = 0;
	VectorNormalize(g_globalvars.v_forward);
	VectorScale(g_globalvars.v_forward, 10, g_globalvars.v_forward);
	traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0] + g_globalvars.v_forward[0], self->s.v.origin[1] + g_globalvars.v_forward[1], self->s.v.origin[2] + g_globalvars.v_forward[2] -40, TRUE, self);
	return (g_globalvars.trace_fraction != 1);
}

void lava_jump() {
	gedict_t *e,
	         *pt;
	float bdist,
	      byaw;
	bdist = 1001;
	pt = self;
	for (e = world; e = trap_findradius(e, e->s.v.origin, 1000); world) {
		if (streq(e->s.v.classname, "marker")) {
			if (VectorDistance(t->s.v.origin, e->s.v.origin) < bdist) {
				bdist = VectorDistance(t->s.v.origin, e->s.v.origin);
				pt = e;
			}
		}
		e = &g_edicts[e->s.v.chain];
	}
	byaw = vectoyaw(t->s.v.origin);
	self->fb.real_yaw = (360 - byaw);
	self->fb.yawaccel = 0;
	self->fb.yawspeed = 0;
	if (self->s.v.waterlevel == 3) {
		self->fb.real_pitch = 78.75;
		new_pitch = 78.75;
		self->fb.pitchaccel = 0;
		self->fb.pitchspeed = 0;
		self->fb.arrow = BACK;
		VelocityForArrow();
	}
	if (self->s.v.waterlevel == 2) {
		if (self->fb.arrow == BACK) {
			self->fb.real_pitch = 78.75;
			new_pitch = 78.75;
			self->fb.pitchaccel = 0;
			self->fb.pitchspeed = 0;
			self->fb.arrow = BACK;
			VelocityForArrow();
			self->fb.rocketjumping = 1;
			self->fb.botchose = 1;
			self->s.v.impulse = 7;
			self->fb.button0_ = 1;
			self->fb.up_finished = g_globalvars.time + 0.1;
		}
		else  {
			if (g_globalvars.time > self->fb.up_finished) {
				self->fb.swim_arrow = DOWN;
				VelocityForArrow();
			}
		}
	}
}

void a_rocketjump() {
	self->fb.rocketjumping = 0;
	if (pre_game && counting_down) {
		return;
	}
	if (!self->s.v.ammo_rockets) {
		return;
	}
	if (!((int)self->s.v.items & IT_ROCKET_LAUNCHER)) {
		return;
	}
	if (self->s.v.waterlevel > 1) {
		vec3_t point = { self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] - 24 };
		if (trap_pointcontents(point[0], point[1], point[2]) == CONTENT_LAVA) {
			if (checkboven()) {
				lava_jump();
				return;
			}
		}
	}
	if (!self->fb.willRocketJumpThisTic) {
		return;
	}
	if ((int)self->s.v.items & IT_QUAD) {
		if (!((int)self->s.v.items & IT_INVULNERABILITY)) {
			return;
		}
	}
	if (!((int)self->fb.path_state & ROCKET_JUMP)) {
		return;
	}
	if (self->fb.button0_) {
		return;
	}
	if (self->fb.button2_) {
		return;
	}
	if (near_teammate(self)) {
		return;
	}
	if (self->fb.attack_finished > g_globalvars.time) {
		return;
	}
	if (!((int)self->s.v.flags & FL_ONGROUND)) {
		return;
	}
	if (VectorDistance(self->s.v.origin, self->fb.touch_marker->s.v.origin) > 100) {
		return;
	}
	if (!checkboven()) {
		return;
	}
	if (!checkground()) {
		return;
	}
	if (!right_direction()) {
		return;
	}
	BestArrowForDirection();
	VelocityForArrow();
	new_pitch = 78.75;
	self->fb.real_pitch = 78.75;
	self->fb.pitchspeed = 0;
	self->fb.pitchaccel = 0;
	self->fb.rocketjumping = 1;
	self->fb.botchose = 1;
	self->s.v.impulse = 7;
	self->fb.button0_ = 1;
	self->fb.button2_ = 1;
}

void CheckCombatJump() {
	if (game_botjump) {
		if (self->fb.frogbot) {
			if (!self->s.v.waterlevel) {
				if (self->fb.allowedMakeNoise) {
					if ((int)self->s.v.flags & FL_ONGROUND) {
						if (self->s.v.weapon != IT_LIGHTNING) {
							if (look_object_ == enemy_) {
								if (random() < 0.2) {
									if (!self->fb.rocketjumping) {
										self->fb.button2_ = 1;
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

void BotInLava() {
	if (streq(g_globalvars.mapname, "amphi2")) {
		if (self->fb.frogbot) {
			if (g_globalvars.time > self->fb.arrow_time) {
				if (self->s.v.waterlevel == 1) {
					vec3_t point = { self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] - 24 };
					if (trap_pointcontents(point[0], point[1], point[2]) == CONTENT_LAVA) {
						if ((int)self->s.v.flags & FL_ONGROUND) {
							if (!enemy_shaft_attack()) {
								if (!self->fb.rocketjumping) {
									BestArrowForDirection();
									VelocityForArrow();
									self->fb.button2_ = 1;
								}
							}
						}
					}
				}
			}
		}
	}
}

