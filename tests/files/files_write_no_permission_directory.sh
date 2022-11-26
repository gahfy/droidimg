#!/bin/sh

result=$(cat files_write_no_permission_directory.log)
expected_result="Failed to write files/no_permission_file
Caused by: Permission denied
XFAIL files_write_no_permission_directory (exit status: 1)"

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result\n\n"
    echo "Expected result: $expected_result\n\n"
    rm -rf ./files
    exit 1
fi

rm -rf ./files