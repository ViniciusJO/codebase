/*
 * ANSI.h - Lightweight ANSI Terminal Control
 *
 * ANSI.h provides a simplified, runtime-agnostic set of ANSI escape code
 * macros and static inline functions for terminal styling. It's a minimal
 * alternative to ansi.h without runtime queries, ideal for simple color and
 * style needs. Does not require IMPLEMENTATIONS definition.
 *
 * ## Quick Start
 *
 * ### Basic Usage
 *
 *   #include <stdio.h>
 *   #include "ANSI.h"
 *
 *   int main(void) {
 *       printf("%s", STYLE_BOLD);
 *       printf("%s", FG_RED);
 *       printf("Bold red text\n");
 *       printf("%s", RESET);
 *
 *       return 0;
 *   }
 *
 * ### Combining Styles
 *
 *   printf("%s%s%s", STYLE_BOLD, FG_GREEN, BG_BLACK);
 *   printf("Bold green on black\n");
 *   printf("%s", RESET);
 *
 * ## Text Styles
 *
 * ### Styling Options
 * - STYLE_BOLD           - Make text bold
 * - STYLE_DIM            - Dim/faint text
 * - STYLE_ITALIC         - Italic text
 * - STYLE_UNDERLINE      - Underline text
 * - STYLE_BLINK          - Blinking text
 * - STYLE_INVERSE        - Reverse video
 * - STYLE_HIDDEN         - Hidden text
 * - STYLE_STRIKETHROUGH  - Strikethrough text
 *
 * ### Turning Off Styles
 * - STYLE_BOLDOFF           - Turn off bold
 * - STYLE_DIMOFF            - Turn off dim
 * - STYLE_ITALICOFF         - Turn off italic
 * - STYLE_UNDERLINEOFF      - Turn off underline
 * - STYLE_BLINKOFF          - Turn off blink
 * - STYLE_INVERSEOFF        - Turn off inverse
 * - STYLE_HIDDENOFF         - Turn off hidden
 * - STYLE_STRIKETHROUGHOFF  - Turn off strikethrough
 *
 * ## Color Options
 *
 * ### Foreground Colors (8 standard colors)
 * - FG_BLACK, FG_RED, FG_GREEN, FG_YELLOW
 * - FG_BLUE, FG_MAGENTA, FG_CYAN, FG_WHITE
 *
 * ### Foreground Bright Colors
 * - FG_BRIGHTBLACK, FG_BRIGHTRED, FG_BRIGHTGREEN, FG_BRIGHTYELLOW
 * - FG_BRIGHTBLUE, FG_BRIGHTMAGENTA, FG_BRIGHTCYAN, FG_BRIGHTWHITE
 *
 * ### Background Colors (8 standard colors)
 * - BG_BLACK, BG_RED, BG_GREEN, BG_YELLOW
 * - BG_BLUE, BG_MAGENTA, BG_CYAN, BG_WHITE
 *
 * ### Background Bright Colors
 * - BG_BRIGHTBLACK, BG_BRIGHTRED, BG_BRIGHTGREEN, BG_BRIGHTYELLOW
 * - BG_BRIGHTBLUE, BG_BRIGHTMAGENTA, BG_BRIGHTCYAN, BG_BRIGHTWHITE
 *
 * ### Color Reset
 * - FG_RESET - Reset foreground to default
 * - BG_RESET - Reset background to default
 * - RESET    - Reset all formatting
 *
 * ## Advanced Colors
 *
 * ### 256-Color Palette
 *
 *   char buffer[32];
 *   fg_256(42, buffer);  // Color 42 from 256-color palette
 *   printf("%s", buffer);
 *
 *   char bg_buffer[32];
 *   bg_256(100, bg_buffer);
 *   printf("%s", bg_buffer);
 *
 * ### True Color (RGB)
 *
 *   char buffer[32];
 *   fg_rgb(255, 128, 0, buffer);  // RGB orange
 *   printf("%s", buffer);
 *   printf("Orange text\n");
 *
 *   bg_rgb(50, 50, 50, buffer);   // Dark gray background
 *   printf("%s", buffer);
 *   printf("On dark gray\n");
 *
 * ### Hex Color
 *
 *   #define HEX_TO_RGB(hex) ((hex >> 24) & 0xFF), ((hex >> 16) & 0xFF), ((hex >> 8) & 0xFF)
 *
 *   char buffer[32];
 *   fg_hex(0xFF8000);  // Orange: R=255, G=128, B=0
 *   printf("%s", buffer);
 *
 * ## Cursor Control
 *
 * ### Cursor Movement
 * - CURSOR_UP(n)      - Move cursor up n lines
 * - CURSOR_DOWN(n)    - Move cursor down n lines
 * - CURSOR_FORWARD(n) - Move cursor right n columns
 * - CURSOR_BACK(n)    - Move cursor left n columns
 * - CURSOR_POSITION(row, col) - Move to specific position
 * - CURSOR_COLUMN(n)  - Move to column n
 *
 * ### Cursor Special
 * - CURSOR_HIDE        - Hide cursor
 * - CURSOR_SHOW        - Show cursor
 * - CURSOR_SAVE        - Save cursor position
 * - CURSOR_RESTORE     - Restore saved position
 *
 * ## Screen Control
 *
 * ### Clearing
 * - CLEAR_SCREEN       - Clear entire screen
 * - CLEAR_SCREENDOWN   - Clear from cursor to bottom
 * - CLEAR_SCREENUP     - Clear from top to cursor
 * - CLEAR_LINE         - Clear entire line
 * - CLEAR_LINERIGHT    - Clear from cursor to right
 * - CLEAR_LINELEFT     - Clear from left to cursor
 *
 * ## Common Patterns
 *
 * ### Function: Style Wrapper
 *
 *   void print_error(const char *msg) {
 *       printf("%s%sERROR:%s %s\n",
 *              STYLE_BOLD, FG_RED, RESET, msg);
 *   }
 *
 *   void print_success(const char *msg) {
 *       printf("%s%sSUCCESS:%s %s\n",
 *              STYLE_BOLD, FG_GREEN, RESET, msg);
 *   }
 *
 * ### Function: Table with Colors
 *
 *   void print_header(void) {
 *       printf("%s%s", STYLE_BOLD, FG_WHITE);
 *       printf("%-20s %-20s %-20s\n", "Name", "Age", "City");
 *       printf("%s", RESET);
 *   }
 *
 * ### Function: Progress Bar
 *
 *   void progress_bar(int percent) {
 *       printf("%s", FG_GREEN);
 *       for (int i = 0; i < percent / 5; i++) printf("=");
 *       printf("%s %d%%\n", RESET, percent);
 *   }
 *
 * ### RGB Color Palette
 *
 *   for (int r = 0; r < 256; r += 50) {
 *       for (int g = 0; g < 256; g += 50) {
 *           char buf[32];
 *           fg_rgb(r, g, 128, buf);
 *           printf("%s█", buf);
 *       }
 *       printf("%s\n", RESET);
 *   }
 *
 * ## Configuration
 *
 * Define before inclusion:
 *
 * - ESC   - Escape character (default: "\\x1b")
 * - CSI   - Control Sequence Introducer (default: ESC"[")
 *
 * ## Important Notes
 *
 * - All macros produce strings; use in printf-like functions
 * - RGB functions allocate memory; free the returned pointer
 * - 256-color and RGB macros require runtime functions (allocate)
 * - Hex color helper converts format: 0xRRGGBB to RGB components
 * - No terminal capability detection; codes work on all ANSI terminals
 * - Color codes are literal; use strategically to avoid overhead
 *
 * ## Limitations
 *
 * - No runtime terminal detection or capability checking
 * - No cursor position queries
 * - RGB functions require manual memory management
 * - No mouse or advanced terminal features
 * - Terminal support varies; some systems may not support all codes
 *
 * ## Differences from ansi.h
 *
 * ANSI.h is simpler and has no runtime overhead. ansi.h has terminal
 * queries and more features but requires ANSI_IMPLEMENTATIONS. Choose
 * ANSI.h for simple use cases, ansi.h for advanced features.
 *
 */
