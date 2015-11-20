#include "types.h"
#include "user.h"
#include "param.h"

#define TEST_NAME "crowd_tspawn_texit_stress_test"
#include "318_test-tapish.h"

/* Authors: Bailey & Sam */

#define STKSIZE 1024
#define THREADS (NTHR - 4)
#define SPINS 8192
#define PASSES 16

// We have to use an array to verify that everything passes, because
// we can't TEST_FINI from the threads as this spams the terminal
// too quickly and is almost guaranteed to overwrite something else important
int finished[THREADS];

/* tspawns a bunch of threads that texit() at different times to stress test whatever
   datastructure you use for your threads. Great for shaking out linked datastructure bugs */

void stress_test(int pass);
void thread(void *a);


int
main(void)
{
  int i;

  TEST_STRT(1);

  TEST_DIAG("this test will timeout if it fails");

  for(i = 0; i < PASSES; i++) {
    stress_test(i);
  }

  TEST_FINI("stress test passed");

  exit();
}

void
stress_test(int pass)
{
  int i;
  void *stack;

  // Unset finished (for multiple runs)
  for(i = 0; i < THREADS; i++) {
    finished[i] = 0;
  }

  // Spawn threads
  for(i = 0; i < THREADS; i++) {
    TEST_EXIT_IF((stack = malloc(STKSIZE)) == 0, "malloc failed");
    stack += STKSIZE;
    TEST_EXIT_IF(tspawn(stack, thread, (void *) i) < 0, "tspawn %d failed", pass * THREADS + i);
  }

  // Unfortunately, this is the best way to check if everyone has finished
  // We can't use a counter with a lock because locking would mess up the timing
  // (this test relies on the set finished operation being constant time)
  for(i = 0; i < THREADS; i++) {
    while(!finished[i]) {
      yield(-1);
    }
  }
}

// We don't care at all about the value of this (so no locks), we just don't want gcc
// to optimize it out
long long temp = 1;

void thread(void *a) {
  int i, j;

  for(i = 0; i < (((int) a) / 16); i++) {
    // Do some useless somewhat time-consuming task
    // This should deterministically take the same amount of usertime
    for(j = 1; j < SPINS; j++) {
      temp *= j;
    }
  }

  finished[(int) a] = 1; // mark this thread as finished
  texit();
}
