#include "types.h"
#include "user.h"
#include "memlayout.h"

#define TEST_NAME "crowd_check_tspawn_bounds"
#include "318_test-tapish.h"

/* Authors: Bailey & Sam */

#define STKSIZE 1024

void dummy(void *a);

/* Checks that tspawn doesn't allow the stack or function pointer to be outside the proc's memory */
int
main(void)
{
  void *stack;

  TEST_STRT(2);

  TEST_FAIL_IF(tspawn((void *) KERNBASE, dummy, 0) > 0, "stack allowed outside proc's memory");
  TEST_FINI("stack bounds");

  TEST_EXIT_IF((stack = malloc(STKSIZE)) == 0, "malloc failed");
  stack += STKSIZE;

  TEST_FAIL_IF(tspawn((void *) stack, (void (*)(void *)) KERNBASE, 0) > 0, "f allowed outside proc's memory");
  TEST_FINI("f bounds");

  exit();
}

void dummy(void *a)
{
  texit();
}
