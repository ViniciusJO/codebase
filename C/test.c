/*
 *  TODO: Correct bracket classification
 *  TODO: Identify OPERATOR (+,-,*,/,|,||,&,&&,^,!,?,:,=,...)
 *  TODO: Identify types
 *  TODO: Identify keywords
 *
 *  TODO: Descendant parsing (tokenize?)
 *  ...
 * */

#include <assert.h>
#include <stdio.h>

#include "vector.h"
#include "colors.h"

#define DEFINE_TOKEN() \
  TOKEN_VARIANT(IDENTIFIER) \
  TOKEN_VARIANT(KEYWORD) \
  TOKEN_VARIANT(COMMENT) \
  TOKEN_VARIANT(PREPROC) \
  TOKEN_VARIANT(STRING) \
  TOKEN_VARIANT(NUMBER) \
  TOKEN_VARIANT(PUNCT) \
  TOKEN_VARIANT(BRACE) \
  TOKEN_VARIANT(NEWLINE) \
  TOKEN_VARIANT(INCLUDE) \
  TOKEN_VARIANT(FUNCTION) \
  TOKEN_VARIANT(SPACE) \
  TOKEN_VARIANT(NAME) \
  TOKEN_VARIANT(TAB) \
  TOKEN_VARIANT(EOF) \

#define LEXER_IMPLEMENTATIONS
#include "lexer.h"

const char* keywords[] = { "alignas", "alignof", "auto", "bool", "break", "case", "char", "const", "constexpr", "continue", "default", "do", "double", "else", "enum", "extern", "false", "float", "for", "goto", "if", "inline", "int", "long", "nullptr", "register", "restrict", "return", "short", "signed", "sizeof", "static", "static_assert", "struct", "switch", "thread_local", "true", "typedef", "typeof", "typeof_unqual", "union", "unsigned", "void", "volatile", "while", "_Alignas", "_Alignof", "_Atomic", "_BitInt", "_Bool", "_Complex", "_Decimal128", "_Decimal32", "_Decimal64", "_Generic", "_Imaginary", "_Noreturn", "_Static_assert", "_Thread_local" };

void print_token(Token *t) {
  printf("%.*s\t\t(%s)\n", (int)t->size, t->text, token_kind_name[t->kind]);
}

#define MIN(A,B) ((A) < (B)) ? (A) : (B)

Tokens tokenize(Lexer *l, char *src) {
  char c;

  while((c = lexer_consume(l))) {
    /*printf(COLOR_BLUE"%c\n"COLOR_RESET, c);*/
    Token t = {
      .text = l->src + l->pos - 1,
      .size = 1,
      0
    };

    if(c == ' ') t.kind = TOKEN_SPACE;
    else if(c == '\n') t.kind = TOKEN_NEWLINE;
    else if('\t' == c) t.kind = TOKEN_TAB;
    else if(isNum(c)) { 
      t.kind = TOKEN_NUMBER;
      t.size += lexer_consume_literal_number(l);
    }
    else if(c == '#') {
      t.kind = TOKEN_PREPROC;
      t.size += lexer_consume_identifier(l);
    } 
    else if('/' == c && '/' == lexer_current(l)) {
      t.size += lexer_consume_until_char(l, '\n', 0);
      t.kind = TOKEN_COMMENT;
    }
    else if(c == '(' || c == ')') t.kind = TOKEN_BRACE; 
    else if(c == '[' || c == ']') t.kind = TOKEN_BRACE; 
    else if(c == '{' || c == '}') t.kind = TOKEN_BRACE; 
    else if(c == '.' || c == ';' || c == ',') t.kind = TOKEN_PUNCT; 
    else if(c == '<') {
      t.kind = TOKEN_INCLUDE;
      t.size += lexer_consume_until_char(l, '>', 0);
    } 
    else if(c == '"') {
      t.kind = TOKEN_STRING;
      t.size += lexer_consume_until_char(l, '"', '\\');
    } 
    else if(c == '\n') {
      t.kind = TOKEN_NEWLINE;
    }
    else {
      t.size += lexer_consume_identifier(l);
      /*while(isValidIdChar(lexer_consume(l))) {*/
      /*  t.size++;*/
      /*}*/
      /*l->pos--;*/
      /*printf(COLOR_RED"\n%c[%d]\n"COLOR_RESET, l->src[l->pos], l->src[l->pos]);*/
      bool is_keywork = false;
      for(int i = 0; i < sizeof(keywords)/sizeof(char*); i++) {
        /*printf("%s {%ld} == %.*s {%d} ? %s, %s\n", keywords[i], strlen(keywords[i]), (int)t.size, t.text, (int)t.size, strlen(keywords[i]) == t.size ? "true" : "false", !strncmp(keywords[i], t.text, strlen(keywords[i])) ? "true" : "false");*/
        if(strlen(keywords[i]) == t.size && !strncmp(keywords[i], t.text, strlen(keywords[i]))) {
          is_keywork = true;
          break;
        }
      }
      if(is_keywork) t.kind = TOKEN_KEYWORD;
      else if(lexer_current(l) == '(') t.kind = TOKEN_FUNCTION;
      else t.kind = TOKEN_IDENTIFIER;
    }

    vector_append(l->tokens, t);
  }
  
  Token t = { .kind = TOKEN_EOF, 0 };
  vector_append(l->tokens, t);

  return l->tokens;
}

