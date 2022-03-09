#!/bin/bash

# This script generates a header file that is used for linkstamping bazel targets.

readonly status_file=$1

while IFS= read -r line
do
  set $line
  param=$(echo $1 | sed 's/^STABLE_//')
  shift
  value="$@"
  echo "extern const char $param[];"
  echo "const char $param[] = \"$value\";"
done < "$status_file"