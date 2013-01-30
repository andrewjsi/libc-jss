/* altalanos fuggvenyek */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//~ #include "debug.h"

char *chomp (char *str) {
    if (str == NULL)
        return NULL;
    size_t len = strlen(str);
    if (str[len - 1] == '\n')
        str[len - 1] = '\0';
    return str;
}

/* formázott dialstring készítés. Formátum:
 *   %n -- hívószám behelyettesítése
 *   \% -- % jel beírása
 */
int fmtsub (char *dest, size_t size, const char *pattern, const char *arg1) {
    char buf[256];

    if (arg1 == NULL)
        return -1;

    if (pattern == NULL || strlen(pattern) == 0)
        return -1;


    int i, pos;
    int len = strlen(pattern);
    buf[0] = '\0';
    for (i = 0, pos = 0; i < len && pos < sizeof(buf) - 1; i++, pos++) {
        const char *sub = &pattern[i];

        /// %n
        if (!strncmp(sub, "%n", 2)) {
            strncat(buf, arg1, sizeof(buf) - pos - 1);
            pos += (strlen(arg1) - 1);
            i++;

            // védelem, különben a pos túlszaladhat és a lenti buf[pos] = '\0' segfaultol
            if (pos >= sizeof(buf) - 1)
                pos = sizeof(buf) - 1;


        /// \% (backslash + percentage)
        } else if (!strncmp(sub, "\\%", 2)) { // az egyik \ joker, tehát a string \%
            strncat(buf, "%", sizeof(buf) - pos - 1);
            i++;

        /// minden egyéb
        } else {
            buf[pos] = pattern[i];
            buf[pos+1] = '\0';
        }
    }

    buf[pos] = '\0'; // lezáró NULL
    strncpy(dest, buf, size - 1);
    return 0;
}

