/* altalanos fuggvenyek */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "debug.h"

char *chomp (char *str) {
    if (str == NULL)
        return NULL;
    size_t len = strlen(str);
    if (str[len - 1] == '\n')
        str[len - 1] = '\0';
    return str;
}

/*
fmtsub - mintából, változó-érték párokból és készít stringet behelyettesítéssel

paraméterek:
    dest        ebbe a stringbe menti el az eredményt
    size        dest mérete
    pattern     minta
    args        változó-érték táblázat, nullával lezárva

visszatérési érték:
    0           sikerült a behelyettesítés
    -1          valamelyik mutató == NULL vagy a size == 0

A táblázat megadása:

const char *args[][2] = {
    {"%n", "2463434"},
    {"%v", "0.9.6-rc2"},
    {"version", "Linux 2.6.32"},
    {"boo", "baa"},
    {0}}; */
int fmtsub (char *dest, size_t size, const char *pattern, const char *args[][2]) {
    char buf[4096];

    if (dest == NULL || pattern == NULL || args == NULL || size == 0)
        return -1;

    // ha a pattern üres string, akkor üres stringet adunk vissza
    if (strlen(pattern) == 0) {
        strcpy(dest, "");
        return 0;
    }

    int pospat, posbuf, a;
    int lenpat = strlen(pattern);
    buf[0] = '\0';
    // byteonként végigmegyünk a pattern stringen
    for (pospat = 0, posbuf = 0; pospat < lenpat && posbuf < sizeof(buf) - 1;) {
        const char *subpat = &pattern[pospat];
        int match = 0;

        // végigmegyünk az args táblázaton
        for (a = 0; args[a][0] != 0; a++) {
            const char *var = args[a][0]; // "%n"
            const char *exp = args[a][1]; // "2463434"

            // ha a változónév megtalálható a pattern jelenlegi pozíciójánál
            if (strlen(var) != 0 && !strncmp(subpat, var, strlen(var))) {
                if (exp  != NULL) {
                    // bemásoljuk a változó értékét a bufferbe
                    strncat(buf, exp, sizeof(buf) - posbuf - 1);
                    posbuf += strlen(exp);
                }

                /* ezen a ponton a posbuf nagyobb lehet, mint a buf, ami 
                igen kellemetlenül érintené a lenti buf[posbuf] = '\0' 
                értékadást, ezért a posbuf pozícionálót beállítjuk a buf 
                utolsó byte-jára. ide fog kerülni a NULL. */
                if (posbuf >= sizeof(buf) - 1)
                    posbuf = sizeof(buf) - 1;

                // pattern pozícióját a változónév utáni karakterre állítjuk
                pospat += strlen(var);

                // kihagyjuk a lenti if-et
                match = 1;
                break;
            }
        }

        // 1 bájt másolása pattern-ből buf-ba
        if (!match) {
            buf[posbuf] = pattern[pospat];
            buf[posbuf+1] = '\0';
            pospat++;
            posbuf++;
        }
    }

    buf[posbuf] = '\0'; // lezáró NULL
    strncpy(dest, buf, size - 1);
    return 0;
}

