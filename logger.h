#ifndef LOGGER_H_LOADED
#define LOGGER_H_LOADED
void con_init ();
void conft (const char *fmt, ...);
void con_logfile (char *file);
void con_timestamp_format (char *format);
void _con_debugf (char *file, int line, const char *function, const char *fmt, ...);
#endif

// ha a DEBUG makró 1, akkor a debug() makrók életbe lépnek,
// ellenkező esetben a kódba sem kerül bele :)
#ifdef CON_DEBUG
#define con_debug(...) _con_debugf(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#else
#define con_debug(...)
#endif

