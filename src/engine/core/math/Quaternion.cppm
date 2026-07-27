export module EE.Math.Quaternion;

import EE.Math.Vector;

#ifdef _WIN32
import <cmath>;
import <algorithm>;
#else
#include <cmath>
#endif

export namespace EE {
    struct Quaternion {
        static const Quaternion Identity;
        static const Quaternion Zero;

        float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;

        Quaternion() = default;
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        static Quaternion fromAxisAngle(const Vector3& axis, float angle) {
            float halfAngle = angle * 0.5f;
            float s = std::sin(halfAngle);
            Vector3 norm = axis.normalized();
            return {norm.x * s, norm.y * s, norm.z * s, std::cos(halfAngle)};
        }

        static Quaternion fromEuler(float pitch, float yaw, float roll) {
            float halfPitch = pitch * 0.5f;
            float halfYaw = yaw * 0.5f;
            float halfRoll = roll * 0.5f;

            float sp = std::sin(halfPitch);
            float cp = std::cos(halfPitch);
            float sy = std::sin(halfYaw);
            float cy = std::cos(halfYaw);
            float sr = std::sin(halfRoll);
            float cr = std::cos(halfRoll);

            Quaternion q;
            q.x = cy * sp * cr + sy * cp * sr;
            q.y = sy * cp * cr - cy * sp * sr;
            q.z = cy * cp * sr - sy * sp * cr;
            q.w = cy * cp * cr + sy * sp * sr;
            return q;
        }

        static Quaternion fromEuler(const Vector3& euler) {
            return fromEuler(euler.x, euler.y, euler.z);
        }

        Quaternion operator*(const Quaternion& other) const {
            return {
                w * other.x + x * other.w + y * other.z - z * other.y,
                w * other.y - x * other.z + y * other.w + z * other.x,
                w * other.z + x * other.y - y * other.x + z * other.w,
                w * other.w - x * other.x - y * other.y - z * other.z
            };
        }

        Quaternion& operator*=(const Quaternion& other) {
            *this = *this * other;
            return *this;
        }

        Vector3 operator*(const Vector3& v) const {
            Vector3 qv(x, y, z);
            Vector3 uv = qv.cross(v);
            Vector3 uuv = qv.cross(uv);
            uv = uv * (2.0f * w);
            uuv = uuv * 2.0f;
            return v + uv + uuv;
        }

        float lengthSquared() const {
            return x*x + y*y + z*z + w*w;
        }

        float length() const {
            return std::sqrt(lengthSquared());
        }

        Quaternion normalized() const {
            float l = length();
            return l > 0 ? Quaternion(x/l, y/l, z/l, w/l) : Quaternion();
        }

        void normalize() {
            *this = normalized();
        }

        Quaternion conjugate() const {
            return {-x, -y, -z, w};
        }

        Quaternion inverse() const {
            float l2 = lengthSquared();
            return l2 > 0 ? conjugate() * (1.0f / l2) : Quaternion();
        }

        Quaternion operator*(float scalar) const {
            return {x * scalar, y * scalar, z * scalar, w * scalar};
        }

        friend Quaternion operator*(float scalar, const Quaternion& q) {
            return q * scalar;
        }
    };
}
