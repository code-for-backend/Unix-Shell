
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

extern char** environ;

int main(int argc,char* argv[])
{
   

    char* argvec[100];

    char command[500];

    int i=0;

    char* delim=" \t";

    char* token;

    pid_t child_pid;


    while(1)
    {
        i=0;
        printf(":)");
        fgets(command,500,stdin);
        command[strcspn(command,"\n")]='\0';
        token=strtok(command,delim);

        if(token==NULL)
        continue;
        while(token!=NULL)
        {
          //  printf("%s",token);
            argvec[i++]=token;
            token=strtok(NULL,delim);

        }

      //printf("%s",argvec[0]);
        argvec[i]=NULL;


  //Now we have built the arg vec

//if user types exit

    if(!strcmp(argvec[0],"exit"))
    {
        exit(EXIT_SUCCESS);

    }


    switch(child_pid=fork())
    { 
        //child
        case 0:
        if(execvp(argvec[0],argvec)==-1)
        {
            perror("execvp");
            exit(1);
        }


      

        case -1: continue;


        default:
        waitpid(child_pid,NULL,0); //wait for child
    
       
        




    }

    

    }



    return 0;


}


/*
echo    hello world






*/