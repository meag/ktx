// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

float EntVisible(vec3_t vec) {
	traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], vec[0], vec[1], vec[2], TRUE, self);
	if ((g_globalvars.trace_fraction == 1) && !(g_globalvars.trace_inopen && g_globalvars.trace_inwater)) {
		return TRUE;
	}
	return FALSE;
}

float IsVisible(gedict_t* ent) {
	vec3_t vec;
	if (EntVisible(ent->s.v.origin)) {
		return TRUE;
	}
	VectorCopy(ent->s.v.origin, vec);
	vec[2] = ent->s.v.absmin[2];
	if (EntVisible(vec)) {
		return TRUE;
	}
	vec[2] = ent->s.v.absmax[2];
	if (EntVisible(vec)) {
		return TRUE;
	}
	return FALSE;
}

float VisibleEntity(gedict_t* ent) {
	vec3_t vec;
	if (EntVisible(ent->s.v.origin)) {
		return TRUE;
	}
	VectorCopy(ent->s.v.origin, vec);
	vec[2] = ent->s.v.absmin[2];
	if (EntVisible(vec)) {
		return TRUE;
	}
	vec[2] = ent->s.v.absmax[2];
	return EntVisible(vec);
}

gedict_t* identify_teammate_(gedict_t* me) {
	gedict_t* p, *g;
	float closeness;
	vec3_t diff, point;
	float currclose;

	closeness = -1;
	p = find(world, FOFS(fb.stringname), "gamer");
	while (p && p != world) {
		if (me->fb.realteam == p->fb.realteam) {
			VectorSubtract(p->s.v.origin, me->s.v.origin, diff);
			VectorNormalize(diff);
			normalize(me->s.v.angles, point);
			VectorSubtract(diff, point, diff);
			currclose = vlen(diff);
			traceline(me->s.v.origin[0], me->s.v.origin[1], me->s.v.origin[2], p->s.v.origin[0], p->s.v.origin[1], p->s.v.origin[2], FALSE, me);
			if (g_globalvars.trace_ent == NUM_FOR_EDICT(p)) {
				if (closeness == -1) {
					closeness = currclose;
					g = p;
				}
				else if (currclose < closeness) {
					closeness = currclose;
					g = p;
				}
			}
		}
		p = find(p, FOFS(fb.stringname), "gamer");
	}
	if (g != world) {
		return g;
	}
	return world;
}
/*
float anglemod(float v) {
	while (v >= 360) {
		v = v - 360;
	}
	while (v < 0) {
		v = v + 360;
	}
	return v;
}
*/
float visible_teammate(gedict_t* me) {
	float ang,
	      curang;
	gedict_t* p;

	if (!teamplay || (teamplay && (healthplay == TEAM_TOTAL_HEALTH_PROTECT))) {
		return 0;
	}
	p = find(world, FOFS(fb.stringname), "gamer");
	while (p && p != world) {
		if (p != me) {
			if (me->fb.realteam == p->fb.realteam) {
				if (VisibleEntity(p)) {
					vec3_t diff;
					VectorSubtract(p->s.v.origin, me->s.v.origin, diff);
					curang = vectoyaw(diff);
					ang = anglemod(me->s.v.angles[1] - curang);
					if (ang < 20 || ang > 340) {
						return 1;
					}
				}
			}
		}
		p = find(p, FOFS(fb.stringname), "gamer");
	}
	return 0;
}

float near_teammate(gedict_t* me) {
	gedict_t* p;

	if (!teamplay || (teamplay && (healthplay == TEAM_TOTAL_HEALTH_PROTECT))) {
		return 0;
	}
	p = identify_teammate_(me);
	return ((VectorDistance(p->s.v.origin, me->s.v.origin)) < 140);
}

gedict_t* HelpTeammate() {
	gedict_t* goalent;
	gedict_t* head, *selected1, *selected2;
	float d,
	      bdist,
	      best_dist1,
	      best_dist2;

	if (!teamplay) {
		return world;
	}
	if (self->fb.state & WAIT) {
		return world;
	}
	if (self->fb.state & RUNAWAY) {
		return world;
	}
	if (self->fb.state & NOTARGET_ENEMY) {
		return world;
	}

	goalent = &g_edicts[self->s.v.goalentity];
	if (goalent->s.v.goalentity == NUM_FOR_EDICT(self)) {
		return world;
	}
	if ((goalent->fb.client_) && (goalent != self)) {
		if (goalent->fb.realteam == self->fb.realteam) {
			if ((goalent->s.v.health < 30) && !((int)goalent->s.v.items & IT_INVULNERABILITY) && (goalent->s.v.waterlevel == 0)) {
				if (((int)self->s.v.items & (IT_ROCKET_LAUNCHER | IT_LIGHTNING)) && (self->s.v.health > 65)) {
					if ((self->s.v.ammo_rockets > 2) || (self->s.v.ammo_cells > 10)) {
						if (IsVisible(goalent)) {
							self->fb.state = self->fb.state | HELP_TEAMMATE;
							return goalent;
						}
					}
				}
			}
		}
	}
	bdist = 500;
	if (g_globalvars.time < self->fb.help_teammate_time) {
		return world;
	}
	self->fb.help_teammate_time = g_globalvars.time + 20 + 3 * random();
	selected1 = world;
	selected2 = world;
	best_dist1 = 99999999;
	best_dist2 = 99999999;
	for (head = world; head = trap_findradius(head, self->s.v.origin, bdist); ) {
		if (head->fb.client_) {
			if (head->fb.realteam == self->fb.realteam) {
				if (head != self) {
					d = VectorDistance(head->s.v.origin, self->s.v.origin);
					if (NUM_FOR_EDICT(self) != head->s.v.goalentity) {
						if (d < best_dist1) {
							if (VisibleEntity(head) && !((int)head->s.v.items & IT_INVULNERABILITY) && (head->s.v.health < 40) && (head->s.v.armorvalue < 11) && (head->s.v.waterlevel == 0)) {
								if ((int)self->s.v.items & (IT_ROCKET_LAUNCHER | IT_LIGHTNING)) {
									if ((self->s.v.ammo_cells > 10) || (self->s.v.ammo_rockets > 2)) {
										selected1 = head;
										self->fb.state = self->fb.state | HELP_TEAMMATE;
										best_dist1 = d;
									}
								}
							}
						}
					}
					if (head->fb.realteam == self->fb.realteam) {
						if (NUM_FOR_EDICT(self) != head->s.v.goalentity) {
							if (d < best_dist2) {
								if (VisibleEntity(head) && !((int)head->s.v.items & IT_INVULNERABILITY) && (head->s.v.health < 30) && (head->s.v.armorvalue < 20) && (head->s.v.waterlevel == 0)) {
									if ((int)self->s.v.items & (IT_ROCKET_LAUNCHER | IT_LIGHTNING)) {
										if ((self->s.v.ammo_cells > 10) || (self->s.v.ammo_rockets > 2)) {
											selected2 = head;
											self->fb.state = self->fb.state | HELP_TEAMMATE;
											best_dist2 = d;
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
	if (selected1 != world) {
		return selected1;
	}
	else if (selected2 != world) {
		return selected2;
	}
	return world;
}

