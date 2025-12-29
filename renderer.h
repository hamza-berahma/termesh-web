#pragma once
#include <vector>
#include <string>
#include "math3d.h"
#include "model.h"

// Screen dimensions
constexpr int SCREEN_WIDTH = 240;
constexpr int SCREEN_HEIGHT = 80;

// ASCII characters for shading (darkest to brightest)
constexpr const char* SHADE_CHARS = " .:-=+*#%@";
constexpr int SHADE_LEVELS = 10;

// Clears the screen and z-buffers
void clearBuffers(std::vector<std::string>& buffer, std::vector<float>& zbuffer);

// Prints the character buffer to the console
void printBuffer(const std::vector<std::string>& buffer);

// Renders a single frame
void renderFrame(std::vector<std::string>& buffer, std::vector<float>& zbuffer,
                 const std::vector<Triangle>& model, const Mat3& rotation, 
                 const Vec3& lightDir);