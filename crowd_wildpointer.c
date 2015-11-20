#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"
#include "traps.h"

#define TEST_NAME "crowd_wildpointer"
#include "318_test-tapish.h"

#define STKSIZE 1024

#define ARG ((void *)0x15410DE0U)

/*Authors: Jose and Emily*/

/*test that bad dereferences don't kill the whole process*/

static void
goodie(void *a) {
  while(1){yield(-1);}
}
static void
baddie(void *a) {
  int *addr = (int *) 0xdeadbea7;
  TEST_DIAG("%d", *addr);
  TEST_FAIL("Baddie wasn't killed");
  while(1);
}

int main() {
  int i;
  void *s;
  TEST_STRT(1);
  for (i = 0; i < 10; i++) {
    TEST_EXIT_IF((s = malloc(STKSIZE)) == 0, "oom");
    s += STKSIZE;
    if (i == 0) {
      TEST_EXIT_IF(tspawn(s, baddie, ARG) < 0, 
        "spawn fail at %d", i);
    } else {
      TEST_EXIT_IF(tspawn(s, goodie, ARG) < 0, "spawn fail at %d", i);
    }
    sleep(10);
  }
  TEST_DIAG("You can see the thread being killed well"\
  " before this test finishes.");
  TEST_FINI();
  exit();
}
