#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int ms;

  if(argc <= 1){
    fprintf(2, "Usage: sleep <ms>\n");
    exit(1);
  }

  ms = atoi(argv[1]);
  if(sleep(ms) < 0){
    fprintf(2, "sleep: failed to sleep\n");
    exit(1);
  }

  exit(0);
}
