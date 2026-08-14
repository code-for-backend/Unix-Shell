#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 128
#define MAX_WORD_LEN 256

typedef enum {
    TOKEN_WORD,
    TOKEN_REDIR_IN,   // <
    TOKEN_REDIR_OUT,  // >

} TokenType;


typedef struct {
    TokenType type;
    char text[MAX_WORD_LEN]; // only used when type == TOKEN_WORD
} Token;


int tokenize(const char *input, Token tokens[MAX_TOKENS]);
const char *token_type_name(TokenType t);


#endif