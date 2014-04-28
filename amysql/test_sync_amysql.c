#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amysql.h"
#include "debug.h"

int main (int argc, char *argv[]) {
    amysql_option_user ("proba");
    amysql_option_password ("proba");
    amysql_option_database ("proba");
    amysql_option_charset ("utf8");

    // if (amysql_sync_query(NULL, NULL, "INSERT INTO proba (name, comment) VALUES ('%s', '%s')", "egy", "kettő")) {
        // printf("ERROR: %s\n", amysql_strerror());
    // }

    int parc;
    char **parv;
    if (amysql_sync_query(&parc, &parv, "select * from proba order by RAND()")) {
        printf("ERROR: %s\n", amysql_strerror());
    }

    int i;
    for (i = 0; i < parc; i++) {
        printf("%d --- %s\n", i, parv[i]);
    }

    return 0;
}

