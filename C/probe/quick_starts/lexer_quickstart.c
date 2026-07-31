/*
 * lexer.h - Lexical Analyzer Quickstart
 *
 * lexer.h provides a lightweight tokenizer/lexer for C source code. It breaks
 * input text into tokens like identifiers, keywords, numbers, strings, comments,
 * and punctuation. Tracks position (row, col) for each token for error reporting.
 * Requires LEXER_IMPLEMENTATIONS to use the functions.
 *
 * ## Quick Start
 *
 * ### Basic Usage
 *
 *   #define LEXER_IMPLEMENTATIONS
 *   #include \"lexer.h\"
 *   #include <stdio.h>
 *
 *   int main(void) {
 *       char code[] = \"int x = 42;\";
 *       Lx lexer = {0};
 *       lx_init(&lexer, code);
 *
 *       // Tokenize and print
 *       // ... requires additional implementation ...
 *
 *       return 0;
 *   }
 *
 * ## Token Types
 *
 * ### TokenKind Enumeration
 *
 * - TOKEN_IDENTIFIER  - Variable/function names (e.g., x, myVar)
 * - TOKEN_KEYWORD     - Language keywords (e.g., int, if, while)
 * - TOKEN_COMMENT     - Comments (line-based or block)
 * - TOKEN_PREPROC     - Preprocessor directives (#include, #define)
 * - TOKEN_STRING      - String literals (\"...\")
 * - TOKEN_NUMBER      - Numeric literals (42, 3.14, 0xFF)
 * - TOKEN_PUNCT       - Punctuation (+, -, *, /)
 * - TOKEN_BRACE       - Braces ({, }, [, ])
 * - TOKEN_NEWLINE     - Line breaks
 * - TOKEN_INCLUDE     - Include statements
 * - TOKEN_FUNCTION    - Function definitions
 * - TOKEN_SPACE       - Whitespace
 * - TOKEN_MAX         - Sentinel value
 *
 * ## Data Structures
 *
 * ### Token Struct
 *
 *   typedef struct {
 *       TokenKind kind;   // Type of token
 *       char *text;       // Token text
 *       size_t size;      // Token length
 *       size_t row, col;  // Position in source
 *   } Token;
 *
 * ### Tokens Array
 *
 *   typedef struct {
 *       Token *data;      // Array of tokens
 *       size_t length;    // Number of tokens
 *       size_t capacity;  // Allocated capacity
 *   } Tokens;
 *
 * ### Lexer State
 *
 *   typedef struct {
 *       char *src;        // Source code
 *       size_t length;    // Source length
 *       size_t pos;       // Current position
 *       size_t row, col;  // Current row/col
 *       Tokens tokens;    // Collected tokens
 *   } Lx;
 *
 * ## Core Functions
 *
 * ### Initialization
 * - void lx_init(Lx *l, char *src)
 *   Initialize lexer with source code
 *
 * ### Character Processing
 * - char lx_consume(Lx *l)
 *   Consume and return next character; updates row/col
 * - char lx_current(Lx *l)
 *   Get current character without consuming
 * - char lx_peak(Lx *l)
 *   Look ahead one character
 * - char lx_peak_prev(Lx *l)
 *   Look back one character
 *
 * ### Token Building
 * - size_t lx_consume_until_char(Lx *l, const char c, const char preced)
 *   Consume characters until target; optionally check preceding char
 * - size_t lx_consume_identifier(Lx *l)
 *   Consume valid identifier characters
 * - size_t lx_consume_literal_number(Lx *l)
 *   Consume numeric representation (decimal, hex, binary, octal)
 *
 * ### Character Classification
 * - bool isWhitespace(char c) - Space or newline
 * - bool isAlpha(char c) - Letter (a-z, A-Z)
 * - bool isNum(char c) - Digit (0-9)
 * - bool isAlphaNum(char c) - Letter or digit
 * - bool isHex(char c) - Hexadecimal digit (0-F)
 * - bool isOct(char c) - Octal digit (0-7)
 * - bool isNumericRepresentation(char c) - Digit or number marker (b, x, o, d)
 * - bool isValidIdChar(char c) - Valid in identifier (letter or underscore)
 *
 * ## Common Patterns
 *
 * ### Simple Tokenization
 *
 *   #define LEXER_IMPLEMENTATIONS
 *   #include "lexer.h"
 *   #include <stdio.h>
 *
 *   int main(void) {
 *       const char *code = "int main() { return 0; }";
 *       Lx lexer = {0};
 *       lx_init(&lexer, (char*)code);
 *
 *       // Manual token collection would go here
 *       // Library provides character utilities but token collection
 *       // is user's responsibility
 *
 *       return 0;
 *   }
 *
 * ### Custom Token Collector
 *
 *   void collect_tokens(Lx *lexer, TokenKind kind) {
 *       while (lexer->pos < lexer->length) {
 *           if (isAlpha(lx_current(lexer))) {
 *               Token token = {0};
 *               token.kind = TOKEN_IDENTIFIER;
 *               token.row = lexer->row;
 *               token.col = lexer->col;
 *               token.size = lx_consume_identifier(lexer);
 *               // Save token...
 *           } else {
 *               lx_consume(lexer);
 *           }
 *       }
 *   }
 *
 * ### Keyword Recognition
 *
 *   const char *keywords[] = {"int", "if", "while", "for", NULL};
 *
 *   bool is_keyword(const char *text, size_t len) {
 *       for (const char **kw = keywords; *kw; kw++) {
 *           if (strlen(*kw) == len && strncmp(text, *kw, len) == 0) {
 *               return true;
 *           }
 *       }
 *       return false;
 *   }
 *
 * ### Position Tracking for Error Messages
 *
 *   // When error occurs at token:
 *   printf("Error at line %zu, column %zu: %s\n",
 *          token.row + 1,   // +1 for 1-based display
 *          token.col + 1,
 *          error_message);
 *
 * ### Number Parsing
 *
 *   size_t num_len = lx_consume_literal_number(&lexer);
 *   // num_len tells you how many characters were consumed
 *   // Could be: 42, 3.14, 0xFF, 0b1010, 0o755
 *
 * ### String Literal Extraction
 *
 *   if (lx_current(&lexer) == '"') {
 *       lx_consume(&lexer);  // Skip opening quote
 *       size_t len = lx_consume_until_char(&lexer, '"', 92);
 *       // len includes characters until closing quote
 *   }
 *
 * ### Comment Handling
 *
 *   if (lx_current(&lexer) == '/' && lx_peak(&lexer) == '/') {
 *       // Single-line comment
 *       lx_consume_until_char(&lexer, 10, 0);
 *   }
 *
 *   if (lx_current(&lexer) == '/' && lx_peak(&lexer) == '*') {
 *       // Multi-line comment
 *       lx_consume(&lexer);  // /
 *       lx_consume(&lexer);  // *
 *       while (lexer->pos < lexer->length) {
 *           if (lx_current(&lexer) == '*' && lx_peak(&lexer) == '/') {
 *               lx_consume(&lexer);  // *
 *               lx_consume(&lexer);  // /
 *               break;
 *           }
 *           lx_consume(&lexer);
 *       }
 *   }
 *
 * ## Important Notes
 *
 * - Row and column start at 0 (zero-based)
 * - lx_consume() updates row/col for newlines
 * - isValidIdChar() only checks letter or underscore (not digits)
 * - lx_consume_until_char() handles escaped characters
 * - No automatic token collection; you build tokens manually
 * - Character utilities work on single characters only
 * - Token text is stored but managed by caller
 *
 * ## Limitations
 *
 * - No built-in token collection (manual management required)
 * - No semantic token classification (all similar tokens = same type)
 * - Character-by-character processing only (no buffering)
 * - Limited escape sequence handling
 * - No automatic keyword detection (implement yourself)
 * - Position tracking is manual responsibility
 * - No Unicode support
 *
 * ## When to Use
 *
 * Use lexer.h when:
 * - You need a lightweight tokenizer
 * - You want character-level control
 * - You're building a custom parser
 * - You need position tracking for errors
 *
 * Don't use lexer.h when:
 * - You need complete semantic analysis
 * - You need full C parser
 * - You prefer higher-level abstractions
 * - Performance is critical (roll your own optimized version)
 *
 * ## Configuration
 *
 * Define DEFINE_TOKEN before inclusion to provide custom token kinds:
 *
 *   #define DEFINE_TOKEN() \\
 *       TOKEN_VARIANT(CUSTOM1) \\
 *       TOKEN_VARIANT(CUSTOM2)
 *   #define LEXER_IMPLEMENTATIONS
 *   #include \"lexer.h\"
 *
 * This generates custom TokenKind enum and highlight_colors array.
 *
 */
