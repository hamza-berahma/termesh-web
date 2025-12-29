#!/bin/bash
# Run all unit tests

set -e

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Build directory
TEST_BUILD_DIR="../build/tests"

# Make sure build directory exists
mkdir -p "$TEST_BUILD_DIR"

# Change to project root
cd "$(dirname "$0")/.."

# Build tests
echo "Building tests..."
make tests || {
    echo -e "${RED}Failed to build tests${NC}"
    exit 1
}

# Run each test
echo ""
echo "Running tests..."
echo "=================="
echo ""

PASSED=0
FAILED=0

for test in "$TEST_BUILD_DIR"/test_*; do
    if [ -f "$test" ] && [ -x "$test" ]; then
        test_name=$(basename "$test")
        echo "Running $test_name..."
        if "$test"; then
            ((PASSED++))
            echo -e "${GREEN}✓ $test_name passed${NC}"
        else
            ((FAILED++))
            echo -e "${RED}✗ $test_name failed${NC}"
        fi
        echo ""
    fi
done

# Summary
echo "=================="
echo -e "${GREEN}Passed: $PASSED${NC}"
if [ $FAILED -gt 0 ]; then
    echo -e "${RED}Failed: $FAILED${NC}"
    exit 1
else
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
fi

