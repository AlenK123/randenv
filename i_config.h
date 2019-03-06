#ifndef I_CONFIG_H
#define I_CONFIG_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#include "i_enviroment.h"

#define I_WALLPAPER_FORMAT_FILE "wallpaper_format_file"
#define I_WALLPAPER_DIR_KEY "wallpaper_dir"
#define I_COLOR_FORMAT_FILE "color_format_file"



/* 
	structure to contin the config part of the project
	i.e config file object, path and the chosen enviroment
*/

typedef struct dirent dirent;

typedef struct i_config {
	char * i_config_path;
	
	char * i_wallpaper_format_file;
	char * i_colors_format_file;

	char * i_wallpaper_config_file;
	char * i_colors_config_file;

	char * i_wallpaper_dir_name;
	char ** wallpaper_dir_list;
	size_t dir_len;

	i_enviroment * i_env_node;
} i_config;


i_config * init(const char * config_path);
void clean_up(i_config * i_config_node);

char * load_env(const char * env_buffer, const char * key);
ssize_t read_from_config_file(const char * config_path, i_config * config);
ssize_t get_wallpapers(DIR * directory, i_config * i_config_node);
ssize_t write_to_format_file(i_config * i_config_node);

ssize_t replace(char ** _in, const char * _with, const char * filename);
ssize_t safe_read_data_from_file(const char * file_name, char ** buffer);
ssize_t file_size(int fd);

#endif