void render_source(char *src) {
  Lexer l = {0};
  lexer_init(&l, src);

  Tokens tokens = tokenize(&l, src);

  Token tk = (Token){ .kind = TOKEN_NAME, .size = 3, .text = "_a_" };
  vector_append(tokens, tk);
   
  /*vector_foreach(&tokens, i, {*/
  /*  print_token(tokens.data + i);*/
  /*});*/

  printf("\n\n");

  vector_foreach(&tokens, i, {
      Token t = tokens.data[i];
      /*printf("%s = %.*s\n", token_kind_name[t.kind]+6, (int)t.size, t.text);*/
      switch(t.kind) {
        case TOKEN_IDENTIFIER: printf("%.*s", (int)t.size, t.text); break;
				case TOKEN_KEYWORD: printf(COLOR_PURPLE"%.*s"COLOR_RESET, (int)t.size, t.text); break;
        case TOKEN_PREPROC: printf(COLORIZE_RGB("%.*s", 0xFF7777FF), (int)t.size, t.text); break;
        case TOKEN_COMMENT: printf(COLOR_RGB("%.*s", 0x555555FF, 0xFFFFFFFF), (int)t.size, t.text); break;
        case TOKEN_INCLUDE:
        case TOKEN_STRING: printf(COLORIZE_STRING("%.*s", COLOR_GREEN), (int)t.size, t.text); break;
				case TOKEN_NUMBER: printf(COLOR_RED"%.*s"COLOR_RESET, (int)t.size, t.text); break;
				case TOKEN_PUNCT: printf("%.*s", (int)t.size, t.text); break;
				case TOKEN_BRACE: printf("%.*s", (int)t.size, t.text); break;
        case TOKEN_NEWLINE: printf("\n"); break;
				case TOKEN_FUNCTION: printf(COLOR_RGB("%.*s", 0xFFAA00FF, 0), (int)t.size, t.text); break;
        case TOKEN_SPACE: printf(" "); break;
        case TOKEN_TAB: printf("\t"); break;
        case TOKEN_EOF:
        case TOKEN_NAME:
        case TOKEN_MAX: color_string("UNINPLEMENTED", UNHEXF_RGB(0xFF00FFFF)); break;
        default: assert(0 && "UNRECHEABLE");
      }
    
  });

  /*printf("%s\n", src);*/
}

typedef struct {
  char *data;
  size_t length;
  size_t capacity;
} string;

int main(void) {

  char *source =
    "#include <stdio.h>\n"
    "#include \"stdlib.h\"\n"
    "const char* str = \"\\\"name\\\"\";\n"
    "\n"
    "// THIS is a comment\n"
    "\n"
    "void a();\n"
    "int main(void) {\n"
    "\tint i = 0x258;\n"
    "\tint j = 0b1001;\n"
    "\tint k = 0v1001;\n"
    "\tputs(\"Hello World!\");\n"
    "\treturn 100;\n"
    "}\n";

  FILE *this = fopen("test.c", "r");

  if(NULL == this) {
    fprintf(stderr, "Cannot read file!\n");
    exit(1);
  }

  string str = {0};

  char s[4096];
  while(fgets(s, 4096, this)){
    for(size_t i = 0; i < strlen(s); i++) vector_append(str, s[i]);
    vector_append(str, '\n');
  }
  fclose(this);

  vector_append(str, 0);
  /*printf("%s\n", str.data);*/
  /*render_source(str.data);*/

  /*color_string(source, 180, 100, 0);*/
  /*return 0;*/
  /*highlight_colors[TOKEN_STRING] = 0x00FF88FF;*/

  render_source(source);

  return 0;
}
