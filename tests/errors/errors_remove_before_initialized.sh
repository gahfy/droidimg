#!/bin/sh

result=$(cat errors_remove_before_initialized.log)
expected_result="Adding/Removing message to errors queue before initializing it.
XFAIL errors_remove_before_initialized (exit status: 1)"

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result\n\n"
    echo "Expected result: $expected_result\n\n"
    exit 1
fi