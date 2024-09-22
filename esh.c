#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>

#define ERROR_INFO "An error has occurred\n"
char *PATH[100];

//
// You should use the following functions to print information
// Do not modify these functions
//
void print_error_info() {
    printf(ERROR_INFO);
    fflush(stdout);
}

void print_path_info(int index, char *path) {
    printf("%d\t %s\n", index, path);
    fflush(stdout);
}

void print_bg_info(int index, int pid, char *cmd) {
    printf("%d\t %s\t %s\n", index, pid, cmd);
    fflush(stdout);
}

void print_current_bg(int pid, char *cmd) {
    printf("Process %d %s: running in background\n", pid, cmd);
    fflush(stdout);
}
int builtin_cmd(char *token){
    if (strcmp(token, "exit") == 0) {
      exit(0);
    }else if (strcmp(token, "cd") == 0) {
      token = strtok(NULL, " ");
      if (token == NULL) {
          print_error_info();
      }else{
          if (strcmp(token, "~") == 0) {
              token = getenv("HOME");
          }
          if (chdir(token) == -1) {
              print_error_info();
          }else{
              printf("current directory is %s\n", token);
          }
      }
      return 0;
    }else if (strcmp(token, "paths") == 0) {
      token = strtok(NULL, " ");
      if (token == NULL) {
          int index = 0;
          while (PATH[index] != NULL){
              print_path_info(index + 1, PATH[index]);
              index++;
          }
      }else{
          memset(PATH, 0, sizeof(PATH));
          int i = 1;
          while (token != NULL) {
              PATH[i] = token;
              print_path_info(i, PATH[i]);
              token = strtok(NULL, " ");
              i++;
          }
      }
      return 0;
    }else if (strcmp(token, "bg") == 0) {
      token = strtok(NULL, " ");
      return 0;
    }
}

void execute_1_cmd(char *input){
  printf("%s", input);
  input[strlen(input) - 1] = '\0';
  char *token = strtok(input, " ");
  if(builtin_cmd(token) == 0){
    return;
  }else{
    char *cmd = token;
    //less than 100 arguments
    char *argv[100];
    int i = 0;
    int fd = -1;
    char *del = strstr(input, ">");
    printf("del: %s\n", del);
    while (token != NULL) {
      if (strcmp(token, ">") == 0) {
        token = strtok(NULL, " ");
        fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
          print_error_info();
          break;
        }
      }else{
        argv[i] = token;
        i++;
      }
      token = strtok(NULL, " ");
    }
    printf("i: %d\n", i);
    argv[i] = NULL;
    pid_t pid = fork();
    if (pid == -1) {
      print_error_info();
    }else if (pid == 0) {//child process
      if (fd != -1) {
        dup2(fd, STDOUT_FILENO);
        close(fd);
      }
      int j = 0;
      while (PATH[j] != NULL) {
        char *path = malloc(strlen(PATH[j]) + strlen(cmd) + 2);
        strcpy(path, PATH[j]);
        strcat(path, "/");
        strcat(path, cmd);
        execv(path, argv);
        j++;
      }
      print_error_info();
      exit(1);
    }else{
      int status;
      waitpid(pid, &status, 0);
      printf("child process %d exit with status %d\n", pid, status);
    }
  }
}

int main() {
    PATH[0] = "/bin";
    while(1) {
        printf("esh > ");
        fflush(stdout);

        // TODO
        // Show your intelligence
        size_t size = 100;
        char *input = malloc(size);
        int res = getline(&input, &size, stdin);
        if (res == -1) {
            print_error_info();
            return 1;
        }else{
            execute_1_cmd(input);
        }
        free(input);
    }
}
