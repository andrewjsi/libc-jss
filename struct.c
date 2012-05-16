
#include <stdio.h>

struct table {
    int num;
    char *name;
};


int main () {
    struct table proba;
    int i;
    proba.num = 5;
    i = fgetc(stdin);
    printf("Hello %c - %d\n", i, proba.num);
    return 0;
};

