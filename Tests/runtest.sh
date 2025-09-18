TEST_FILE="$1"  # тестовый файл (.dat)
EXECUTABLE="$2" # исполняемый файл

result=$(${EXECUTABLE} < "${TEST_FILE}")

expected_file="${TEST_FILE}.expected"
expected=$(cat "${expected_file}")

if [ "$result" = "$expected" ]; then
    exit 0
else
    echo "FAIL:     ${TEST_FILE}"
    echo "Expected: $expected"
    echo "Got:      $result"
    exit 1
fi