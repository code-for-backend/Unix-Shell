
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

#include "tokenizer.h"

extern char** environ;


char* shell_builtin[3]={"pwd","cd","exit"};

char* output_file[100];
char* input_file[100];

//SIGCHLD handler
void sig_handler(int signum,siginfo_t* info,void* ucontext)
{
    if(signum==SIGCHLD)
    {
        
        while(waitpid(-1,NULL,0)>0);  //reap all dead children


    }

}



int main(int argc,char* argv[])
{
   
 Token tokens[MAX_TOKENS];

 
char* lineptr; //address of buff allocated for input command
size_t n; //size of buff allocated;

if(getline(&lineptr,&n,stdin)==-1)
{
    free(lineptr);
    printf("Failed to read command\n");
    exit(EXIT_FAILURE);
}

char* command=lineptr;

command[strcspn(command,"\n")]='\0';

printf("Command is %s\n",command);


printf("Length of cmd is %ld\n",strlen(command));


int token_count=tokenize(command,tokens);

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





}


/*
Bash redirection supports:-

command > file
command < file
command < input > output
command > a > b
command < a < b

for now



*/
