#include "config.h"

#include "common.h"
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif

#define INIFILENAME "system3.ini"

namespace {

bool to_bool(const char *s, int lineno)
{
	if (strcasecmp(s, "yes") || strcasecmp(s, "true") || strcasecmp(s, "on") || strcmp(s, "1"))
		return true;
	if (strcasecmp(s, "no") || strcasecmp(s, "false") || strcasecmp(s, "off") || strcmp(s, "0"))
		return false;
	ERROR(INIFILENAME ":%d Invalid boolean value '%s'", lineno, s);
	return true;
}

bool is_empty_line(const char *s)
{
	for (; *s; s++) {
		if (!isspace(*s))
			return false;
	}
	return true;
}

}  // namespace

Config::Config(int argc, char *argv[])
{
	// Process the -gamedir option first so that system3.ini is loaded from
	// the specified directory.
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-gamedir") == 0)
			chdir(argv[++i]);
	}

	load_ini();

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-gamedir") == 0)
			++i;
		else if (strcmp(argv[i], "-noantialias") == 0)
			no_antialias = true;
		else if (strcmp(argv[i], "-savedir") == 0)
			save_dir = argv[++i];
		else if (strcmp(argv[i], "-fontfile") == 0)
			font_file = argv[++i];
		else if (strcmp(argv[i], "-playlist") == 0)
			playlist = argv[++i];
		else if (strcmp(argv[i], "-fm") == 0)
			use_fm = true;
		else if (strcmp(argv[i], "-game") == 0)
			game_id = argv[++i];
	}
}

void Config::load_ini()
{
	FILE *fp = fopen(INIFILENAME, "r");
	if (!fp)
		return;

	enum {
		NO_SECTION,
		CONFIG,
	} current_section = NO_SECTION;

	char line[256];
	for (int lineno = 1; fgets(line, sizeof(line), fp); lineno++) {
		char val[256];

		if (line[0] == ';')  // comment
			continue;

		if (sscanf(line, "[%[^]]]", val)) {
			if (strcasecmp(val, "config") == 0)
				current_section = CONFIG;
			else
				WARNING(INIFILENAME ":%d Unknown section \"%s\"", lineno, val);
		}
		if (current_section == CONFIG) {
			if (sscanf(line, "noantialias = %s", val))
				no_antialias = to_bool(val, lineno);
			else if (sscanf(line, "savedir = %s", val))
				save_dir = val;
			else if (sscanf(line, "fontfile = %s", val))
				font_file = val;
			else if (sscanf(line, "playlist = %s", val))
				playlist = val;
			else if (sscanf(line, "fm = %s", val))
				use_fm = to_bool(val, lineno);
			else if (sscanf(line, "game = %s", val))
				game_id = val;
			else if (!is_empty_line(line))
				WARNING(INIFILENAME ":%d parse error", lineno);
		} else if (!is_empty_line(line)) {
			WARNING(INIFILENAME ":%d parse error", lineno);
		}
	}
	fclose(fp);
}
