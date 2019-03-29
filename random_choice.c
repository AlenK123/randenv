#include "random_choice.h"

ssize_t random_choice(char ** _dst, char ** _list, size_t size) {
    
    if (!_dst || !_list || size == 0) {
        return -1;
    }

    int n = 0;
    
    /* using system time as a radomizing seed */
    srand(time(NULL));

    /* randomize between 0 ~ size */
    n = (rand() % size);
    
    (*_dst) = strdup(_list[n]);

    return n; 
}