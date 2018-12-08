#!/bin/bash
set -e

# This script will be run by bazel when building process starts to
# generate key-value information that represents the status of the
# workspace. The output should be like
#
# KEY1 VALUE1
# KEY2 VALUE2

gitrev="$(git rev-parse HEAD)"
echo "BUILD_SCM_REVISION $gitrev"

# Check whether there are any uncommited changes
if git diff-index --quiet HEAD; then
  tree_status="clean"
else
  tree_status="modified"
fi
echo "BUILD_SCM_STATUS ${tree_status}"

gitbranch="$(git rev-parse --abbrev-ref HEAD | tr -d '\n\r' |  tr -c '[:alnum:]-/_' '_')"

echo "BUILD_SCM_BRANCH ${gitbranch}"
