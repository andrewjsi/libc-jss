#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>

#include "logger.h"
#include "debug.h"

#include "utlist.h"

#define concat(dst, src) strncat((dst), (src), ((sizeof(dst)) - strlen(dst) - 1))
#define concatf(dst, ...) snprintf((dst) + strlen((dst)), sizeof((dst)) - strlen((dst)), __VA_ARGS__)


char *logfile = NULL;
char *timestamp_format = "%a %H:%M:%S";

static int initialised = 0;
struct timeval first_time = {0, 0};
char ela[16];

typedef struct el {
	struct timeval tm;
    char *line;
    struct el *prev; /* needed for a doubly-linked list only */
    struct el *next; /* needed for singly- or doubly-linked lists */
} el;

/* Ha a tv (első paraméter) meg van adva, akkor nincs gettimeofday hívás, hanem 
 * a tv-ben beállított idő lesz a mérvadó. Ha a tv értéke NULL, akkor a 
 * jelenlegi idővel (gettimeofday) számolunk. Ha meg van adva a ptm, akkor az is
 * be lesz állítva. Ha az elapsed nem NULL, akkor kiszámolja az indítástól 
 * eltelt időt, és visszaad egy mutatót, ami a kiszámolt érték ssss.mmmu 
 * formátumú stringre mutat.
 */
static struct timeval get_time (struct timeval *tv, struct tm **ptm, char *elapsed) {
	struct timeval mytv;
	
	if (tv == NULL) {
		gettimeofday(&mytv, NULL);
	} else {
		mytv.tv_sec = tv->tv_sec;
		mytv.tv_usec = tv->tv_usec;
	}

	if (tv)
		*tv = mytv;

	if (ptm)
		*ptm = localtime(&mytv.tv_sec); // thread safe?

	if (elapsed) {
		long int delta = ((mytv.tv_sec * 1000000) + mytv.tv_usec) - ((first_time.tv_sec * 1000000) + first_time.tv_usec);
		char nowstr[16] = {0};
		elapsed[1] = 0;
		memset(elapsed, 0, sizeof(elapsed)); // enélkül az strncpy szar helyre ír
		// delta -> elapased atalakitas. mikroszekundumokbol ssss.mmmu formatum
		snprintf(nowstr, 16, "%14.14ld\n", delta);
		strncpy(elapsed, nowstr + 4, 4);
		strncat(elapsed, ".", 1);
		strncat(elapsed, nowstr + 8, 4);
	}

	return mytv;
}

void con_init () {
	if (initialised)
		return;
	first_time = get_time(NULL, NULL, NULL);
	initialised = 1;
	return;
}

// formázott konzol üzenet kiírása időbélyeggel
void conft (const char *fmt, ...) {
	if (!initialised)
		con_init();
		
	struct timeval tv;
	struct tm *ptm; // thread safe?
	char line[1024] = {0};
	char tmp[1024];
	
	tv = get_time(NULL, &ptm, NULL); // idő, ami alapján a továbbiakban számolunk
	strftime(tmp, sizeof(tmp), timestamp_format, ptm); // timestamp
	concatf(line, "[%s", tmp);
	get_time(&tv, &ptm, tmp); // elapsed lekérdezése TODO: ez borzalmas, átírni!!!
	concatf(line, " %s] ", tmp);

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(tmp, sizeof(tmp), fmt, ap);
	va_end(ap);
	concat(line, tmp);

	printf("S %s S\n", line);

	if (logfile) {
		FILE *f = fopen(logfile, "a");
		if (f != NULL) {
			// flock(fileno(f), LOCK_EX);
			fprintf(f, "%s\n", line);
			// flock(fileno(f), LOCK_UN);
			fclose(f);
		}
	}

	fflush(stdout);
}

void con_logfile (char *file) {
	logfile = file;
}

void con_timestamp_format (char *format) {
	timestamp_format = format;
}

// gány, memóriazabáló függvény :)
void _con_debugf (char *file, int line, const char *function, const char *fmt, ...) {
	char tmp[1024] = {0};
	char tmp2[1024];

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(tmp2, sizeof(tmp), fmt, ap);
	va_end(ap);

	concatf(tmp, "%s:%d %s: %s", file, line, function, tmp2);
	printf("%s\n", tmp);
}

