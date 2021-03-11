#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char *message = "!";
  int pid;
  int to_child[2];
  int to_parent[2];

  if(pipe(to_child) < 0 || pipe(to_parent) < 0){
    fprintf(2, "pingpong: unable to open pipes\n");
  }

  pid = fork();
  if(pid == 0){
    // Child
    pid = getpid();
    read(to_child[0], (void *) 0, 1);
    printf("%d: received ping\n", pid);
    write(to_parent[1], message, 1);
    exit(0);
  } else{
    // Parent
    pid = getpid();
    write(to_child[1], message, 1);
    read(to_parent[0], (void *) 0, 1);
    printf("%d: received pong\n", pid);
    exit(0);
  }
}
