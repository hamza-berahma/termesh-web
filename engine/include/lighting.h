#pragma once
#include "math3d.h"

/**
 * @file lighting.h
 * @brief Lighting calculation utilities.
 */

/**
 * @brief Calculates lighting intensity using Lambertian shading.
 * @param normal The surface normal (should be normalized).
 * @param lightDir The light direction (should be normalized).
 * @param ambientIntensity Ambient light contribution (0-1).
 * @param diffuseIntensity Diffuse light contribution (0-1).
 * @return Lighting intensity value between 0 and 1.
 */
float calculateLighting(const Vec3& normal, const Vec3& lightDir, 
                       float ambientIntensity = 0.2f, 
                       float diffuseIntensity = 0.8f);

