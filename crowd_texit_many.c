#include "types.h"
#include "stat.h"
#include "user.h"

#define TEST_NAME "crowd_texit_many"
#include "318_test-tapish.h"

/* Authors Sam and Bailey */

/* Test to confirm that a thread can texit() without killing the process itself
 * This test is perhaps a bit racy. */

void thread(void *garbage) { 
  texit();
}

int main() {
  int tid;
  void * stack;

  TEST_STRT(1);

  stack = malloc(1024); // Real sized stack for test realism
  TEST_EXIT_IF((tid = tspawn(stack, thread, 0)) <= 0, "tspawn failed");
  yield(tid); // yield twice for good measure :)
  yield(tid);
  TEST_FINI();
  exit();
}
