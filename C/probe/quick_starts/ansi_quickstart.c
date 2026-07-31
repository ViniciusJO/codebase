/*
 * ansi.h - ANSI Terminal Control Quickstart
 *
 * ansi.h provides comprehensive macros and functions for terminal control
 * including colors (standard, bright, 256-color, and true color RGB), text
 * styles, cursor movement, screen clearing, and terminal queries. Requires
 * define ANSI_IMPLEMENTATIONS in one .c file to enable functions.
 *
 * ## Quick Start
 *
 * ### Basic Colors
 *
 *   #include <stdio.h>
 *   #include "ansi.h"
 *
 *   int main(void) {
 *       printf("%s", ANSI_FG_RED);
 *       printf("This is red text\n");
 *       printf("%s", ANSI_RESET);
 *
 *       printf("%s%s", ANSI_FG_GREEN, ANSI_BOLD);
 *       printf("Bold green text\n");
 *       printf("%s", ANSI_RESET);
 *
 *       return 0;
 *   }
 *
 * ### Cursor Control
 *
 *   printf("%s", ANSI_CURSOR_HIDE);
 *   // ... do work ...
 *   printf("%s", ANSI_CURSOR_SHOW);
 *
 *   printf("%s", ANSI_CURSOR_POSITION(10, 5));  // Row 10, Col 5
 *   printf("Positioned text");
 *
 * ### Screen Control
 *
 *   printf("%s", ANSI_CLEAR_SCREEN);  // Clear entire screen
 *   printf("%s", ANSI_CLEAR_LINE);    // Clear current line
 *
 * ## Color Options
 *
 * ### Standard Colors (8 colors)
 * - ANSI_FG_BLACK, ANSI_FG_RED, ANSI_FG_GREEN, ANSI_FG_YELLOW
 * - ANSI_FG_BLUE, ANSI_FG_MAGENTA, ANSI_FG_CYAN, ANSI_FG_WHITE
 * - ANSI_FG_DEFAULT (reset to default)
 *
 * ### Bright Colors (8 colors)
 * - ANSI_FG_BBLACK, ANSI_FG_BRED, ANSI_FG_BGREEN, ANSI_FG_BYELLOW
 * - ANSI_FG_BBLUE, ANSI_FG_BMAGENTA, ANSI_FG_BCYAN, ANSI_FG_BWHITE
 *
 * ### Background Colors
 * - ANSI_BG_BLACK through ANSI_BG_WHITE
 * - ANSI_BG_BBLACK through ANSI_BG_BWHITE (bright)
 *
 * ### 256-Color Mode
 * - ANSI_FG_256(n)  - Foreground color 0-255
 * - ANSI_BG_256(n)  - Background color 0-255
 *
 * ### True Color (RGB)
 * - ANSI_FG_RGB(r, g, b)  - RGB foreground (requires runtime function)
 * - ANSI_BG_RGB(r, g, b)  - RGB background (requires runtime function)
 * - ansi_fg_hex(color)     - Hex color (requires implementation)
 * - ansi_bg_hex(color)     - Hex background (requires implementation)
 *
 * ## Text Styles
 *
 * - ANSI_BOLD           - Bold text
 * - ANSI_DIM            - Dim/faint text
 * - ANSI_ITALIC         - Italic text
 * - ANSI_UNDERLINE      - Underline text
 * - ANSI_BLINK          - Blinking text
 * - ANSI_REVERSE        - Reverse video (swap fg/bg)
 * - ANSI_HIDDEN         - Hidden text
 * - ANSI_STRIKE         - Strikethrough text
 *
 * ### Style Off Variants
 * - ANSI_BOLDOFF, ANSI_DIMOFF, ANSI_ITALICOFF, ANSI_UNDERLINEOFF
 * - ANSI_BLINKOFF, ANSI_INVERSEOFF, ANSI_HIDDENOFF, ANSI_STRIKEOFF
 *
 * ## Cursor Control
 *
 * ### Cursor Movement
 * - ANSI_CURSOR_UP(n)           - Move up n lines
 * - ANSI_CURSOR_DOWN(n)         - Move down n lines
 * - ANSI_CURSOR_FORWARD(n)      - Move forward n columns
 * - ANSI_CURSOR_BACK(n)         - Move back n columns
 * - ANSI_CURSOR_POSITION(row, col) - Move to specific position
 * - ANSI_CURSOR_COLUMN(n)       - Move to column n
 * - ANSI_CURSOR_NEXTLINE(n)     - Move to next line
 * - ANSI_CURSOR_PREVLINE(n)     - Move to previous line
 *
 * ### Cursor Visibility
 * - ANSI_CURSOR_HIDE  - Hide cursor
 * - ANSI_CURSOR_SHOW  - Show cursor
 * - ANSI_CURSOR_SAVE  - Save cursor position
 * - ANSI_CURSOR_RESTORE - Restore saved position
 *
 * ## Screen Control
 *
 * - ANSI_CLEAR_SCREEN        - Clear entire screen
 * - ANSI_CLEAR_SCREENDOWN    - Clear from cursor to bottom
 * - ANSI_CLEAR_SCREENUP      - Clear from top to cursor
 * - ANSI_CLEAR_LINE          - Clear entire line
 * - ANSI_CLEAR_LINE_RIGHT    - Clear from cursor to right
 * - ANSI_CLEAR_LINE_LEFT     - Clear from left to cursor
 * - ANSI_SCROLL_UP(n)        - Scroll up n lines
 * - ANSI_SCROLL_DOWN(n)      - Scroll down n lines
 * - ANSI_ALTERNATE_BUFFER    - Switch to alternate buffer
 * - ANSI_MAIN_BUFFER         - Switch back to main buffer
 *
 * ## Terminal Information
 *
 * ### Query Terminal Capabilities
 *
 *   #define ANSI_IMPLEMENTATIONS
 *   #include "ansi.h"
 *
 *   int main(void) {
 *       ansi_terminal_info_t info = {0};
 *       ansi_get_terminal_info(&info);
 *
 *       printf("Is TTY: %d\n", info.is_tty);
 *       printf("Has color: %d\n", info.has_color);
 *       printf("Size: %d x %d\n", info.rows, info.cols);
 *
 *       return 0;
 *   }
 *
 * ### Mouse Support
 *
 *   ansi_mouse_enable();
 *   // ... read mouse events from stdin ...
 *   ansi_mouse_disable();
 *
 * ## Common Patterns
 *
 * ### Progress Bar
 *
 *   printf("%s", ANSI_FG_GREEN);
 *   for (int i = 0; i < 100; i++) {
 *       printf("=");
 *       fflush(stdout);
 *       usleep(50000);
 *   }
 *   printf("%s\n", ANSI_RESET);
 *
 * ### Colored Error Messages
 *
 *   #define ERROR(msg) printf("%sERROR:%s %s\n", ANSI_FG_RED, ANSI_RESET, msg)
 *   #define INFO(msg)  printf("%sINFO:%s %s\n", ANSI_FG_BLUE, ANSI_RESET, msg)
 *
 * ### Rainbow Text
 *
 *   const char *colors[] = {
 *       ANSI_FG_RED, ANSI_FG_YELLOW, ANSI_FG_GREEN,
 *       ANSI_FG_BLUE, ANSI_FG_MAGENTA
 *   };
 *   for (int i = 0; i < 5; i++) {
 *       printf("%s%c", colors[i], "HELLO"[i]);
 *   }
 *   printf("%s\n", ANSI_RESET);
 *
 * ## Important Notes
 *
 * - ANSI codes are disabled if NO_COLORS is defined
 * - Colors only work on TTY terminals (automatically detected)
 * - RGB and 256-color modes require terminal support
 * - Runtime RGB functions (ansi_fg_rgb) require ANSI_IMPLEMENTATIONS
 * - Always ANSI_RESET after applying styles to avoid spillover
 * - Nested styles can be combined without issues
 * - Cursor position queries may be slow; cache if possible
 *
 * ## Limitations
 *
 * - Not all terminals support all features (e.g., mouse support)
 * - RGB colors may degrade to 256-color or standard colors
 * - Cursor queries block and may fail on non-interactive streams
 * - Terminal detection via TERM environment variable is heuristic
 * - Some escape sequences are terminal-specific
 *
 */
