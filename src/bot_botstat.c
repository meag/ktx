// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

// TODO: Call this every time the player's statistics change (item pickups etc)
void UpdateTotalDamage(gedict_t* client)
{
	float min_first = client->s.v.health / (1 - client->s.v.armortype);
	float min_second = client->s.v.health + client->s.v.armorvalue;
	client->fb.total_armor = client->s.v.armortype * client->s.v.armorvalue;
	client->fb.total_damage = min (min_first, min_second);

	// 160 = 200RA
	if (client->fb.total_armor == 160) {
		client->fb.desire_armor1 = client->fb.desire_armor2 = client->fb.desire_armorInv = 0;
	}
	else {
		min_first = client->s.v.health / 0.2;
		min_second = client->s.v.health + 200;
		if (min_first <= min_second) {
			client->fb.desire_armorInv = min_first - client->fb.total_damage;
		}
		else {
			client->fb.desire_armorInv = min_second - client->fb.total_damage;
		}
		if (client->fb.total_armor >= 90) {
			client->fb.desire_armor1 = client->fb.desire_armor2 = 0;
		}
		else {
			min_first = client->s.v.health / 0.4;
			min_second = client->s.v.health + 150;
			if (min_first <= min_second) {
				client->fb.desire_armor2 = min_first - client->fb.total_damage;
			}
			else {
				client->fb.desire_armor2 = min_second - client->fb.total_damage;
			}
			if (client->fb.desire_armor2 < 0) {
				client->fb.desire_armor2 = 0;
			}
			if (client->fb.total_armor >= 30) {
				client->fb.desire_armor1 = 0;
			}
			else {
				min_first = client->s.v.health / 0.7;
				min_second = client->s.v.health + 100;
				if (min_first <= min_second) {
					client->fb.desire_armor1 = 2 * (min_first - client->fb.total_damage);
				}
				else {
					client->fb.desire_armor1 = 2 * (min_second - client->fb.total_damage);
				}
				if (client->fb.desire_armor1 < 0) {
					client->fb.desire_armor1 = 0;
				}
			}
		}
	}
	if (client->s.v.health < 250) {
		float new_health = min (client->s.v.health + 100, 250);
		min_first = new_health / (1 - client->s.v.armortype);
		min_second = new_health + client->s.v.armorvalue;
		client->fb.desire_health2 = min (min_first, min_second) - client->fb.total_damage;

		if (client->s.v.health < 100) {
			if (client->s.v.health < 75) {
				new_health = client->s.v.health + 25;
			}
			else {
				new_health = 100;
			}
			min_first = new_health / (1 - client->s.v.armortype);
			min_second = new_health + client->s.v.armorvalue;
			if (min_first <= min_second) {
				client->fb.desire_health0 = 2 * (min_first - client->fb.total_damage);
			}
			else {
				client->fb.desire_health0 = 2 * (min_second - client->fb.total_damage);
			}
		}
		else {
			client->fb.desire_health0 = 0;
		}
	}
	else {
		client->fb.desire_health0 = client->fb.desire_health2 = 0;
	}

	if ((int)client->ctf_flag & CTF_RUNE_RES) {
		client->fb.total_damage *= 2;
	}
}

void UpdateWeapons(gedict_t* self)
{
	int items_ = (int) self->s.v.items;
	float firepower_ = 100.0f;
	int attackbonus = 0;

	self->fb.weapon_refresh_time = 1000000;
	if (deathmatch != 4) {
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
			else {
				firepower_ = firepower_ + self->s.v.ammo_shells * 0.4;
			}
		}
		else {
			if (self->s.v.ammo_shells >= 25) {
				firepower_ = firepower_ + 10;
			}
			else {
				firepower_ = firepower_ + self->s.v.ammo_shells * 0.4;
			}
		}
		firepower_ = min (firepower_, 100);

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
