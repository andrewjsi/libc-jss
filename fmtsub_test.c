#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "misc.h"

int main (int argc, char **argv) {
    char buf2[32];
    char buf[64];
    
    char *pattern = argv[1];
    char *arg1 = argv[2];

    fmtsub(buf, sizeof(buf), pattern, arg1);

    printf("%d s %s s\n", (int)strlen(buf), buf);

    strncpy(buf2, buf, sizeof(buf2)-1);
    printf("%d s %s s\n", (int)strlen(buf2), buf2);
    
    return 0;
}
