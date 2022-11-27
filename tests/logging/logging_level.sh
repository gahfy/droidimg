#!/bin/bash

result=$(cat logging_level.log)
error=0

if ! grep -q "Level Error: Error" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Warning: Error" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Info: Error" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Debug: Error" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Verbose: Error" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Warning: Warning" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Info: Warning" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Info: Info" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Debug: Warning" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Debug: Info" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Debug: Debug" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Verbose: Warning" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Verbose: Info" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Verbose: Debug" "logging_level.log"; then
  error = 1
fi
if ! grep -q "Level Verbose: Verbose" "logging_level.log"; then
  error = 1
fi
if [ $(wc -l "logging_level.log") != "16" ]; then
  error = 1
fi

if [ "$error" != "0" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result\n\n"
    echo "Expected result: $expected_result\n\n"
    rm -rf ./files
    exit 1
fi

rm -rf ./files