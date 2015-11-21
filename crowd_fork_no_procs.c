#include "types.h"
#include "user.h"
#include "param.h"


#define TEST_NAME "crowd_fork_no_procs"
#include "318_test-tapish.h"

/* Authors: Bailey & Sam */

/* Ensures fork fails gracefully if there are no procs left */ 

#define FORKS (NPROC - 3)

void recursive_fork(int depth);
int test_fork(void);


int
main(void)
{
  TEST_STRT(FORKS);

  if(test_fork() == 0) {
    recursive_fork(1);
    exit();
  } else {
    wait();
  }

  TEST_FINI("fork with no procs");
  exit();
}

void
recursive_fork(int depth)
{
  int pid;

  TEST_FINI("fork %d", depth);

  if(depth == FORKS) {
    TEST_FAIL_IF(fork() >= 0, "fork %d succeeded, but shouldn't have", depth);
    exit();
  } else if(depth < FORKS) {
    depth++;
    TEST_FAIL_IF((pid = fork()) < 0, "fork %d failed", depth);

    if(pid == 0) {
      recursive_fork(depth);
      exit();
    } else {
      wait();
      exit();
    }
  }
}

int
test_fork(void)
{
  int pid;
  pid = fork();
  TEST_EXIT_IF(pid < 0, "fork failed");

  return pid;
}
