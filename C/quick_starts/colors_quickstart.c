/*
 * colors.h - Terminal Colors and Styling Quickstart
 *
 * colors.h provides ANSI escape code macros and helper functions for
 * terminal text coloring with both foreground and background colors.
 * It includes standard colors, bright colors, and RGB color support
 * with utility macros for unpacking hex colors.
 *
 * ## Quick Start
 *
 * ### Basic Usage
 *
 *   #include <stdio.h>
 *   #include "colors.h"
 *
 *   int main(void) {
 *       printf("%sRed text%s
", COLOR_RED, COLOR_RESET);
 *       printf("%sGreen on dark gray%s%s
",
 *              COLOR_GREEN, BG_COLOR_DARK_GRAY, COLOR_RESET);
 *
 *       return 0;
 *   }
 *
 * ## Color Constants
 *
 * ### Foreground Colors
 *
 * Standard Colors:
 * - COLOR_BLACK
 * - COLOR_RED
 * - COLOR_GREEN
 * - COLOR_ORANGE (dark yellow)
 * - COLOR_BLUE
 * - COLOR_PURPLE
 * - COLOR_CYAN
 * - COLOR_LIGHT_GRAY
 *
 * Bright Colors:
 * - COLOR_DARK_GRAY
 * - COLOR_LIGHT_RED
 * - COLOR_LIGHT_GREEN
 * - COLOR_YELLOW
 * - COLOR_LIGHT_BLUE
 * - COLOR_LIGHT_PURPLE
 * - COLOR_LIGHT_CYAN
 * - COLOR_WHITE
 *
 * Reset:
 * - COLOR_RESET - Reset all colors
 *
 * ### Background Colors
 *
 * Same palette available as BG_COLOR_* variants (defined elsewhere)
 *
 * ## Functions
 *
 * ### RGB Color Output
 *
 *   void color_string(const char *str, int r, int g, int b)
 *   Print a string with specified RGB foreground color
 *
 *   Example:
 *   color_string(\"Hello\", 255, 128, 0);  // Orange text
 *
 * ## Color Macros
 *
 * ### String Colorization
 *
 *   COLORIZE_STRING(string, color)
 *   Wraps a string with color codes; returns concatenated result
 *   Note: This requires a compiler supporting string concatenation
 *
 *   printf(\"%s\
\", COLORIZE_STRING(\"Red text\", COLOR_RED));
 *
 * ### Hex Color Unpacking
 *
 * Helper macros to extract RGB components from 24-bit hex colors:
 *
 *   UNHEX_RGB(rgb)        - Extract RGB tuple: (r, g, b)
 *   UNHEXF_RGB(rgb)       - Extract normalized RGB: (r/255.0f, g/255.0f, b/255.0f)
 *   UNHEX_RGBA(rgba)      - Extract RGBA tuple: (r, g, b, a)
 *   UNHEXF_RGBA(rgba)     - Extract normalized RGBA: (r/255.0f, g/255.0f, b/255.0f, a/255.0f)
 *
 *   Example:
 *   #define ORANGE 0xFF8000
 *   color_string(\"Text\", UNHEX_RGB(ORANGE));
 *
 * ### RGB Colorization Macros
 *
 *   COLORIZE_RGB(string, rgb)
 *   Color text with RGB value and gray background (0x373737)
 *
 *   COLOR_RGB(string, fg_rgb, bg_rgb)
 *   Color text with separate RGB foreground and background
 *
 *   Both return format string for printf; usage:
 *   printf(COLORIZE_RGB(\"Text\", 0xFF0000));  // Red on gray
 *
 * ## Common Patterns
 *
 * ### Color Palette Demo
 *
 *   int main(void) {
 *       printf(\"%sStandard Colors:%s\
\", COLOR_WHITE, COLOR_RESET);
 *       printf(\"%sBlack%s \", COLOR_BLACK, COLOR_RESET);
 *       printf(\"%sRed%s \", COLOR_RED, COLOR_RESET);
 *       printf(\"%sGreen%s \", COLOR_GREEN, COLOR_RESET);
 *       printf(\"%sYellow%s\
\", COLOR_YELLOW, COLOR_RESET);
 *
 *       printf(\"%sBright Colors:%s\
\", COLOR_LIGHT_WHITE, COLOR_RESET);
 *       printf(\"%sWhite%s \", COLOR_WHITE, COLOR_WHITE);
 *       printf(\"%sLight Red%s \", COLOR_LIGHT_RED, COLOR_RESET);
 *       printf(\"%sLight Green%s\
\", COLOR_LIGHT_GREEN, COLOR_RESET);
 *
 *       return 0;
 *   }
 *
 * ### Status Messages
 *
 *   #define ERROR_MSG(msg) do { \\
 *       printf(\"%sERROR:%s %s\
\", COLOR_RED, COLOR_RESET, msg); \\
 *   } while(0)
 *
 *   #define SUCCESS_MSG(msg) do { \\
 *       printf(\"%sSUCCESS:%s %s\
\", COLOR_GREEN, COLOR_RESET, msg); \\
 *   } while(0)
 *
 *   #define WARNING_MSG(msg) do { \\
 *       printf(\"%sWARNING:%s %s\
\", COLOR_YELLOW, COLOR_RESET, msg); \\
 *   } while(0)
 *
 * ### RGB Color Functions
 *
 *   void print_gradient(void) {
 *       for (int i = 0; i < 256; i += 10) {
 *           color_string(\"█\", i, 0, 255-i);  // Blue-ish to magenta
 *       }
 *       printf(\"\
\");
 *   }
 *
 * ### Hex Color Usage
 *
 *   #define BRAND_COLOR 0x007AFF  // Apple blue
 *   #define SUCCESS     0x34C759  // Apple green
 *   #define WARNING     0xFF9500  // Apple orange
 *   #define ERROR       0xFF3B30  // Apple red
 *
 *   int main(void) {
 *       color_string(\"App\", UNHEX_RGB(BRAND_COLOR));
 *       printf(\" \");
 *       color_string(\"Status\", UNHEX_RGB(SUCCESS));
 *       printf(\"\
\");
 *
 *       return 0;
 *   }
 *
 * ## Important Notes
 *
 * - All color constants are ANSI escape code strings
 * - color_string() function directly prints (no return value)
 * - RGB unpacking macros work with 24-bit hex colors (0xRRGGBB)
 * - Always use COLOR_RESET to prevent color spillover
 * - Functions require stdio.h inclusion
 *
 * ## Limitations
 *
 * - Limited to ANSI-compatible terminals
 * - No text styles (bold, italic, underline)
 * - No cursor control
 * - No 256-color palette support (only standard + bright)
 * - RGB support via color_string() function only
 * - No terminal capability detection
 *
 * ## When to Use
 *
 * Use colors.h when you need:
 * - Simple ANSI color support
 * - Direct string coloring with color_string()
 * - RGB color unpacking from hex values
 * - Minimal dependencies and straightforward API
 *
 * Use ANSI.h when you need:
 * - More text styles and formatting options
 * - Cursor control and screen management
 * - Terminal capability detection
 * - More comprehensive feature set
 *
 */
