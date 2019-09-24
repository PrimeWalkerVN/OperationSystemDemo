#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <malloc.h>

#define MAXLINE 80
int main(void)
{
    char **args = (char**)malloc(MAXLINE/2 + 1);

    int shouldRun = 1;

    while (shouldRun) {
        printf("osh>");
        char *input = (char *) malloc(MAXLINE);
        fgets(input, MAXLINE, stdin);
        //sscanf(stdin,%[^ \n],input)
        int nLine = 0;
        while (strcmp(input, "") != 0) {
            *(args + nLine) = (char *) malloc(MAXLINE / 2 + 1);
            sscanf(input, "%[^ \n]", *(args + nLine));
            input += strlen(*(args + nLine));
            input++;
            nLine++;
        }

        //GIAI QUYET NHIEU KHOANG TRAG`
        //...

        *(args+nLine)=(char*)malloc(80);
        *(args+nLine)=NULL;

        execvp(args[0], args);

        int wait = 1;

        pid_t pid = 0;

        pid = fork();
        if (pid < 0) {
            printf("error when create child process\n");
        } else {
            if (pid == 0) {
                execvp(args[0], args);
            } else
                printf(">0\n");
        }
        break;


    }
    return 0;
}