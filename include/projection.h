#pragma once
#include "math3d.h"

/**
 * @file projection.h
 * @brief 3D to 2D projection utilities.
 */

// Perspective projection parameters
struct ProjectionParams {
    float fov = 50.0f;              // Distance to projection plane
    float screenWidth = 240.0f;
    float screenHeight = 80.0f;
    float scaleFactor = 0.8f;
};

/**
 * @brief Projects a 3D point to 2D screen coordinates using perspective projection.
 * @param v The 3D point to project.
 * @param params Projection parameters.
 * @return The projected 2D point (x, y) with z-depth preserved.
 */
Vec3 project(const Vec3& v, const ProjectionParams& params = ProjectionParams());

