// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

static int marker_index = 0;
#define MAX_TXTLEN	128

void AddToQue(gedict_t* ent) {
	markers[marker_index] = ent;
	ent->fb.index = marker_index++;

	ent->fb.zone_marker = Z_marker_error;
	ent->fb.sub_arrival_time = S_time_error;
	ent->fb.sub_path_marker = S_path_marker_error;
	ent->fb.path_marker = Z_path_marker_error;
	ent->fb.next_zone_marker = Z_next_zone_marker_error;
	ent->fb.sight_from_marker = Z_sight_from_marker_error;
	ent->fb.higher_sight_from_marker = Z_higher_sight_from_marker_error;
	ent->fb.sight_from_time = Z_sight_from_time_error;
}

void CreateMarker(float x, float y, float z) {
	vec3_t org = { x, y, z };
	spawn_marker(org);
	self = marker_;
	AddToQue(marker_);
}

void AllMarkersLoaded() {
	self = NULL;
	m_zone = zone_stack_head;
	path_normal = true;

	InitialiseMarkerRoutes();
}

void SetMarkerPathFlags(int marker_number, int path_index, int flags) {
	--marker_number;

	if (marker_number < 0 || marker_number >= NUMBER_MARKERS) {
		return;
	}

	markers[marker_number]->fb.paths[path_index].flags = flags;
}

void SetMarkerPath(int source_marker, int path_index, int next_marker) {
	--source_marker;
	--next_marker;

	if (source_marker < 0 || source_marker >= NUMBER_MARKERS) {
		return;
	}
	if (next_marker < 0 || next_marker >= NUMBER_MARKERS) {
		return;
	}

	if (source_marker == 32)
		Com_Printf ("SetMarkerPath(%d, %d, %d) = %d\n", source_marker, path_index, next_marker, markers[next_marker] == 0 ? -1 : markers[next_marker]->fb.index);
	markers[source_marker]->fb.paths[path_index].next_marker = markers[next_marker];
}

void SetMarkerViewOffset (int marker, float zOffset)
{
	--marker;

	if (marker < 0 || marker >= NUMBER_MARKERS || markers[marker] == 0) {
		return;
	}

	markers[marker]->s.v.view_ofs[2] = zOffset;
}

fileHandle_t std_fropen( const char *fmt, ... )
{
	va_list      argptr;
	fileHandle_t handle;
	char         text[MAX_TXTLEN] = {0};

	va_start( argptr, fmt );
	Q_vsnprintf( text, sizeof(text), fmt, argptr );
	va_end( argptr );

	text[sizeof(text)-1] = 0;

	if ( trap_FS_OpenFile( text, &handle, FS_READ_BIN ) < 0 )
	{
		//G_bprint( 2, "Failed to open file: %s\n", text );
		return -1;
	}

	//G_bprint( 2, "Succesfully opened file: %s\n", text );
	return handle;
}

int std_fgetc( fileHandle_t handle )
{
	char c;
	int retval;

	if ( handle < 0 )
		return -2;

	retval = trap_FS_ReadFile( &c, 1, handle );
	//G_bprint( 2, "====> Read char: %d\n", c );

	return ( retval == 1 ? c : -1 );
}

char *std_fgets( fileHandle_t handle, char *buf, int limit )
{
	int c = '\0';
	char *string;

	if ( handle < 0 )
		return NULL;

	string = buf;
	while ( --limit > 0 && ( ( c = std_fgetc( handle ) ) != -1 ) )
		if ( ( *string++ = c ) == '\n' )
			break;
	*string = '\0';
	//G_bprint( 2, "====> Read string: %s\n", buf );

	return ( ( c == -1 ) && ( string = buf ) ) ? NULL : buf;
}

void std_fclose( fileHandle_t handle )
{
	if ( handle < 0 )
		return;

	trap_FS_CloseFile( handle );
}

