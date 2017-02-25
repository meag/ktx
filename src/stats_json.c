
#include "g_local.h"
#include "stats.h"

#define STATS_VERSION_NUMBER 3

static char* json_string(const char* input)
{
	// >>>> like va(...) ... eugh
	static char	string[MAX_STRINGS][1024];
	static int index = 0;
	char *ch, *start;

	index %= MAX_STRINGS;
	// <<<<

	start = ch = string[index];
	while (*input) {
		if (ch - start >= 1000) {
			break;
		}

		if (*input == '\\' || *input == '"') {
			*ch++ = '\\';
			*ch++ = *input;
		}
		else if (*input == '\n') {
			*ch++ = '\\';
			*ch++ = 'n';
		}
		else if (*input == '\r') {
			*ch++ = '\\';
			*ch++ = 'r';
		}
		else if (*input == '\b') {
			*ch++ = '\\';
			*ch++ = 'b';
		}
		else if (*input == '\t') {
			*ch++ = '\\';
			*ch++ = 't';
		}
		else if (*input == '\f') {
			*ch++ = '\\';
			*ch++ = 'f';
		}
		else if (*input < ' ') {
			*ch++ = '\\';
			*ch++ = 'u';
			*ch++ = '0';
			*ch++ = '0';
			if (*input < 16) {
				*ch++ = '0';
				*ch++ = "0123456789ABCDEF"[*input];
			}
			else {
				*ch++ = "0123456789ABCDEF"[(*input) >> 4];
				*ch++ = "0123456789ABCDEF"[(*input) & 15];
			}
		}
		else {
			*ch++ = *input;
		}
		++input;
	}
	*ch = '\0';
	return start;
}

static void json_weap_header(fileHandle_t handle)
{
	s2di(handle, "      \"weapons\": {\n");
}

static void json_weap_detail(fileHandle_t handle, int weapon_num, wpType_t* stats)
{
	if (weapon_num) {
		s2di(handle, ",\n");
	}
	s2di(handle, "        \"%s\": [%d, %d, %d, %d, %d, %d, %d, %d, %d]",
		json_string(WpName(weapon_num + 1)), stats->hits, stats->attacks, stats->kills, stats->deaths, 
		stats->tkills, stats->ekills, stats->drops, stats->tooks, stats->ttooks
	);
}

static void json_weap_footer(fileHandle_t handle, int num)
{
	if (num) {
		s2di(handle, "\n");
	}
	s2di(handle, "      },\n");
}

void json_teams_header(fileHandle_t handle)
{
	char tmp[1024] = { 0 };
	int i = 0;

	for (tmp[0] = i = 0; i < min(tmStats_cnt, MAX_TM_STATS); i++) {
		if (i) {
			strlcat(tmp, ", ", sizeof(tmp));
		}
		strlcat(tmp, "\"", sizeof(tmp));
		strlcat(tmp, json_string(tmStats[i].name), sizeof(tmp));
		strlcat(tmp, "\"", sizeof(tmp));
	}

	if (i) {
		s2di(handle, "  \"teams\": [%s],\n", tmp);
	}
}

void json_teams_footer(fileHandle_t handle, int teams)
{
}

void json_team_detail(fileHandle_t handle, int num, teamStats_t* stats)
{
}

static void json_team_footer(fileHandle_t handle)
{
}

static void json_items_header(fileHandle_t handle)
{
	s2di(handle, "      \"items\": {\n");
}

static void json_items_detail(fileHandle_t handle, int item_num, itType_t* stats)
{
	if (item_num) {
		s2di(handle, ",\n");
	}
	s2di(handle, "        \"%s\": [%d, %d]", json_string(ItName(item_num+1)), stats->tooks, (int)stats->time);
}

static void json_items_footer(fileHandle_t handle, int item_num)
{
	if (item_num) {
		s2di(handle, "\n");
	}
	s2di(handle, "      }\n");
}

void json_players_header(fileHandle_t handle)
{
	s2di(handle, "  \"players\": [\n");
}

void json_players_footer(fileHandle_t handle, int player_count)
{
	if (player_count) {
		s2di(handle, "\n");
	}
	s2di(handle, "  ]\n");
}

void json_player_detail(fileHandle_t handle, gedict_t* player, char* team)
{
	s2di(handle, "    {\n");
	s2di(handle, "      \"name\": \"%s\",\n", json_string(getname(player)));
	s2di(handle, "      \"team\": \"%s\",\n", json_string(team));
	s2di(handle, "      \"stats\": [%d, %d, %d, %d],\n", (int)player->s.v.frags, (int)player->deaths, (int)player->friendly, player->ps.spawn_frags);
	s2di(handle, "      \"dmg\": [%d, %d, %d],\n", (int)player->ps.dmg_t, (int)player->ps.dmg_g, (int)player->ps.dmg_team);
	s2di(handle, "      \"xfer\": %d,\n", player->ps.transferred_packs);
	s2di(handle, "      \"spree\": [%d %d],\n", player->ps.spree_max, player->ps.spree_max_q);
	s2di(handle, "      \"control\": %f,\n", player->ps.control_time);
}

