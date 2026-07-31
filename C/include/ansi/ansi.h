#ifndef __ANSI_H__
#define __ANSI_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

/*
 * ANSI Toolkit - Full Terminal Control & Queries
 * Features:
 *  - Styles, colors (standard, bright, RGB, 256-color)
 *  - Cursor & screen control
 *  - Window/tab control
 *  - Mouse support
 *  - Runtime queries via ansi_terminal_info_t struct
 */

#ifndef ANSI_ATTRIBUTES
#define ANSI_ATTRIBUTES inline static
#endif//ANSI_ATTRIBUTES
      //
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif//STDOUT_FILENO

/* ---------------------- ENABLE / DISABLE ANSI ---------------------- */
#if defined(NO_COLORS)
#  define ANSI_ENABLED 0
#else
#  define ANSI_ENABLED 1
#endif

#define ESC "\x1b["
#define __ANSI_ACTIVE (ANSI_ENABLED && isatty(STDOUT_FILENO))
// #define __ANSI_SEQ(seq) (__ANSI_ACTIVE ? ESC seq : "")
#define __ANSI_SEQ(seq) ESC seq

/* ─────────────────────────── Reset ─────────────────────────── */

#define ANSI_RESET       __ANSI_SEQ("0m")

/* ─────────────────────────── Text styles ─────────────────────────── */
#define ANSI_BOLD           __ANSI_SEQ("1m")
#define ANSI_DIM            __ANSI_SEQ("2m")
#define ANSI_ITALIC         __ANSI_SEQ("3m")
#define ANSI_UNDERLINE      __ANSI_SEQ("4m")
#define ANSI_BLINK          __ANSI_SEQ("5m")
#define ANSI_REVERSE        __ANSI_SEQ("7m")
#define ANSI_HIDDEN         __ANSI_SEQ("8m")
#define ANSI_STRIKE         __ANSI_SEQ("9m")

#define ANSI_BOLDOFF        __ANSI_SEQ("22m")
#define ANSI_DIMOFF         __ANSI_SEQ("22m")
#define ANSI_ITALICOFF      __ANSI_SEQ("23m")
#define ANSI_UNDERLINEOFF   __ANSI_SEQ("24m")
#define ANSI_BLINKOFF       __ANSI_SEQ("25m")
#define ANSI_INVERSEOFF     __ANSI_SEQ("27m")
#define ANSI_HIDDENOFF      __ANSI_SEQ("28m")
#define ANSI_STRIKEOFF      __ANSI_SEQ("29m")

/* ─────────────────────────── Foreground colors ─────────────────────────── */

#define ANSI_FG_BLACK       __ANSI_SEQ("30m")
#define ANSI_FG_RED         __ANSI_SEQ("31m")
#define ANSI_FG_GREEN       __ANSI_SEQ("32m")
#define ANSI_FG_YELLOW      __ANSI_SEQ("33m")
#define ANSI_FG_BLUE        __ANSI_SEQ("34m")
#define ANSI_FG_MAGENTA     __ANSI_SEQ("35m")
#define ANSI_FG_CYAN        __ANSI_SEQ("36m")
#define ANSI_FG_WHITE       __ANSI_SEQ("37m")

#define ANSI_FG_DEFAULT     __ANSI_SEQ("39m")

#define ANSI_FG_BBLACK      __ANSI_SEQ("90m")
#define ANSI_FG_BRED        __ANSI_SEQ("91m")
#define ANSI_FG_BGREEN      __ANSI_SEQ("92m")
#define ANSI_FG_BYELLOW     __ANSI_SEQ("93m")
#define ANSI_FG_BBLUE       __ANSI_SEQ("94m")
#define ANSI_FG_BMAGENTA    __ANSI_SEQ("95m")
#define ANSI_FG_BCYAN       __ANSI_SEQ("96m")
#define ANSI_FG_BWHITE      __ANSI_SEQ("97m")

#define ANSI_FG_RESET       __ANSI_SEQ("39m")

/* ─────────────────────────── Background colors ─────────────────────────── */

