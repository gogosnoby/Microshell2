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

int main()
{
    char polecenie[100];
    char cwd[BUFFER_SIZE];
    while(true)
    {
        char *login=getlogin();
        getcwd(cwd, sizeof(cwd));

        printf(KGRN "%s:" RESET, login);
        printf(KBLU "[%s]" RESET, cwd);
        printf("$ ");

        fgets(polecenie, sizeof(polecenie), stdin);
        polecenie[strlen(polecenie)-1]=0;


        return 0;
    }
    return 0;
}
