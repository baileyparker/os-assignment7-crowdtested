#include "types.h"
#include "stat.h"
#include "user.h"

#define TEST_NAME "crowd_texit_late"
#include "318_test-tapish.h"

/* Authors Sam and Bailey */

/* Test to make sure that texit() will cause a program to exit when it is the
 * only thread and it will leave the state such that if the parent calls wait
 * it will behave properly */

int main() {
  int pid;
  TEST_STRT(1);

  if((pid = fork()) == 0) {
    texit();
  } else {
    yield(pid); // Make sure the child texit()'d before we hit wait
    yield(pid); // Twice for good measure :)
    wait();
    TEST_FINI();
  }
  exit();
}
