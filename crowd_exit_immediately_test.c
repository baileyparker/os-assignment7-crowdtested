#include "types.h"
#include "user.h"
#include "param.h"

#define TEST_NAME "crowd_tspawn_texit_stress_test"
#include "318_test-tapish.h"

/* Authors: Bailey & Sam */

#define STKSIZE 1024
#define THREADS (NTHR - 4)
#define YIELDS 16

int running[THREADS];

int test_fork(void);
void thread(void *a);


/* tspawns a bunch of threads that texit() at different times, immediately
   exit()s, and then checks that all the threads were cleaned up
   datastructure you use for your threads. Great for shaking out linked datastructure bugs */
int
main(void)
{
  int i;
  void *stack;

  TEST_STRT(1);

  if(test_fork() == 0) {
    for(i = 0; i < THREADS; i++) {
      TEST_EXIT_IF((stack = malloc(STKSIZE)) == 0, "malloc failed");
      stack += STKSIZE;
      TEST_EXIT_IF(tspawn(stack, thread, (void *) i) < 0, "tspawn %d failed", i);
    }

    // This should kill all running threads
    exit();
  } else {
    wait();
  }

  // Set all threads to not running
  for(i = 0; i < THREADS; i++) {
    running[THREADS] = 0;
  }

  // Yield a few times to give any stragglers a chance to run
  for(i = 0; i < YIELDS; i++) {
    yield(-1);
  }

  // Presumably by this time, any remaining threads have had a chace
  // to run and have spammed their running state to be true, so we'll check for that
  for(i = 0; i < THREADS; i++) {
    TEST_FAIL_IF(running[i], "test %d still running after exit()", i);
  }

  TEST_FINI("stress test passed");

  exit();
}

void thread(void *a) {
  for(;;) {
    // This thread will constantly set its running state to true
    // We can abuse this to check if a thread is still running
    running[(int) a] = 1;
  }

  // We should never reach here
  texit();
}

int test_fork(void) {
  int pid;
  pid = fork();
  TEST_EXIT_IF(pid < 0, "fork failed");

  return pid;
}
