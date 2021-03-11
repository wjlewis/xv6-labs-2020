// Print all primes from 2-35 using a concurrent prime sieve

#include "kernel/types.h"
#include "user/user.h"

void
filter(int in)
{
  // d is the divisor which we'll use to test the remaining nats we
  // receive
  int d, n, pid;
  int pipefd[2];

  if(read(in, &d, 4) == 0){
    exit(0);
  }

  fprintf(1, "prime: %d\n", d);

  if(pipe(pipefd) < 0){
    fprintf(2, "primes: unable to open pipe\n");
    exit(1);
  }

  pid = fork();
  if(pid == 0){
    close(in);
    close(pipefd[1]);

    filter(pipefd[0]);
  } else {
    close(pipefd[0]);

    while(read(in, &n, 4) > 0){
      if(n % d != 0){
        write(pipefd[1], &n, 4);
      }
    }

    close(pipefd[1]);
    wait((int *) 0);
    exit(0);
  }
}

int
main(int argc, char *argv[])
{
  int n, pid, pipefd[2];

  if(pipe(pipefd) < 0){
    fprintf(2, "primes: unable to open pipe\n");
    exit(1);
  }

  pid = fork();
  if(pid == 0){
    close(pipefd[1]);

    filter(pipefd[0]);
  } else {
    close(pipefd[0]);

    for(n = 2; n <= 35; n++){
      write(pipefd[1], &n, 4);
    }

    close(pipefd[1]);
    wait((int *) 0);
  }

  exit(0);
}
