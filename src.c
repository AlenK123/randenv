/*
 * program that randomizes the wallpaper and system colours
 * then puts the calculated data in the desired config files
 * AlenK123 march 2019
 */

#include <stdio.h>
#include "i_config.h"
#define KILOBYTE 1024

extern int errno;

int main(int argc, char ** argv, char ** envp) {
	if (argc < 2) {
		printf("randenv <Path to config file>" "\n");
		return EXIT_SUCCESS;
	}
	
	i_config * i_config_node = init(argv[1]);

	if (i_config_node == NULL) {
		return EXIT_FAILURE;
	}

	write_to_format_file(i_config_node);

	clean_up(i_config_node);
	return EXIT_SUCCESS;
}