#define ANSI_BG_BLACK       __ANSI_SEQ("40m")
#define ANSI_BG_RED         __ANSI_SEQ("41m")
#define ANSI_BG_GREEN       __ANSI_SEQ("42m")
#define ANSI_BG_YELLOW      __ANSI_SEQ("43m")
#define ANSI_BG_BLUE        __ANSI_SEQ("44m")
#define ANSI_BG_MAGENTA     __ANSI_SEQ("45m")
#define ANSI_BG_CYAN        __ANSI_SEQ("46m")
#define ANSI_BG_WHITE       __ANSI_SEQ("47m")
#define ANSI_BG_DEFAULT     __ANSI_SEQ("49m")

#define ANSI_BG_BBLACK      __ANSI_SEQ("100m")
#define ANSI_BG_BRED        __ANSI_SEQ("101m")
#define ANSI_BG_BGREEN      __ANSI_SEQ("102m")
#define ANSI_BG_BYELLOW     __ANSI_SEQ("103m")
#define ANSI_BG_BBLUE       __ANSI_SEQ("104m")
#define ANSI_BG_BMAGENTA    __ANSI_SEQ("105m")
#define ANSI_BG_BCYAN       __ANSI_SEQ("106m")
#define ANSI_BG_BWHITE      __ANSI_SEQ("107m")

#define ANSI_BG_RESET       __ANSI_SEQ("49m")

/* ─────────────────────────── True color (24-bit RGB) ─────────────────────────── */


#define ANSI_FG_RGB(LITERAL_R, LITERAL_G, LITERAL_B) CSI "38;2;" #LITERAL_R ";" #LITERAL_G ";" #LITERAL_B "m"
#define ANSI_BG_RGB(LITERAL_R, LITERAL_G, LITERAL_B) CSI "48;2;" #LITERAL_R ";" #LITERAL_G ";" #LITERAL_B "m"

#define ANSI_FG_BUFF_RGB(LITERAL_R,LITERAL_G,LITERAL_B,BUF) sprintf(BUF, "\x1b[38;2;%d;%d;%dm", LITERAL_R,LITERAL_G,LITERAL_B)
#define ANSI_BG_BUFF_RGB(LITERAL_R,LITERAL_G,LITERAL_B,BUF) sprintf(BUF, "\x1b[48;2;%d;%d;%dm", LITERAL_R,LITERAL_G,LITERAL_B)

ANSI_ATTRIBUTES char *ansi_fg_rgb(unsigned char r, unsigned char g, unsigned char b);
ANSI_ATTRIBUTES char *ansi_bg_rgb(unsigned char r, unsigned char g, unsigned char b);
ANSI_ATTRIBUTES char *ansi_fg_hex(unsigned int color);
ANSI_ATTRIBUTES char *ansi_bg_hex(unsigned int color);

#define HEX_TO_RGB(HEX) (((HEX) >> 24) & 0xFF), (((HEX) >> 16) & 0xFF), (((HEX) >> 8) & 0xFF)

/* ─────────────────────────── 256-color support ─────────────────────────── */

#define ANSI_FG_256(LITERAL_COLOR_NUMBER) CSI "38;5;" #LITERAL_COLOR_NUMBER "m"
#define ANSI_BG_256(LITERAL_COLOR_NUMBER) CSI "48;5;" #LITERAL_COLOR_NUMBER "m"

#define ANSI_FG_BUFF_256(LITERAL_N,BUF) sprintf(BUF, "\x1b[38;5;%dm", LITERAL_N)
#define ANSI_BG_BUFF_256(LITERAL_N,BUF) sprintf(BUF, "\x1b[48;5;%dm", LITERAL_N)

ANSI_ATTRIBUTES char *ansi_fg_256(unsigned char color);
ANSI_ATTRIBUTES char *ansi_bg_256(unsigned char color);

// ─────────────────────────── Cursor ───────────────────────────

