#ifndef __COLORS_H__
#define __COLORS_H__

#include <stdio.h>
#define COLOR_BLACK "\033[0;30m"
#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_ORANGE "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_PURPLE "\033[0;35m"
#define COLOR_CYAN "\033[0;36m"
#define COLOR_LIGHT_GRAY "\033[0;37m"
#define COLOR_DARK_GRAY "\033[1;30m"
#define COLOR_LIGHT_RED "\033[1;31m"
#define COLOR_LIGHT_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_LIGHT_BLUE "\033[1;34m"
#define COLOR_LIGHT_PURPLE "\033[1;35m"
#define COLOR_LIGHT_CYAN "\033[1;36m"
#define COLOR_WHITE "\033[1;37m"
#define COLOR_RESET "\033[0m"

#define COLORIZE_STRING(string, color) color string COLOR_RESET 

#define UNHEX_RGB(rgb) (rgb>>24)&0xFF, (rgb>>16)&0xFF, (rgb>>8)&0xFF
#define UNHEXF_RGB(rgb) ((rgb>>24)&0xFF)/255.f, ((rgb>>16)&0xFF)/255.f, ((rgb>>8)&0xFF)/255.f
#define UNHEX_RGBA(rgba) (rgb>>24)&0xFF, (rgb>>16)&0xFF, (rgb>>8)&0xFF, (rgb>>0)&0xFF
#define UNHEXF_RGBA(rgba) ((rgb>>24)&0xFF)/255.f, ((rgb>>16)&0xFF)/255.f, ((rgb>>8)&0xFF)/255.f, ((rgb>>0)&0xFF)/255.f

#define COLORIZE_RGB(string, rgba) "\033[38;2;%d;%d;%dm" "\033[48;2;55;55;55m" string COLOR_RESET, (rgba>>24)&0xFF, (rgba>>16)&0xFF, (rgba>>8)&0xFF
#define COLOR_RGB(string, fg, bg) "\033[38;2;%d;%d;%dm" "\033[48;2;%d;%d;%dm" string COLOR_RESET, (fg>>24)&0xFF, (fg>>16)&0xFF, (fg>>8)&0xFF, (bg>>24)&0xFF, (bg>>16)&0xFF, (bg>>8)&0xFF

void color_string(const char *str, int r, int g, int b) {
    printf("\033[38;2;%d;%d;%dm%s\033[0m", r, g, b, str);
}

#endif //__COLORS_H__
