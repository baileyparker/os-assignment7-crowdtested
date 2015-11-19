#include "types.h"
#include "stat.h"
#include "user.h"

#define TEST_NAME "crowd_yieldother"
#include "318_test-tapish.h"

/*Authors: Emily and Jose*/

/*Test ping-ponging yield between parent and child. Recommended to run
with both 1 and 2 cpus, and maybe add a print statement when you 
switch to the other process in yield.  1 CPU should print all 2000 times,
where 2 CPUS, we found printed about 220 times, because the process was
already on core on the other core (We didn't have anything other than sh and
initproc running at this time.)*/

int main() {
 TEST_STRT(2);
 int i;
 int parent_tid = gettid();
 int f = fork();
 if (f < 0) {
   TEST_EXIT("Fork failed");
 } else if (f == 0) {
   for (i = 0; i < 1000; i++) {
     yield(parent_tid);
   }
   TEST_FINI("child"); /* success 1 */
 } else {
   for (i = 0; i < 1000; i++) {
     yield(f);
   }
   TEST_FINI("parent"); /* success 2 */
 }
 exit();
}
