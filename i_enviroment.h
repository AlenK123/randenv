#ifndef I_ENVIROMENT_H
#define I_ENVIROMENT_H
#include <stdio.h>

/*
	structure to contain the main parts of the enviroment
	i.e system colors and the wallpaper
*/  

typedef struct i_enviroment {
	char * i_wallpaper_config;
	void * i_colors_config;
} i_enviroment;

void print_env(i_enviroment * env);

#endif