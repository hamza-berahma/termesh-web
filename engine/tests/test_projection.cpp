#include "test_framework.h"
#include "projection.h"
#include "math3d.h"
#include <cmath>

void testProjectionDefaultParams() {
    ProjectionParams params;
    ASSERT_FLOAT_EQ(params.fov, 50.0f, 1e-5f);
    ASSERT_FLOAT_EQ(params.screenWidth, 240.0f, 1e-5f);
    ASSERT_FLOAT_EQ(params.screenHeight, 80.0f, 1e-5f);
    ASSERT_FLOAT_EQ(params.scaleFactor, 0.8f, 1e-5f);
}

void testProjectionOrigin() {
    Vec3 v(0.0f, 0.0f, 0.0f);
    Vec3 result = project(v);
    
    // Origin should project to center of screen
    ASSERT_FLOAT_EQ(result.x, 120.0f, 1e-5f); // SCREEN_WIDTH / 2
    ASSERT_FLOAT_EQ(result.y, 40.0f, 1e-5f);  // SCREEN_HEIGHT / 2
}

void testProjectionDepthPreservation() {
    Vec3 v(0.0f, 0.0f, 10.0f);
    Vec3 result = project(v);
    
    // Z-depth should be preserved
    ASSERT_FLOAT_EQ(result.z, 10.0f, 1e-5f);
}

void testProjectionPositiveZ() {
    Vec3 v(0.0f, 0.0f, 50.0f); // At fov distance
    Vec3 result = project(v);
    
    // Should project to center
    ASSERT_FLOAT_EQ(result.x, 120.0f, 1e-1f);
    ASSERT_FLOAT_EQ(result.y, 40.0f, 1e-1f);
}

void testProjectionCustomParams() {
    ProjectionParams params;
    params.fov = 100.0f;
    params.screenWidth = 100.0f;
    params.screenHeight = 100.0f;
    
    Vec3 v(0.0f, 0.0f, 0.0f);
    Vec3 result = project(v, params);
    
    // Should project to center of custom screen size
    ASSERT_FLOAT_EQ(result.x, 50.0f, 1e-5f);
    ASSERT_FLOAT_EQ(result.y, 50.0f, 1e-5f);
}

void testProjectionYInversion() {
    Vec3 v(0.0f, 10.0f, 0.0f); // Positive Y in 3D
    Vec3 result = project(v);
    
    // Y should be inverted (positive Y goes down on screen)
    ASSERT_TRUE(result.y < 40.0f); // Should be below center
}

void testProjectionNearZeroZ() {
    Vec3 v(0.0f, 0.0f, -49.0f); // Very close to -fov
    Vec3 result = project(v);
    
    // Should handle near-zero z gracefully (minimum 0.1f)
    ASSERT_TRUE(result.z >= -49.0f);
}

int main() {
    std::cout << "Running projection tests..." << std::endl;
    RUN_TEST(testProjectionDefaultParams);
    RUN_TEST(testProjectionOrigin);
    RUN_TEST(testProjectionDepthPreservation);
    RUN_TEST(testProjectionPositiveZ);
    RUN_TEST(testProjectionCustomParams);
    RUN_TEST(testProjectionYInversion);
    RUN_TEST(testProjectionNearZeroZ);

    TestFramework::instance().printSummary();
    return TestFramework::instance().getExitCode();
}

