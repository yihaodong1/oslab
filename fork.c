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
int main()
{
  size_t size = 100;
  char *input = malloc(size * sizeof(char));
  int res = getline(&input, &size, stdin);
  char *saveptr;
  char *token = strtok_r(input, ";", &saveptr);
  printf("token: %s\n", token);
  printf("saveptr: %s\n", saveptr);
  token = strtok_r(NULL, ";", &saveptr);
  printf("token: %s\n", token);
  printf("saveptr: %s\n", saveptr);
}
