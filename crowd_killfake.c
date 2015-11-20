#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"
#include "traps.h"

#define TEST_NAME "crowd_killfake"
#include "318_test-tapish.h"

/*Authors: Jose and Emily*/

#define ARG 999
#define LOTS NTHR-4
#define STKSIZE 1024

static void
spawnee(void *a) {
  while(1){yield(-1);}
}

int main() {
  void *s;
  int tid;
  TEST_STRT(1);
  TEST_EXIT_IF((s = malloc(STKSIZE)) == 0, "oom");
  s += STKSIZE;
  tid = tspawn(s, spawnee, (void *) ARG);
  TEST_EXIT_IF(tid < 0, "spawn fail");
  TEST_EXIT_IF(kill(tid) >= 0, "Kill a tid that isn't a pid succeeded.");
  TEST_FINI();
  exit();
}
