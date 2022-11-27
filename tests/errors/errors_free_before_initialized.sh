#!/bin/bash

file="errors_free_before_initialized.log"

wc_output=$(wc -l < $file)
length=${wc_output##*( )}
lines_kept=$(echo `expr $length - 1`)

result=$(head -n $lines_kept $file | perl -0p -e "s/\R\z//g" | perl -0p -e "s/\r\n/\n/g")
expected_result="Freeing errors pointer which has not been initialized."

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result"
    echo "Expected result: $expected_result"
    exit 1
fi