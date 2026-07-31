/*
 * ascii_colors.h - ASCII Terminal Colors Quickstart
 *
 * ascii_colors.h is a simple header-only library that provides basic ANSI
 * escape code macros for terminal colors. It's a lightweight alternative
 * offering 8 standard colors, 8 bright colors, and their background variants.
 *
 * ## Quick Start
 *
 * ### Basic Usage
 *
 *   #include <stdio.h>
 *   #include "ascii_colors.h"
 *
 *   int main(void) {
 *       printf("%sHello in red%s\n", COLOR_RED, COLOR_RESET);
 *       printf("%sHello in bright green%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
 *       printf("%sHello on blue background%s\n", BG_COLOR_BLUE, COLOR_RESET);
 *
 *       return 0;
 *   }
 *
 * ## Color Constants
 *
 * ### Standard Text Colors (8 colors)
 * - COLOR_BLACK
 * - COLOR_RED
 * - COLOR_GREEN
 * - COLOR_YELLOW
 * - COLOR_BLUE
 * - COLOR_MAGENTA
 * - COLOR_CYAN
 * - COLOR_WHITE
 *
 * ### Bright Text Colors (8 colors)
 * - COLOR_BRIGHT_BLACK
 * - COLOR_BRIGHT_RED
 * - COLOR_BRIGHT_GREEN
 * - COLOR_BRIGHT_YELLOW
 * - COLOR_BRIGHT_BLUE
 * - COLOR_BRIGHT_MAGENTA
 * - COLOR_BRIGHT_CYAN
 * - COLOR_BRIGHT_WHITE
 *
 * ### Standard Background Colors (8 colors)
 * - BG_COLOR_BLACK
 * - BG_COLOR_RED
 * - BG_COLOR_GREEN
 * - BG_COLOR_YELLOW
 * - BG_COLOR_BLUE
 * - BG_COLOR_MAGENTA
 * - BG_COLOR_CYAN
 * - BG_COLOR_WHITE
 *
 * ### Bright Background Colors (8 colors)
 * - BG_COLOR_BRIGHT_BLACK
 * - BG_COLOR_BRIGHT_RED
 * - BG_COLOR_BRIGHT_GREEN
 * - BG_COLOR_BRIGHT_YELLOW
 * - BG_COLOR_BRIGHT_BLUE
 * - BG_COLOR_BRIGHT_MAGENTA
 * - BG_COLOR_BRIGHT_CYAN
 * - BG_COLOR_BRIGHT_WHITE
 *
 * ### Reset
 * - COLOR_RESET - Reset all colors to default
 *
 * ## Common Patterns
 *
 * ### Logging with Colors
 *
 *   #define LOG_ERROR(msg) printf("%s[ERROR]%s %s\n", COLOR_RED, COLOR_RESET, msg)
 *   #define LOG_WARN(msg)  printf("%s[WARN]%s %s\n", COLOR_YELLOW, COLOR_RESET, msg)
 *   #define LOG_INFO(msg)  printf("%s[INFO]%s %s\n", COLOR_BLUE, COLOR_RESET, msg)
 *
 *   LOG_ERROR("Something went wrong!");
 *   LOG_WARN("Be careful!");
 *   LOG_INFO("Process started");
 *
 * ### Multi-Color Output
 *
 *   printf("%sRed%s ", COLOR_RED, COLOR_RESET);
 *   printf("%sGreen%s ", COLOR_GREEN, COLOR_RESET);
 *   printf("%sBlue%s\n", COLOR_BLUE, COLOR_RESET);
 *
 * ### Colored Boxes
 *
 *   printf("%s%s", BG_COLOR_BLUE, COLOR_WHITE);
 *   printf("Message in blue box ");
 *   printf("%s\n", COLOR_RESET);
 *
 * ### Simple Table Headers
 *
 *   printf("%s%s%-20s%-20s%-20s%s\n",
 *          BG_COLOR_BLUE, COLOR_WHITE,
 *          "Name", "Age", "City",
 *          COLOR_RESET);
 *
 * ### Status Indicators
 *
 *   #define STATUS_OK    COLOR_GREEN "OK" COLOR_RESET
 *   #define STATUS_FAIL  COLOR_RED "FAIL" COLOR_RESET
 *   #define STATUS_WARN  COLOR_YELLOW "WARN" COLOR_RESET
 *
 *   printf("Status: %s\n", STATUS_OK);
 *   printf("Status: %s\n", STATUS_FAIL);
 *
 * ## Examples
 *
 * ### Full Color Palette Demo
 *
 *   #include <stdio.h>
 *   #include "ascii_colors.h"
 *
 *   int main(void) {
 *       // Standard colors
 *       printf("%sStandard Colors:\n%s", COLOR_WHITE, COLOR_RESET);
 *       const char *colors[] = {
 *           COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
 *           COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE
 *       };
 *       const char *names[] = {
 *           "Black", "Red", "Green", "Yellow",
 *           "Blue", "Magenta", "Cyan", "White"
 *       };
 *
 *       for (int i = 0; i < 8; i++) {
 *           printf("%s%s%s ", colors[i], names[i], COLOR_RESET);
 *       }
 *       printf("\n\n");
 *
 *       // Bright colors
 *       printf("%sBright Colors:\n%s", COLOR_BRIGHT_WHITE, COLOR_RESET);
 *       const char *bright_colors[] = {
 *           COLOR_BRIGHT_BLACK, COLOR_BRIGHT_RED, COLOR_BRIGHT_GREEN,
 *           COLOR_BRIGHT_YELLOW, COLOR_BRIGHT_BLUE, COLOR_BRIGHT_MAGENTA,
 *           COLOR_BRIGHT_CYAN, COLOR_BRIGHT_WHITE
 *       };
 *
 *       for (int i = 0; i < 8; i++) {
 *           printf("%s%s%s ", bright_colors[i], names[i], COLOR_RESET);
 *       }
 *       printf("\n");
 *
 *       return 0;
 *   }
 *
 * ### Progress Indicator with Color
 *
 *   void progress_indicator(int step, int total) {
 *       printf("%s[", COLOR_RESET);
 *       for (int i = 0; i < step; i++) {
 *           printf("%s=", COLOR_GREEN);
 *       }
 *       for (int i = step; i < total; i++) {
 *           printf("%s-", COLOR_RESET);
 *       }
 *       printf("%s] %d%%\n", COLOR_RESET, (step * 100) / total);
 *   }
 *
 * ### Build Status Report
 *
 *   void print_result(const char *test, int passed) {
 *       if (passed) {
 *           printf("%s", COLOR_GREEN);
 *           printf("✓ %s", test);
 *       } else {
 *           printf("%s", COLOR_RED);
 *           printf("✗ %s", test);
 *       }
 *       printf("%s\n", COLOR_RESET);
 *   }
 *
 * ## Important Notes
 *
 * - All constants are ANSI escape code strings
 * - Always use COLOR_RESET to avoid color spillover
 * - Works with any ANSI-compatible terminal
 * - No runtime overhead; all are compile-time constants
 * - No feature detection or platform-specific behavior
 * - Simple and predictable; no hidden complexity
 *
 * ## Limitations
 *
 * - Only 8 standard colors + 8 bright variants (no 256-color support)
 * - No RGB/true color support
 * - No text styles (bold, italic, underline, etc.)
 * - No cursor control
 * - No terminal capability detection
 * - Codes apply to all output until reset
 *
 * ## When to Use
 *
 * Use ascii_colors.h when you need:
 * - Simple color support with minimal overhead
 * - Just the basic 16 colors (8 standard + 8 bright)
 * - A lightweight header with no dependencies
 * - Maximum compatibility across terminals
 *
 * Use ANSI.h or ansi.h when you need:
 * - Text styles (bold, italic, underline)
 * - Cursor control
 * - 256-color or RGB support
 * - More advanced terminal features
 *
 */
