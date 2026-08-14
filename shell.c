
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
 #include <sys/types.h>
 #include <sys/stat.h>
  #include <fcntl.h>
#include "utility.h"
#include "tokenizer.h"

extern char** environ;



char* shell_builtin[3]={"pwd","cd","exit"};



int main(int argc,char* argv[])
{
   
 Token tokens[MAX_TOKENS];
 char* lineptr;
 size_t n;
 char* command;
 char* argvec[MAX_TOKENS+1];
 int token_count;
 const char* tok_type;
 pid_t child_pid;

int last_exit_status=0; //bash style $?

 char* files[2]; //files[0] is stdin and file[1] is stdout provided by user in command
 //REPL
 while(1)
 {
    printf(":)");
   fflush(stdout); //we want : to appear immediately
 lineptr=NULL; //address of buff allocated for input command
 n=0; //size of buff allocated;


if(getline(&lineptr,&n,stdin)==-1)
{
    free(lineptr);
    printf("Failed to read command\n");
    exit(EXIT_FAILURE);
}

command=lineptr;

command[strcspn(command,"\n")]='\0';

//printf("Command is %s\n",command);


//printf("Length of cmd is %ld\n",strlen(command));


token_count=tokenize(command,tokens);

/*
Output the list of tokens. works correctly

 for (int i = 0; i < token_count; i++) {
            if (tokens[i].type == TOKEN_WORD) {
                printf("%s  %s\n", token_type_name(tokens[i].type), tokens[i].text);
            } else {
                printf("%s\n", token_type_name(tokens[i].type));
            }
        }

*/

build_argvec(argvec,tokens,token_count,files);

if(argvec[0]==NULL)
continue;


//execute the command
switch(child_pid=fork())
 {

    case 0: // child

    apply_redirections(files);

    if (execvp(argvec[0], argvec) == -1) {
        perror("execvp");
        exit(EXIT_FAILURE);
    }




case -1: 
        fprintf(stderr,"Failed to fork a child process\n");
        exit(EXIT_FAILURE);


default:
    {
        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            last_exit_status = WEXITSTATUS(status);
           // printf("[exit status: %d]\n", last_exit_status);
        } else if (WIFSIGNALED(status)) {
            last_exit_status = 128 + WTERMSIG(status); //bash style when terminated by signal
           // printf("[terminated by signal %d, exit status: %d]\n",
                //   WTERMSIG(status), last_exit_status);
        }
        printf("\n");
    }


 }


 }// while



return 0;


}

