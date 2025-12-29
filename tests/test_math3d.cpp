#include "test_framework.h"
#include "math3d.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Vec3 Tests
void testVec3DefaultConstructor() {
    Vec3 v;
    ASSERT_FLOAT_EQ(v.x, 0.0f, 1e-5f);
    ASSERT_FLOAT_EQ(v.y, 0.0f, 1e-5f);
    ASSERT_FLOAT_EQ(v.z, 0.0f, 1e-5f);
}

void testVec3ParameterizedConstructor() {
    Vec3 v(1.0f, 2.0f, 3.0f);
    ASSERT_FLOAT_EQ(v.x, 1.0f, 1e-5f);
    ASSERT_FLOAT_EQ(v.y, 2.0f, 1e-5f);
    ASSERT_FLOAT_EQ(v.z, 3.0f, 1e-5f);
}

void testVec3Addition() {
    Vec3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(4.0f, 5.0f, 6.0f);
    Vec3 result = a + b;
    ASSERT_VEC3_EQ(result, Vec3(5.0f, 7.0f, 9.0f), 1e-5f);
}

void testVec3Subtraction() {
    Vec3 a(5.0f, 7.0f, 9.0f);
    Vec3 b(1.0f, 2.0f, 3.0f);
    Vec3 result = a - b;
    ASSERT_VEC3_EQ(result, Vec3(4.0f, 5.0f, 6.0f), 1e-5f);
}

void testVec3ScalarMultiplication() {
    Vec3 v(1.0f, 2.0f, 3.0f);
    Vec3 result = v * 2.0f;
    ASSERT_VEC3_EQ(result, Vec3(2.0f, 4.0f, 6.0f), 1e-5f);
}

void testVec3ScalarDivision() {
    Vec3 v(2.0f, 4.0f, 6.0f);
    Vec3 result = v / 2.0f;
    ASSERT_VEC3_EQ(result, Vec3(1.0f, 2.0f, 3.0f), 1e-5f);
}

void testVec3DotProduct() {
    Vec3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(4.0f, 5.0f, 6.0f);
    float result = a.dot(b);
    ASSERT_FLOAT_EQ(result, 32.0f, 1e-5f); // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
}

void testVec3CrossProduct() {
    Vec3 a(1.0f, 0.0f, 0.0f);
    Vec3 b(0.0f, 1.0f, 0.0f);
    Vec3 result = a.cross(b);
    ASSERT_VEC3_EQ(result, Vec3(0.0f, 0.0f, 1.0f), 1e-5f);
}

void testVec3Length() {
    Vec3 v(3.0f, 4.0f, 0.0f);
    float len = v.length();
    ASSERT_FLOAT_EQ(len, 5.0f, 1e-5f); // 3-4-5 triangle
}

void testVec3LengthSquared() {
    Vec3 v(3.0f, 4.0f, 0.0f);
    float lenSq = v.lengthSquared();
    ASSERT_FLOAT_EQ(lenSq, 25.0f, 1e-5f);
}

void testVec3Normalize() {
    Vec3 v(3.0f, 4.0f, 0.0f);
    Vec3 normalized = v.normalize();
    float len = normalized.length();
    ASSERT_FLOAT_EQ(len, 1.0f, 1e-5f);
    ASSERT_VEC3_EQ(normalized, Vec3(0.6f, 0.8f, 0.0f), 1e-5f);
}

void testVec3NormalizeZero() {
    Vec3 v(0.0f, 0.0f, 0.0f);
    Vec3 normalized = v.normalize();
    ASSERT_VEC3_EQ(normalized, Vec3(0.0f, 0.0f, 0.0f), 1e-5f);
}

void testVec3IsApproxEqual() {
    Vec3 a(1.0f, 2.0f, 3.0f);
    Vec3 b(1.00001f, 2.00001f, 3.00001f);
    ASSERT_TRUE(a.isApproxEqual(b, 1e-4f));
    ASSERT_FALSE(a.isApproxEqual(b, 1e-6f));
}

// Mat3 Tests
void testMat3DefaultConstructor() {
    Mat3 m;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float expected = (i == j) ? 1.0f : 0.0f;
            ASSERT_FLOAT_EQ(m.m[i][j], expected, 1e-5f);
        }
    }
}

void testMat3VectorMultiplication() {
    Mat3 m;
    // Identity matrix should not change vector
    Vec3 v(1.0f, 2.0f, 3.0f);
    Vec3 result = m * v;
    ASSERT_VEC3_EQ(result, v, 1e-5f);
}

