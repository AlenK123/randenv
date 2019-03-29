#ifndef RANDOM_CHOICE_H
#define RANDOM_CHOICE_H


#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

ssize_t random_choice(char ** _dst, char ** _list, size_t size);

#endif