#include "i_config.h"

i_config * init(const char * config_path) {
	i_config * i_config_node = (i_config*)malloc(sizeof(i_config));

	if (i_config_node == NULL) {
		clean_up(i_config_node);
		perror("Allocation Error");
		return NULL;
	}

	if (read_from_config_file(config_path, i_config_node) < 0) {
		perror("Read error");
		clean_up(i_config_node);
		return NULL;
	}

	DIR * wallpaper_dir = opendir(i_config_node->i_wallpaper_dir_name);
	
	if (get_wallpapers(wallpaper_dir, i_config_node) < 0) {
		clean_up(i_config_node);
		return NULL;
	}

	i_config_node->i_env_node = (i_enviroment*)malloc(sizeof(i_enviroment));

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
	ssize_t size = 0;
	char * env_buffer = NULL;
	bool validator = false;

	size = safe_read_data_from_file(config_path, &env_buffer);

	if (size < 0) {
		return -1;
	}

    validator |= (config->i_config_path = strdup(config_path)) == NULL;
	validator |= (config->i_wallpaper_dir_name = load_env(env_buffer, I_WALLPAPER_DIR_KEY)) == NULL;
	validator |= (config->i_wallpaper_format_file = load_env(env_buffer, I_WALLPAPER_FORMAT_FILE)) == NULL;
	validator |= (config->i_colors_format_file = load_env(env_buffer, I_COLOR_FORMAT_FILE)) == NULL;
	validator |= (config->i_wallpaper_config_file = load_env(env_buffer, I_WALLPAPER_CONFIG_FILE)) == NULL;
	validator |= (config->i_colors_config_file = load_env(env_buffer, I_COLORS_CONFIG_FILE)) == NULL;
	
	free(env_buffer);

    if (validator) {
		perror("");
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

ssize_t i_sprintf(char ** _out, char * _format, i_config * _i_node) {
	if (!_out || !_format || !_i_node) {
		return -1;	
	}
	
	size_t n = strlen(_format);
	size_t len = 0;
	char * _format_string = NULL;
	char * _data = NULL; 
	char * out = NULL;

	for (int i = 0; i < n; i++) {
		if (_format[i] == '$') {
			char * p = NULL;

	 		if ((p = strstr(_format, I_WALLPAPER_FORMAT_STRING)) == (_format + i + 1)) {	
				_format_string = I_WALLPAPER_FORMAT_STRING;
				_data = (char*)malloc((
					strlen(_i_node->i_wallpaper_dir_name) + 
					strlen(_i_node->i_env_node->i_wallpaper_config) + 1) * 
					sizeof(char)
				);

				sprintf(_data, "%s%s", _i_node->i_wallpaper_dir_name, _i_node->i_env_node->i_wallpaper_config);
		    }

			/* TODO: other cases */

			if (_data && _format_string) {
				out = strndup(_format, i);
				len = strlen(_format) + (strlen(_data) - strlen(_format_string));
				(*_out) = (char*)calloc(len + 1, sizeof(char));
				sprintf(*_out, "%s%s%s", out, _data, p + strlen(_format_string));
				free(out);
				free(_data);
				return strlen(*_out);
			}
		}
	}
	fprintf(stderr, "the string \'%s\' in one of the format files does not match the program formatting scheme" "\n ", _format);
	return -1;
}

ssize_t write_to_format_file(i_config * i_config_node) {
	char * in_buffer = NULL;
	char * out_buffer = NULL;
	ssize_t size = 0;

	char * _wallpaper = NULL;

	random_choice(&_wallpaper, i_config_node->wallpaper_dir_list, i_config_node->dir_len);
	i_config_node->i_env_node->i_wallpaper_config = _wallpaper;
	
	size = safe_read_data_from_file(i_config_node->i_wallpaper_format_file, &in_buffer);

	if (size < 0) {
		return -1;
	}

	/* TODO: handle errors and stuff */
	
	size = i_sprintf(&out_buffer, in_buffer, i_config_node);

	if (size < 0) {
		return -1;
	}

	if (safe_write_data_to_file(i_config_node->i_wallpaper_config_file, out_buffer) < 0) {

	}

	free(in_buffer);
	free(out_buffer);
	return 0;
}

ssize_t safe_write_data_to_file(const char * filename, const char * buffer) {
	int fd = open(filename, O_WRONLY);	
	size_t _n = strlen(buffer);	
	if (fd < 0) {
		perror(filename);
		return -1;
	}

	if (write(fd, buffer, _n) < _n) {
		char msg[256] = "";
		sprintf(msg, "could not write to %s", filename);
		perror(msg);
		return -1;
	}

	return _n;
}

ssize_t safe_read_data_from_file(const char * file_name, char ** buffer) {
    char * out_buffer = NULL;
	int fd = 0;
	ssize_t size = 0;

	if ((fd = open(file_name, O_RDONLY)) < 0) {
		perror("failed to open file");
		return -1;
		(*buffer) = NULL;
	}

	size = file_size(fd);

	if (size < 0) {
		char err_msg[256] = "";
		sprintf(err_msg, "Error reading from %s", file_name);
		perror(err_msg);
		close(fd);
		(*buffer) = NULL;
		return -1;
	}

	out_buffer = (char*)calloc((size_t)size + 1, sizeof(char));

	if (out_buffer == NULL) {
		perror("Allocation error");
		close(fd);
		(*buffer) = NULL;
		return -1;
	}

	if (read(fd, out_buffer, size) < 0) {
		char err_msg[256] = "";
		sprintf(err_msg, "Error reading from %s", file_name);
		perror(err_msg);
		(*buffer) = NULL;
		free(out_buffer);
		close(fd);
		return -1;
	}
	
	(*buffer) = out_buffer;
	close(fd);
	return size;		
}

ssize_t file_size(int fd) {
	off_t start = lseek(fd, (size_t)0, SEEK_CUR);
	ssize_t size = lseek(fd, (off_t)0, SEEK_END);
	lseek(fd, start, SEEK_SET);
	return size;
}