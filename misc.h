/* misc.h
 * Copyright © 2014, Andras Jeszenszky, JSS & Hayer IT - http://www.jsshayer.hu
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 */

char *chomp (char *str);

#define concat(dst, src) strncat((dst), (src), ((sizeof(dst)) - strlen(dst) - 1))
#define concatf(dst, ...) snprintf((dst) + strlen((dst)), sizeof((dst)) - strlen((dst)), __VA_ARGS__)
int fmtsub (char *dest, size_t size, const char *pattern, const char *args[][2]);
char *encode_rot13 (char *s);

char *_trim (char *s, int trim_from_left, int trim_from_right);
#define trim(s) _trim(s, 1, 1)
#define ltrim(s) _trim(s, 1, 0)
#define rtrim(s) _trim(s, 0, 1)

// Nincs még doksi, ezért ide írom...
// A chomp, trim, encode_rot13 függvények használhatok beágyazva is, tehát:
// char buf[64];
// encode_rot13(trim(chomp(buf)));

// visszatér egy pointer tömbbel, ami az elemekre mutat, a tömböt 0 pointerrel zárja. A függvény felülírja a buffer stringben lévő delimetereket. Magyarul módosítja a kapott stringet, ami nem biztos, hogy jó dolog. Pl. char *x = "hello" változóknál segfault.
char **split (char *buffer, int delimeter);

// 1-et ad vissza, ha érvényes a kapott IP cím
// 0-át ha nem
int is_valid_ip (const char *ip);

// mint Perl-ben:)
void die (const char *fmt, ...);

