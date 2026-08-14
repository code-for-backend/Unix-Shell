#ifndef UTILITY_H
#define UTILITY_H

#include "tokenizer.h"
void build_argvec(char** argvec,Token tokens[MAX_TOKENS],int token_count,char** files);
void apply_redirections(char** files);

#endif