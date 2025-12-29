#pragma once
#include <vector>
#include <string>
#include "math3d.h"

// Triangle structure
struct Triangle {
    Vec3 vertices[3];
    Vec3 normal;
};

// Load STL file (supports both ASCII and binary formats)
std::vector<Triangle> loadSTL(const std::string& filename);

// Calculate bounding box and center model
void normalizeModel(std::vector<Triangle>& triangles, float& scale);