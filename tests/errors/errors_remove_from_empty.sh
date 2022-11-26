#!/bin/sh

result=$(cat errors_remove_from_empty.log)
expected_result="Removing last error from an empty queue.
XFAIL errors_remove_from_empty (exit status: 1)"

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result\n\n"
    echo "Expected result: $expected_result\n\n"
    exit 1
fi