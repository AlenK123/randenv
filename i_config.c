#include "i_config.h"

char * load_env(const char * env_buffer, const char * key) {
	size_t env_len = 0;
	char * occ = NULL;
    if ((occ = strstr(env_buffer, key)) == NULL) {
        fprintf(stderr, "Base enviroment argument %s missing. Could not set enviroment\n", key);
        return NULL;
    } 

	char * value = NULL;
	char * tmp_value = (occ + strlen(key) + 1);
	char * tmp_tmp_value = tmp_value;

	while (*tmp_tmp_value++ != '\n') env_len++;

	value = strndup(tmp_value, env_len);
	return value;
}


ssize_t read_from_config_file(const char * config_path, i_config * config) {
	int config_fd = 0;
	ssize_t size = 0;
	char * env_buffer = NULL;

    bool validator = false;
	
	if ((config_fd = open(config_path, O_RDONLY)) < 0) {
		perror("Error opening file");
		return -1;
	}

	off_t start = lseek(config_fd, (size_t)0, SEEK_CUR);
	size = (long)lseek(config_fd, (off_t)0, SEEK_END);
	lseek(config_fd, start, SEEK_SET);
	
	env_buffer = (char*)calloc(size + 1, sizeof(char));
	
	if (env_buffer == NULL) {
		perror("Allocation error");
		close(config_fd);
		return -1;
	}

	if (read(config_fd, env_buffer, size) < 0) {
		perror("Error reading from file");
		close(config_fd);
		free(env_buffer);
		return -1;
	}

    validator |= (config->i_config_path = strdup(config_path)) == NULL;
	validator |= (config->i_wallpaper_dir_name = load_env(env_buffer, I_WALLPAPER_DIR_KEY)) == NULL;
	validator |= (config->i_wallpaper_format_file = load_env(env_buffer, I_WALLPAPER_FORMAT_FILE)) == NULL;
	validator |= (config->i_colors_format_file = load_env(env_buffer, I_COLOR_FORMAT_FILE)) == NULL;
	config->i_env_node = NULL;

	close(config_fd);
	free(env_buffer);

    if (validator) {
        return -1;
    }
	return size;
}

ssize_t get_wallpapers(DIR * directory, i_config * i_config_node) {
	dirent * curr_file = NULL;
	ssize_t n = 0;

	if (directory == NULL) {
		char err_msg[256] = "";
		sprintf(err_msg, "cannot access directory %s", i_config_node->i_wallpaper_dir_name);
		perror(err_msg);
		return -1;
	}

	i_config_node->wallpaper_dir_list = NULL;
    while ((curr_file = readdir(directory)) != NULL) {
		if (curr_file->d_type == 8 /* && ends with picture tld*/) {
			
			i_config_node->wallpaper_dir_list = (i_config_node->wallpaper_dir_list == NULL) ? 
			(char**)calloc(n + 1, sizeof(char*)) :
			(char**)realloc(i_config_node->wallpaper_dir_list, (n + 1) * sizeof(char*));

			if (i_config_node->wallpaper_dir_list == NULL) return -1;
			(i_config_node->wallpaper_dir_list)[n++] = strdup(curr_file->d_name);
		}
	}
	i_config_node->dir_len = n;
	return n;
}

i_config * init(const char * config_path) {
	i_config * i_config_node = (i_config*)malloc(sizeof(i_config));

	if (i_config_node == NULL) {
		clean_up(i_config_node);
		perror("Allocation Error");
		return NULL;
	}

	if (read_from_config_file(config_path, i_config_node) < 0) {
		clean_up(i_config_node);
		return NULL;
	}

	DIR * wallpaper_dir = opendir(i_config_node->i_wallpaper_dir_name);
	
	if (get_wallpapers(wallpaper_dir, i_config_node) < 0) {
		clean_up(i_config_node);
		return NULL;
	}

	closedir(wallpaper_dir);
	
	return i_config_node;
}

void clean_up(i_config * i_config_node) {
	free(i_config_node->i_wallpaper_dir_name);
	free(i_config_node->i_config_path);
	free(i_config_node->i_env_node);
	free(i_config_node->i_wallpaper_format_file);
	free(i_config_node->i_colors_format_file);

	for (int i = 0; i < i_config_node->dir_len; i++) {
		free(i_config_node->wallpaper_dir_list[i]);
		i_config_node->wallpaper_dir_list[i] = NULL;
	}
	free(i_config_node->wallpaper_dir_list);
	i_config_node->wallpaper_dir_list = NULL;
}

ssize_t write_to_format_file(const char * filename, const char * value) {
    return 0;
}