
char *chomp (char *str);

#define concat(dst, src) strncat((dst), (src), ((sizeof(dst)) - strlen(dst) - 1))
#define concatf(dst, ...) snprintf((dst) + strlen((dst)), sizeof((dst)) - strlen((dst)), __VA_ARGS__)
