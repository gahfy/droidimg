result=$(cat files_read_no_permission.log)
expected_result="Failed to read files/no_permission_file
Caused by: Permission denied
XFAIL files_read_no_permission (exit status: 1)"

if [ "$result" != "$expected_result" ]; then
    echo "Result of command is not as expected"
    echo "Result: $result\n\n"
    echo "Expected result: $expected_result\n\n"
    rm -rf ./files
    exit 1
fi

rm -rf ./files