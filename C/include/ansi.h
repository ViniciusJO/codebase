// Runtime-agnostic ANSI escape codes for console styling

#ifndef __ANSI_H__
#define __ANSI_H__

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stddef.h>

extern void *malloc (size_t __size);
extern int snprintf (char *__s, size_t __maxlen, const char *__format, ...);

#ifndef ANSI_ATTRIBUTES
#define ANSI_ATTRIBUTES
#endif//ANSI_ATTRIBUTES

#define ESC "\x1b"
#define CSI ESC"["

/* ─────────────────────────── Reset ─────────────────────────── */

#define RESET CSI"0m"

/* ─────────────────────────── Text styles ─────────────────────────── */

#define STYLE_BOLD CSI "1m"
#define STYLE_DIM CSI "2m"
#define STYLE_ITALIC CSI "3m"
#define STYLE_UNDERLINE CSI "4m"
#define STYLE_BLINK CSI "5m"
#define STYLE_INVERSE CSI "7m"
#define STYLE_HIDDEN CSI "8m"
#define STYLE_STRIKETHROUGH CSI "9m"

#define STYLE_BOLDOFF CSI "22m"
#define STYLE_DIMOFF CSI "22m"
#define STYLE_ITALICOFF CSI "23m"
#define STYLE_UNDERLINEOFF CSI "24m"
#define STYLE_BLINKOFF CSI "25m"
#define STYLE_INVERSEOFF CSI "27m"
#define STYLE_HIDDENOFF CSI "28m"
#define STYLE_STRIKETHROUGHOFF CSI "29m"

/* ─────────────────────────── Foreground colors ─────────────────────────── */

#define FG_BLACK CSI "30m"
#define FG_RED CSI "31m"
#define FG_GREEN CSI "32m"
#define FG_YELLOW CSI "33m"
#define FG_BLUE CSI "34m"
#define FG_MAGENTA CSI "35m"
#define FG_CYAN CSI "36m"
#define FG_WHITE CSI "37m"

#define FG_BRIGHTBLACK CSI "90m"
#define FG_BRIGHTRED CSI "91m"
#define FG_BRIGHTGREEN CSI "92m"
#define FG_BRIGHTYELLOW CSI "93m"
#define FG_BRIGHTBLUE CSI "94m"
#define FG_BRIGHTMAGENTA CSI "95m"
#define FG_BRIGHTCYAN CSI "96m"
#define FG_BRIGHTWHITE CSI "97m"

#define FG_RESET CSI "39m"

/* ─────────────────────────── Background colors ─────────────────────────── */

#define BG_BLACK CSI "40m"
#define BG_RED CSI "41m"
#define BG_GREEN CSI "42m"
#define BG_YELLOW CSI "43m"
#define BG_BLUE CSI "44m"
#define BG_MAGENTA CSI "45m"
#define BG_CYAN CSI "46m"
#define BG_WHITE CSI "47m"

#define BG_BRIGHTBLACK CSI "100m"
#define BG_BRIGHTRED CSI "101m"
#define BG_BRIGHTGREEN CSI "102m"
#define BG_BRIGHTYELLOW CSI "103m"
#define BG_BRIGHTBLUE CSI "104m"
#define BG_BRIGHTMAGENTA CSI "105m"
#define BG_BRIGHTCYAN CSI "106m"
#define BG_BRIGHTWHITE CSI "107m"

#define BG_RESET CSI "49m"

/* ─────────────────────────── True color (24-bit RGB) ─────────────────────────── */

#define FG_RGB(LITERAL_R, LITERAL_G, LITERAL_B) CSI "38;2;" #LITERAL_R ";" #LITERAL_G ";" #LITERAL_B "m";
#define BG_RGB(LITERAL_R, LITERAL_G, LITERAL_B) CSI "48;2;" #LITERAL_R ";" #LITERAL_G ";" #LITERAL_B "m";

ANSI_ATTRIBUTES char *fg_rgb(unsigned char r, unsigned char g, unsigned char b);
ANSI_ATTRIBUTES char *bg_rgb(unsigned char r, unsigned char g, unsigned char b);

