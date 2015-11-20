#include "types.h"
#include "user.h"
#include "memlayout.h"

#define TEST_NAME "crowd_check_desch_bounds"
#include "318_test-tapish.h"

/* Authors: Bailey & Sam */

#define STKSIZE 1024

#define S_CHILD_READY 1
#define S_PARENT_READY 2
#define S_CHILD_STILL_RUNNING 3

volatile int signal = 0;

void thread(void *a);

/* Checks that desch doesn't allow the guard to be outside the proc's memory */
int
main(void)
{
  void *stack;

  TEST_STRT(1);

  TEST_DIAG("This test will timeout if it fails");

  TEST_EXIT_IF((stack = malloc(STKSIZE)) == 0, "malloc failed");
  stack += STKSIZE;

  TEST_EXIT_IF(tspawn((void *) stack, thread, 0) < 0, "tspawn failed");

  // Wait for child to signal that it's ready
  while(signal != S_CHILD_READY);

  signal = S_PARENT_READY;

  // Wait (and hope) for child to awake (never get desch)
  while(signal != S_CHILD_STILL_RUNNING);

  TEST_FINI("desch guard bounds");

  exit();
}

void thread(void *a)
{
  signal = S_CHILD_READY;

  // Wait for parent to signal that it's ready
  while(signal != S_PARENT_READY);

  desch((void *) KERNBASE);

  // No one will mkrun() us so if we hit here, the desch failed as expected
  signal = S_CHILD_STILL_RUNNING;

  texit();
}
