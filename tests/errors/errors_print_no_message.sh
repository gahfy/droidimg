#!/bin/sh

result=$(cat errors_print_no_message.log)
expected_result="PASS errors_print_no_message (exit status: 0)"

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result\n\n"
    echo "Expected result: $expected_result\n\n"
    exit 1
fi