void testMat3MatrixMultiplication() {
    Mat3 a; // Identity
    Mat3 b; // Identity
    Mat3 result = a * b;
    ASSERT_TRUE(result.isApproxEqual(Mat3(), 1e-5f));
}

void testMat3IsApproxEqual() {
    Mat3 a;
    Mat3 b;
    ASSERT_TRUE(a.isApproxEqual(b, 1e-5f));
    
    b.m[0][0] = 1.00001f;
    ASSERT_TRUE(a.isApproxEqual(b, 1e-4f));
    ASSERT_FALSE(a.isApproxEqual(b, 1e-6f));
}

// Rotation Matrix Tests
void testRotationX() {
    Mat3 rot = rotationX(M_PI / 2.0f); // 90 degrees
    Vec3 v(0.0f, 1.0f, 0.0f);
    Vec3 result = rot * v;
    // Rotating (0,1,0) around X by 90° should give (0,0,1)
    ASSERT_VEC3_EQ(result, Vec3(0.0f, 0.0f, 1.0f), 1e-5f);
}

void testRotationY() {
    Mat3 rot = rotationY(M_PI / 2.0f); // 90 degrees
    Vec3 v(1.0f, 0.0f, 0.0f);
    Vec3 result = rot * v;
    // Rotating (1,0,0) around Y by 90° should give (0,0,-1)
    ASSERT_VEC3_EQ(result, Vec3(0.0f, 0.0f, -1.0f), 1e-5f);
}

void testRotationZ() {
    Mat3 rot = rotationZ(M_PI / 2.0f); // 90 degrees
    Vec3 v(1.0f, 0.0f, 0.0f);
    Vec3 result = rot * v;
    // Rotating (1,0,0) around Z by 90° should give (0,1,0)
    ASSERT_VEC3_EQ(result, Vec3(0.0f, 1.0f, 0.0f), 1e-5f);
}

void testRotationZeroAngle() {
    Mat3 rotX = rotationX(0.0f);
    Mat3 rotY = rotationY(0.0f);
    Mat3 rotZ = rotationZ(0.0f);
    Mat3 identity;
    
    ASSERT_TRUE(rotX.isApproxEqual(identity, 1e-5f));
    ASSERT_TRUE(rotY.isApproxEqual(identity, 1e-5f));
    ASSERT_TRUE(rotZ.isApproxEqual(identity, 1e-5f));
}

void testRotationComposition() {
    Mat3 rotX = rotationX(M_PI / 2.0f);
    Mat3 rotY = rotationY(M_PI / 2.0f);
    Mat3 composed = rotX * rotY;
    
    // Composition should produce a valid rotation matrix
    Vec3 v(1.0f, 0.0f, 0.0f);
    Vec3 result = composed * v;
    
    // Result should have unit length (rotation preserves length)
    ASSERT_FLOAT_EQ(result.length(), 1.0f, 1e-5f);
}

int main() {
    std::cout << "Running Vec3 tests..." << std::endl;
    RUN_TEST(testVec3DefaultConstructor);
    RUN_TEST(testVec3ParameterizedConstructor);
    RUN_TEST(testVec3Addition);
    RUN_TEST(testVec3Subtraction);
    RUN_TEST(testVec3ScalarMultiplication);
    RUN_TEST(testVec3ScalarDivision);
    RUN_TEST(testVec3DotProduct);
    RUN_TEST(testVec3CrossProduct);
    RUN_TEST(testVec3Length);
    RUN_TEST(testVec3LengthSquared);
    RUN_TEST(testVec3Normalize);
    RUN_TEST(testVec3NormalizeZero);
    RUN_TEST(testVec3IsApproxEqual);

    std::cout << "\nRunning Mat3 tests..." << std::endl;
    RUN_TEST(testMat3DefaultConstructor);
    RUN_TEST(testMat3VectorMultiplication);
    RUN_TEST(testMat3MatrixMultiplication);
    RUN_TEST(testMat3IsApproxEqual);

    std::cout << "\nRunning rotation matrix tests..." << std::endl;
    RUN_TEST(testRotationX);
    RUN_TEST(testRotationY);
    RUN_TEST(testRotationZ);
    RUN_TEST(testRotationZeroAngle);
    RUN_TEST(testRotationComposition);

    TestFramework::instance().printSummary();
    return TestFramework::instance().getExitCode();
}

