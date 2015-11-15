/*
 * Macros for test emission.  Compatible with the Test Anything Protocol if
 * TEST_PREFIX is defined to be the empty string.
 */

/* Isn't this gross */
#define TEST_ID_(x) x
#define TEST_CONCAT_(a,b) a b

#ifndef TEST_NAME
#error Define TEST_NAME before including test-tapish
#endif
#ifndef TEST_PREFIX_PREFIX
#define TEST_PREFIX_PREFIX "TAPME: "
#endif
/* Prefix for test harness aid, since QEMU and other tools output, too */
#define TEST_PREFIX TEST_CONCAT_(TEST_CONCAT_(TEST_PREFIX_PREFIX, TEST_NAME), " ")

/* Primitives */
#define TEST_STRT(n)        do { printf(2, TEST_PREFIX "1..%d\n"            , n            );         } while(0);
#define TEST_FINI(msg, ...) do { printf(2, TEST_PREFIX "ok - "      msg "\n", ##__VA_ARGS__);         } while(0);
#define TEST_FAIL(msg, ...) do { printf(2, TEST_PREFIX "not ok - "  msg "\n", ##__VA_ARGS__);         } while(0);
#define TEST_EXIT(msg, ...) do { printf(2, TEST_PREFIX "Bail out! " msg "\n", ##__VA_ARGS__); exit(); } while(0);
#define TEST_DIAG(msg, ...) do { printf(2, TEST_PREFIX "# "         msg "\n", ##__VA_ARGS__);         } while(0);

/* Conditionals */
#define TEST_FAIL_IF(cond, msg, ...) if(cond) { TEST_FAIL(msg, ##__VA_ARGS__); }
#define TEST_EXIT_IF(cond, msg, ...) if(cond) { TEST_EXIT(msg, ##__VA_ARGS__); }
#define TEST_DIAG_IF(cond, msg, ...) if(cond) { TEST_DIAG(msg, ##__VA_ARGS__); }

#define TEST_FAIL_IF_THEN(cond, then, msg, ...) if(cond) { TEST_FAIL(msg, ##__VA_ARGS__); then; }
#define TEST_DIAG_IF_THEN(cond, then, msg, ...) if(cond) { TEST_DIAG(msg, ##__VA_ARGS__); then; }
