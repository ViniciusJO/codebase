/*
 * nob.h - No-Build System Quickstart
 *
 * nob.h is a header-only build system and scripting utility library. It provides
 * tools for running commands, managing files and directories, and automating build
 * processes. The library abstracts OS-specific details (Windows vs Unix) and makes
 * it easy to write portable build scripts in C. Perfect for build.c or nob.c files.
 *
 * ## Quick Start
 *
 * ### Minimal Build Script
 *
 *   // nob.c
 *   #define NOB_IMPLEMENTATION
 *   #include \"nob.h\"
 *
  *   int main(int argc, char **argv) {
 *       NOB_GO_REBUILD_URSELF(argc, argv);
 *
  *       Nob_Cmd cmd = {0};
 *       nob_cmd_append(&cmd, \"cc\", \"-Wall\", \"-o\", \"app\", \"main.c\");
 *       if (!nob_cmd_run(&cmd)) return 1;
 *
 *       return 0;
 *   }
 *
 *   // Compile and run:
 *   $ cc -o nob nob.c
 *   $ ./nob
 *
 * ## Core Features
 *
 * ### Command Building and Execution
 * - Nob_Cmd structure for building commands
 * - nob_cmd_append() - Add arguments to command
 * - nob_cmd_run() - Execute command and wait
 * - nob_cmd_run_sync() - Synchronous execution variant
 *
 * ### File Operations
 * - nob_read_entire_file() - Read file contents
 * - nob_write_entire_file() - Write file contents
 * - nob_file_exists() - Check if file exists
 * - nob_copy_file() - Copy file to new location
 * - nob_remove_file() - Delete file
 *
 * ### Directory Operations
 * - nob_mkdir() - Create directory
 * - nob_remove_dir() - Remove directory
 * - nob_list_dir() - List directory contents
 *
 * ### Logging
 * - nob_log() - Print informational message with prefix
 * - NOB_INFO - Info level
 * - NOB_WARN - Warning level
 * - NOB_ERROR - Error level
 *
 * ### Auto-Rebuild Feature
 * - NOB_GO_REBUILD_URSELF() - Macro that rebuilds nob.c if it changes
 *
  * ## Common Patterns
 *
 * ### Simple Compilation
 *
  *   Nob_Cmd cc = {0};
 *   nob_cmd_append(&cc, \"cc\");
 *   nob_cmd_append(&cc, \"-Wall\", \"-Wextra\");
 *   nob_cmd_append(&cc, \"-o\", \"myapp\");
 *   nob_cmd_append(&cc, \"main.c\", \"util.c\");
 *   if (!nob_cmd_run(&cc)) return 1;
 *
  * ### Multi-File Build
 *
 *   Nob_Cmd cc = {0};
 *   nob_cmd_append(&cc, \"cc\", \"-O2\");
 *
 *   const char *sources[] = {\"main.c\", \"foo.c\", \"bar.c\"};
 *   for (size_t i = 0; i < 3; i++) {
 *       nob_cmd_append(&cc, sources[i]);
 *   }
 *
 *   nob_cmd_append(&cc, \"-o\", \"app\");
 *   if (!nob_cmd_run(&cc)) return 1;
 *
 * ### Conditional Compilation
 *
 *   #ifdef _WIN32
 *   const char *output = \"app.exe\";
 *   #else
 *   const char *output = \"app\";
 *   #endif
 *
  *   Nob_Cmd cc = {0};
 *   nob_cmd_append(&cc, \"cc\", \"-o\", output, \"main.c\");
 *   if (!nob_cmd_run(&cc)) return 1;
 *
 * ### Clean Target
 *
 *   int clean(void) {
 *       nob_remove_file(\"app\");
 *       nob_remove_file(\"app.exe\");
 *       // Remove all object files
 *       return 0;
 *   }
 *
 * ### Directory Management
 *
 *   nob_mkdir(\"build\");
 *   nob_mkdir(\"build/obj\");
 *   nob_mkdir(\"build/bin\");
 *
 * ### File I/O
 *
 *   // Read file
 *   Nob_String_Builder content = {0};
 *   content = nob_read_entire_file(\"config.txt\");
 *   printf(\"%s\", (char*)content.items);
 *
 *   // Write file
 *   nob_write_entire_file(\"output.txt\", \"Hello, world!\", 13);
 *
 * ### Logging and Progress
 *
 *   nob_log(NOB_INFO, \"Building project...\");
 *   nob_log(NOB_WARN, \"Using deprecated API\");
 *   nob_log(NOB_ERROR, \"Build failed!\");
 *
 * ## Full Build Script Example
 *
 *   #define NOB_IMPLEMENTATION
 *   #include \"nob.h\"
 *
 *   int main(int argc, char **argv) {
 *       NOB_GO_REBUILD_URSELF(argc, argv);
 *
 *       nob_log(NOB_INFO, \"Building project...\");
 *
 *       nob_mkdir(\"build\");
 *
 *       Nob_Cmd cc = {0};
 *       nob_cmd_append(&cc, \"cc\");
 *       nob_cmd_append(&cc, \"-Wall\", \"-Wextra\", \"-std=c99\");
 *       nob_cmd_append(&cc, \"-o\", \"build/myapp\");
 *       nob_cmd_append(&cc, \"src/main.c\", \"src/util.c\");
 *
 *       if (!nob_cmd_run(&cc)) {
 *           nob_log(NOB_ERROR, \"Compilation failed!\");
 *           return 1;
 *       }
 *
 *       nob_log(NOB_INFO, \"Build successful: build/myapp\");
 *       return 0;
 *   }
 *
 * ## Configuration
 *
 * ### NOB_IMPLEMENTATION
 *
 * Must define before inclusion in exactly one .c file:
 *
 *   #define NOB_IMPLEMENTATION
 *   #include \"nob.h\"
 *
 * ### NOBDEF
 *
 * Customize function visibility:
 *
 *   #define NOBDEF static inline
 *   #define NOB_IMPLEMENTATION
 *   #include \"nob.h\"
 *
 * ### Memory and Assert
 *
 * Customize allocation and assertions:
 *
 *   #define NOB_REALLOC my_realloc
 *   #define NOB_FREE my_free
 *   #define NOB_ASSERT my_assert
 *   #define NOB_IMPLEMENTATION
 *   #include \"nob.h\"
 *
 * ### Strip Prefix
 *
 * Remove nob_ prefix from all names:
 *
 *   #define NOB_STRIP_PREFIX
 *   #define NOB_IMPLEMENTATION
 *   #include \"nob.h\"
 *
 *   // Now use: cmd_append(), cmd_run(), log(), etc.
 *
 * ## Important Notes
 *
 * - Commands are cross-platform (abstracted for Windows/Unix)
 * - nob_cmd_run() both compiles command and executes
 * - Auto-rebuild feature detects nob.c changes via modification time
 * - All file operations return success/failure status
 * - String builders are dynamically allocated
 * - No implicit error handling; check return values
 * - File operations are POSIX/Windows compatible
 *
 * ## Limitations
 *
 * - No parallel build support (execute sequentially)
 * - No incremental builds (no dependency tracking)
 * - File operations are basic (no advanced features)
 * - Limited error context (boolean returns only)
 * - No process environment customization
 * - No stdout/stderr capture
 *
 * ## When to Use
 *
  * Use nob.h for:
 * - Simple build scripts
 * - Portable C-based build systems
 * - Automating file operations
 * - Quick prototyping of build logic
 *
 * Use traditional build systems (make, CMake) when:
 * - You need complex dependency management
 * - You need parallel builds
 * - You have large projects with many targets
 * - You need IDE integration
 *
 * ## Comparison
 *
 * nob.h advantages:
 * - No external dependencies
 * - Works with just a C compiler
 * - Builds to single executable
 * - Easy to understand and modify
 *
 * nob.h disadvantages:
 * - No automatic dependency tracking
 * - No parallel execution
 * - Limited to capabilities exposed in header
 * - No standard ecosystem
 *
 */
