#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MSGSIZE 10
char *msg[] = {"Message #1", "Message #2", "Message #3"};

int main(void) {
  char inbuf[MSGSIZE];
  int fd[2];

  if (pipe(&fd[0]) < 0) {
    perror("pipe failed!\n");
    exit(1);
  } else { 
    printf("pipe file descriptor: fd[0] = %d, fd[1] = %d\n", fd[0], fd[1]);
    
    int pid = fork();
    if (pid < 0) {
      perror("fork failed!\n");
      exit(1);
    } else if (pid > 0) {
      // parent process (writer)
      close(fd[0]);
      for (int i = 0; i < 2; i++) {
        write(fd[1], msg[i], MSGSIZE);
        printf("Send: %s\n", msg[i]);
        sleep(2);
      }
      wait(NULL);
    } else {
      // child process (reader)
      close(fd[1]);
      for (int j = 0 ; j < 3 ; j++) {
        int ret = read(fd[0], inbuf, MSGSIZE);
        printf("Receive: %s (ret = %d)\n", inbuf, ret);
      }
    }
  }
  return 0;
}
