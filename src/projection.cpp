#include "projection.h"
#include <cmath>

Vec3 project(const Vec3& v, const ProjectionParams& params) {
    float z = v.z + params.fov;
    if (z <= 0) z = 0.1f; // Prevent division by zero
    
    return Vec3(
        (v.x / z) * params.screenWidth * params.scaleFactor + params.screenWidth / 2.0f,
        (-v.y / z) * params.screenHeight * params.scaleFactor + params.screenHeight / 2.0f,
        v.z
    );
}

