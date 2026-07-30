module;

#include <cmath>
#include "../ReflectionMacro.hpp"

#ifdef _WIN32
#include <algorithm>
#endif

export module EE.Math.Vector;

import EE.Core.Reflection;

export namespace EE {
    struct Vector3 {
        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 Up;
        static const Vector3 Right;
        static const Vector3 Forward;

        float x = 0.0f, y = 0.0f, z = 0.0f;

        Vector3() = default;
        explicit Vector3(const float v) : Vector3(v, v, v) {}
        Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3& other) const { return {x+other.x, y+other.y, z+other.z}; }
        Vector3 operator-(const Vector3& other) const { return {x-other.x, y-other.y, z-other.z}; }
        Vector3 operator*(const Vector3& other) const { return {x*other.x, y*other.y, z*other.z}; }
        Vector3 operator/(const Vector3& other) const { return {x/other.x, y/other.y, z/other.z}; }

        bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
        bool operator!=(const Vector3& other) const { return !(*this == other); }

        Vector3& operator+=(const Vector3& other) { x+=other.x; y+=other.y; z+=other.z; return *this; }
        Vector3& operator*=(const float scalar) { x*=scalar; y*=scalar; z*=scalar; return *this; }
        Vector3& operator/=(const float scalar) { x/=scalar; y/=scalar; z/=scalar; return *this; }

        Vector3 operator*(const float scalar) const { return {x*scalar, y*scalar, z*scalar}; }
        friend Vector3 operator*(const float scalar, const Vector3& v) { return v * scalar; }
        Vector3 operator/(const float scalar) const { return {x/scalar, y/scalar, z/scalar}; }
        friend Vector3 operator/(const float scalar, const Vector3& v) { return v / scalar; }
        Vector3 operator-() const { return {-x, -y, -z}; }

        float& operator[](const size_t i) { return (&x)[i]; }
        const float& operator[](const size_t i) const { return (&x)[i]; }

        [[nodiscard]] Vector3 updateX(const float newX) const { return {newX, y, z}; }
        [[nodiscard]] Vector3 updateY(const float newY) const { return {x, newY, z}; }
        [[nodiscard]] Vector3 updateZ(const float newZ) const { return {x, y, newZ}; }
        [[nodiscard]] Vector3 addToX(const float value) const { return {x + value, y, z}; }
        [[nodiscard]] Vector3 addToY(const float value) const { return {x, y + value, z}; }
        [[nodiscard]] Vector3 addToZ(const float value) const { return {x, y, z + value}; }
        [[nodiscard]] Vector3 minusFromX(const float value) const { return {x - value, y, z}; }
        [[nodiscard]] Vector3 minusFromY(const float value) const { return {x, y - value, z}; }
        [[nodiscard]] Vector3 minusFromZ(const float value) const { return {x, y, z - value}; }
        [[nodiscard]] Vector3 multiplyX(const float value) const { return {x * value, y, z}; }
        [[nodiscard]] Vector3 multiplyY(const float value) const { return {x, y * value, z}; }
        [[nodiscard]] Vector3 multiplyZ(const float value) const { return {x, y, z * value}; }
        [[nodiscard]] Vector3 divideX(const float value) const { return {x / value, y, z}; }
        [[nodiscard]] Vector3 divideY(const float value) const { return {x, y / value, z}; }
        [[nodiscard]] Vector3 divideZ(const float value) const { return {x, y, z / value}; }


        [[nodiscard]] float lengthSquared() const { return x*x + y*y + z*z; }
        [[nodiscard]]float distanceSquared(const Vector3& other) const { return (*this - other).lengthSquared(); }
        [[nodiscard]] float length() const { return std::sqrt(lengthSquared()); }
        [[nodiscard]] float dot(const Vector3& other) const { return x*other.x + y*other.y + z*other.z; }
        [[nodiscard]] float distance(const Vector3& other) const { return (*this - other).length(); }
        [[nodiscard]] float angle(const Vector3& other) const { return std::acos(dot(other) / (length() * other.length())); }

