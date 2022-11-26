#!/bin/sh

result=$(cat logging_level.log)
expected_result="Level Error: Error
Level Warning: Error
Level Info: Error
Level Debug: Error
Level Verbose: Error
Level Warning: Warning
Level Info: Warning
Level Info: Info
Level Debug: Warning
Level Debug: Info
Level Debug: Debug
Level Verbose: Warning
Level Verbose: Info
Level Verbose: Debug
Level Verbose: Verbose
PASS logging_level (exit status: 0)"

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result\n\n"
    echo "Expected result: $expected_result\n\n"
    rm -rf ./files
    exit 1
fi

rm -rf ./files