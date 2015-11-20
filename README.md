Crowdtesting for OS (3|4)18 Assignment 7
========================================

![It's dangerous to go alone! (Share yours) Take this.](https://craftinggeek.files.wordpress.com/2014/05/th-63.jpg)

If you go at this one alone, you're going to have a bad time. If you write tests, you'll have a better time. If you share tests, you'll have an amazing time. Help your peers, help yourself (and "insire" NWF's test cases)! May the odds be ever in your favor.

## Thanks! This is great...but how do I run it?

  1. `cp` this repo on top of your assignment (don't worry, there's nothing nefarious here, see for yourself!)
  2. Run `patch -i crowdtests.patch` from your assignment's root directory (this patches your `Makefile` to include the crowdsourced tests)
  3. Test away: `./crowdtest.sh` (you may need to `chmod +x ./crowdtest.sh`)
  4. Repeat steps 1-2 if you `git pull` new tests from the repo

## How to Share

  1. Submit a PR with your test cases 
    1. Please use NWF's TAP harness
    2. Please follow the guidelines in part 3 of the assignment for shared tests
  2. Only submit your test's `*.c` files (please name them `crowd_*.c` where `*` is some descritive name)
  3. Please verify that your tests work!\*
  4. Pat yourself on the back for being an awesome person :)

Note: make sure that your tests properly define their name **before** including `318_test-tapish.h`. Example:

```
// Note that the crowd_ prefix is important, this MUST match the binary's name
#define TEST_NAME "crowd_can_test"
#include "318_test-tapish.h"
```

## FAQ

### When I run `./crowdtest.sh`, I get `send: spawn id exp8 not open`

This probably means that your environment doesn't support kvm. Remove `-enable-kvm` from `runme.expect` (line 44) and retry.

### When I run `./crowdtest.sh`, I get `make fs.img failed`

You need to increase the `FSSIZE` constant in `param.h`, because the the filesystem is too small for all of the test executables (you can confirm this by looking at the `mkfs.c` output, it should say something like 900 of total 1000 blocks used). Increase it to something reasonable like `3000`. This requires you to do a `make clean`.

## License

See `license.txt`. Also: [wtfpl.net](http://www.wtfpl.net/).

## Shoutouts

SO to these awesome people who've contributed a lot of test cases:

  - @ewagner46 (Emily Wagner) & @junr03 (Jose Ulises Nino Rivera)
