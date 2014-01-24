// © Copyright 2014 JSS & Hayer. All Rights Reserved.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdarg.h>

#include "htclient.h"
#include "debug.h"

/* void ERR(...)
 *
 *  A paraméterül átadott VA_ARGS formátumú hibaüzenetet bemásolja a req->error
 *  -ba, majd visszatér return -1 -el. Figyelni kell a blokkos szerkezeteknél:
 *  Mindenképp használni kell a { } zárójeleket, mivel ez a makró két utasítást
 *  tartalmaz. A { } elhagyásával logikai hiba keletkezhet a programban.
 */
#define ERR(...)  snprintf(htc->error, sizeof(htc->error), __VA_ARGS__); return -1

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

void htclient_dump (htclient_t *htc) {
    #define HTCLIENT_DUMPS(s) printf("htc->%s = %s\n", #s, htc->s)
    #define HTCLIENT_DUMPI(i) printf("htc->%s = %d\n", #i, htc->i)
//    char host[64];
//    char port[8];
//    char request_data[HTCLIENT_BUFSIZ];
//    char response_data[HTCLIENT_BUFSIZ];
//    struct {
//        char *name;
//        char *value;
//    } headers[16];
//    int num_headers;
//    char *header_buf;
//    char error[128];
    HTCLIENT_DUMPS(host);
    HTCLIENT_DUMPS(port);
    HTCLIENT_DUMPS(request_data);
    HTCLIENT_DUMPS(response_data);
    HTCLIENT_DUMPI(num_headers);
    HTCLIENT_DUMPS(header_buf);
    HTCLIENT_DUMPS(error);
}

// Skip the characters until one of the delimiters characters found.
// 0-terminate resulting word. Skip the rest of the delimiters if any. Advance
// pointer to buffer to the next word. Return found 0-terminated word.
// (function from Mongoose project)
static char *skip (char **buf, const char *delimiters) {
    char *p, *begin_word, *end_word, *end_delimiters;

    begin_word = *buf;
    end_word = begin_word + strcspn(begin_word, delimiters);
    end_delimiters = end_word + strspn(end_word, delimiters);

    for (p = end_word; p < end_delimiters; p++) {
        *p = '\0';
    }

    *buf = end_delimiters;

    return begin_word;
}

// Parse HTTP headers from the given buffer, advance buffer to the point where
// parsing stopped. (function from Mongoose project)
static void parse_http_headers(char **buf, struct htclient_t *htc) {
    size_t i;

    for (i = 0; i < ARRAY_SIZE(htc->headers); i++) {
        htc->headers[i].name = skip(buf, ": ");
        htc->headers[i].value = skip(buf, "\r\n");
        if (htc->headers[i].name[0] == '\0')
            break;
        htc->num_headers = i + 1;
    }
}

char *htclient_error (htclient_t *htc) {
    if (htc->error == NULL || !strlen(htc->error))
        return NULL;
    return htc->error;
}

int htclient_url (htclient_t *htc, const char *fmt, ...) {
    char url[HTCLIENT_BUFSIZ];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(url, sizeof(url), fmt, ap);
    va_end(ap);
puts(url); return 0;
//    char host[20];
//    char port[20];
//    char URL[20];
//    int *prt;
//    char URI[20];
//    if ( sscanf ( http, "http://%[^:]:%[^/]/%s", host, port, URI ) == 3 ) {
//        *prt = atoi(port);
//        strcpy(URL,"/");
//        strcat(URL,URI);
//        strcpy(URI,URL);
//    } else if ( sscanf ( http, "http://%[^:]:%[^/]/%s", host, port, URI ) == 2 ) {
//        *prt = atoi(port);
//        strcpy(URI,"/");
//    } else if(sscanf( http, "http://%[^/]/%s", host, URI ) == 2 ) {
//        *prt = 80;
//        strcpy(URL,"/");
//        strcat(URL,URI);
//        strcpy(URI,URL);
//    } else if (sscanf( http, "http://%[^/]", host ) == 1) {
//        *prt = 80;
//        strcpy(URI,"/");
//    } else if (sscanf( http, "http://%[^/]", host ) == 0) {
//        printf("Invalid URL\n");
//    }
}

int htclient_perform (htclient_t *htc) {
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int port;
    int sockfd;
    int rv;

    port = atoi(htc->port);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        ERR("error opening socket: %s", strerror(errno));
    }

    server = gethostbyname(htc->host);
    if (server == NULL) {
        ERR("host not found: %s", htc->host);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        ERR("can't connect to %s:%d: %s", htc->host, port, strerror(errno));
    }

    rv = write(sockfd, htc->request_data, strlen(htc->request_data));
    if (rv < 0) {
        ERR("ERROR writing to socket");
    }

    memset(htc->response_data, 0, sizeof(htc->response_data));
    rv = read(sockfd, htc->response_data, sizeof(htc->response_data) - 1);
    if (rv < 0) {
        ERR("ERROR reading from socket");
    }

    htc->header_buf = strdup(htc->response_data);
    char *buf = htc->header_buf;

    parse_http_headers(&buf, htc);

    return 0;
}

char *htclient_header_get (htclient_t *htc, const char *name) {
    int i;
    for (i = 0; i < htc->num_headers; i++) {
        if (htc->headers[i].name == NULL || htc->headers[i].value == NULL)
            continue;
        if (!strcmp(htc->headers[i].name, name))
            return htc->headers[i].value;
    }
    return NULL;
}


htclient_t *htclient_new () {
    htclient_t *htc = malloc(sizeof(*htc));
    if (htc == NULL) {
        fprintf(stderr, "out of memory");
        return NULL;
    }
    memset(htc, 0, sizeof(*htc));
    return htc;
}

void htclient_request_set (htclient_t *htc, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(htc->request_data, sizeof(htc->request_data), fmt, ap);
    va_end(ap);
}


void htclient_destroy (htclient_t *htc) {
    if (htc == NULL)
        return;
    if (htc->header_buf != NULL) {
        free(htc->header_buf);
        htc->header_buf = NULL;
    }
    free(htc);
}