void json_match_header(fileHandle_t handle, char* ip, int port)
{
	char date[64] = { 0 };
	char matchtag[64] = { 0 };
	const char* mode = cvar("k_instagib") ? "instagib" : (isRACE() ? "race" : GetMode());

	infokey(world, "matchtag", matchtag, sizeof(matchtag));

	if (!QVMstrftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S %Z", 0))
		date[0] = 0; // bad date

	s2di(handle, "{\n");
	s2di(handle, "  \"version\": %d\n", STATS_VERSION_NUMBER);
	s2di(handle, "  \"date\": \"%s\",\n", date);
	s2di(handle, "  \"map\": \"%s\",\n", json_string(g_globalvars.mapname));
	s2di(handle, "  \"hostname\": \"%s\",\n", json_string(striphigh(cvar_string("hostname"))));
	s2di(handle, "  \"ip\": \"%s\",\n", json_string(ip));
	s2di(handle, "  \"port\": %d,\n", port);
	if (!strnull(matchtag)) {
		s2di(handle, "  \"matchtag\": \"%s\",\n", json_string(matchtag));
	}
	if (timelimit) {
		s2di(handle, "  \"tl\": %d,\n", timelimit);
	}
	if (fraglimit) {
		s2di(handle, "  \"fl\": %d,\n", timelimit);
	}
	if (deathmatch) {
		s2di(handle, "  \"dm\": %d,\n", deathmatch);
	}
	if (teamplay) {
		s2di(handle, "  \"tp\": %d,\n", teamplay);
	}
	if (!strnull(cvar_string("serverdemo"))) {
		s2di(handle, "  \"demo\": \"%s\",\n", json_string(cvar_string("serverdemo")));
	}
}

void json_match_footer(fileHandle_t handle)
{
	s2di(handle, "}\n");
}

static void json_player_ctf_stats(fileHandle_t handle, player_stats_t* stats)
{
	s2di(handle, "      \"ctf\": {\n");
	s2di(handle, "        \"points\": %d,\n", stats->ctf_points);
	s2di(handle, "        \"caps\": %d,\n", stats->caps);
	s2di(handle, "        \"defends\": [%d, %d],\n", stats->f_defends, stats->c_defends);
	s2di(handle, "        \"frags\": %d,\n", stats->c_frags);
	s2di(handle, "        \"pickups\": %d,\n", stats->pickups);
	s2di(handle, "        \"returns\": %d,\n", stats->returns);
	s2di(handle, "        \"runes\": [%d %d %d %d]\n", (int)(stats->res_time + 0.5f), (int)(stats->str_time + 0.5f), (int)(stats->hst_time + 0.5f), (int)(stats->rgn_time + 0.5f));
	s2di(handle, "      }\n");
}

static void json_player_instagib_stats(fileHandle_t handle, player_stats_t* stats)
{
	s2di(handle, "    \"instagib\": {\n");
	s2di(handle, "      \"height\": [%d, %d]\n", stats->i_height, stats->i_maxheight);
	s2di(handle, "      \"gibs\": [%d, %d, %d, %d, %d, %d, %d],\n", stats->i_cggibs, stats->i_axegibs, stats->i_stompgibs, stats->i_multigibs, stats->i_airgibs, stats->i_maxmultigibs);
	s2di(handle, "      \"rings\": %d\n", stats->i_rings);
	s2di(handle, "    }\n");
}

static void json_player_midair_stats(fileHandle_t handle, player_stats_t* stats)
{
	s2di(handle, "    \"midair\": {\n");
	s2di(handle, "      \"stomps\": %d,\n", stats->mid_stomps);
	s2di(handle, "      \"midairs\": [%d, %d, %d, %d],\n", stats->mid_bronze, stats->mid_silver, stats->mid_gold, stats->mid_platinum);
	s2di(handle, "      \"total\": %d,\n", stats->mid_total);
	s2di(handle, "      \"bonus\": %d,\n", stats->mid_bonus);
	s2di(handle, "      \"heights\": [%f, %f, %f]\n", stats->mid_totalheight, stats->mid_maxheight, stats->mid_avgheight);
	s2di(handle, "    }\n");
}

static void json_player_ra_stats(fileHandle_t handle, player_stats_t* stats)
{
	s2di(handle, "    \"ra\": [%d, %d]\n", stats->wins, stats->loses);
}

void json_race_detail(fileHandle_t handle)
{
	extern gedict_t* race_find_racer(gedict_t* p);
	qbool first = true;
	gedict_t* p;

	s2di(handle, "  \"race\": {\n");
	s2di(handle, "    \"route\": %d,\n", race.active_route);
	s2di(handle, "    \"modes\": [%d, %d],\n", race.weapon, race.falsestart);
	if (!strnull(race.pacemaker_nick)) {
		s2di(handle, "    \"pacemaker\": { \"time\": %f, \"name\": \"%s\" },\n", race.pacemaker_time * 0.001f, json_string(race.pacemaker_nick));
	}
	s2di(handle, "    \"racers\": [\n");
	for (p = world; p = race_find_race_participants(p); /**/) {
		int player_number = NUM_FOR_EDICT(p) - 1;
		raceRecord_t* record = NULL;
		if (player_number < 0 || player_number >= sizeof(race.currentrace) / sizeof(race.currentrace[0])) {
			continue;
		}
		record = &race.currentrace[player_number];

		if (!first) {
			s2di(handle, ",\n");
		}
		s2di(handle, "      { \"avgspeed\": %f, \"distance\": %f, \"time\": %f, "
			"\"racer\": \"%s\" \"maxspeed\": %f }",
			record->avgspeed / record->avgcount, record->distance, record->time,
			striphigh(p->s.v.netname), record->maxspeed
		);
	}
	if (!first) {
		s2di(handle, "\n");
	}
	s2di(handle, "    ]\n");
	s2di(handle, "  }\n");
}
