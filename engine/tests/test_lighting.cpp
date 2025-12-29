#include "test_framework.h"
#include "lighting.h"
#include "math3d.h"
#include <cmath>

void testLightingDirectLight() {
    Vec3 normal(0.0f, 0.0f, 1.0f); // Face pointing towards +Z
    Vec3 lightDir(0.0f, 0.0f, 1.0f); // Light coming from +Z (same direction)
    
    float intensity = calculateLighting(normal, lightDir);
    
    // Should be maximum intensity (1.0) when light hits directly
    ASSERT_FLOAT_EQ(intensity, 1.0f, 1e-5f);
}

void testLightingPerpendicularLight() {
    Vec3 normal(0.0f, 0.0f, 1.0f); // Face pointing towards +Z
    Vec3 lightDir(1.0f, 0.0f, 0.0f); // Light coming from side (perpendicular)
    
    float intensity = calculateLighting(normal, lightDir);
    
    // Should be ambient only when perpendicular (0.2)
    ASSERT_FLOAT_EQ(intensity, 0.2f, 1e-5f);
}

void testLightingOppositeLight() {
    Vec3 normal(0.0f, 0.0f, 1.0f); // Face pointing towards +Z
    Vec3 lightDir(0.0f, 0.0f, -1.0f); // Light coming from opposite direction
    
    float intensity = calculateLighting(normal, lightDir);
    
    // Should be ambient only when light is behind (0.2)
    ASSERT_FLOAT_EQ(intensity, 0.2f, 1e-5f);
}

void testLightingCustomAmbient() {
    Vec3 normal(0.0f, 0.0f, 1.0f);
    Vec3 lightDir(0.0f, 0.0f, -1.0f);
    
    float intensity = calculateLighting(normal, lightDir, 0.5f, 0.8f);
    
    // Should be custom ambient (0.5) when light is behind
    ASSERT_FLOAT_EQ(intensity, 0.5f, 1e-5f);
}

void testLightingAngledLight() {
    Vec3 normal(0.0f, 0.0f, 1.0f); // Face pointing towards +Z
    Vec3 lightDir(0.0f, 1.0f, 1.0f); // Light at 45 degrees
    lightDir = lightDir.normalize();
    
    float intensity = calculateLighting(normal, lightDir);
    
    // Should be between ambient and max
    ASSERT_TRUE(intensity > 0.2f);
    ASSERT_TRUE(intensity < 1.0f);
    
    // At 45°, dot product should be cos(45°) ≈ 0.707
    float expected = 0.707f * 0.8f + 0.2f;
    ASSERT_FLOAT_EQ(intensity, expected, 0.1f);
}

void testLightingUnnormalizedInputs() {
    Vec3 normal(0.0f, 0.0f, 2.0f); // Not normalized
    Vec3 lightDir(0.0f, 0.0f, 3.0f); // Not normalized
    
    // Function should still work, though results may vary
    float intensity = calculateLighting(normal, lightDir);
    
    // Should return a valid intensity value (may be > 1.0 if inputs not normalized)
    ASSERT_TRUE(intensity >= 0.0f);
    // Note: With unnormalized inputs, intensity can exceed 1.0
    // This is acceptable behavior - users should normalize inputs
}

int main() {
    std::cout << "Running lighting tests..." << std::endl;
    RUN_TEST(testLightingDirectLight);
    RUN_TEST(testLightingPerpendicularLight);
    RUN_TEST(testLightingOppositeLight);
    RUN_TEST(testLightingCustomAmbient);
    RUN_TEST(testLightingAngledLight);
    RUN_TEST(testLightingUnnormalizedInputs);

    TestFramework::instance().printSummary();
    return TestFramework::instance().getExitCode();
}

