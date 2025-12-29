#pragma once
#include <vector>
#include <string>
#include "math3d.h"
#include "model.h"
#include "rasterizer.h"

/**
 * @file renderer.h
 * @brief High-level rendering functions.
 */

/**
 * @brief Renders a single frame of the model.
 * @param buffer Character buffer for output.
 * @param zbuffer Depth buffer for z-testing.
 * @param model The triangle mesh to render.
 * @param rotation Rotation matrix to apply to the model.
 * @param lightDir Light direction vector (should be normalized).
 */
void renderFrame(std::vector<std::string>& buffer, 
                std::vector<float>& zbuffer,
                const std::vector<Triangle>& model, 
                const Mat3& rotation, 
                const Vec3& lightDir);

/**
 * @brief Prints the character buffer. In WASM builds, this updates the display element.
 * @param buffer The character buffer to print.
 */
void printBuffer(const std::vector<std::string>& buffer);

