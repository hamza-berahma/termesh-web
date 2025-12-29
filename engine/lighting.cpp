#include "lighting.h"
#include <algorithm>

float calculateLighting(const Vec3& normal, const Vec3& lightDir, 
                       float ambientIntensity, 
                       float diffuseIntensity) {
    float dotProduct = normal.dot(lightDir);
    float diffuse = std::max(0.0f, dotProduct);
    return diffuse * diffuseIntensity + ambientIntensity;
}

