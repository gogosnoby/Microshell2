#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

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
    printf(KRED "Obslugiwane komendy: \n-help\n-exit" RESET);
    printf("\n\n");
}

void argument(char *command, char **arg)
{
    char korektor[]=" \n";
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
    char uinput[BUFFER_SIZE];
    char cwd[BUFFER_SIZE];
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

        if(strcmp(arg[0],"help")==0)
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


    }
    return 0;
}