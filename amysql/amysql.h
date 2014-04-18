#include <ev.h>

/* Azért kell a loop paraméter, mert innen tudja az amysql, hogy melyik
event loop-ban kell majd meghívni a callback-et.*/

int amysql_init (struct ev_loop *loop);

/* Az amysql_query() azonnal visszaadja a vezérlést, és a háttérben egy
másik szálon megkezdi a paraméterben megadott lekérdezést az adatbázis
szerver felé. Amikor megjön a válasz, akkor lezárja a szerverrel a
kapcsolatot és meghívja a paraméterben megadott callback függvényt.

Figyelem! A callback függvény végén, de tényleg a legvégén lehetőség van új
amysql_query() függvény hívására. Hatására a fieldc/fieldv és a hibaváltozók
törlődni fognak, amiket a callback függvény is kapott.*/

int amysql_query (void *cb, const char *fmt, ...);

// beállítások
void amysql_option_host (const char *host);
void amysql_option_user (const char *user);
void amysql_option_password (const char *password);
void amysql_option_database (const char *database);
void amysql_option_connect_timeout (int connect_timeout);
void amysql_option_data_timeout (int data_timeout);

// visszaadja a hiba szövegét vagy NULL-t, ha nem volt hiba
const char *amysql_strerror ();

// nem-aszinkron lekérdezés. A lekérdezés ejéig az amysql_sync_query() hívás
// blokkol. Az eredményt a parc és a parv-be menti el.
int amysql_sync_query (int *parc, char **parv[], const char *fmt, ...);

