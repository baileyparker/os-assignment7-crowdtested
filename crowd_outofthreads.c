#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"
#include "traps.h"

#define TEST_NAME "crowd_outofthreads"
#include "318_test-tapish.h"

#define LOTS NTHR-3
#define SLEEP_EVERY 4
#define STKSIZE 1024

#define STRINGIFY(x) # x
#define XTRINGIFY(x) STRINGIFY(x)

#define BAILOUT (*(int *)0x1000000) = 3;
#define ARG ((void *)0x15410DE0U)

/*Authors: Jose and Emily*/

/*This just allocated all available thread slots and checks that doing one
* more causes tspawn to return -1.*/
/*Meant to be run when you only have init and sh running. Check to make 
* that everyone gets cleaned up at the end with ^P. This will take a while
* as resources get shared between more and more threads.*/
static void
spawnee(void *a) {
  TEST_EXIT_IF(a != ARG, "arg");
  while(1){yield(-1);}
}
int main() {
  int i;
  void *s;
  TEST_STRT(1);
  TEST_DIAG("WARNING: this test will fail if run with more than two existing processes");
  for(i = 0; i <= LOTS; i++) {
    TEST_EXIT_IF((s = malloc(STKSIZE)) == 0, "oom");
    s += STKSIZE;
    if (i < LOTS) {
      TEST_EXIT_IF(tspawn(s, spawnee, ARG) < 0, 
        "spawn fail before full at i=%d", i);
    } else {
      TEST_EXIT_IF(tspawn(s, spawnee, ARG) > 0,"spawn success after full");
    }
  }
  TEST_FINI();
  exit();
}
