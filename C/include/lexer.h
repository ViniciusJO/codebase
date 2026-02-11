#ifndef __LEXER_H__
#define __LEXER_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef DEFINE_TOKEN
char *token_kind_name[] = {
  "TOKEN_IDENTIFIER",
  "TOKEN_KEYWORD",
  "TOKEN_COMMENT",
  "TOKEN_PREPROC",
  "TOKEN_STRING",
  "TOKEN_NUMBER",
  "TOKEN_PUNCT",
  "TOKEN_BRACE",
  "TOKEN_NEWLINE",
  "TOKEN_INCLUDE",
  "TOKEN_FUNCTION",
  "TOKEN_SPACE",
};

typedef enum {
  TOKEN_IDENTIFIER,
  TOKEN_KEYWORD,
  TOKEN_COMMENT,
  TOKEN_PREPROC,
  TOKEN_STRING,
  TOKEN_NUMBER,
  TOKEN_PUNCT,
  TOKEN_BRACE,
  TOKEN_NEWLINE,
  TOKEN_INCLUDE,
  TOKEN_FUNCTION,
  TOKEN_SPACE,
  TOKEN_MAX
} TokenKind;

#else

#define TOKEN_VARIANT(VARIANT) "TOKEN_"#VARIANT,
char *token_kind_name[] = {
  DEFINE_TOKEN()
  "TOKEN_MAX"
};
#undef TOKEN_VARIANT


#define TOKEN_VARIANT(VARIANT) TOKEN_##VARIANT,
typedef enum {
  DEFINE_TOKEN()
  TOKEN_MAX
} TokenKind;
#undef TOKEN_VARIANT

#define TOKEN_VARIANT(VARIANT) HIGHLIGHT_##VARIANT,
typedef enum {
  DEFINE_TOKEN()
  HIGHLIGHT_MAX
} HighlightColor;
#undef TOKEN_VARIANT

#endif

typedef struct {
  TokenKind kind;
  char *text;
  size_t size;
  size_t row, col;
} Token;

typedef struct {
  Token *data;
  size_t length;
  size_t capacity;
} Tokens;

typedef struct {
  char *src;
  size_t length;
  size_t pos;
  size_t row, col;
  Tokens tokens;
} Lx;

typedef uint32_t Color;

extern Color highlight_colors[TOKEN_MAX];

void lx_init(Lx *l, char *src);
char lx_consume(Lx *l);

size_t lx_consume_until_char(Lx *l, const char c, const char preced);
size_t lx_consume_identifier(Lx *l);
size_t lx_consume_literal_number(Lx *l);

char lx_peak(Lx *l);
char lx_peak_prev(Lx *l);
char lx_current(Lx *l);

bool isWhitespace(char c);
bool isAlpha(char c);
bool isNum(char c);
bool isNumericRepresentation(char c);
bool isAlphaNum(char c);
bool isHex(char c);
bool isOct(char c);
bool isValidIdChar(char c);

#endif//__LEXER_H__

/*#define LEXER_IMPLEMENTATIONS*/
#ifdef LEXER_IMPLEMENTATIONS
#ifndef __LEXER_IMPLEMENTATIONS
#define __LEXER_IMPLEMENTATIONS

#include <string.h>

Color highlight_colors[TOKEN_MAX] = {0};

void lx_init(Lx *l, char *src) {
  l->src = src;
  l->length = strlen(src);
  l->pos = l->col = l->col = 0;  
  memset(&l->tokens, 0, sizeof(Tokens));
} 

char lx_consume(Lx *l) {
  if(l->pos == l->length) return 0;
  if(lx_current(l) == '\n') {
    l->row++;
    l->col = 0;
  } else l->col++;
  return l->src[l->pos++];
}

size_t lx_consume_until_char(Lx *l, const char c, const char preced) {
  if(l->pos == l->length) return 0;
  int count = 0;
  while(c != lx_current(l) || (preced && preced == lx_peak_prev(l))) {
    lx_consume(l);
    count++;
  }
  lx_consume(l);
  /*lx_consume(l);*/
  return count+1;
}

size_t lx_consume_identifier(Lx *l) {
  if(l->pos == l->length) return 0;
  int count = 0;
  while(isValidIdChar(lx_current(l))) {
    lx_consume(l);
    count++;
  }
  return count;
}

size_t lx_consume_literal_number(Lx *l) {
  if(l->pos == l->length) return 0;
  int count = 0;
  while(isNumericRepresentation(lx_current(l))) {
    lx_consume(l);
    count++;
  }
  return count;
}

char lx_peak(Lx *l) {
  return l->src[l->pos+1];
}

char lx_peak_prev(Lx *l) {
  return l->src[l->pos-1];
}

char lx_current(Lx *l) {
  return l->src[l->pos];
}





// Char verify
bool isWhitespace(char c) {
  return (c == ' ') || (c == '\n');
}
bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isNum(char c) {
  return c >= '0' && c <= '9';
}

bool isNumericRepresentation(char c) {
  return isNum(c) || 'b' == c || 'x' == c || 'o' == c || 'd' == c;
}

bool isAlphaNum(char c) {
  return isNum(c) || isAlpha(c);
}

bool isHex(char c) {
  return isNum(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

bool isOct(char c) {
  return c >= '0' && c <= '8';
}

bool isValidIdChar(char c) {
  return isAlpha(c) || c == '_';
}

#endif //__LEXER_IMPLEMENTATIONS
#endif //LEXER_IMPLEMENTATIONS

