#include "renderer.h"
#include "projection.h"
#include "lighting.h"
#include <sstream>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>

// Define a JavaScript function from C++
// This function will find an HTML element with id="display"
// and set its text content.
EM_JS(void, update_display, (const char* str), {
    var text = UTF8ToString(str);
    var displayElement = document.getElementById('display');
    if (displayElement) {
        displayElement.textContent = text;
    }
});
#endif

void printBuffer(const std::vector<std::string>& buffer) {
    std::stringstream ss;
    for (const auto& line : buffer) {
        ss << line << '\n';
    }
    
#ifdef __EMSCRIPTEN__
    // Call our JavaScript function in WASM builds
    update_display(ss.str().c_str());
#else
    // Print to stdout in native builds
    std::cout << ss.str();
#endif
}

void renderFrame(std::vector<std::string>& buffer, 
                std::vector<float>& zbuffer,
                const std::vector<Triangle>& model, 
                const Mat3& rotation, 
                const Vec3& lightDir) {
    
    ProjectionParams projParams;
    
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
                projected[i] = project(transformed[i], projParams);
            }
            
            // Calculate intensities per-vertex
            float intensities[3];
            for (int i = 0; i < 3; i++) {
                intensities[i] = calculateLighting(transformedNormals[i], lightDir);
            }
            
            // Draw triangle
            rasterizeTriangle(buffer, zbuffer, projected, intensities);
        }
    }
}