        [[nodiscard]] Vector3 normalized() const { const float l = length(); return l > 0 ? *this * (1.0f/l) : Vector3(0); }
        [[nodiscard]] Vector3 lerp(const Vector3& other, float t) const { return *this * (1.0f-t) + other * t; }
        [[nodiscard]] Vector3 cross(const Vector3& other) const { return {
            (y * other.z) - (z * other.y),
            (z * other.x) - (x * other.z),
            (x * other.y) - (y * other.x)
        }; }
        [[nodiscard]] Vector3 projectOn(const Vector3& other) const { return other * (dot(other) / other.dot(other)); }
        [[nodiscard]] Vector3 reflect(const Vector3& normal) const { return *this - normal * (2.0f * dot(normal)); }
        [[nodiscard]] Vector3 min(const Vector3& other) const { return {
            std::min(x, other.x),
            std::min(y, other.y),
            std::min(z, other.z)
        }; }
        [[nodiscard]] Vector3 max(const Vector3& other) const { return {
            std::max(x, other.x),
            std::max(y, other.y),
            std::max(z, other.z)
        }; }
        [[nodiscard]] Vector3 abs() const { return {std::abs(x), std::abs(y), std::abs(z)}; }
    };

    struct Vector2 {
        float x = 0.0f, y = 0.0f;

        Vector2() = default;
        explicit Vector2(const float v) : Vector2(v, v) {}
        Vector2(const float x, const float y) : x(x), y(y) {}

        Vector2 operator+(const Vector2& other) const { return {x+other.x, y+other.y}; }
        Vector2 operator-(const Vector2& other) const { return {x-other.x, y-other.y}; }
        Vector2 operator*(const Vector2& other) const { return {x*other.x, y*other.y}; }
        Vector2 operator/(const Vector2& other) const { return {x/other.x, y/other.y}; }

        bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
        bool operator!=(const Vector2& other) const { return !(*this == other); }

        Vector2& operator+=(const Vector2& other) { x+=other.x; y+=other.y; return *this; }

        Vector2& operator*=(const float scalar) { x*=scalar; y*=scalar; return *this; }
        Vector2& operator/=(const float scalar) { x/=scalar; y/=scalar; return *this; }

        Vector2 operator*(const float scalar) const { return {x*scalar, y*scalar}; }
        friend Vector2 operator*(const float scalar, const Vector2& v) {
            return v * scalar;
        }
        Vector2 operator/(const float scalar) const { return {x/scalar, y/scalar}; }
        friend Vector2 operator/(const float scalar, const Vector2& v) {
            return v / scalar;
        }

        Vector2 operator-() const { return {-x, -y}; }

        [[nodiscard]] float lengthSquared() const { return x*x + y*y; }
        [[nodiscard]] float distanceSquared(const Vector2& other) const { return (*this - other).lengthSquared(); }
        [[nodiscard]] float length() const { return std::sqrt(lengthSquared()); }
        [[nodiscard]] float dot(const Vector2& other) const { return x*other.x + y*other.y; }
        [[nodiscard]] float distance(const Vector2& other) const { return (*this - other).length(); }
        [[nodiscard]] float angle(const Vector2& other) const { return std::acos(dot(other) / (length() * other.length())); }

        [[nodiscard]] Vector2 normalized() const { const float l = length(); return l > 0 ? *this * (1.0f/l) : Vector2(0); }
        [[nodiscard]] Vector2 lerp(const Vector2& other, const float t) const { return *this * (1.0f-t) + other * t; }

        [[nodiscard]] float cross(const Vector2& other) const {
            return x * other.y - y * other.x;
        }
        [[nodiscard]] Vector2 projectOn(const Vector2& other) const { return other * (dot(other) / other.dot(other)); }
        [[nodiscard]]Vector2 reflect(const Vector2& normal) const { return *this - normal * (2.0f * dot(normal)); }
        [[nodiscard]] Vector2 min(const Vector2& other) const { return {
            std::min(x, other.x),
            std::min(y, other.y),
        }; }
        [[nodiscard]] Vector2 max(const Vector2& other) const { return {
            std::max(x, other.x),
            std::max(y, other.y),
        }; }
        [[nodiscard]] Vector2 abs() const { return {std::abs(x), std::abs(y)}; }
    };

    struct Vector4 {
        float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

        Vector4() = default;
        explicit Vector4(const float v) : Vector4(v, v, v, v) {}
        Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

        Vector4 operator+(const Vector4& other) const { return {x+other.x, y+other.y, z+other.z, w+other.w}; }
        Vector4 operator-(const Vector4& other) const { return {x-other.x, y-other.y, z-other.z, w-other.w}; }
        Vector4 operator*(const Vector4& other) const { return {x*other.x, y*other.y, z*other.z, w*other.w}; }
        Vector4 operator/(const Vector4& other) const { return {x/other.x, y/other.y, z/other.z, w/other.w}; }

        bool operator==(const Vector4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        bool operator!=(const Vector4& other) const { return !(*this == other); }

        Vector4 operator-() const { return {-x, -y, -z, -w}; }

        Vector4 operator*(const float scalar) const { return {x*scalar, y*scalar, z*scalar, w*scalar}; }
        friend Vector4 operator*(const float scalar, const Vector4& v) { return v * scalar; }
        Vector4 operator/(const float scalar) const { return {x/scalar, y/scalar, z/scalar, w/scalar}; }

        [[nodiscard]] float lengthSquared() const { return x*x + y*y + z*z + w*w; }
        [[nodiscard]] float length() const { return std::sqrt(lengthSquared()); }
        [[nodiscard]] float dot(const Vector4& other) const { return x*other.x + y*other.y + z*other.z + w*other.w; }

        [[nodiscard]] Vector4 normalized() const { float l = length(); return l > 0 ? *this * (1.0f/l) : Vector4(0); }

        [[nodiscard]] Vector3 xyz() const { return {x, y, z}; }

        [[nodiscard]] Vector3 position() const { return {x / w, y / w, z / w}; }
    };
}

REFLECT_COMPONENT(EE::Vector3, {
    t.fields = {
        {"x", offsetof(EE::Vector3, x), nullptr},
        {"y", offsetof(EE::Vector3, y), nullptr},
        {"z", offsetof(EE::Vector3, z), nullptr}
    };
})
