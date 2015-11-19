#include "types.h"
#include "user.h"

// Authors: Bailey & Sam
#define TEST_NAME "crowd_basic_mkrun_desch"
#include "318_test-tapish.h"

#define ITERATIONS 4
#define STACKSZ 1024

void thread_func(void *a);


// This has to be an array, because the iteations can run
// concurrently so they could compete for a single has_mkran
int has_mkran[ITERATIONS];
int guard = 1;

/* Tests that a thread returns from desch() only after being mkrun()'d */
int
main(void)
{
  int i, child;
  char *stack;

  TEST_STRT(ITERATIONS);

  for(i = 0; i < ITERATIONS; i++) {
    stack = malloc(STACKSZ);
    TEST_EXIT_IF(stack == 0, "malloc failed");
    stack += STACKSZ; // Must give pointer to top of the stack

    child = tspawn(stack, thread_func, (void *) i);

    TEST_EXIT_IF(child < 0, "tspawn failed");

    sleep(64); // #YOLOMAGICNUMBER
    has_mkran[i] = 1; // inform child they have actually been awoken
    TEST_EXIT_IF(mkrun(child) < 0, "mkrun failed");

    // #YOLOLEAK because we have no uthr_join
    // and we can't free until texit()
    // free(stack);
  }

  texit();
}

void
thread_func(void *a)
{
  desch(&guard); // sleeps here because guard is always non-zero

  TEST_FAIL_IF(!has_mkran[(int) a], "came out of desch before mkrun");
  TEST_FINI("iteration %d", (int) a);
  texit();
}
