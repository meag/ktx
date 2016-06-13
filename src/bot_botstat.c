// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

// TODO: Call this every time the player's statistics change (item pickups etc)
void UpdateTotalDamage(gedict_t* client) {
	float min_first = client->s.v.health / (1 - client->s.v.armortype);
	float min_second = client->s.v.health + client->s.v.armorvalue;
	client->fb.total_armor = client->s.v.armortype * client->s.v.armorvalue;
	client->fb.total_damage = min (min_first, min_second);

	// 160 = 200RA
	if (client->fb.total_armor == 160) {
		client->fb.desire_armor1 = client->fb.desire_armor2 = client->fb.desire_armorInv = 0;
	}
	else  {
		min_first = client->s.v.health / 0.2;
		min_second = client->s.v.health + 200;
		if (min_first <= min_second) {
			client->fb.desire_armorInv = min_first - client->fb.total_damage;
		}
		else  {
			client->fb.desire_armorInv = min_second - client->fb.total_damage;
		}
		if (client->fb.total_armor >= 90) {
			client->fb.desire_armor1 = client->fb.desire_armor2 = 0;
		}
		else  {
			min_first = client->s.v.health / 0.4;
			min_second = client->s.v.health + 150;
			if (min_first <= min_second) {
				client->fb.desire_armor2 = min_first - client->fb.total_damage;
			}
			else  {
				client->fb.desire_armor2 = min_second - client->fb.total_damage;
			}
			if (client->fb.desire_armor2 < 0) {
				client->fb.desire_armor2 = 0;
			}
			if (client->fb.total_armor >= 30) {
				client->fb.desire_armor1 = 0;
			}
			else  {
				min_first = client->s.v.health / 0.7;
				min_second = client->s.v.health + 100;
				if (min_first <= min_second) {
					client->fb.desire_armor1 = 2 * (min_first - client->fb.total_damage);
				}
				else  {
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
			else  {
				new_health = 100;
			}
			min_first = new_health / (1 - client->s.v.armortype);
			min_second = new_health + client->s.v.armorvalue;
			if (min_first <= min_second) {
				client->fb.desire_health0 = 2 * (min_first - client->fb.total_damage);
			}
			else  {
				client->fb.desire_health0 = 2 * (min_second - client->fb.total_damage);
			}
		}
		else  {
			client->fb.desire_health0 = 0;
		}
	}
	else  {
		client->fb.desire_health0 = client->fb.desire_health2 = 0;
	}

	if ((int)client->ctf_flag & CTF_RUNE_RES) {
		client->fb.total_damage *= 2;
	}
}

