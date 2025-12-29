#include "rasterizer.h"
#include <algorithm>
#include <cmath>

void rasterizeTriangle(std::vector<std::string>& buffer, 
                      std::vector<float>& zbuffer,
                      const Vec3 projected[3], 
                      const float intensities[3]) {
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

void clearBuffers(std::vector<std::string>& buffer, std::vector<float>& zbuffer) {
    for (auto& line : buffer) {
        line.assign(SCREEN_WIDTH, ' '); // Efficiently reset the string
    }
    std::fill(zbuffer.begin(), zbuffer.end(), -1e10f);
}

