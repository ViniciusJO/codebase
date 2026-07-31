/*
 * flags.h - Command-Line Flags Parser Quickstart
 *
 * flags.h is a header-only, STB-style command-line argument parser supporting
 * boolean, integer, and string flags. It provides a clean macro-based API for
 * defining command-line flags and automatically parses argc/argv with support
 * for both long (--flag) and short (-f) forms, assignment (--flag=value) and
 * argument (--flag value) syntax.
 *
 * ## Quick Start
 *
 * ### Basic Usage
 *
 *   #define FLAGS_IMPLEMENTATIONS
 *   #include \"flags.h\"
 *   #include <stdio.h>
 *
 *   int main(int argc, char **argv) {
 *       bool *verbose = flag_bool(\"verbose\", \"Enable verbose output\",
 *                               .alt_name = \"v\");
 *       uint64_t *count = flag_uint64(\"count\", \"Number of items\",
 *                                   .alt_name = \"c\", .default_value = 10);
 *       char **name = flag_str(\"name\", \"Your name\",
 *                            .alt_name = \"n\");
 *
 *       flag_parse(argc, argv);
 *
 *       printf(\"Verbose: %s\
\", *verbose ? \"yes\" : \"no\");
 *       printf(\"Count: \" PRIu64 \"\
\", *count);
 *       printf(\"Name: %s\
\", *name ? *name : \"unknown\");
 *
 *       return 0;
 *   }
 *
 *   // Usage:
 *   // ./prog --verbose --count=20 --name=John
 *   // ./prog -v -c 20 -n John
 *
 * ## Flag Types
 *
 * ### FLAG_BOOL
 * - No value required
 * - Presence in command-line sets to true
 * - Usage: --flag or -f
 *
 *   bool *debug = flag_bool(\"debug\", \"Enable debug mode\", .alt_name = \"d\");
 *
 * ### FLAG_UINT64
 * - Requires a numeric value
 * - Supports decimal numbers only
 * - Usage: --flag=value or --flag value
 *
 *   uint64_t *threads = flag_uint64(\"threads\", \"Number of threads\",
 *                                 .default_value = 4);
 *
 * ### FLAG_STR
 * - Requires a string value
 * - Accepts any non-flag string
 * - Usage: --flag=value or --flag value
 *
 *   char **config = flag_str(\"config\", \"Config file path\",
 *                        .default_value = (uintptr_t)(void*)\"default.conf\");
 *
 * ## Macro API
 *
 * ### flag_bool(name, description, ...)
 * Create a boolean flag
 *
 *   bool *flag = flag_bool(\"name\", \"description\",
 *                      .alt_name = \"n\",
 *                      .mandatory = true);
 *
  * ### flag_uint64(name, description, ...)
 * Create a 64-bit unsigned integer flag
 *
 *   uint64_t *flag = flag_uint64(\"count\", \"Number of items\",
 *                             .alt_name = \"c\",
 *                             .default_value = 100,
 *                             .mandatory = true);
 *
 * ### flag_str(name, description, ...)
 * Create a string flag
 *
 *   char **flag = flag_str(\"output\", \"Output file\",
 *                      .alt_name = \"o\",
 *                      .default_value = (uintptr_t)(void*)\"output.txt\");
 *
 * ### flag_parse(argc, argv)
 * Parse command-line arguments
 *
 *   flag_parse(argc, argv);
 *
 * ### flag_print_help(stream)
 * Print formatted help message to file stream
 *
 *   flag_print_help(stderr);
 *
 * ## Common Patterns
 *
 * ### Simple Program with Flags
 *
 *   #define FLAGS_IMPLEMENTATIONS
 *   #include \"flags.h\"
 *   #include <stdio.h>
 *
 *   int main(int argc, char **argv) {
 *       bool *help = flag_bool(\"help\", \"Show this help\", .alt_name = \"h\");
 *       bool *verbose = flag_bool(\"verbose\", \"Verbose output\", .alt_name = \"v\");
 *       uint64_t *jobs = flag_uint64(\"jobs\", \"Parallel jobs\",
 *                                   .alt_name = \"j\", .default_value = 1);
 *       char **input = flag_str(\"input\", \"Input file\", .alt_name = \"i\");
 *       char **output = flag_str(\"output\", \"Output file\", .alt_name = \"o\");
 *
 *       flag_parse(argc, argv);
 *
 *       if (*help) {
 *           flag_print_help(stdout);
 *           return 0;
 *       }
 *
 *       if (!*input) {
 *           fprintf(stderr, \"Error: --input is required\
\");
 *           return 1;
 *       }
 *
 *       printf(\"Processing %s -> %s\
\", *input, *output ? *output : \"stdout\");
 *       printf(\"Using %\" PRIu64 \" jobs (verbose: %s)\
\",
 *              *jobs, *verbose ? \"yes\" : \"no\");
 *
 *       return 0;
 *   }
 *
 * ### Help System
 *
 *   if (*help) {
 *       printf(\"Usage: %s [OPTIONS]\
\", argv[0]);
 *       printf(\"\
Description:\
\");
 *       printf(\"  This program does amazing things.\
\");
 *       printf(\"\
Options:\
\");
 *       flag_print_help(stdout);
 *       return 0;
 *   }
 *
  * ### Mandatory Flags
 *
  *   char **api_key = flag_str(\"api-key\", \"API key (required)\",
 *                           .alt_name = \"k\",
 *                           .mandatory = true);
 *
 *   flag_parse(argc, argv);
 *
 *   // Will exit with error if --api-key not provided
 *
 * ## Command-Line Syntax
 *
 * ### Boolean Flags
 *
 *   ./prog --debug
 *   ./prog -d
 *
 * ### Integer Flags
 *
 *   ./prog --threads=4
 *   ./prog --threads 4
 *   ./prog -t=4
 *   ./prog -t 4
 *
 * ### String Flags
 *
 *   ./prog --name=John
 *   ./prog --name John
 *   ./prog -n=John
 *   ./prog -n John
 *
  * ### Combined Usage
 *
 *   ./prog -v -d --count=50 --name Alice
 *
 * ## Configuration
 *
 * ### FLAGS_IMPLEMENTATIONS
 *
 * Required in exactly one .c file to enable the implementation.
 *
 *   #define FLAGS_IMPLEMENTATIONS
 *   #include \"flags.h\"
 *
 * ### FLAGS_CAP
 *
 * Maximum number of flags (default: 32)
 *
 *   #define FLAGS_CAP 64
 *   #define FLAGS_IMPLEMENTATIONS
 *   #include \"flags.h\"
 *
 * ### FLAGS_ATTRIBUTES
 *
 * Apply attributes to all functions (e.g., for static linking)
 *
 *   #define FLAGS_ATTRIBUTES static
 *   #define FLAGS_IMPLEMENTATIONS
 *   #include \"flags.h\"
 *
  * ## Important Notes
 *
 * - All flag pointers point to flag value storage
 * - Default values are cast via (uintptr_t)
 * - For strings, cast pointer: (uintptr_t)(void*)\"string\"
 * - Flags must be defined before flag_parse() call
 * - Short names must be single characters
 * - Integer flags parse decimal only (no hex/octal)
 * - Missing mandatory flags cause program exit
 * - Invalid arguments cause program exit with error message
 * - Supports both = and space-separated assignment
 *
 * ## Limitations
 *
 * - Maximum FLAGS_CAP flags per program (default 32)
 * - Integer flags are always uint64_t (no signed)
 * - No positional arguments (flags only)
 * - No subcommands or complex nesting
 * - Integer parsing is decimal-only
 * - String values cannot contain spaces (no quoting)
 * - No environment variable support
 * - No config file support
 *
  * ## Error Handling
 *
 * The parser exits with error code 1 on:
 * - Unknown flag
 * - Invalid integer value
 * - Missing required argument
 * - Mandatory flag not provided
 * - Flag overflow (more than FLAGS_CAP)
 *
 * Check exit codes in scripts:
 *
 *   ./prog --valid-flag || echo \"Flag parsing failed\"
 *
 * ## Differences from Other Parsers
 *
 * - STB-style: single header, no external dependencies
 * - Macro-based: clean type-safe API
 * - Auto-help: type information available for documentation
 * - Simple: minimal overhead, easy to understand
 *
 */
