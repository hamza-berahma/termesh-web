#include "model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <cmath>

namespace {
    // Read 32-bit float from binary file
    float readFloat(std::ifstream& file) {
        float val;
        file.read(reinterpret_cast<char*>(&val), sizeof(float));
        return val;
    }

    // Read 16-bit unsigned integer from binary file
    uint16_t readUint16(std::ifstream& file) {
        uint16_t val;
        file.read(reinterpret_cast<char*>(&val), sizeof(uint16_t));
        return val;
    }
} // anonymous namespace

std::vector<Triangle> loadSTL(const std::string& filename) {
    std::vector<Triangle> triangles;
    std::ifstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return triangles;
    }
    
    // Check if file is ASCII or binary
    char header[6];
    file.read(header, 5);
    header[5] = '\0';
    file.seekg(0);
    
    if (std::string(header) == "solid") {
        // ASCII STL format
        std::string line, keyword;
        Triangle tri;
        int vertexIdx = 0;
        
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            iss >> keyword;
            
            if (keyword == "facet") {
                iss >> keyword; // "normal"
                iss >> tri.normal.x >> tri.normal.y >> tri.normal.z;
                vertexIdx = 0;
            } else if (keyword == "vertex") {
                iss >> tri.vertices[vertexIdx].x >> tri.vertices[vertexIdx].y >> tri.vertices[vertexIdx].z;
                vertexIdx++;
                if (vertexIdx == 3) {
                    triangles.push_back(tri);
                }
            }
        }
    } else {
        // Binary STL format
        file.seekg(80); // Skip header
        uint32_t numTriangles;
        file.read(reinterpret_cast<char*>(&numTriangles), sizeof(uint32_t));
        
        triangles.reserve(numTriangles);
        
        for (uint32_t i = 0; i < numTriangles; i++) {
            Triangle tri;
            
            // Read normal
            tri.normal.x = readFloat(file);
            tri.normal.y = readFloat(file);
            tri.normal.z = readFloat(file);
            
            // Read vertices
            for (int j = 0; j < 3; j++) {
                tri.vertices[j].x = readFloat(file);
                tri.vertices[j].y = readFloat(file);
                tri.vertices[j].z = readFloat(file);
            }
            
            // Skip attribute byte count
            readUint16(file);
            
            triangles.push_back(tri);
        }
    }
    
    file.close();
    std::cout << "Loaded " << triangles.size() << " triangles from " << filename << std::endl;
    return triangles;
}

void normalizeModel(std::vector<Triangle>& triangles, float& scale) {
    if (triangles.empty()) return;
    
    Vec3 minBounds(1e10f, 1e10f, 1e10f);
    Vec3 maxBounds(-1e10f, -1e10f, -1e10f);
    
    // Find bounding box
    for (const auto& tri : triangles) {
        for (int i = 0; i < 3; i++) {
            minBounds.x = std::min(minBounds.x, tri.vertices[i].x);
            minBounds.y = std::min(minBounds.y, tri.vertices[i].y);
            minBounds.z = std::min(minBounds.z, tri.vertices[i].z);
            maxBounds.x = std::max(maxBounds.x, tri.vertices[i].x);
            maxBounds.y = std::max(maxBounds.y, tri.vertices[i].y);
            maxBounds.z = std::max(maxBounds.z, tri.vertices[i].z);
        }
    }
    
    // Calculate center and scale
    Vec3 center = (minBounds + maxBounds) * 0.5f;
    Vec3 size = maxBounds - minBounds;
    float maxDim = std::max({size.x, size.y, size.z});
    scale = 30.0f / maxDim; // Scale to fit in view
    
    // Center and scale model
    for (auto& tri : triangles) {
        for (int i = 0; i < 3; i++) {
            tri.vertices[i] = (tri.vertices[i] - center) * scale;
        }
    }
}

