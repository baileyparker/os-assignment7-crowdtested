#include "types.h"
#include "stat.h"
#include "user.h"

#define TEST_NAME "crowd_texit_single"
#include "318_test-tapish.h"

/* Authors Sam and Bailey */

/* Test to make sure that texit() will cause a program to exit when it is the
 * only thread by ensuring that a parent in wait() will return */

int main() {
  TEST_STRT(1);

  if(fork() == 0) {
    sleep(10); //Make sure the parent made it to wait();
    texit();
  } else {
    wait();
    TEST_FINI();
  }
  exit();
}
