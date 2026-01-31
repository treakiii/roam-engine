// Quaternion.h - The rotation specialist
// This handles smooth rotations and orientations

#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector3.h"
#include "Matrix4x4.h"
#include <iostream>
#include <cmath>

// The Quaternion class - our rotation guru
class Quaternion {
public:
    // Components
    float x, y, z, w;

    // Constructors - create the quaternion
    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Quaternion(const Vector3& axis, float angle) {
        float halfAngle = angle * 0.5f;
        float sinHalf = std::sin(halfAngle);
        x = axis.x * sinHalf;
        y = axis.y * sinHalf;
        z = axis.z * sinHalf;
        w = std::cos(halfAngle);
        Normalize();
    }

    // Copy constructor
    Quaternion(const Quaternion& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

    // Assignment operator
    Quaternion& operator=(const Quaternion& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
        }
        return *this;
    }

    // Arithmetic operators
    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    Quaternion operator*(float scalar) const {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    // Compound assignment
    Quaternion& operator*=(const Quaternion& other) {
        *this = *this * other;
        return *this;
    }

    // Conjugate - flip the imaginary parts
    Quaternion Conjugated() const {
        return Quaternion(-x, -y, -z, w);
    }

    // Inverse - the opposite rotation
    Quaternion Inverted() const {
        float normSq = x*x + y*y + z*z + w*w;
        if (normSq > 0) {
            float invNormSq = 1.0f / normSq;
            return Quaternion(-x * invNormSq, -y * invNormSq, -z * invNormSq, w * invNormSq);
        }
        return Quaternion();
    }

    // Normalize - make it unit length
    void Normalize() {
        float length = std::sqrt(x*x + y*y + z*z + w*w);
        if (length > 0) {
            float invLength = 1.0f / length;
            x *= invLength;
            y *= invLength;
            z *= invLength;
            w *= invLength;
        }
    }

    Quaternion Normalized() const {
        Quaternion result = *this;
        result.Normalize();
        return result;
    }

    // Length and squared length
    float Length() const { return std::sqrt(x*x + y*y + z*z + w*w); }
    float LengthSquared() const { return x*x + y*y + z*z + w*w; }

    // Convert to matrix - for use in shaders
    Matrix4x4 ToMatrix() const {
        float xx = x * x, yy = y * y, zz = z * z;
        float xy = x * y, xz = x * z, yz = y * z;
        float wx = w * x, wy = w * y, wz = w * z;

        return Matrix4x4(
            1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
            2.0f * (xy + wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx), 0.0f,
            2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (xx + yy), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    // Convert to Euler angles - for human-readable rotation
    Vector3 ToEulerAngles() const {
        Vector3 angles;

        // Roll (x-axis rotation)
        float sinr_cosp = 2 * (w * x + y * z);
        float cosr_cosp = 1 - 2 * (x * x + y * y);
        angles.z = std::atan2(sinr_cosp, cosr_cosp);

        // Pitch (y-axis rotation)
        float sinp = 2 * (w * y - z * x);
        if (std::abs(sinp) >= 1)
            angles.x = std::copysign(M_PI / 2, sinp); // Use 90 degrees if out of range
        else
            angles.x = std::asin(sinp);

        // Yaw (z-axis rotation)
        float siny_cosp = 2 * (w * z + x * y);
        float cosy_cosp = 1 - 2 * (y * y + z * z);
        angles.y = std::atan2(siny_cosp, cosy_cosp);

        return angles;
    }

    // Static creation functions - build common rotations
    static Quaternion FromEulerAngles(const Vector3& eulerAngles) {
        float cr = std::cos(eulerAngles.z * 0.5f);
        float sr = std::sin(eulerAngles.z * 0.5f);
        float cp = std::cos(eulerAngles.x * 0.5f);
        float sp = std::sin(eulerAngles.x * 0.5f);
        float cy = std::cos(eulerAngles.y * 0.5f);
        float sy = std::sin(eulerAngles.y * 0.5f);

        return Quaternion(
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        );
    }

    static Quaternion FromAxisAngle(const Vector3& axis, float angle) {
        return Quaternion(axis, angle);
    }

    static Quaternion FromToRotation(const Vector3& from, const Vector3& to) {
        Vector3 axis = Vector3::Cross(from, to);
        float angle = std::acos(Vector3::Dot(from.Normalized(), to.Normalized()));
        return Quaternion(axis, angle);
    }

    // Spherical linear interpolation - smooth rotation transitions
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) {
        float cosHalfTheta = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

        if (std::abs(cosHalfTheta) >= 1.0f) {
            return a; // No rotation needed
        }

        float halfTheta = std::acos(cosHalfTheta);
        float sinHalfTheta = std::sqrt(1.0f - cosHalfTheta * cosHalfTheta);

        if (std::abs(sinHalfTheta) < 0.001f) {
            return Quaternion(
                a.x * 0.5f + b.x * 0.5f,
                a.y * 0.5f + b.y * 0.5f,
                a.z * 0.5f + b.z * 0.5f,
                a.w * 0.5f + b.w * 0.5f
            );
        }

        float ratioA = std::sin((1 - t) * halfTheta) / sinHalfTheta;
        float ratioB = std::sin(t * halfTheta) / sinHalfTheta;

        return Quaternion(
            a.x * ratioA + b.x * ratioB,
            a.y * ratioA + b.y * ratioB,
            a.z * ratioA + b.z * ratioB,
            a.w * ratioA + b.w * ratioB
        );
    }

    // Output operator - for debugging
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
        os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
        return os;
    }

    // Common quaternions - shortcuts
    static const Quaternion Identity;
};

// Static constants
const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

#endif // QUATERNION_H
