#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <malloc.h>
#include<fcntl.h> 

#define MAX_SIZE_LENGTH 80

int isPipeCmd(char *inputCmd);  //Check whether an input cmd is a pipe cmd.
int executePipeCmd(char *inputCmd); //Final module, execute the passed pipe cmd. Return values are similar to those of execvp() syscall
char* strStandardize(char* s);  //Utility, standardize given string. Return a new string, the original string is modified.
char** strTokenizer(char *s); //Tokenizer the string into seperate parts. In this project, I do that with the default delim "|". Returns a 2d pointer, each pointers points to a char*, or a string
int execOneSeperateCmd(char *inputCmd); //Execute one command, e.g: ls -l, dir, fdisk -l,... Return values are similar to those of execvp() syscall.


int main(void){
  char s[] = "cat ahihi.c | less";
  executePipeCmd(s);
}


int isPipeCmd(char *inputCmd){
  for(int i = 0;i<strlen(inputCmd); i++){
    if(*(inputCmd+i) == '|'){ //Just check whether a '|' character exists in the given string
      return 1;
    }
  }
  return 0;
}


int executePipeCmd(char *inputCmd){
  char **twoSeperateCmd = strTokenizer(inputCmd); //Tokenize given string into seperate commands, with the delim is "|"

  int fds[2];   //File descriptors
  pipe(fds);    //Link these 2 fds, fds[1] is the write ends, fds[0] is the read ends. After this syscall, 2 ends (fds) are linked. We will use dup2 to output the result of a commands to fds[0] instead of STDOUT_FILENO and also another dup2 to use fds[1] as the input stream instead of STDIN_FILENO. As 2 ends are linked, the command whose result is writen to the write ends will served as the input for the command which use the read end as the input stream.

  pid_t pid;
  pid = fork(); //Create 2 process, the child will run the first command, its counterpart, the parent, will run the second command

  if(pid == 0){ //Deal with the child piocess
    dup2(fds[1], STDOUT_FILENO);  //Redirect the output stream to fds[1]

    //Execute the command
    //The command which is executed will output to the respective file description
    execOneSeperateCmd(*twoSeperateCmd);
  }
  else if(pid > 0){ //Deal with the parent process
    wait(NULL);   //Wait for the child to complete as we need the output of that command in order to have it served as the input for the second one

    dup2(fds[0], STDIN_FILENO);   //Redirect the input stream to fds[0], which is the read ends

    //Execute the command. This command will retrieve input from the respective file descriptor.
    execOneSeperateCmd(*(twoSeperateCmd + 1));
  }
  else{
    printf("%d\n", "Error creating new process");
    return;
  }
}

char* strStandardize(char* s){
  //Delete spaces before the first not-space character
  while(s[0]==' '){
    for(int j=0; j<strlen(s); j++){
      s[j] = s[j+1];
    }
  }

  //Delete spaces after the last not-space character
  for(int i = strlen(s) - 1; i >= 0; i--){
    if(s[i] != ' '){
      s[i + 1] = '\0';
      break;
    }
  }

  //Delete double, tripple, space
  for(int i=0; i<strlen(s); i++){
    if(s[i]==' ' && s[i+1]==' '){
      for(int j=i+1; j<strlen(s); j++){
        s[j] = s[j+1];
      }
      i--;
    }
  }

  return s;
}


char** strTokenizer(char *s){
  char **ret = (char**)malloc(2); //The result will be stored in a 2d arr, each arr is a sting
  char delim[] = "|";   //For this project only, I specify the delim with the character '|'
  *ret = strdup(strtok(s, delim));
  *(ret + 1) = strdup(strtok(NULL, delim));
  strStandardize(*ret); //if we pass "ls -l | more", *ret will be "ls -l ", we have to delete the last space. The same as *(ret + 1)
  strStandardize(*(ret + 1));
  return ret;
}


int execOneSeperateCmd(char *inputCmd){
  int nLine = 0;
  char **args = (char**)malloc(MAX_SIZE_LENGTH / 2 + 1);  //Array of arguments of the command, including the command itself. Each pointers points to a char*


  //Read each args to the args pointer.
  while (strcmp(inputCmd, "") != 0) {
    *(args + nLine) = (char*) malloc(MAX_SIZE_LENGTH / 2 + 1);
    sscanf(inputCmd, "%[^ \n\t]", *(args + nLine));  //Read untils the pointer reach a \n or \t
    inputCmd += strlen(*(args + nLine));
    while(*inputCmd==' ' || *inputCmd=='\t' || *inputCmd=='\n')
        inputCmd++;   //Skip the pointer to 1 unit if it reachs one of the above character
    nLine++;  //Move to the next pointer
  }


  //Create process and execute the command, as explained in the executePipeCmd() function
  pid_t pid;
  pid = fork();
  if (pid < 0) {
    printf("Error when create child process\n");
  } 
  else {
    if (pid == 0) {
      if(execvp(*args, args)) {
        printf("Invalid command");
        return 1;
      }
    }
    else {
      wait(NULL); //Wait for the child to complete as we need the output of that command in order to have it served as the input for the second one
    }
  }
}