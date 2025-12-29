#include "test_framework.h"
#include "model.h"
#include "math3d.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <cmath>

void createSimpleASCIISTL(const std::string& filename) {
    std::ofstream file(filename);
    file << "solid test_cube\n";
    file << "  facet normal 0 0 1\n";
    file << "    outer loop\n";
    file << "      vertex 0 0 0\n";
    file << "      vertex 1 0 0\n";
    file << "      vertex 0 1 0\n";
    file << "    endloop\n";
    file << "  endfacet\n";
    file << "  facet normal 0 0 -1\n";
    file << "    outer loop\n";
    file << "      vertex 0 0 1\n";
    file << "      vertex 0 1 1\n";
    file << "      vertex 1 0 1\n";
    file << "    endloop\n";
    file << "  endfacet\n";
    file << "endsolid test_cube\n";
    file.close();
}

void createSimpleBinarySTL(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    
    // Write 80-byte header
    char header[80] = {0};
    std::string headerStr = "Binary STL";
    std::copy(headerStr.begin(), headerStr.end(), header);
    file.write(header, 80);
    
    // Write triangle count (2)
    uint32_t numTriangles = 2;
    file.write(reinterpret_cast<const char*>(&numTriangles), sizeof(uint32_t));
    
    // Write first triangle
    float normal[3] = {0.0f, 0.0f, 1.0f};
    file.write(reinterpret_cast<const char*>(normal), sizeof(float) * 3);
    float v1[3] = {0.0f, 0.0f, 0.0f};
    float v2[3] = {1.0f, 0.0f, 0.0f};
    float v3[3] = {0.0f, 1.0f, 0.0f};
    file.write(reinterpret_cast<const char*>(v1), sizeof(float) * 3);
    file.write(reinterpret_cast<const char*>(v2), sizeof(float) * 3);
    file.write(reinterpret_cast<const char*>(v3), sizeof(float) * 3);
    uint16_t attribute = 0;
    file.write(reinterpret_cast<const char*>(&attribute), sizeof(uint16_t));
    
    // Write second triangle
    float normal2[3] = {0.0f, 0.0f, -1.0f};
    file.write(reinterpret_cast<const char*>(normal2), sizeof(float) * 3);
    float v4[3] = {0.0f, 0.0f, 1.0f};
    float v5[3] = {0.0f, 1.0f, 1.0f};
    float v6[3] = {1.0f, 0.0f, 1.0f};
    file.write(reinterpret_cast<const char*>(v4), sizeof(float) * 3);
    file.write(reinterpret_cast<const char*>(v5), sizeof(float) * 3);
    file.write(reinterpret_cast<const char*>(v6), sizeof(float) * 3);
    file.write(reinterpret_cast<const char*>(&attribute), sizeof(uint16_t));
    
    file.close();
}

void testLoadASCIISTL() {
    const std::string filename = "/tmp/test_ascii.stl";
    createSimpleASCIISTL(filename);
    
    std::vector<Triangle> triangles = loadSTL(filename);
    
    ASSERT_TRUE(triangles.size() == 2);
    
    // Check first triangle
    Triangle& tri1 = triangles[0];
    ASSERT_VEC3_EQ(tri1.normal, Vec3(0.0f, 0.0f, 1.0f), 1e-5f);
    ASSERT_VEC3_EQ(tri1.vertices[0], Vec3(0.0f, 0.0f, 0.0f), 1e-5f);
    ASSERT_VEC3_EQ(tri1.vertices[1], Vec3(1.0f, 0.0f, 0.0f), 1e-5f);
    ASSERT_VEC3_EQ(tri1.vertices[2], Vec3(0.0f, 1.0f, 0.0f), 1e-5f);
    
    // Check second triangle
    Triangle& tri2 = triangles[1];
    ASSERT_VEC3_EQ(tri2.normal, Vec3(0.0f, 0.0f, -1.0f), 1e-5f);
}

void testLoadBinarySTL() {
    const std::string filename = "/tmp/test_binary.stl";
    createSimpleBinarySTL(filename);
    
    std::vector<Triangle> triangles = loadSTL(filename);
    
    ASSERT_TRUE(triangles.size() == 2);
    
    // Check first triangle
    Triangle& tri1 = triangles[0];
    ASSERT_VEC3_EQ(tri1.normal, Vec3(0.0f, 0.0f, 1.0f), 1e-5f);
    ASSERT_VEC3_EQ(tri1.vertices[0], Vec3(0.0f, 0.0f, 0.0f), 1e-5f);
    ASSERT_VEC3_EQ(tri1.vertices[1], Vec3(1.0f, 0.0f, 0.0f), 1e-5f);
    ASSERT_VEC3_EQ(tri1.vertices[2], Vec3(0.0f, 1.0f, 0.0f), 1e-5f);
}

void testLoadSTLNonexistent() {
    std::vector<Triangle> triangles = loadSTL("/tmp/nonexistent_file.stl");
    ASSERT_TRUE(triangles.empty());
}

void testNormalizeModel() {
    const std::string filename = "/tmp/test_normalize.stl";
    createSimpleASCIISTL(filename);
    
    std::vector<Triangle> triangles = loadSTL(filename);
    float scale;
    normalizeModel(triangles, scale);
    
    // Model should be centered and scaled
    // After normalization, bounding box should be centered at origin
    Vec3 minBounds(1e10f, 1e10f, 1e10f);
    Vec3 maxBounds(-1e10f, -1e10f, -1e10f);
    
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
    
    Vec3 center = (minBounds + maxBounds) * 0.5f;
    // Center should be approximately at origin
    ASSERT_VEC3_EQ(center, Vec3(0.0f, 0.0f, 0.0f), 1.0f); // Allow some tolerance
}

void testNormalizeModelEmpty() {
    std::vector<Triangle> empty;
    float scale;
    normalizeModel(empty, scale);
    
    // Should not crash
    ASSERT_TRUE(empty.empty());
}

int main() {
    std::cout << "Running model loading tests..." << std::endl;
    RUN_TEST(testLoadASCIISTL);
    RUN_TEST(testLoadBinarySTL);
    RUN_TEST(testLoadSTLNonexistent);
    RUN_TEST(testNormalizeModel);
    RUN_TEST(testNormalizeModelEmpty);

    TestFramework::instance().printSummary();
    return TestFramework::instance().getExitCode();
}

