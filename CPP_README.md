# C++ Codebase Structure

This document describes the modular C++ codebase structure for the Termesh project.

## Directory Structure

```
.
├── include/          # Header files
│   ├── math3d.h      # 3D math library (Vec3, Mat3, rotations)
│   ├── model.h       # Triangle mesh and STL loading
│   ├── projection.h  # 3D-to-2D projection utilities
│   ├── lighting.h    # Lighting calculation functions
│   ├── rasterizer.h  # Triangle rasterization
│   ├── renderer.h    # High-level rendering functions
│   └── test_framework.h  # Unit test framework
│
├── src/              # Implementation files
│   ├── main.cpp      # Application entry point
│   ├── model.cpp     # STL file loading and model normalization
│   ├── projection.cpp # Perspective projection implementation
│   ├── lighting.cpp  # Lambertian lighting calculations
│   ├── rasterizer.cpp # Triangle rasterization with z-buffer
│   └── renderer.cpp  # Frame rendering orchestration
│
└── tests/            # Unit tests
    ├── test_math3d.cpp      # Math library tests
    ├── test_projection.cpp  # Projection tests
    ├── test_lighting.cpp    # Lighting tests
    ├── test_rasterizer.cpp  # Rasterizer tests
    ├── test_model.cpp       # Model loading tests
    └── run_all_tests.sh     # Test runner script
```

## Module Overview

### Math Library (`math3d.h`)

**Purpose**: Core 3D mathematics primitives

**Components**:
- `Vec3`: 3D vector with arithmetic operations
- `Mat3`: 3x3 matrix for rotations
- `rotationX/Y/Z()`: Rotation matrix generators

**Dependencies**: None (header-only, inline implementations)

**Usage**:
```cpp
#include "math3d.h"

Vec3 v(1.0f, 2.0f, 3.0f);
Mat3 rot = rotationY(M_PI / 4);
Vec3 rotated = rot * v;
```

### Model Loading (`model.h` / `model.cpp`)

**Purpose**: Load and process 3D triangle meshes from STL files

**Components**:
- `Triangle`: Triangle structure with vertices and normal
- `loadSTL()`: Loads ASCII or binary STL files
- `normalizeModel()`: Centers and scales model to fit viewport

**Dependencies**: `math3d.h`

**Usage**:
```cpp
#include "model.h"

std::vector<Triangle> triangles = loadSTL("model.stl");
float scale;
normalizeModel(triangles, scale);
```

### Projection (`projection.h` / `projection.cpp`)

**Purpose**: Transform 3D points to 2D screen coordinates

**Components**:
- `ProjectionParams`: Configurable projection parameters
- `project()`: Perspective projection function

**Dependencies**: `math3d.h`

**Usage**:
```cpp
#include "projection.h"

Vec3 worldPos(10.0f, 20.0f, 50.0f);
Vec3 screenPos = project(worldPos);
```

### Lighting (`lighting.h` / `lighting.cpp`)

**Purpose**: Calculate lighting intensity for surfaces

**Components**:
- `calculateLighting()`: Lambertian shading with ambient and diffuse components

**Dependencies**: `math3d.h`

**Usage**:
```cpp
#include "lighting.h"

Vec3 normal(0.0f, 0.0f, 1.0f);
Vec3 lightDir(0.5f, -0.7f, -0.5f);
float intensity = calculateLighting(normal, lightDir);
```

### Rasterizer (`rasterizer.h` / `rasterizer.cpp`)

**Purpose**: Rasterize triangles into character buffer

**Components**:
- `rasterizeTriangle()`: Barycentric interpolation rasterization
- `clearBuffers()`: Clear frame and z-buffers
- Constants: `SCREEN_WIDTH`, `SCREEN_HEIGHT`, `SHADE_CHARS`

**Dependencies**: `math3d.h`

**Usage**:
```cpp
#include "rasterizer.h"

std::vector<std::string> buffer(SCREEN_HEIGHT, std::string(SCREEN_WIDTH, ' '));
std::vector<float> zbuffer(SCREEN_WIDTH * SCREEN_HEIGHT);
Vec3 projected[3] = {...};
float intensities[3] = {...};
rasterizeTriangle(buffer, zbuffer, projected, intensities);
```

### Renderer (`renderer.h` / `renderer.cpp`)

**Purpose**: High-level rendering orchestration

**Components**:
- `renderFrame()`: Renders entire model frame
- `printBuffer()`: Outputs buffer (to stdout or WASM display)

**Dependencies**: All other modules

**Usage**:
```cpp
#include "renderer.h"

renderFrame(buffer, zbuffer, triangles, rotation, lightDir);
printBuffer(buffer);
```

## Build System

### Native Build (Make)

```bash
# Build main executable
make

# Build all tests
make tests

# Build and run all tests
make test

# Clean build artifacts
make clean
```

### WASM Build (Emscripten)

```bash
# Build WASM version
bash setup.sh

# Or use make
make wasm
```

## Design Principles

1. **Modularity**: Each module has a single, clear responsibility
2. **Header-only where appropriate**: Math library is header-only for inlining
3. **Testability**: All modules are designed to be easily unit tested
4. **Dependency minimization**: Modules depend only on what they need
5. **Clear interfaces**: Headers document expected behavior

## Adding New Modules

To add a new module:

1. Create header in `include/your_module.h`
2. Create implementation in `src/your_module.cpp`
3. Add to `Makefile` if needed
4. Write tests in `tests/test_your_module.cpp`
5. Document in this README

## Dependencies

- **C++17**: Required for all modules
- **Standard Library**: `<vector>`, `<string>`, `<algorithm>`, `<cmath>`, `<fstream>`, `<iostream>`
- **Emscripten**: Only needed for WASM builds

No external libraries required for native builds!

