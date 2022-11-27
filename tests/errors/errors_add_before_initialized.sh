#!/bin/sh

file="errors_add_before_initialized.log"

wc_output=$(wc -l < $file)
length=${wc_output##*( )}
lines_kept=$(echo `expr $length - 1`)

result=$(head -n $lines_kept $file | perl -0p -e "s/\R\z//g")
expected_result="Adding/Removing message to errors queue before initializing it."

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result"
    echo "Expected result: $expected_result"
    exit 1
fi