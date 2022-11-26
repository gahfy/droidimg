#!/bin/sh

result=$(cat errors_print_two_messages.log)
expected_result="Test error one
Caused by: Test error two
PASS errors_print_two_messages (exit status: 0)"

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result\n\n"
    echo "Expected result: $expected_result\n\n"
    exit 1
fi