#define HEX_TO_RGB(HEX) (((HEX) >> 24) & 0xFF), (((HEX) >> 16) & 0xFF), (((HEX) >> 8) & 0xFF)
ANSI_ATTRIBUTES char *fg_hex(unsigned int color);
ANSI_ATTRIBUTES char *bg_hex(unsigned int color);

/* ─────────────────────────── 256-color support ─────────────────────────── */

#define FG_256(LITERAL_COLOR_NUMBER) CSI "38;5;" #LITERAL_COLOR_NUMBER "m"
#define BG_256(LITERAL_COLOR_NUMBER) CSI "48;5;" #LITERAL_COLOR_NUMBER "m"

ANSI_ATTRIBUTES char *fg_256(unsigned char color);
ANSI_ATTRIBUTES char *bg_256(unsigned char color);

// ─────────────────────────── Cursor ───────────────────────────

#define CURSOR_UP(LITERAL_N) CSI #LITERAL_N "A"
#define CURSOR_DOWN(LITERAL_N) CSI #LITERAL_N "B"
#define CURSOR_FORWARD(LITERAL_N) CSI #LITERAL_N "C"
#define CURSOR_BACK(LITERAL_N) CSI #LITERAL_N "D"
#define CURSOR_NEXTLINE(LITERAL_N) CSI #LITERAL_N "E"
#define CURSOR_PREVLINE(LITERAL_N) CSI #LITERAL_N "F"
#define CURSOR_COLUMN(LITERAL_N) CSI #LITERAL_N "G"
#define CURSOR_POSITION(LITERAL_ROW, LITERAL_COL) CSI #LITERAL_ROW ";" #LITERAL_COL "H"
#define CURSOR_HIDE CSI "?25l"
#define CURSOR_SHOW CSI "?25h"
#define CURSOR_SAVE ESC "7"
#define CURSOR_RESTORE ESC "8"

/* ─────────────────────────── Clear screen / line ─────────────────────────── */

#define CLEAR_SCREEN CSI "2J"
#define CLEAR_SCREENDOWN CSI "0J"
#define CLEAR_SCREENUP CSI "1J"
#define CLEAR_LINE CSI "2K"
#define CLEAR_LINERIGHT CSI "0K"
#define CLEAR_LINELEFT CSI "1K"

// /* ─────────────────────────── Helper ─────────────────────────── */
//
// export function style(text: string, ...codes: string[]): string {
//   return codes.join("") + text + RESET;
// }

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//__ANSI_H__

//#define ANSI_IMPLEMENTATIONS
#ifdef ANSI_IMPLEMENTATIONS
#ifndef __ANSI_IMP__
#define __ANSI_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

ANSI_ATTRIBUTES char *fg_rgb(unsigned char r, unsigned char g, unsigned char b) {
  const unsigned char len = 20;
  char *out = (char*)malloc(len);
  snprintf(out, len, CSI "38;2;%d;%d;%dm", r, g, b);
  return out;
};

ANSI_ATTRIBUTES char *bg_rgb(unsigned char r, unsigned char g, unsigned char b) {
  const unsigned char len = 20;
  char *out = (char*)malloc(len);
  snprintf(out, len, CSI "48;2;%d;%d;%dm", r, g, b);
  return out;
};

ANSI_ATTRIBUTES char *fg_hex(unsigned int color) { return fg_rgb(HEX_TO_RGB(color)); }
ANSI_ATTRIBUTES char *bg_hex(unsigned int color) { return bg_rgb(HEX_TO_RGB(color)); }

ANSI_ATTRIBUTES char *fg_256(unsigned char color) {
  const unsigned char len = 20;
  char *out = (char*)malloc(len);
  snprintf(out, len, CSI "38;5;%dm", color);
  return out;
}

ANSI_ATTRIBUTES char *bg_256(unsigned char color) {
  const unsigned char len = 20;
  char *out = (char*)malloc(len);
  snprintf(out, len, CSI "38;5;%dm", color);
  return out;
}

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ANSI_IMP__
#undef ANSI_IMPLEMENTATIONS
#endif//ANSI_IMPLEMENTATIONS

