# API Reference

## math3d.h

### Vec3

```cpp
struct Vec3 {
    float x, y, z;
    Vec3 operator+(const Vec3& v) const;
    Vec3 operator-(const Vec3& v) const;
    Vec3 operator*(float s) const;
    Vec3 operator/(float s) const;
    float dot(const Vec3& v) const;
    Vec3 cross(const Vec3& v) const;
    float length() const;
    Vec3 normalize() const;
};
```

### Mat3

```cpp
struct Mat3 {
    float m[3][3];
    Vec3 operator*(const Vec3& v) const;
    Mat3 operator*(const Mat3& other) const;
};
```

### Rotation Functions

```cpp
Mat3 rotationX(float angle);
Mat3 rotationY(float angle);
Mat3 rotationZ(float angle);
```

## model.h

```cpp
struct Triangle {
    Vec3 vertices[3];
    Vec3 normal;
};

std::vector<Triangle> loadSTL(const std::string& filename);
void normalizeModel(std::vector<Triangle>& triangles, float& scale);
```

## projection.h

```cpp
struct ProjectionParams {
    float fov = 50.0f;
    float screenWidth = 240.0f;
    float screenHeight = 80.0f;
    float scaleFactor = 0.8f;
};

Vec3 project(const Vec3& v, const ProjectionParams& params = ProjectionParams());
```

## lighting.h

```cpp
float calculateLighting(
    const Vec3& normal,
    const Vec3& lightDir,
    float ambientIntensity = 0.2f,
    float diffuseIntensity = 0.8f
);
```

## rasterizer.h

```cpp
constexpr int SCREEN_WIDTH = 240;
constexpr int SCREEN_HEIGHT = 80;
constexpr const char* SHADE_CHARS = " .:-=+*#%@";
constexpr int SHADE_LEVELS = 10;

void rasterizeTriangle(
    std::vector<std::string>& buffer,
    std::vector<float>& zbuffer,
    const Vec3 projected[3],
    const float intensities[3]
);

void clearBuffers(
    std::vector<std::string>& buffer,
    std::vector<float>& zbuffer
);
```

## renderer.h

```cpp
void renderFrame(
    std::vector<std::string>& buffer,
    std::vector<float>& zbuffer,
    const std::vector<Triangle>& model,
    const Mat3& rotation,
    const Vec3& lightDir
);

void printBuffer(const std::vector<std::string>& buffer);
```

