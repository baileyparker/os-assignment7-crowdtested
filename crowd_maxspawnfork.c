#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"
#include "traps.h"

#define TEST_NAME "crowd_maxspawnfork"
#include "318_test-tapish.h"

#define LOTS NTHR-4
#define STKSIZE 1024

#define BAILOUT (*(int *)0x1000000) = 3;

/*Authors: Jose and Emily*/

/*Check that fork fails if there are available process slots but no available
* thread slots.*/
static void
spawnee(void *a) {
  while(1){yield((int) a);}
}
int main() {
  int i, pid;
  void *s;
  int guard = 1;
  TEST_STRT(1);
  TEST_DIAG("WARNING: this test will fail if run with more than two existing processes");
  int parent_pid = getpid();
  pid = fork();
  if (pid == 0) {
     for(i = 0; i < LOTS; i++) {
        TEST_DIAG("spawned %d", i + 5);
        TEST_EXIT_IF((s = malloc(STKSIZE)) == 0, "oom");
        s += STKSIZE;
        TEST_EXIT_IF(tspawn(s, spawnee, (void *) parent_pid) < 0, 
          "spawn fail before full at i=%d", i);
     }
     // inherently racy - if mkrun happens before desch, we'll never wake up
     // not too likely, esp since every thread is yielding to the parent
     mkrun(parent_pid);
     while(1);
  }
  desch(&guard);
  TEST_EXIT_IF(fork() > 0, "Fork succeeded even though there were no "
    "more threads available.");
  TEST_FINI();
  kill(pid);
  wait();
  exit();
}
