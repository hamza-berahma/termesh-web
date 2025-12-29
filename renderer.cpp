#include "renderer.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iostream>

// NEW: Include for EM_JS
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

// REMOVE Terminal control sequences
// #ifdef _WIN32
// ...
// #else
// #define CLEAR_SCREEN() ...
// #endif

// NEW: Define a JavaScript function from C++
// This function will find an HTML element with id="display"
// and set its text content.
EM_JS(void, update_display, (const char* str), {
    // 'str' is a pointer to the C++ string in WASM memory
    // UTF8ToString converts it to a JavaScript string
    var text = UTF8ToString(str);
    var displayElement = document.getElementById('display');
    if (displayElement) {
        displayElement.textContent = text;
    }
});

namespace {
    // Perspective projection
    Vec3 project(const Vec3& v) {
        float fov = 50.0f; // Distance to projection plane
        float z = v.z + fov;
        if (z <= 0) z = 0.1f; // Prevent division by zero
        
        return Vec3(
            (v.x / z) * SCREEN_WIDTH * 0.8f + SCREEN_WIDTH / 2.0f,
            (-v.y / z) * SCREEN_HEIGHT * 0.8f + SCREEN_HEIGHT / 2.0f,
            v.z
        );
    }

    // Calculate lighting intensity (0 to 1)
    float calculateLighting(const Vec3& normal, const Vec3& lightDir) {
        float intensity = std::max(0.0f, normal.dot(lightDir));
        return intensity * 0.8f + 0.2f; // Add ambient light
    }

    // --- Optimization 1: Modified drawTriangle (Gouraud shading) ---
    void drawTriangle(std::vector<std::string>& buffer, std::vector<float>& zbuffer,
                      const Vec3 projected[3], const float intensities[3]) {
        
        // Find bounding box
        int minX = std::max(0, (int)std::min({projected[0].x, projected[1].x, projected[2].x}));
        int maxX = std::min(SCREEN_WIDTH - 1, (int)std::max({projected[0].x, projected[1].x, projected[2].x}));
        int minY = std::max(0, (int)std::min({projected[0].y, projected[1].y, projected[2].y}));
        int maxY = std::min(SCREEN_HEIGHT - 1, (int)std::max({projected[0].y, projected[1].y, projected[2].y}));
        
        // Calculate edge vectors for barycentric coordinates
        Vec3 v0 = projected[1] - projected[0];
        Vec3 v1 = projected[2] - projected[0];
        float denom = v0.x * v1.y - v1.x * v0.y;
        if (std::abs(denom) < 0.001f) return; // Degenerate triangle
        
        // Rasterize triangle
        for (int y = minY; y <= maxY; y++) {
            for (int x = minX; x <= maxX; x++) {
                Vec3 p(x, y, 0);
                Vec3 v2 = p - projected[0];
                
                // Calculate barycentric coordinates
                float u = (v2.x * v1.y - v1.x * v2.y) / denom;
                float v = (v0.x * v2.y - v2.x * v0.y) / denom;
                float w = 1.0f - u - v;
                
                // Check if point is inside triangle
                if (u >= 0 && v >= 0 && w >= 0) {
                    // Interpolate depth
                    float z = w * projected[0].z + u * projected[1].z + v * projected[2].z;
                    
                    int idx = y * SCREEN_WIDTH + x;
                    if (z > zbuffer[idx]) {
                        zbuffer[idx] = z;
                        
                        // Interpolate intensity
                        float intensity = w * intensities[0] + u * intensities[1] + v * intensities[2];
                        int shadeIdx = std::min(SHADE_LEVELS - 1, (int)(intensity * SHADE_LEVELS));
                        
                        buffer[y][x] = SHADE_CHARS[shadeIdx];
                    }
                }
            }
        }
    }
} // anonymous namespace

void clearBuffers(std::vector<std::string>& buffer, std::vector<float>& zbuffer) {
    for (auto& line : buffer) {
        line.assign(SCREEN_WIDTH, ' '); // Efficiently reset the string
    }
    std::fill(zbuffer.begin(), zbuffer.end(), -1e10f);
}

void printBuffer(const std::vector<std::string>& buffer) {
    // --- MODIFIED: Single output to JavaScript ---
    std::stringstream ss;
    for (const auto& line : buffer) {
        ss << line << '\n';
    }
    
    // Call our JavaScript function
    update_display(ss.str().c_str());
}


void renderFrame(std::vector<std::string>& buffer, std::vector<float>& zbuffer,
                 const std::vector<Triangle>& model, const Mat3& rotation, 
                 const Vec3& lightDir) {
    
    // Transform and render all triangles
    for (const auto& tri : model) {
        // Transform vertices and normals
        Vec3 transformed[3];
        Vec3 transformedNormals[3];
        
        for (int i = 0; i < 3; i++) {
            transformed[i] = rotation * tri.vertices[i];
            transformedNormals[i] = (rotation * tri.normal).normalize();
        }
        
        // Backface culling
        Vec3 edge1 = transformed[1] - transformed[0];
        Vec3 edge2 = transformed[2] - transformed[0];
        Vec3 faceNormal = edge1.cross(edge2).normalize();
        Vec3 viewDir = Vec3(0, 0, -1);
        
        if (faceNormal.dot(viewDir) > 0) {
            // Project to 2D
            Vec3 projected[3];
            for (int i = 0; i < 3; i++) {
                projected[i] = project(transformed[i]);
            }
            
            // Calculate intensities per-vertex
            float intensities[3];
            for (int i = 0; i < 3; i++) {
                intensities[i] = calculateLighting(transformedNormals[i], lightDir);
            }
            
            // Draw triangle
            drawTriangle(buffer, zbuffer, projected, intensities);
        }
    }
}