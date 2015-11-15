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

  - Submit a PR with your test cases (please use NWF's TAP test harness)
  - Only submit your test's `*.c` files (please name them `crowd_*.c` where `*` is some descritive name)
  - Please verify that your tests work!
  - Pat yourself on the back for being an awesome person :)