#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"
#include "traps.h"

#define TEST_NAME "crowd_yieldself"
#include "318_test-tapish.h"

#define LOTS 20
#define SLEEP_EVERY 4
#define STKSIZE 1024

#define STRINGIFY(x) # x
#define XTRINGIFY(x) STRINGIFY(x)

#define BAILOUT (*(int *)0x1000000) = 3;
#define ARG ((void *)0x15410DE0U)

/*Authors: Jose and Emily*/

/*everyone yield to themselves!*/

static void
spawnee(void *a) {
  TEST_EXIT_IF(a != ARG, "arg");
  while(1){yield(gettid());} //everyone yield to main thread
}
int main() {
  int i;
  void *s;
  TEST_STRT(1);
  for(i = 0; i < LOTS; i++) {
    TEST_EXIT_IF((s = malloc(STKSIZE)) == 0, "oom");
    s += STKSIZE;
    TEST_EXIT_IF(tspawn(s, spawnee, ARG) < 0, 
      "spawn fail before full at i=%d", i);
  }
  TEST_FINI();
  exit();
}
