/*
 * AT.h - AT Command Parser Quickstart
 *
 * AT.h provides utilities for parsing AT commands used in serial communication
 * protocols (commonly with modems and microcontrollers). AT commands follow
 * a standard format: AT+COMMAND=param1,param2,param3. The library parses
 * these commands into structured data and can serialize them back to strings.
 *
 * ## Quick Start
 *
 * ### Basic Parsing
 *
 *   #define AT_IMPLEMENTATION
 *   #include "AT.h"
 *   #include <stdio.h>
 *
 *   int main(void) {
 *       // Parse an AT command
 *       AT_Command cmd = at_command_parse("AT+RESET=0,1");
 *
 *       printf("Command: %s
", cmd.command);      // Output: "RESET"
 *       printf("Param count: %zu
", cmd.param_count); // Output: 2
 *
 *       if (cmd.param_count > 0) {
 *           printf("Param 1: %s
", cmd.param[0]); // Output: "0"
 *           printf("Param 2: %s
", cmd.param[1]); // Output: "1"
 *       }
 *
 *       return 0;
 *   }
 *
 * ## Data Structure
 *
 * The AT_Command struct:
 *
 *   typedef struct {
 *       char *command;      // The command name (e.g., "RESET")
 *       char **param;       // Array of parameter strings
 *       size_t param_count; // Number of parameters
 *   } AT_Command;
 *
 * ## Core Operations
 *
 * ### Parsing
 * - AT_Command at_command_parse(char *str)
 *   Parse an AT command string. The string should follow format:
 *   AT+COMMAND or AT+COMMAND=param1,param2,...
 *
 * ### Serializing
 * - char *at_command_serialize(AT_Command at)
 *   Convert AT_Command back to string. Returns allocated string; must free.
 *
 * ### Pretty Printing
 * - char *at_command_pretty_string(AT_Command at)
 *   Generate formatted display string with indentation. Returns allocated string; must free.
 *
 * ### Helper
 * - char *tok_in_bounds(char *start, char *end)
 *   Extract token between two pointers. Returns allocated string; must free.
 *
 * ## AT Command Format
 *
 * ### Command without Parameters
 *   AT+RESET
 *   Parsed as: command=\"RESET\", param_count=0
 *
 * ### Command with Parameters
 *   AT+CONFIG=value1,value2,value3
 *   Parsed as: command=\"CONFIG\", param[0]=\"value1\", param[1]=\"value2\", etc.
 *
 * ### Parameter Constraints
 *   - Maximum 256 bytes total per command
 *   - Parameters separated by commas
 *   - No spaces in parameters (implementation specific)
 *   - Empty parameters are possible: AT+CMD=,value,
 *
 * ## Common Patterns
 *
 * ### Simple Command Handler
 *
 *   void handle_command(const char *input) {
 *       AT_Command cmd = at_command_parse((char*)input);
 *
 *       if (strcmp(cmd.command, \"RESET\") == 0) {
 *           printf(\"Resetting device\
\");
 *       } else if (strcmp(cmd.command, \"CONFIG\") == 0) {
 *           if (cmd.param_count > 0) {
 *               printf(\"Setting config to: %s\
\", cmd.param[0]);
 *           }
 *       }
 *
 *       // Don't forget to clean up
 *       for (size_t i = 0; i < cmd.param_count; i++) {
 *           free(cmd.param[i]);
 *       }
 *       free(cmd.param);
 *   }
 *
 * ### Command Builder
 *
 *   AT_Command build_reset_cmd(void) {
 *       AT_Command cmd = {0};
 *       cmd.command = \"RESET\";
 *       cmd.param_count = 0;
 *       return cmd;
 *   }
 *
 *   AT_Command build_config_cmd(const char *v1, const char *v2) {
 *       AT_Command cmd = {0};
 *       cmd.command = \"CONFIG\";
 *       cmd.param_count = 2;
 *       cmd.param = malloc(2 * sizeof(char*));
 *       cmd.param[0] = strdup(v1);
 *       cmd.param[1] = strdup(v2);
 *       return cmd;
 *   }
 *
 * ### Command Round-Trip
 *
 *   AT_Command cmd1 = at_command_parse(\"AT+TEST=a,b,c\");
 *   char *serialized = at_command_serialize(cmd1);
 *   // serialized is now \"AT+TEST=a,b,c\"
 *   AT_Command cmd2 = at_command_parse(serialized);
 *   // cmd2 is equivalent to cmd1
 *
 * ### Parameter Parsing
 *
 *   void process_parameters(AT_Command *cmd) {
 *       for (size_t i = 0; i < cmd->param_count; i++) {
 *           char *param = cmd->param[i];
 *           // Process each parameter
 *           if (strlen(param) > 0) {
 *               printf(\"[%zu] = %s\
\", i, param);
 *           } else {
 *               printf(\"[%zu] = <empty>\
\", i);
 *           }
 *       }
 *   }
 *
 * ## Memory Management
 *
 * ### Important
 *
 * - at_command_parse() allocates memory for parameters
 * - Each parameter is a separate malloc'd string
 * - at_command_serialize() returns malloc'd string
 * - at_command_pretty_string() returns malloc'd string
 * - You MUST free all allocated memory
 *
 * ### Cleanup Pattern
 *
 *   AT_Command cmd = at_command_parse(input);
 *
 *   // Use cmd...
 *
 *   // Cleanup
 *   for (size_t i = 0; i < cmd.param_count; i++) {
 *       free(cmd.param[i]);
 *   }
 *   free(cmd.param);
 *
 * ## Examples
 *
 * ### Device Configuration Sequence
 *
 *   AT_Command init = at_command_parse(\"AT+INIT\");
 *   printf(\"Initializing: %s\
\", at_command_pretty_string(init));
 *
 *   AT_Command config = at_command_parse(\"AT+BAUD=115200,8,1,N\");
 *   printf(\"Baud rate: %s\
\", config.param[0]);
 *
 * ### Generic Command Dispatcher
 *
 *   int execute_command(const char *input) {
 *       AT_Command cmd = at_command_parse((char*)input);
 *       int result = 0;
 *
 *       if (strcmp(cmd.command, \"HELP\") == 0) {
 *           result = cmd_help();
 *       } else if (strcmp(cmd.command, \"STATUS\") == 0) {
 *           result = cmd_status();
 *       } else if (strcmp(cmd.command, \"SEND\") == 0) {
 *           if (cmd.param_count > 0) {
 *               result = cmd_send(cmd.param[0]);
 *           }
 *       }
 *
 *       for (size_t i = 0; i < cmd.param_count; i++) {
 *           free(cmd.param[i]);
 *       }
 *       free(cmd.param);
 *       return result;
 *   }
 *
 * ## Important Notes
 *
 * - Command format is case-sensitive
 * - Parser expects \"AT+\" prefix (handled by string offset)
 * - Parameter count is zero-based indexing
 * - Empty parameters are allowed: AT+CMD=,value, is valid
 * - No validation of parameter values; you handle that
 * - Requires AT_IMPLEMENTATION to compile functions
 *
 * ## Limitations
 *
 * - Limited to 256 bytes total command length
 * - No nested parameters or complex structures
 * - No validation of command names or parameter format
 * - Simple comma-separated format only
 * - No escape sequences for special characters
 * - Each parameter is a simple string
 *
 */
