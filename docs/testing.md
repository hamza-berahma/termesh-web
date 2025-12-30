# Testing

## Framework

Custom test framework in `engine/include/test_framework.h`.

Assertions:
- `ASSERT_TRUE(condition)`
- `ASSERT_FALSE(condition)`
- `ASSERT_EQ(a, b)`
- `ASSERT_FLOAT_EQ(a, b, epsilon)`
- `ASSERT_VEC3_EQ(a, b, epsilon)`

## Running Tests

```bash
cd engine
make tests    # build
make test     # build and run
```

Individual tests:
```bash
./build/tests/test_math3d
./build/tests/test_projection
./build/tests/test_lighting
./build/tests/test_rasterizer
./build/tests/test_model
```

## Test Coverage

- **math3d**: vector/matrix ops, rotations (~20 cases)
- **projection**: perspective transform, edge cases (~7 cases)
- **lighting**: Lambertian shading, angles (~6 cases)
- **rasterizer**: barycentric, z-buffer, bounds (~6 cases)
- **model**: STL parsing (ASCII/binary), normalization (~6 cases)

