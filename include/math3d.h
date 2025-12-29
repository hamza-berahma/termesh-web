/**
 * @file math3d.h
 * @brief A simple 3D math library for vector and matrix operations.
 *
 * This file defines a 3D vector (`Vec3`) and a 3x3 matrix (`Mat3`) class,
 * along with common mathematical operations for 3D graphics applications.
 * These include vector arithmetic, dot and cross products, normalization,
 * matrix-vector multiplication, matrix-matrix multiplication, and functions
 * to generate rotation matrices.
 */

#pragma once
#include <cmath>

/**
 * @struct Vec3
 * @brief Represents a 3D vector with x, y, and z components.
 *
 * Provides basic vector operations such as addition, subtraction,
 * scalar multiplication, and division. It also includes methods for
 * calculating the dot product, cross product, magnitude (length),
 * and for normalizing the vector.
 */
struct Vec3 {
    float x, y, z;

    /**
     * @brief Default constructor. Initializes the vector to (0, 0, 0).
     */
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    /**
     * @brief Adds two vectors.
     * @param v The vector to add.
     * @return The sum of the two vectors.
     */
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }

    /**
     * @brief Subtracts one vector from another.
     * @param v The vector to subtract.
     * @return The difference of the two vectors.
     */
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }

    /**
     * @brief Multiplies the vector by a scalar.
     * @param s The scalar value.
     * @return The scaled vector.
     */
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }

    /**
     * @brief Divides the vector by a scalar.
     * @param s The scalar value.
     * @return The scaled vector.
     */
    Vec3 operator/(float s) const { return Vec3(x / s, y / s, z / s); }

    /**
     * @brief Computes the dot product with another vector.
     * @param v The other vector.
     * @return The dot product.
     */
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }

    /**
     * @brief Computes the cross product with another vector.
     * @param v The other vector.
     * @return The cross product.
     */
    Vec3 cross(const Vec3& v) const {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    /**
     * @brief Computes the magnitude (length) of the vector.
     * @return The length of the vector.
     */
    float length() const { return std::sqrt(x * x + y * y + z * z); }

    /**
     * @brief Computes the squared magnitude (length^2) of the vector.
     * Useful for comparisons without needing sqrt.
     * @return The squared length of the vector.
     */
    float lengthSquared() const { return x * x + y * y + z * z; }

    /**
     * @brief Normalizes the vector.
     * @return A new vector with unit length. If the original vector has zero length,
     *         it returns a zero vector.
     */
    Vec3 normalize() const {
        float len = length();
        return len > 0 ? (*this) / len : Vec3(0, 0, 0);
    }

    /**
     * @brief Checks if two vectors are approximately equal within a tolerance.
     * @param v The other vector.
     * @param epsilon The tolerance for comparison.
     * @return True if vectors are approximately equal.
     */
    bool isApproxEqual(const Vec3& v, float epsilon = 1e-5f) const {
        return std::abs(x - v.x) < epsilon && 
               std::abs(y - v.y) < epsilon && 
               std::abs(z - v.z) < epsilon;
    }
};

/**
 * @struct Mat3
 * @brief Represents a 3x3 matrix, typically used for rotations.
 *
 * Provides functionality for matrix-vector multiplication and matrix-matrix
 * multiplication. The default constructor initializes it as an identity matrix.
 */
struct Mat3 {
    float m[3][3];

    /**
     * @brief Default constructor. Initializes the matrix to the identity matrix.
     */
    Mat3() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }

    /**
     * @brief Multiplies the matrix by a 3D vector.
     * @param v The vector to multiply.
     * @return The transformed vector.
     */
    Vec3 operator*(const Vec3& v) const {
        return Vec3(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
        );
    }

    /**
     * @brief Multiplies this matrix by another 3x3 matrix.
     * @param other The matrix to multiply by.
     * @return The resulting matrix.
     */
    Mat3 operator*(const Mat3& other) const {
        Mat3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = 0;
                for (int k = 0; k < 3; k++)
                    result.m[i][j] += m[i][k] * other.m[k][j];
            }
        return result;
    }

    /**
     * @brief Checks if two matrices are approximately equal within a tolerance.
     * @param other The other matrix.
     * @param epsilon The tolerance for comparison.
     * @return True if matrices are approximately equal.
     */
    bool isApproxEqual(const Mat3& other, float epsilon = 1e-5f) const {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (std::abs(m[i][j] - other.m[i][j]) >= epsilon)
                    return false;
        return true;
    }
};

/**
 * @brief Creates a rotation matrix for a rotation around the X-axis.
 * @param angle The angle of rotation in radians.
 * @return A 3x3 rotation matrix.
 */
inline Mat3 rotationX(float angle) {
    Mat3 mat;
    float c = std::cos(angle), s = std::sin(angle);
    mat.m[1][1] = c; mat.m[1][2] = -s;
    mat.m[2][1] = s; mat.m[2][2] = c;
    return mat;
}

/**
 * @brief Creates a rotation matrix for a rotation around the Y-axis.
 * @param angle The angle of rotation in radians.
 * @return A 3x3 rotation matrix.
 */
inline Mat3 rotationY(float angle) {
    Mat3 mat;
    float c = std::cos(angle), s = std::sin(angle);
    mat.m[0][0] = c; mat.m[0][2] = s;
    mat.m[2][0] = -s; mat.m[2][2] = c;
    return mat;
}

/**
 * @brief Creates a rotation matrix for a rotation around the Z-axis.
 * @param angle The angle of rotation in radians.
 * @return A 3x3 rotation matrix.
 */
inline Mat3 rotationZ(float angle) {
    Mat3 mat;
    float c = std::cos(angle), s = std::sin(angle);
    mat.m[0][0] = c; mat.m[0][1] = -s;
    mat.m[1][0] = s; mat.m[1][1] = c;
    return mat;
}

