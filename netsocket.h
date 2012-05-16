#include <netdb.h>
#include <ev.h>

#define NETSOCKET_IN				1
#define NETSOCKET_OUT				2

#define NETSOCKET_CLIENT			1
#define NETSOCKET_SERVER			2

#define NETSOCKET_TCP				1
#define NETSOCKET_UNIX				2

#define NETSOCKET_STATE_RESOLVING	1
#define NETSOCKET_STATE_CONNECTING	2
#define NETSOCKET_STATE_CONNECTED	3

#define NETSOCKET_EVENT_ERROR			1
#define NETSOCKET_EVENT_CONNECT			2
#define NETSOCKET_EVENT_READ			3
#define NETSOCKET_EVENT_DISCONNECT		4

typedef struct netsocket_t {
	char *host;
	int port;
	char *lhost;
	int lport;
	int connect_timeout;
	char *ip;
	int sock;
	struct hostent *hostent;
	struct sockaddr_in addr;
	char inbuf[1024];
	int inbuf_len;
	ev_io w_out;
	ev_io w_in;
	ev_timer w_connect_timeout;
	void (*callback)(void*, int);
	void *userdata;	// user data
	int err;
	int event;
	char *disconnect_reason;
	int connected;
	int mode;
	struct netsocket_t *parent;
	int direction;
	int destroy_request;
	int in_callback;
	int disable_lookup_on_accept;
} netsocket_t;

netsocket_t *netsocket_new (void *callback, void *userdata);
void netsocket_destroy (netsocket_t *obj);
int netsocket_connect (netsocket_t *obj);
int netsocket_listen (netsocket_t *obj);
void netsocket_disconnect (netsocket_t *obj, char *reason);
void netsocket_disconnect_withevent (netsocket_t *obj, char *reason);
int netsocket_write (netsocket_t *obj, char *data, int length);
int netsocket_printf (netsocket_t *obj, const char *fmt, ...);
void netsocket_disable_lookup_on_accept (netsocket_t *obj);


