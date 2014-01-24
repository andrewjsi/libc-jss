#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "htclient.h"
#include "debug.h"

void url_test () {
    htclient_t *h = htclient_new();

    htclient_url(h, "http://jss.hu:3000/akarmi.jpg");
    htclient_dump(h);

    htclient_destroy(h);
}

int main (int argc, const char *argv[]) {
    url_test(); return(0);

    htclient_t *shiva;
    shiva = htclient_new();
    if (shiva == NULL) {
        printf("out of memory\n");
        return -1;
    }

    htclient_url(shiva, "http://jsi.jss.hu:3000/auth");
    htclient_request_set(shiva,
        "GET /auth\r\n"
        "Auth-server: akarmi\r\n"
        "Auth-port: %d\r\n"
        , 5555
    );
    htclient_perform(shiva);

    if (htclient_error(shiva)) {
        printf("error: %s\n", htclient_error(shiva));
        goto err;
    }
    
    char *hdr = htclient_header_get(shiva, "Content-length");
    printf("Header vissza: %s\n", hdr);

    htclient_destroy(shiva);
    return 0;

err:
    htclient_destroy(shiva);
    return -1;
}
