#include "types.h"
#include "user.h"
#include "fcntl.h"

#define TEST_NAME "crowd_file_rc_contention"
#include "318_test-tapish.h"

/* Authors: Bailey & Sam */

/* Confirms graceful failure if one thread reads a file
   while another closes it */

#define STACKSZ 1024
#define ROUNDS 512

void contender(void *a);
void make_file(void);

#define S_CONTENDER_READY 1
#define S_START 2

volatile int state = 0;

char *contents = "stuff";
char *path = "rccont1";
int file;


int
main(void)
{
  void *stack;
  char buf[1];

  TEST_STRT(1);

  make_file();

  TEST_EXIT_IF((file = open(path, O_RDONLY)) < 0, "failed to open for reading");

  TEST_EXIT_IF((stack = malloc(STACKSZ)) == 0, "malloc failed");
  stack += STACKSZ;

  TEST_EXIT_IF(tspawn(stack, contender, (void *)0x0) < 0, "tspawn failed");

  while(state != S_CONTENDER_READY) {
    yield(-1);
  }

  state = S_START; // there's a bit of a race after here, but what can you do?

  while(read(file, buf, sizeof(buf)) > 0);

  TEST_FINI("graceful error");
  exit();
}

void
contender(void *a)
{
  state = S_CONTENDER_READY;

  while(state != S_START) {
    yield(-1);
  }

  close(file);

  texit();
}

void
make_file(void)
{
  int i, f;

  TEST_EXIT_IF((f = open(path, O_CREATE|O_WRONLY)) < 0, "failed to open for writing");

  for(i = 0; i < ROUNDS; i++) {
    TEST_EXIT_IF(write(f, contents, sizeof(contents)) != sizeof(contents), "write failed");
  }

  close(f);
}
