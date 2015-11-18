#include "types.h"
#include "stat.h"
#include "user.h"

#define TEST_NAME "318_parentb4child"
#include "318_test-tapish.h"

/*Authors: Emily and Jose*/

/*Test exiting the parent before the child.  Both should still get cleaned
up -- the parent cleaned by sh, the child by initproc since it is abandoned.
* Shakes out sleeping channel bugs.  Recommended to ^P after to see process
and thread statuses.*/

int main() {
 TEST_STRT(2);
 int f = fork();
 if (f < 0) {
   TEST_EXIT("Fork failed");
 } else if (f == 0) {
   sleep(20);
   TEST_FINI("child"); /* success 1 */
 } else {
   TEST_FINI("parent"); /* success 2 */
 }
 exit();
}