#define ANSI_CURSOR_UP(LITERAL_N) __ANSI_SEQ(#LITERAL_N "A")
#define ANSI_CURSOR_DOWN(LITERAL_N) __ANSI_SEQ(#LITERAL_N "B")
#define ANSI_CURSOR_FORWARD(LITERAL_N) __ANSI_SEQ(#LITERAL_N "C")
#define ANSI_CURSOR_BACK(LITERAL_N) __ANSI_SEQ(#LITERAL_N "D")
#define ANSI_CURSOR_NEXTLINE(LITERAL_N) __ANSI_SEQ(#LITERAL_N "E")
#define ANSI_CURSOR_PREVLINE(LITERAL_N) __ANSI_SEQ(#LITERAL_N "F")
#define ANSI_CURSOR_COLUMN(LITERAL_N) __ANSI_SEQ(#LITERAL_N "G")
#define ANSI_CURSOR_POSITION(LITERAL_ROW, LITERAL_COL) __ANSI_SEQ(#LITERAL_ROW ";" #LITERAL_COL "H")
#define ANSI_CURSOR_HIDE __ANSI_SEQ("?25l")
#define ANSI_CURSOR_SHOW __ANSI_SEQ("?25h")
#define ANSI_CURSOR_SAVE ESC "7"
#define ANSI_CURSOR_RESTORE ESC "8"

// #define ANSI_CURSOR_UP(n)       __ANSI_SEQ(#n "A")
// #define ANSI_CURSOR_DOWN(n)     __ANSI_SEQ(#n "B")
// #define ANSI_CURSOR_RIGHT(n)    __ANSI_SEQ(#n "C")
// #define ANSI_CURSOR_LEFT(n)     __ANSI_SEQ(#n "D")
// #define ANSI_CURSOR_COLUMN(n)   __ANSI_SEQ(#n "G")
// #define ANSI_CURSOR_POSITION(row,col) __ANSI_SEQ(#row ";" #col "H")
#define ANSI_CURSOR_LINE_START  __ANSI_SEQ("1G")
#define ANSI_CURSOR_LINE_END    __ANSI_SEQ("1000G")
#define ANSI_CURSOR_SCREEN_START __ANSI_SEQ("H")
#define ANSI_CURSOR_SCREEN_END   __ANSI_SEQ("999;999H")
// #define ANSI_CURSOR_SAVE        __ANSI_SEQ("s")
// #define ANSI_CURSOR_RESTORE     __ANSI_SEQ("u")
// #define ANSI_CURSOR_HIDE        __ANSI_SEQ("?25l")
// #define ANSI_CURSOR_SHOW        __ANSI_SEQ("?25h")


/* ─────────────────────────── Clear screen / line ─────────────────────────── */

#define ANSI_CLEAR_SCREEN     __ANSI_SEQ("2J")
#define ANSI_CLEAR_LINE       __ANSI_SEQ("2K")
#define ANSI_CLEAR_LINE_RIGHT __ANSI_SEQ("0K")
#define ANSI_CLEAR_LINE_LEFT  __ANSI_SEQ("1K")
#define ANSI_SCROLL_UP(n)     __ANSI_SEQ(#n "S")
#define ANSI_SCROLL_DOWN(n)   __ANSI_SEQ(#n "T")
#define ANSI_ALTERNATE_BUFFER __ANSI_SEQ("?1049h")
#define ANSI_MAIN_BUFFER      __ANSI_SEQ("?1049l")
#define ANSI_CLEAR_SCREENDOWN __ANSI_SEQ("0J")
#define ANSI_CLEAR_SCREENUP   __ANSI_SEQ("1J")

/* ────────────────────────── Window / TAB/ Misc ────────────────────────── */

#define ANSI_TITLE_FMT "\x1b]0;%s\x07"
#define ANSI_SET_TITLE(TITLE, BUFF)   do { if(__ANSI_ACTIVE) sprintf(BUFF, "\x1b]0;%s\x07", TITLE); } while(0)
#define ANSI_BELL "\a"
#define ANSI_HYPERLINK(URL,TEXT) "\x1b]8;;" URL "\x1b\\" TEXT "\x1b]8;;\x1b\\"
#define ANSI_TAB_SET   __ANSI_SEQ("H")
#define ANSI_TAB_CLEAR __ANSI_SEQ("3g")

/* ──────────────────────────── Terminal Info  ─────────────────────────── */

