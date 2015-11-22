#include "types.h"
#include "stat.h"
#include "user.h"

#define TEST_NAME "crowd_monkey"
#include "318_test-tapish.h"

/* Authors Sam and Bailey */

/* A basic monkey test, performs a lot of operations at random based on the
 * output of a random number generator.  The random number generator (which
 * is kinda crap) was borrowed from usertests */

// Potential improvement - just include a global variable from /dev/random
// and use that to generate random numbers


void monkey(void *);
void thread(void *);

int numthr;

unsigned long randstate = 1;
int counter = 0;
unsigned int rand()
{
  if(++counter > 100) {
    TEST_FINI();
    exit();
  }
  randstate = randstate * 1664525 + 1013904223;
  return randstate;
}

void do_operation() {
  unsigned int rand40;
  void *stack;
  int tid = gettid();

  rand40 = rand()%40;

  if( rand40 < 10 ) {
    numthr--;
    texit();
  } else if( rand40 < 30 ){
    stack = malloc(2048);
    if(stack != 0) {
       numthr++;
       tspawn(stack, monkey, 0);
    }
  } else if(rand40 < 50) {
    TEST_EXIT_IF(tid != gettid(), "TID inaccurate");
  }
}

void monkey(void *garbage) {
  while(1) {
    do_operation();
  }
}

int main() {
  void * stack;
  int forks = 0;


  TEST_STRT(100);
  while(forks < 100) {
    forks++;
    printf(1, "Fork #%d\n", forks);
    if(fork() == 0) {
      numthr = 0;
      while(numthr < 1) {
        stack = malloc(2048);
        TEST_EXIT_IF(stack == 0, "malloc fail"); 
        TEST_EXIT_IF(tspawn(stack, monkey, 0) <= 0, "tspawn failed");
      }
    } else {
      wait();
    }
  }
  exit();
}
