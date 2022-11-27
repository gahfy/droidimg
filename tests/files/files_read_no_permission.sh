#!/bin/bash

file="files_read_no_permission.log"

wc_output=$(wc -l < $file)
length=${wc_output##*( )}
lines_kept=$(echo `expr $length - 1`)

result=$(head -n $lines_kept $file | perl -0p -e "s/\R\z//g" | perl -0p -e "s/\r\n/\n/g")
expected_result="Failed to read files/no_permission_file
Caused by: Permission denied"

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result"
    echo "Expected result: $expected_result"
    rm -rf ./files
    exit 1
fi

rm -rf ./files