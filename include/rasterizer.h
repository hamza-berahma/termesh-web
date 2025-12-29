#pragma once
#include <vector>
#include <string>
#include "math3d.h"

/**
 * @file rasterizer.h
 * @brief Triangle rasterization utilities.
 */

// Screen dimensions
constexpr int SCREEN_WIDTH = 240;
constexpr int SCREEN_HEIGHT = 80;

// ASCII characters for shading (darkest to brightest)
constexpr const char* SHADE_CHARS = " .:-=+*#%@";
constexpr int SHADE_LEVELS = 10;

/**
 * @brief Rasterizes a triangle into the frame buffer using barycentric interpolation.
 * @param buffer Character buffer (SCREEN_HEIGHT rows of SCREEN_WIDTH characters).
 * @param zbuffer Depth buffer for z-testing.
 * @param projected Array of 3 projected vertices (x, y, z).
 * @param intensities Array of 3 lighting intensities (one per vertex).
 */
void rasterizeTriangle(std::vector<std::string>& buffer, 
                      std::vector<float>& zbuffer,
                      const Vec3 projected[3], 
                      const float intensities[3]);

/**
 * @brief Clears the screen and z-buffers.
 * @param buffer Character buffer to clear.
 * @param zbuffer Depth buffer to clear.
 */
void clearBuffers(std::vector<std::string>& buffer, std::vector<float>& zbuffer);

