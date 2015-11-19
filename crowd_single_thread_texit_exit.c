#include "types.h"
#include "user.h"

/* Authors: Bailey & Sam */
#define TEST_NAME "crowd_single_thread_texit_exit"
#include "318_test-tapish.h"


int test_fork(void);


/* Given a single thread, texit() and exit() should behave the same */
int
main(void)
{
  TEST_STRT(2);

  if(test_fork() == 0) {
    exit();
  } else {
    wait();
    TEST_FINI("exit");
  }

  if(test_fork() == 0) {
    texit();
  } else {
    wait();
    TEST_FINI("texit");
  }

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

