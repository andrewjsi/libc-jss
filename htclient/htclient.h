// © Copyright 2014 JSS & Hayer. All Rights Reserved.

#define HTCLIENT_BUFSIZ 1024

typedef struct htclient_t {
    char host[64];
    char port[8];
    char request_data[HTCLIENT_BUFSIZ];
    char response_data[HTCLIENT_BUFSIZ];
    struct {
        char *name;
        char *value;
    } headers[16];
    int num_headers;
    char *header_buf;
    char error[128];
} htclient_t;

htclient_t *htclient_new ();
int htclient_url (htclient_t *htc, const char *fmt, ...);
void htclient_request_set (htclient_t *htc, const char *fmt, ...);
int htclient_perform (htclient_t *htc);
char *htclient_header_get (htclient_t *htc, const char *name);
void htclient_destroy (htclient_t *htc);
char *htclient_error (htclient_t *htc);
void htclient_dump (htclient_t *htc);
