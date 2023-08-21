#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    printf("Child is having ID %d\n", getpid());
    printf("My Parent ID is %d\n", getppid());
    exit(0);
  } else {
    printf("Parent (P) is having ID %d\n", getpid());
    printf("ID of P's Child is %d\n", pid);

    int status;
    wait(&status);

    if (WIFEXITED(status)) {
      NULL;
    }
  }

  return 0;
}
