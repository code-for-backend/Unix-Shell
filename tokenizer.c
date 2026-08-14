

#include "tokenizer.h"


const char *token_type_name(TokenType t) {
    switch (t) {
        case TOKEN_WORD: return "WORD";
        case TOKEN_REDIR_IN: return "REDIR_IN";
        case TOKEN_REDIR_OUT: return "REDIR_OUT";
     
    }
    return "UNKNOWN";
}

/*
 * Tokenize a line into WORD, REDIR_IN ('<'), REDIR_OUT ('>'), and EOF.
 * Fills a fixed-size array of tokens (no dynamic allocation).
 * Returns the number of tokens written 
 */
int tokenize(const char *input, Token tokens[MAX_TOKENS]) {
    int count = 0;
    size_t i = 0;
    size_t len = strlen(input);

    while (i < len && count < MAX_TOKENS - 1) {
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }

        if (input[i] == '<') {
            tokens[count].type = TOKEN_REDIR_IN;
            count++;
            i++;
            continue;
        }

        if (input[i] == '>') {
            tokens[count].type = TOKEN_REDIR_OUT;
            count++;
            i++;
            continue;
        }

        // WORD: copy chars until whitespace, '<', or '>'
        int j = 0;
        while (i < len && !isspace((unsigned char)input[i]) &&
               input[i] != '<' && input[i] != '>' &&
               j < MAX_WORD_LEN - 1) {
            tokens[count].text[j] = input[i];
            j++;
            i++;
        }
        tokens[count].text[j] = '\0';
        tokens[count].type = TOKEN_WORD;
        count++;
    }

    

    return count;
}

