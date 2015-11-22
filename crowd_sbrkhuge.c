#include "types.h"
#include "stat.h"
#include "user.h"

#define TEST_NAME "crowd_sbrkhuge"
#include "318_test-tapish.h"

/*Authors: Emily and Jose*/

/*Sbrk nearly the whole of memory - run this after you run all your other 
* tests to see if it works and catch memory leaks.*/

int main() {
 TEST_STRT(1);
 TEST_DIAG("Run this right on startup and after all tests to reasonably "
  "demonstrate you don't have significant memory leaks.");
 TEST_DIAG("If this fails for you right on startup, your kernel might be "
  "bigger than ours. Reduce the sbrk number.");
 TEST_EXIT_IF(sbrk(56600*4096) < 0, "sbrk failed");
 TEST_FINI(); 
 exit();
}
