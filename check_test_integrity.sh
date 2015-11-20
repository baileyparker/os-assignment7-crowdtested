#!/usr/bin/env bash

TMP_DIR="/tmp/osf15_asgmt7_test_integrity"
WES_KERNEL="https://github.com/nwf/xv6-public"
WES_KERNEL_BRANCH="uthread-assignment-f15"

rm -rf "$TMP_DIR" && \
mkdir -p "$TMP_DIR"

if [ "$?" -ne 0 ]; then
    echo "Temp directory is inaccessible";
    exit 1;
fi;


# Clone NWF's uthread into a tmp folder
git clone -b "$WES_KERNEL_BRANCH" "$WES_KERNEL" "$TMP_DIR"

if [ "$?" -ne 0 ]; then
    echo "Clone failed";
    exit 1;
fi;

# Copy our tests on top of it
cp *.c "$TMP_DIR" && cp *.h "$TMP_DIR" && cp *.expect "$TMP_DIR" && \
cp crowdtest.sh "$TMP_DIR" && cp crowdtests.patch "$TMP_DIR"

if [ "$?" -ne 0 ]; then
    echo "Unable to copy our tests on top of clone";
    exit 1;
fi;

# Run our tests on NWFs kernel
cd "$TMP_DIR" && \
patch -i crowdtests.patch

if [ "$?" -ne 0 ]; then
    echo "Unable to patch Makefile";
    exit 1;
fi;

./crowdtest.sh

cd - && rm -rf "$TMP_DIR"
