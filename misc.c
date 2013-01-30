/* altalanos fuggvenyek */

#include <stdlib.h>
#include <string.h>

char *chomp (char *str) {
    if (str == NULL)
        return NULL;
    size_t len = strlen(str);
    if (str[len - 1] == '\n')
        str[len - 1] = '\0';
    return str;
}

