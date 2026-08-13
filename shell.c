
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char** environ;

int main(int argc,char* argv[])
{
   

    char* argvec[100];

    char command[500];

    int i=0;

    char* delim=" \t";

    char* token;

    pid_t child_pid;

    int* status;

    while(1)
    {
        printf(":)");
        fgets(command,500,stdin);
        token=strtok(command,delim);

        if(token==NULL)
        continue;
        while(token!=NULL)
        {
            argvec[i++]=token;
            token=strtok(NULL,delim);

        }

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
            printf("Couldnt execute the command (execvp failure)\n");
            exit(1);
        }


      

        case -1: continue;


        default:
        waitpid(child_pid,&status,0); //wait for child
        i=0;

        continue;
        




    }

    

    }



    return 0;


}


/*
echo    hello world






*/