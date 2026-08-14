
#include "tokenizer.h"
#include "utility.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/*This will build the argvec to be passed to exe
Note that our shell expects the command to be of the form command args... [redirection]

Also we consider the last valid > op.txt as stdout
Similarly we consider the last valid < inp.txt as stdip 


*/

void build_argvec(char** argvec,Token tokens[MAX_TOKENS],int token_count,char** files)
{

bool seen_redirection = false;
int argc=0; 
int i = 0;
char* outfile=NULL;
char* infile=NULL;

while (i < token_count) {
    switch (tokens[i].type) {
        case TOKEN_WORD:
            if (seen_redirection) {
                fprintf(stderr, "syntax error: unexpected argument after redirection\n");
                exit(EXIT_FAILURE);
            }
            argvec[argc++] = tokens[i].text;
            i++;
            break;

        case TOKEN_REDIR_OUT:
            if (i + 1 >= token_count || tokens[i+1].type != TOKEN_WORD) {
                fprintf(stderr, "syntax error: expected filename after '>'\n");
                exit(EXIT_FAILURE);
            }
            outfile = tokens[i+1].text;
            seen_redirection = true;
            i += 2;
            break;

        case TOKEN_REDIR_IN:
            if (i + 1 >= token_count || tokens[i+1].type != TOKEN_WORD) {
                fprintf(stderr, "syntax error: expected filename after '<'\n");
                exit(EXIT_FAILURE);
            }
            infile = tokens[i+1].text;
            seen_redirection = true;
            i += 2;
            break;

        default:
            i++;
            break;
    }
}

files[0]=infile;
files[1]=outfile;
argvec[argc] = NULL;



}











/*
Opens infile/outfile (if set) and dup2's them onto stdin/stdout...
Must be called in the CHILD, after fork() and before execvp()
Exits the process on failure, since there's no sane way to recover
mid-redirection-setup in the child.
*/
void apply_redirections(char** files)
{
    if (files[0] != NULL) {  // infile
        int fd_in = open(files[0], O_RDONLY);
        if (fd_in == -1) {
            perror("open (infile)");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd_in, STDIN_FILENO) == -1) {
            perror("dup2 (infile)");
            exit(EXIT_FAILURE);
        }
        close(fd_in);
    }

    if (files[1] != NULL) {  // outfile
        int fd_out = open(files[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out == -1) {
            perror("open (outfile)");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd_out, STDOUT_FILENO) == -1) {
            perror("dup2 (outfile)");
            exit(EXIT_FAILURE);
        }
        close(fd_out);
    }
}