qbool LoadBotRoutingFromFile (void)
{
	fileHandle_t file = -1;
	char lineData[128];
	char argument[128];
	
	// Load bot definition file: frogbots rely on objects spawning 
	//    markers, so be aware of alternative .ent files
	char* entityFile = cvar_string ("k_entityfile");
	if (!strnull (entityFile)) {
		file = std_fropen ("maps/%s.bot", entityFile);
		if (file == -1) {
			file = std_fropen ("bots/maps/%s.bot", entityFile);
		}
	}
	
	if (file == -1) {
		file = std_fropen ("maps/%s.bot", g_globalvars.mapname);
		if (file == -1) {
			file = std_fropen ("bots/maps/%s.bot", g_globalvars.mapname);
		}
	}
	
	if (file == -1) {
		return false;
	}

	while (std_fgets (file, lineData, sizeof (lineData))) {
		trap_CmdTokenize (lineData);

		trap_CmdArgv (0, argument, sizeof (argument));

		Com_Printf ("> %s\n", lineData);
		if (strnull (argument) || (argument[0] == '/' && argument[1] == '/'))
			continue;

		if (streq (argument, "CreateMarker")) {
			// CreateMarker %f %f %f
			float x, y, z;

			if (trap_CmdArgc () != 4)
				continue;

			trap_CmdArgv (1, argument, sizeof (argument));
			x = atof (argument);
			trap_CmdArgv (2, argument, sizeof (argument));
			y = atof (argument);
			trap_CmdArgv (3, argument, sizeof (argument));
			z = atof (argument);

			CreateMarker (x, y, z);
		}
		else if (streq (argument, "SetGoal")) {
			// SetGoal %d %d
			int marker, goal;

			if (trap_CmdArgc () != 3)
				continue;

			trap_CmdArgv (1, argument, sizeof (argument));
			goal = atoi (argument);
			trap_CmdArgv (2, argument, sizeof (argument));
			marker = atoi (argument);

			SetGoal (goal, marker);
		}
		else if (streq (argument, "SetZone")) {
			// SetZone %d %d
			int marker, zone;

			if (trap_CmdArgc () != 3)
				continue;

			trap_CmdArgv (1, argument, sizeof (argument));
			zone = atoi (argument);
			trap_CmdArgv (2, argument, sizeof (argument));
			marker = atoi (argument);

			SetZone (zone, marker);
		}
		else if (streq (argument, "SetMarkerPath")) {
			// SetMarkerPath %d %d %d
			int source_marker, path_number, next_marker;

			if (trap_CmdArgc () != 4)
				continue;

			trap_CmdArgv (1, argument, sizeof (argument));
			source_marker = atoi (argument);
			trap_CmdArgv (2, argument, sizeof (argument));
			path_number = atoi (argument);
			trap_CmdArgv (3, argument, sizeof (argument));
			next_marker = atoi (argument);

			SetMarkerPath (source_marker, path_number, next_marker);
		}
		else if (streq (argument, "SetMarkerPathFlags")) {
			// SetMarkerPathFlags %d %d %s
			int source_marker, path_number, path_flags;
			size_t i;

			if (trap_CmdArgc () != 4)
				continue;

			trap_CmdArgv (1, argument, sizeof (argument));
			source_marker = atoi (argument);
			trap_CmdArgv (2, argument, sizeof (argument));
			path_number = atoi (argument);
			trap_CmdArgv (3, argument, sizeof (argument));
			path_flags = 0;
			for (i = 0; i < strlen (argument); ++i) {
				switch (argument[i]) {
				case 'w':
					path_flags |= WATERJUMP_; break;
				case '6':
					path_flags |= DM6_DOOR; break;
				case 'r':
					path_flags |= ROCKET_JUMP; break;
				case 'j':
					path_flags |= JUMP_LEDGE; break;
				}
			}

			SetMarkerPathFlags (source_marker, path_number, path_flags);
		}
		else if (streq (argument, "SetMarkerFlag")) {
			// SetMarkerFlag %d %s
			int source_marker, marker_flags;
			size_t i;

			if (trap_CmdArgc () != 3)
				continue;

			trap_CmdArgv (1, argument, sizeof (argument));
			source_marker = atoi (argument);
			trap_CmdArgv (2, argument, sizeof (argument));
			marker_flags = 0;
			for (i = 0; i < strlen (argument); ++i) {
				switch (argument[i]) {
				case 'u':
					marker_flags |= UNREACHABLE; break;
				case '6':
					marker_flags |= MARKER_IS_DM6_DOOR; break;
				}
			}

			SetMarkerFlag (source_marker, marker_flags);
		}
		else if (streq (argument, "SetMarkerViewOfs")) {
			// SetMarkerViewOfs %d %f
			int source_marker;
			float offset;

			if (trap_CmdArgc () != 3)
				continue;

			trap_CmdArgv (1, argument, sizeof (argument));
			source_marker = atoi (argument);
			trap_CmdArgv (2, argument, sizeof (argument));
			offset = atof (argument);

			SetMarkerViewOffset (source_marker, offset);
		}
		else if (streq (argument, "SetZoneName")) {
			/*
			// SetZoneName %d %s
			int source_marker;

			if (trap_CmdArgc () != 3)
				continue;

			trap_CmdArgv (1, argument, sizeof (argument));
			source_marker = atoi (argument);
			trap_CmdArgv (2, argument, sizeof (argument));

			NameZone (source_marker, argument);
			*/
		}
	}

	std_fclose( file );
	return true;
}