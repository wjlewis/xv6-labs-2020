#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  fprintf(1, "%d\n", uptime());
  exit(0);
}
