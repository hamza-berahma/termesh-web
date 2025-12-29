# Testing Guide

This document describes the testing infrastructure for the Termesh project.

## Test Framework

The project uses a custom, lightweight test framework located in `include/test_framework.h`. It provides:

- Simple assertion macros (`ASSERT_TRUE`, `ASSERT_FALSE`, `ASSERT_EQ`, `ASSERT_FLOAT_EQ`, `ASSERT_VEC3_EQ`)
- Test result tracking and summary
- Clear pass/fail reporting

## Running Tests

### Using Make

```bash
# Build all tests
make tests

# Build and run all tests
make test
```

### Using the Test Script

```bash
cd tests
bash run_all_tests.sh
```

### Running Individual Tests

After building with `make tests`, you can run individual test executables:

```bash
./build/tests/test_math3d
./build/tests/test_projection
./build/tests/test_lighting
./build/tests/test_rasterizer
./build/tests/test_model
```

## Test Coverage

### Math Library Tests (`test_math3d.cpp`)

Tests for the 3D math library including:

- **Vec3**: Vector operations (addition, subtraction, scalar operations, dot/cross products, normalization)
- **Mat3**: Matrix operations (vector/matrix multiplication, matrix composition)
- **Rotations**: Rotation matrix generation (X, Y, Z axes)

**Coverage**: ~20 test cases

### Projection Tests (`test_projection.cpp`)

Tests for 3D-to-2D projection:

- Default parameters
- Origin projection
- Depth preservation
- Custom projection parameters
- Y-axis inversion
- Edge cases (near-zero Z)

**Coverage**: ~7 test cases

### Lighting Tests (`test_lighting.cpp`)

Tests for lighting calculations:

- Direct light (perpendicular)
- Side light (90 degrees)
- Back light (opposite direction)
- Angled light (45 degrees)
- Custom ambient/diffuse settings

**Coverage**: ~6 test cases

### Rasterizer Tests (`test_rasterizer.cpp`)

Tests for triangle rasterization:

- Buffer clearing
- Basic triangle rasterization
- Degenerate triangles
- Out-of-bounds triangles
- Z-buffer depth testing
- Shade character validation

**Coverage**: ~6 test cases

### Model Loading Tests (`test_model.cpp`)

Tests for STL file loading:

- ASCII STL format parsing
- Binary STL format parsing
- Error handling (nonexistent files)
- Model normalization (centering and scaling)
- Empty model handling

**Coverage**: ~6 test cases

**Note**: These tests create temporary STL files in `/tmp/` for testing.

## Writing New Tests

To add a new test, create a test function and register it with `RUN_TEST`:

```cpp
#include "test_framework.h"
#include "your_module.h"

void testYourFeature() {
    // Your test code here
    ASSERT_TRUE(someCondition);
    ASSERT_FLOAT_EQ(result, expected, 1e-5f);
}

int main() {
    RUN_TEST(testYourFeature);
    TestFramework::instance().printSummary();
    return TestFramework::instance().getExitCode();
}
```

## Assertion Macros

- `ASSERT_TRUE(condition)` - Asserts that condition is true
- `ASSERT_FALSE(condition)` - Asserts that condition is false
- `ASSERT_EQ(a, b)` - Asserts that a == b (for numeric types)
- `ASSERT_FLOAT_EQ(a, b, epsilon)` - Asserts that |a - b| < epsilon
- `ASSERT_VEC3_EQ(a, b, epsilon)` - Asserts that Vec3 a ≈ Vec3 b

## Continuous Integration

The test suite is designed to:

- Exit with code 0 if all tests pass
- Exit with code 1 if any test fails
- Provide clear, readable output

This makes it easy to integrate with CI/CD pipelines.

## Known Limitations

1. **No mocking framework**: Tests use real file I/O and create temporary files
2. **Simple framework**: No fixtures, setup/teardown, or parameterized tests
3. **Single-threaded**: Tests run sequentially

For more complex testing needs, consider integrating a framework like Google Test or Catch2.

