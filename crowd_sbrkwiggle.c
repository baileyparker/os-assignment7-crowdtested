#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"
#include "traps.h"

#define TEST_NAME "crowd_sbrkwiggle"
#include "318_test-tapish.h"

#define LOTS 20
#define STKSIZE 1024
#define WIGGLES 20
#define ARG 999

#define BAILOUT (*(int *)0x1000000) = 3;

/*Authors: Jose and Emily*/

/*THREADS V KERNEL - wiggle to the death*/
/*Tries to shake out sz protection bugs by interleaving sbrk growing and
* shrinking between many threads.  Also (racily) tests TLB buffer clearing with
* multiple CPUs since we do memory accesses in the addresses that we
* grow from, and these grow/shrink proceesses are interleaved.*/
static void
spawnee(void *a) {
  int i;
  int * addr;
  for (i = 0; i < WIGGLES; i++) {
    addr = (int *) sbrk(2*KSTACKSIZE);
    TEST_EXIT_IF(addr < 0, "sbrk failed");
    *addr = i;
    addr = (int *) sbrk(-KSTACKSIZE);
    TEST_EXIT_IF(addr < 0, "sbrk failed");
    addr = (int *) sbrk(2*KSTACKSIZE);
    TEST_EXIT_IF(addr < 0, "sbrk failed");
    *addr = i;
  }
  yield(*addr - WIGGLES);
  texit();
}
int main() {
  int i;
  void *s;
  TEST_STRT(1);
  for(i = 0; i < LOTS; i++) {
     TEST_DIAG("spawned %d", i + 5);
     TEST_EXIT_IF((s = malloc(STKSIZE)) == 0, "oom");
     s += STKSIZE;
     TEST_EXIT_IF(tspawn(s, spawnee, (void *) ARG) < 0, 
       "spawn fail before full at i=%d", i);
  }
  TEST_FINI();
  exit();
}
