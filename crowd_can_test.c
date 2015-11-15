#include "types.h"
#include "user.h"

#define TEST_NAME "crowd_can_test"
#include "318_test-tapish.h"


int
main(void)
{
  TEST_STRT(1);
  TEST_FINI("no panics on boot, shell, or fork/exec. Good job!");

  exit();
}
