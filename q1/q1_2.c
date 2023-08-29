#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//child =factorial(4)
int factorial(int n){
    int i=1;
    factor=1;
    for(i;i<=n;i++){
        factor=factor*i;
    }
    printf("%d",factor);
}
//parent =fibonacci(16)
int fibonacci(int n){
    int i=2;
    int n1=0;
    int n2=1;
    int n3;
    printf("%d %d",n1,n2);
    for(i=2;i<n;i++){
        n3=n1+n2;
        n1=n2;
        n2=n3;
    }
    printf("%d",n3);
}
int main() {
  pid_t pid = vfork();

  if (pid < 0) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    factorial(4);
    exit(0);
  } 

  else {
    fibonacci(16)
    int status;
    wait(&status);

    if (WIFEXITED(status)) {
      NULL;
    }
    
  }

  return 0;
}