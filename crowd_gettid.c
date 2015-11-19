#include "types.h"
#include "user.h"

#define TEST_NAME "crowd_gettid"
#include "318_test-tapish.h"

/* Authors: Bailey & Sam */

/* Checks to confirm that gettid() isn't returning garbage by checking it against the
   built in getpid() and ensuring they are the same for the first thread in the process.
   Unfortunately we can't check threads created by tspawn() as we don't know what happens
   under to hood to determine the tid - you should modify the test to check that based
   on whatever your kernel does */

int test_fork(void);


int
main(void)
{
  int pid, tid;

  TEST_STRT(2);

  pid = getpid();
  tid = gettid();

  // For first thread, tid should always equal pid
  TEST_FAIL_IF(pid != tid, "pid (%d) != tid (%d)", pid, tid);

  if(test_fork() == 0) {
    pid = getpid();
    tid = gettid();

    TEST_FAIL_IF(pid != tid, "in fork - pid (%d) != tid (%d)", pid, tid);

    TEST_FINI("in fork - tid == pid");
    exit();
  } else {
    wait();
  }

  TEST_FINI("tid == pid!");
  exit();
}

int
test_fork(void)
{
  int pid;
  pid = fork();
  TEST_EXIT_IF(pid < 0, "fork failed");

  return pid;
}