typedef struct {
    int is_tty;          // stdout is a terminal
    int has_color;       // supports colors
    int rows, cols;      // terminal dimensions
    int cursor_row;      // cursor position
    int cursor_col;
    char device_attrs[64]; // device attributes (DA)
} ansi_terminal_info_t;


ANSI_ATTRIBUTES void ansi_get_terminal_info(ansi_terminal_info_t* info);
ANSI_ATTRIBUTES void ansi_mouse_enable(void);
ANSI_ATTRIBUTES void ansi_mouse_disable(void);


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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

ANSI_ATTRIBUTES char *ansi_fg_rgb(unsigned char r, unsigned char g, unsigned char b) {
  if(isatty(STDOUT_FILENO)) {
    const unsigned char len = 20;
    char *out = (char*)malloc(len);
    snprintf(out, len, ESC "38;2;%d;%d;%dm", r, g, b);
    return out;
  }
  return (char*)"";
};

ANSI_ATTRIBUTES char *ansi_bg_rgb(unsigned char r, unsigned char g, unsigned char b) {
  if(isatty(STDOUT_FILENO)) {
    const unsigned char len = 20;
    char *out = (char*)malloc(len);
    snprintf(out, len, ESC "48;2;%d;%d;%dm", r, g, b);
    return out;
  }
  return (char*)"";
};

ANSI_ATTRIBUTES char *ansi_fg_hex(unsigned int color) { return ansi_fg_rgb(HEX_TO_RGB(color)); }
ANSI_ATTRIBUTES char *ansi_bg_hex(unsigned int color) { return ansi_bg_rgb(HEX_TO_RGB(color)); }

ANSI_ATTRIBUTES char *ansi_fg_256(unsigned char color) {
  if(isatty(STDOUT_FILENO)) {
    const unsigned char len = 20;
    char *out = (char*)malloc(len);
    snprintf(out, len, ESC "38;5;%dm", color);
    return out;
  }
  return (char*)"";
}

ANSI_ATTRIBUTES char *ansi_bg_256(unsigned char color) {
  if(isatty(STDOUT_FILENO)) {
    const unsigned char len = 20;
    char *out = (char*)malloc(len);
    snprintf(out, len, ESC "38;5;%dm", color);
    return out;
  }
  return (char*)"";
}


ANSI_ATTRIBUTES void ansi_get_terminal_info(ansi_terminal_info_t* info) {
    if(!info) return;
    info->is_tty = isatty(STDOUT_FILENO);
    const char* term = getenv("TERM");
    info->has_color = term && (strstr(term,"xterm") || strstr(term,"color") || strstr(term,"ansi"));
    struct winsize ws;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        info->rows = ws.ws_row;
        info->cols = ws.ws_col;
    } else {
        info->rows = 24;
        info->cols = 80;
    }
    /* Query cursor position */
    info->cursor_row = -1;
    info->cursor_col = -1;
    printf("\x1b[6n"); fflush(stdout);
    if(fgets(info->device_attrs, sizeof(info->device_attrs), stdin)) {
        int r,c;
        if(sscanf(info->device_attrs, "\x1b[%d;%dR",&r,&c) == 2) {
            info->cursor_row = r;
            info->cursor_col = c;
        }
    }
    /* Query device attributes */
    printf("\x1b[c"); fflush(stdout);
    if(fgets(info->device_attrs, sizeof(info->device_attrs), stdin) == NULL) {
        strcpy(info->device_attrs,"Unknown");
    }
}

ANSI_ATTRIBUTES void ansi_mouse_enable(void) {
    if(__ANSI_ACTIVE) {
        printf("\x1b[?1000h");   // normal tracking
        printf("\x1b[?1006h");   // SGR extended mode
        fflush(stdout);
    }
}
ANSI_ATTRIBUTES void ansi_mouse_disable(void) {
    if(__ANSI_ACTIVE) {
        printf("\x1b[?1000l");
        printf("\x1b[?1006l");
        fflush(stdout);
    }
}

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ANSI_IMP__
#undef ANSI_IMPLEMENTATIONS
#endif//ANSI_IMPLEMENTATIONS
