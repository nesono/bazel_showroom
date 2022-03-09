#!/bin/bash
set -e

# This script will be run by bazel when building process starts to
# generate key-value information that represents the status of the
# workspace. The output should be like
#
# KEY1 VALUE1
# KEY2 VALUE2

git_revision()
{
  readonly gitrev="$(git rev-parse HEAD)"
  echo "STABLE_BUILD_SCM_REVISION $gitrev"
  echo "BUILD_SCM_REVISION $gitrev"
}


git_scm_status()
{
  # Check whether there are any uncommited changes
  if git diff-index --quiet HEAD; then
    readonly tree_status="clean"
  else
    readonly tree_status="modified"
  fi
  echo "STABLE_BUILD_SCM_STATUS ${tree_status}"
  echo "BUILD_SCM_STATUS ${tree_status}"
}


git_branch()
{
  gitbranch="$(git rev-parse --abbrev-ref HEAD | tr -d '\n\r' |  tr -c '[:alnum:]-/_' '_')"
  echo "STABLE_BUILD_SCM_BRANCH ${gitbranch}"
  echo "BUILD_SCM_BRANCH ${gitbranch}"
}

datestamp()
{
  readonly datestamp=$(date --utc --iso)
  echo "STABLE_BUILD_DATE_STAMP ${datestamp}"
  echo "BUILD_DATE_STAMP ${datestamp}"
}

timestamp()
{
  readonly timestamp=$(date --utc --iso-8601=minutes)
  echo "STABLE_BUILD_TIME_STAMP ${timestamp}"
  echo "BUILD_TIME_STAMP ${timestamp}"
}

git_revision
git_scm_status
git_branch
datestamp
timestamp
