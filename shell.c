
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



#define MAX_ARGS 100 
#define MAX_COMMAND_LEN 100
#define MAX_PATH 100 //pwd

extern char** environ;

pid_t child_pid; 

int status;


char* shell_builtin[3]={"pwd","cd","exit"};

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
   

    char* argvec[MAX_ARGS];

    char command[MAX_COMMAND_LEN];

    int i=0; //no of tokens

    char* delim=" \t";

    char* token;

    struct sigaction action;

    struct sigaction oldaction;

    action.sa_flags=SA_SIGINFO ;//will install the handler referred by sa_sigaction field
    action.sa_sigaction=sig_handler; //Handler for SIGCHLD when child terminates

   
    bool is_background_proc;

    char pwd_buff[MAX_PATH]; 

    char* output_file;

    


    while(1)
    {
        output_file=NULL;
        i=0;
        is_background_proc=false;
       if(getcwd(pwd_buff,MAX_PATH)==NULL)
       {
        perror("getcwd");
        exit(EXIT_FAILURE);

       }

        printf("%s>",pwd_buff);
        if(fgets(command,MAX_COMMAND_LEN,stdin)==NULL)
        {
            continue;
        }
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


 //Implement redirect standard output feature 
 //e.g ls > output.txt

 
 for(int j=0;j<i;j++)
 {
    bool flag=false;

    if(!strcmp(argvec[j],">"))
    {
        if(j==i-1)
        {
            // no file specified to redirect
            printf("Syntax error\n\n");
            flag=true;
            break;
        }

        else
        {
            output_file=argvec[j+1];
            argvec[j]=NULL; // ignore whatever comes after >

        }



    }
    if(flag)
    continue;
 }






//display current work directory

if(i==1&&!strcmp(argvec[0],"pwd"))
{
    if(getcwd(pwd_buff,MAX_PATH)==NULL) //err
    {
        perror("getcwd");
        continue;

    }

    printf("%s\n\n",pwd_buff);
    continue;
    


}

else if(i==1&&!strcmp(argvec[0],"exit"))
{
    printf("exit\n\n");
    exit(EXIT_SUCCESS);

}

//change directory
else if(i==2&&!strcmp(argvec[0],"cd"))
{
    if(chdir(argvec[1])==-1)
    {
        perror("chdir");
      
    }

    continue;
}



  //Now we have built the arg vec...Done parsing input command

  

/*
if its a background process regsiter the SIGCHLD handler
      if(!strcmp(argvec[i-1],"&"))
        {

            argvec[i-1]=NULL; //since its not part of argument for the command
            if(sigaction(SIGCHLD,&action,NULL)==-1)//failed to put process in background
            {
                perror("sigaction");
                exit(EXIT_FAILURE);

            } 

            is_background_proc=true;
        }


*/


   



    switch(child_pid=fork())
    { 
        //child
        case 0:
    
        //check if std output is to be redirected

        if(output_file!=NULL)
        {
            int fd=open(output_file,O_WRONLY|O_CREAT|O_TRUNC,0644);
            if(fd==-1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }

            dup2(fd,STDOUT_FILENO); //file desscriptor 1 will point to the output file


            close(fd);// no more useful


        }

        if(execvp(argvec[0],argvec)==-1)
        {
            perror("execvp");
            exit(1);
        }


      

        case -1: 
        perror("fork");
        continue;


        default:

        //printf("Child pid is %d\n",child_pid);

     
        waitpid(child_pid,NULL,0); //wait for child

        printf("\n"); //presentation
    
       
        




    }

    

    }



    return 0;


}


/*

ls > output.txt 


*/