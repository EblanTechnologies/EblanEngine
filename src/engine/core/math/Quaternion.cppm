module;

#include <cmath>
#include "../ReflectionMacro.hpp"

#ifdef _WIN32
#include <algorithm>
#endif

export module EE.Math.Quaternion;

import EE.Math.Vector;
import EE.Math.Angle;
import EE.Core.Reflection;

export namespace EE {
    struct Quaternion {
        static const Quaternion Identity;
        static const Quaternion Zero;

        float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;

        Quaternion() = default;
        Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

        static Quaternion fromAxisAngle(const Vector3& axis, const float angle) {
            const float halfAngle = angle * 0.5f;
            const float s = std::sin(halfAngle);
            const Vector3 norm = axis.normalized();
            return {norm.x * s, norm.y * s, norm.z * s, std::cos(halfAngle)};
        }

        static Quaternion fromEuler(const Angle pitch, const Angle yaw, const Angle roll) {
            const float halfPitch = pitch.radians() * 0.5f;
            const float halfYaw = yaw.radians() * 0.5f;
            const float halfRoll = roll.radians() * 0.5f;

            const float sp = std::sin(halfPitch);
            const float cp = std::cos(halfPitch);
            const float sy = std::sin(halfYaw);
            const float cy = std::cos(halfYaw);
            const float sr = std::sin(halfRoll);
            const float cr = std::cos(halfRoll);

            Quaternion q;
            q.x = cy * sp * cr + sy * cp * sr;
            q.y = sy * cp * cr - cy * sp * sr;
            q.z = cy * cp * sr - sy * sp * cr;
            q.w = cy * cp * cr + sy * sp * sr;
            return q;
        }

        static Quaternion fromEulerRadians(const Vector3& euler) {
            return fromEuler(
                Angle::fromRadians(euler.x),
                Angle::fromRadians(euler.y),
                Angle::fromRadians(euler.z)
            );
        }

        static Quaternion fromEulerDegrees(const Vector3& euler) {
            return fromEuler(
                Angle::fromDegrees(euler.x),
                Angle::fromDegrees(euler.y),
                Angle::fromDegrees(euler.z)
            );
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
            const Vector3 qv(x, y, z);
            Vector3 uv = qv.cross(v);
            Vector3 uuv = qv.cross(uv);
            uv = uv * (2.0f * w);
            uuv = uuv * 2.0f;
            return v + uv + uuv;
        }

        [[nodiscard]] float lengthSquared() const {
            return x*x + y*y + z*z + w*w;
        }

        [[nodiscard]] float length() const {
            return std::sqrt(lengthSquared());
        }

        [[nodiscard]] Quaternion normalized() const {
            const float l = length();
            return l > 0 ? Quaternion(x/l, y/l, z/l, w/l) : Quaternion();
        }

        void normalize() {
            *this = normalized();
        }

        [[nodiscard]] Quaternion conjugate() const {
            return {-x, -y, -z, w};
        }

        [[nodiscard]] Quaternion inverse() const {
            const float l2 = lengthSquared();
            return l2 > 0 ? conjugate() * (1.0f / l2) : Quaternion();
        }

        [[nodiscard]] Quaternion operator*(const float scalar) const {
            return {x * scalar, y * scalar, z * scalar, w * scalar};
        }

        friend Quaternion operator*(const float scalar, const Quaternion& q) {
            return q * scalar;
        }
    };
}

REFLECT_COMPONENT(EE::Quaternion, {
    t.fields = {
        {"x", offsetof(EE::Quaternion, x), nullptr},
        {"y", offsetof(EE::Quaternion, y), nullptr},
        {"z", offsetof(EE::Quaternion, z), nullptr},
        {"w", offsetof(EE::Quaternion, w), nullptr}
    };
})
