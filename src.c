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

	printf("%s\n%s\n%s\n", i_config_node->i_colors_format_file, 
	i_config_node->i_wallpaper_dir_name, 
	i_config_node->i_wallpaper_format_file);

	clean_up(i_config_node);
	return 0;
}
