#!/bin/bash

C_RED='\e[0;31m'
C_GREEN='\e[0;32m'
C_YELLOW='\e[0;33m'
C_BLUE='\e[0;34m'
C_RESET='\e[0m'

# Without this, things fail catastrophically after changing code
rm -f fs.img

# Try to `make`, and fail on any errors before testing
# (This way each test doesn't fail)
echo "make..."
make_output=$(/usr/bin/env make 2>&1)

if [ $? -ne 0 ]; then
    printf "$make_output\n\n"
    printf "${C_RED}make failed. See above.${C_RESET}\n"
    exit 1;
fi;

# Track any failures
testsran=0;
teststatus=0;
failed='';

# tee to STDOUT and variable BASH Wizardry courtesy of SO:
# http://stackoverflow.com/a/12451419/568785
exec 5>&1

# fail on first pipe fail also courtesy of SO:
# http://unix.stackexchange.com/a/73180/8506
set -o pipefail

# Iterate through all of our crowd_* tests
while IFS= read -r -d $'\0' f; do
    testname="$(echo $f | sed 's/\.c$//')"

    printf "${C_BLUE}============================== $testname ==============================${C_RESET}\n\n";

    if [ "$testname" = "crowd_yield_lots" ]; then
        printf "This test will never exit, you have to run it manually and verify its output.\n"
    else
        # Make sure each test has a clean filesystem
        rm -f fs.img

        # Run each test in a separate xv6
        # (Only starts piping output after we see a prompt)
        testresult=$(/usr/bin/env expect runme.expect "$testname" | sed '1,/\$ crowd_/d' | tee >(cat - >&5))

        # Check for test failure
        if [ "${PIPESTATUS[0]}" -ne 0 ]; then
            teststatus=1;
            failed+=" $testname";

            if echo "$testresult" | grep "exec ${testname} failed"; then
                printf "${C_YELLOW}\nThe test failed to run. Did you \`patch -i crowdtests.patch\`?\n\n${C_RESET}";
            fi;

            printf "${C_RED}!!!!!!!!!!!!!!!!!!!!!!!!!!! $testname FAILED !!!!!!!!!!!!!!!!!!!!!!!!!!${C_RESET}\n";
        fi;

        ((testsran++))
    fi;

    echo "";
    echo "";
done < <(find * -name 'crowd_*.c' -print0);

if [ $testsran -eq 0 ]; then
    printf "${C_YELLOW}No tests to run :(${C_RESET}\n"
    echo "Did you properly copy this repo on top of yours?"
    exit 0;
else
    # Your fate has been decided
    if [ $teststatus -ne 0 ]; then
        printf "${C_RED}SOME TESTS FAILED (╯°□°)╯︵ ┻━┻\n";
        printf "Failures:$failed${C_RESET}\n";
    else
        printf "${C_GREEN}${testsran} TEST(S) PASSED!${C_RESET}\n";
    fi;

    exit $teststatus;
fi;
