#!/bin/sh

result=$(cat files_read_not_existing.log)
expected_result="Failed to read not_existing_file
Caused by: No such file or directory
XFAIL files_read_not_existing (exit status: 1)"

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result\n\n"
    echo "Expected result: $expected_result\n\n"
    exit 1
fi