#!/bin/sh

result=$(cat errors_add_two_remove_one_add_one_print.log)
expected_result="Test error one
Caused by: Test error three
PASS errors_add_two_remove_one_add_one_print (exit status: 0)"

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result\n\n"
    echo "Expected result: $expected_result\n\n"
    exit 1
fi