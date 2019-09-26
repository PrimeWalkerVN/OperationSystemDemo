#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <malloc.h>

#define MAXLINE 80


int copycomand(char **arg,char **history, int nLine){

    for (int i = 0; i < nLine; ++i) {

        *(history+i)=strdup(*(arg+i));
    }
    *(history+nLine)=NULL;

};

void printhistory(char ** history,int nLine)
{
    for (int i = 0; i <= nLine; ++i) {
        if(*(history+i)!=NULL)
            printf("%s ",*(history+i));
    }
    printf("\n");
}



int main(void)
{
    char **args = (char**)malloc(MAXLINE/2 + 1);
    char **history = (char**)malloc(MAXLINE/2 +1);


    while (1) {
        int waitProcess = 1;
        printf("osh>");
        char *input = (char*) malloc(MAXLINE);

        fgets(input, MAXLINE, stdin);

        if(!input)
        {
            printf("input error\n");
            continue;
        }

        while(*input==' ' || *input=='\t')
            input++;

        int nLine = 0;


        while (strcmp(input, "") != 0) {
            *(args + nLine) = (char *) malloc(MAXLINE / 2 + 1);
            sscanf(input, "%[^ \n\t]", *(args + nLine));
            input += strlen(*(args + nLine));
            while(*input==' ' || *input=='\t' || *input=='\n')
                input++;
            nLine++;
        }

        if(strcmp(args[0],"exit")==0)
        {
            free(args);
            return 0;
        }

        if (strcmp(args[nLine-1], "&") ==0 )
        {
            waitProcess=0;
            free(args[nLine-1]);
            args[nLine-1]=NULL;
            nLine--;
            copycomand(args,history,nLine);
            nLine++;

        }else {
            // Tham so cuoi cung luon la NULL
            if(strcmp(args[0],"!!")==0)
            {
                if(history[0]==NULL)
                    printf("No commands in history.");
                else
                printhistory(history,nLine);

            } else {
                *(args + nLine) = (char *) malloc(80);
                *(args + nLine) = NULL;
                //
                copycomand(args, history, nLine);
            }
        }


        printf("\n");
        pid_t pid;
        pid = fork();
        if (pid < 0) {
            printf("error when create child process\n");
            break;
        } else
            if (pid == 0) {
                if(execvp(history[0], history))
                {
                    printf("Invalid command");
                    return 1;
                };
            } else
                if (waitProcess) {
                    wait(NULL);
                }
                printf("\n");

    }


    free(args);
    free(history);
    return 0;
}
