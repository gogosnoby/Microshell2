#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#define BUFFER_SIZE 2048

void help()
{
    printf(KRED "\n===Microshell===\n\n" RESET);
    printf(KRED "Autor: Oskar Winiarski\n\n" RESET);
    printf(KRED "Obslugiwane wlasne implementacje komend: \n-help\n-exit\n-touch\n-cd\n-cp\n\n" RESET);
    printf(KRED "Obslugiwane sygnaly: \n-Ctrl+c" RESET);
    printf("\n\n");
}

void cd(char **arg, char *cwd)
{
    char *homedir = getenv("HOME");
    if(arg[1]==NULL || strcmp(arg[1],"~")==0)
    {
        chdir(homedir);
    }
    else if(strcmp(arg[1],"-")==0)
    {
        chdir(cwd);
    }
    else if(strcmp(arg[1],"..")==0)
    {
        char tempcwd[BUFFER_SIZE];
        getcwd(tempcwd, sizeof(tempcwd));
        char *temppwd1=" ", *temppwd2=" ", *temppwd3;
        temppwd3=strtok(tempcwd,"/");
        chdir(homedir);
        while(temppwd3!=NULL)
        {
            temppwd1=temppwd2;
            temppwd2=temppwd3;
            temppwd3=strtok(NULL,"/");
            chdir(temppwd1);
        }
    }
    else
    {
        if(chdir(arg[1])==0)
            chdir(arg[1]);
        else
            printf("No such directory.\n");
    }

}

void cp(char **arg)
{
    int plik1=open(arg[1],O_RDONLY);
    char buff[BUFFER_SIZE];
    int l;
    if(plik1==-1)
    {
        printf("Missing source file.\n");
        return;
    }
    if(open(arg[2],O_RDONLY)!=-1)
    {
        int plik2=open(arg[2],O_WRONLY);
        while((l=read(plik1,buff,sizeof buff))>0)
        {
            write(plik2,buff,l);
        }
    }
    else if(open(arg[2],O_RDONLY)==-1)
    {
        open(arg[2],O_WRONLY|O_CREAT|O_TRUNC,0666);
        int plik2=open(arg[2],O_WRONLY);
        while((l=read(plik1,buff,sizeof buff))>0)
        {
            write(plik2,buff,l);
        }

    }

}

void touch(char **arg)
{
    if(arg[1]==NULL)
    {
        printf("Missing filename.\n");
        return;
    }
    if(open(arg[1],O_RDONLY)!=-1)
    {
        printf("File already exists.\n");
        return;
    }
    if(arg[2]!=NULL)
    {
        printf("Too many arguments.\n");
        return;
    }
    else
    {
        open(arg[1],O_WRONLY|O_CREAT|O_TRUNC,0666);
    }
}

void argument(char *command, char **arg)
{
    char korektor[]=" \n\t";
    char *com=strtok(command,korektor);
    int i=0;
    while(com!=NULL)
    {
        arg[i]=com;
        i++;
        com=strtok(NULL,korektor);
    }
    arg[i]=0;
}

int main()
{
    signal(SIGINT,SIG_IGN);
    char uinput[BUFFER_SIZE];
    char cwd[BUFFER_SIZE];
    char curcd[2][BUFFER_SIZE];
    getcwd(curcd[1], sizeof(curcd[1]));
    char *arg[100];
    char *command;
    while(true)
    {
        char *login=getlogin();
        getcwd(cwd, sizeof(cwd));
        printf(KGRN "%s:" RESET, login);
        printf(KBLU "[%s]" RESET, cwd);
        printf("$ ");
        command=fgets(uinput, sizeof(uinput), stdin);
        uinput[strlen(uinput)-1]='\0';
        argument(command, arg);

        if (strlen(uinput)==0)
        {
            continue;
        }

        else if(strcmp(arg[0],"help")==0)
        {
            if(arg[1]==NULL)
                help();
            else
                printf("Wrong argument for command %s\n", arg[0]);
        }

        else if(strcmp(arg[0],"exit")==0)
        {
            if(arg[1]==NULL)
                exit(0);
            else
                printf("Wrong argument for command %s\n", arg[0]);
        }

        else if(strcmp(arg[0],"cd")==0)
        {
            strcpy(curcd[0],cwd);
            cd(arg,curcd[1]);
            strcpy(curcd[1],curcd[0]);
        }

        else if(strcmp(arg[0],"touch")==0)
        {
            touch(arg);
        }

        else if(strcmp(arg[0],"cp")==0)
        {
            cp(arg);
        }

        else
        {
            if (fork() == 0)
            {
                if(execvp(arg[0],arg)!=0)
                {
                    printf("Unknown command.\n");
                    exit(0);
                }
                else
                {
                    raise(SIGINT);
                }

            }
            else
            {
                int a=0;
                wait(&a);
                if(a!=0)
                {
                    printf("error code: %d\n",a);
                }
            }
        }

    }
    return 0;
}
