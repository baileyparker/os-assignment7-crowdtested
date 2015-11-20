#include "types.h"
#include "stat.h"
#include "user.h"

/* Authors Sam and Bailey */

/* Test that yield(tid) actually has preference towards that tid, there should be
   close to twice as many prints from the main thread than from all the other threads
   in the ultimate output of this test.  That being said, with CPUS != 1 the output will
   be off, so it is recommended to run this with CPUS = 1 to test that the numbers turn out
   correctly and then with CPUS = 2 to make sure the kernel doesn't panic.  The CPUS = 2 run
   will have less useful output.
   
   Also stress tests gettid() as a side benefit */

#define THREADS 50
#define ITERATIONS 10000

int main() {
  int main_pid;
  int i;
  
  main_pid = gettid();
  
  // Could be changed to tspawn() instead of fork(), but would then
  // require a working tspawn() to test yield()
  for(i = 0; i < THREADS; i++) {
    if(fork() == 0) {
      for(i = 0; i < ITERATIONS; i++) {
        printf(1, "%d\n", gettid());
        yield(main_pid);
        //printf(1, "I'm back from the yield!\n");
      }
    }
  }
  for(i = 0; i < ITERATIONS; i++) {
    printf(1, "%d\n", gettid());
    yield(-1);
  }
  exit();
}
