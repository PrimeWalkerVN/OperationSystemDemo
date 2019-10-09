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
char** strTokenizer(char *s, char *delim); //Tokenizer the string into seperate parts by the delim deliminater. Returns a 2d pointer, each pointers points to a char*, or a string
int execOneSeperateCmd(char *inputCmd); //Execute one command, e.g: ls -l, dir, fdisk -l,... Return values are similar to those of execvp() syscall.


int main(void){
  char s[] = "ls -l | less";
  executePipeCmd(s);

  // char s[] = "cat ahihi.c";
  // execOneSeperateCmd(s);

  // char **args = (char**)malloc(2);
  // *args = strdup("ls");
  // *(args + 1) = strdup("-l");
  // execvp(*args, args);
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
  strStandardize(inputCmd);

  char delim[] = "|";
  char **twoSeperateCmd = strTokenizer(inputCmd, delim); //Tokenize given string into seperate commands, with the delim is "|"

  int fds[2];   //File descriptors
  pipe(fds);    //Link these 2 fds, fds[1] is the write ends, fds[0] is the read ends. After this syscall, 2 ends (fds) are linked. We will use dup2 to output the result of a commands to fds[0] instead of STDOUT_FILENO and also another dup2 to use fds[1] as the input stream instead of STDIN_FILENO. As 2 ends are linked, the command whose result is writen to the write ends will served as the input for the command which use the read end as the input stream.

  pid_t pid;
  pid = fork(); //Create 2 process, the child will run the first command, its counterpart, the parent, will run the second command

  if(pid == 0){ //Deal with the child piocess
    // int d = open("data.txt", O_RDWR);
    // dup2(d, STDOUT_FILENO);  //Redirect the output stream to fds[1]

    // Execute the command
    // The command which is executed will output to the respective file description
    // // execOneSeperateCmd(*twoSeperateCmd);



    close(fds[0]);
    dup2(fds[1], STDOUT_FILENO);
    execOneSeperateCmd(*twoSeperateCmd);



    // int fd[2];
    // pipe(fd);

    // dup2(fd[1], STDOUT_FILENO);

    // char *args1[] = {"cat", "ahihi.c", NULL};
    // execvp(*args1, args1);

    // pid_t r = fork();
    // if(r == 0){
    //   dup2(fd[0], STDIN_FILENO);

    //   char *args2[] = {"less", NULL};
    //   execvp(*args2, args2);
    // }
    // else if(r > 0){

    // }
  }
  else if(pid > 0){ //Deal with the parent process
    wait(NULL);   //Wait for the child to complete as we need the output of that command in order to have it served as the input for the second one

    // int d = open("data.txt", O_RDWR);
    // dup2(d, STDIN_FILENO);   //Redirect the input stream to fds[0], which is the read ends

    // //Execute the command. This command will retrieve input from the respective file descriptor.
    // execOneSeperateCmd(*(twoSeperateCmd + 1));




    close(fds[1]);
    dup2(fds[0], STDIN_FILENO);
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


char** strTokenizer(char *s, char *delim){
  int nTokens = 1;   //Are there any string that has 0 token? Every string has at least 1 token
  for(int i = 0; i < strlen(s); i++){
    if(s[i] == *delim){
      nTokens++;
    }
  }

  char **ret = (char**)malloc(nTokens); //The result will be stored in a 2d arr of nTokens elements, each arr is a sting

  *ret = strdup(strtok(s, delim));  //The first token must be split like this,
  strStandardize(*ret); //if we pass "ls -l | more", *ret will be "ls -l ", we have to delete the last space. The same as *(ret + 1),...

  //From the seconde element, the strtok function should be called with the first param is NULL. For more info, https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-split
  for(int i = 1; i < nTokens; i++){
    *(ret + i) = strdup(strtok(NULL, delim));
    strStandardize(*(ret + i));
  }
  return ret;
}


int execOneSeperateCmd(char *inputCmd){
  strStandardize(inputCmd);

  //Split the inputCmd into seperate part (seperate args), using the strTokenizer() with the delim is " ".
  //ex, with inputCmd = "ls -l", we have a 2d pointer contain 2 1d pointer, each 1d pointer points to a string, each string alternatively is "ls" and -l
  char delim[] = " ";
  char **args = strTokenizer(inputCmd, delim);

  //Create process and execute the command, as explained in the executePipeCmd() function
  // pid_t pid;
  // pid = fork();
  // if (pid < 0) {
  //   return -1;
  // } 
  // else {
  //   if (pid == 0) {
  //     if(execvp(*args, args)) {
  //       return -1;
  //     }
  //   }
  //   else {
  //     wait(NULL); //Wait for the child to complete as we need the output of that command in order to have it served as the input for the second one
  //   }
  // }

  execvp(*args, args);
}