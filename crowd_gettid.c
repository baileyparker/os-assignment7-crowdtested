#include "types.h"
#include "user.h"

/* Authors: Bailey & Sam */
#define TEST_NAME "crowd_gettid"
#include "318_test-tapish.h"


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
