/*
 * program that randomizes the wallpaper and system colours
 * then puts the calculated data in the desired config files
 * AlenK123 march 2019
 */

#include <stdio.h>
#include <string.h>

#include "i_config.h"

#define I_HELP_FLAG_1 "-h"
#define I_HELP_FLAG_2 "--help"

#define I_CONFIG_FLAG_1 "-c"
#define I_CONFIG_FLAG_2 "--config"

#define I_GENERATE_FLAG_1 "-g"
#define I_GENERATE_FLAG_2 "--generate"

#define HELP I_HELP_FLAG_1 "  " I_HELP_FLAG_2 " - displays this message\n"\
I_GENERATE_FLAG_1 "  " I_GENERATE_FLAG_2 " <path> - generates a config file in the path\n"\
I_CONFIG_FLAG_1 "  " I_CONFIG_FLAG_2 " <path to config file> - does the magic"

#define KILOBYTE 1024

extern int errno;

int main(int argc, char ** argv, char ** envp) {
	if (argc < 2) {
		printf(HELP "\n");
		return EXIT_SUCCESS;
	}

	for (int i = 1; i < argc; i++) {
		/* if one of the string is equal it will yeild: !(0 * something) = 1 */
		if ( !(strcmp(argv[i], I_HELP_FLAG_1) && strcmp(argv[i], I_HELP_FLAG_2)) ) {
			printf(HELP "\n");
			return EXIT_SUCCESS;
		}

		if ( !(strcmp(argv[i], I_GENERATE_FLAG_1) && strcmp(argv[i], I_GENERATE_FLAG_2)) ) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Option %s requires the config file argument\n", argv[i]);
				return EXIT_FAILURE;
			}

			return (generate_config_file(argv[i + 1]) == -1) ? EXIT_FAILURE : EXIT_SUCCESS;
		}

		if (!(strcmp(argv[i], I_CONFIG_FLAG_1) && strcmp(argv[i], I_CONFIG_FLAG_2)) ) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Option %s requires the config file argument\n", argv[i]);
				return EXIT_FAILURE;
			}
			i_config * i_config_node = init(argv[i + 1]);

			if (i_config_node == NULL) {
				return EXIT_FAILURE;
			}

			write_to_format_file(i_config_node);

			clean_up(i_config_node);
			return EXIT_SUCCESS;		
		}
	}
}
