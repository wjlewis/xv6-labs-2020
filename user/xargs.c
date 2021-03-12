#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

uint
readline(char *dst[])
{
  static char buf[512];
  char **arg = dst;
  char *c = buf;
  char *start = c;

  while(read(0, c, 1) > 0){
    if(*c == ' ' || *c == '\n'){
      *arg++ = start;

      if(*c == '\n'){
        *c = '\0';
        break;
      }

      *c = '\0';
      start = c + 1;
    }

    c++;
  }

  return arg - dst;
}

int
main(int argc, char *argv[])
{
  uint argcount, pid;
  char *allargs[MAXARG];

  if(argc < 2){
    fprintf(2, "Usage: xargs <command> [<args> ...]\n");
  }

  // Copy over argv (starting with the arg _after_ "xargs")
  for(argcount = 1; argcount < argc; argcount++){
    allargs[argcount - 1] = argv[argcount];
  }

  while((argcount = readline(allargs + argc - 1)) > 0){
    allargs[argcount + argc - 1] = 0;

    pid = fork();
    if(pid == 0){
      exec(allargs[0], allargs);
    } else {
      wait((int *) 0);
    }
  }

  exit(0);
}
