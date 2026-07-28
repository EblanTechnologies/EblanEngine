module;

#ifdef _WIN32
import <cmath>;
import <algorithm>;
#else
#include <cmath>
#endif

export module EE.Math.Vector;

export namespace EE {
    struct Vector3 {
        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 Up;
        static const Vector3 Right;
        static const Vector3 Forward;

        float x = 0.0f, y = 0.0f, z = 0.0f;

        Vector3() = default;
        Vector3(float v) : Vector3(v, v, v) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3& other) const { return {x+other.x, y+other.y, z+other.z}; }
        Vector3 operator-(const Vector3& other) const { return {x-other.x, y-other.y, z-other.z}; }
        Vector3 operator*(const Vector3& other) const { return {x*other.x, y*other.y, z*other.z}; }
        Vector3 operator/(const Vector3& other) const { return {x/other.x, y/other.y, z/other.z}; }

        bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
        bool operator!=(const Vector3& other) const { return !(*this == other); }

        Vector3& operator+=(const Vector3& other) { x+=other.x; y+=other.y; z+=other.z; return *this; }
        Vector3& operator*=(float scalar) { x*=scalar; y*=scalar; z*=scalar; return *this; }
        Vector3& operator/=(float scalar) { x/=scalar; y/=scalar; z/=scalar; return *this; }

        Vector3 operator*(float scalar) const { return {x*scalar, y*scalar, z*scalar}; }
        friend Vector3 operator*(float scalar, const Vector3& v) { return v * scalar; }
        Vector3 operator/(float scalar) const { return {x/scalar, y/scalar, z/scalar}; }
        friend Vector3 operator/(float scalar, const Vector3& v) { return v / scalar; }
        Vector3 operator-() const { return {-x, -y, -z}; }

        float lengthSquared() const { return x*x + y*y + z*z; }
        float distanceSquared(const Vector3& other) const { return (*this - other).lengthSquared(); }
        float length() const { return std::sqrt(lengthSquared()); }
        float dot(const Vector3& other) const { return x*other.x + y*other.y + z*other.z; }
        float distance(const Vector3& other) const { return (*this - other).length(); }
        float angle(const Vector3& other) const { return std::acos(dot(other) / (length() * other.length())); }

        Vector3 normalized() const { float l = length(); return l > 0 ? *this * (1.0f/l) : Vector3(0); }
        Vector3 lerp(const Vector3& other, float t) const { return *this * (1.0f-t) + other * t; }
        Vector3 cross(const Vector3& other) const { return {
            (y * other.z) - (z * other.y),
            (z * other.x) - (x * other.z),
            (x * other.y) - (y * other.x)
        }; }
        Vector3 projectOn(const Vector3& other) const { return other * (dot(other) / other.dot(other)); }
        Vector3 reflect(const Vector3& normal) const { return *this - normal * (2.0f * dot(normal)); }
        Vector3 min(const Vector3& other) const { return {
            std::min(x, other.x),
            std::min(y, other.y),
            std::min(z, other.z)
        }; }
        Vector3 max(const Vector3& other) const { return {
            std::max(x, other.x),
            std::max(y, other.y),
            std::max(z, other.z)
        }; }
        Vector3 abs() const { return {std::abs(x), std::abs(y), std::abs(z)}; }
    };

    struct Vector2 {
        float x = 0.0f, y = 0.0f;

        Vector2() = default;
        Vector2(float v) : Vector2(v, v) {}
        Vector2(float x, float y) : x(x), y(y) {}

        Vector2 operator+(const Vector2& other) const { return {x+other.x, y+other.y}; }
        Vector2 operator-(const Vector2& other) const { return {x-other.x, y-other.y}; }
        Vector2 operator*(const Vector2& other) const { return {x*other.x, y*other.y}; }
        Vector2 operator/(const Vector2& other) const { return {x/other.x, y/other.y}; }

        bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
        bool operator!=(const Vector2& other) const { return !(*this == other); }

        Vector2& operator+=(const Vector2& other) { x+=other.x; y+=other.y; return *this; }

        Vector2& operator*=(float scalar) { x*=scalar; y*=scalar; return *this; }
        Vector2& operator/=(float scalar) { x/=scalar; y/=scalar; return *this; }

        Vector2 operator*(float scalar) const { return {x*scalar, y*scalar}; }
        friend Vector2 operator*(float scalar, const Vector2& v) {
            return v * scalar;
        }
        Vector2 operator/(float scalar) const { return {x/scalar, y/scalar}; }
        friend Vector2 operator/(float scalar, const Vector2& v) {
            return v / scalar;
        }

        Vector2 operator-() const { return {-x, -y}; }

        float lengthSquared() const { return x*x + y*y; }
        float distanceSquared(const Vector2& other) const { return (*this - other).lengthSquared(); }
        float length() const { return std::sqrt(lengthSquared()); }
        float dot(const Vector2& other) const { return x*other.x + y*other.y; }
        float distance(const Vector2& other) const { return (*this - other).length(); }
        float angle(const Vector2& other) const { return std::acos(dot(other) / (length() * other.length())); }

        Vector2 normalized() const { float l = length(); return l > 0 ? *this * (1.0f/l) : Vector2(0); }
        Vector2 lerp(const Vector2& other, float t) const { return *this * (1.0f-t) + other * t; }

        float cross(const Vector2& other) const {
            return x * other.y - y * other.x;
        }
        Vector2 projectOn(const Vector2& other) const { return other * (dot(other) / other.dot(other)); }
        Vector2 reflect(const Vector2& normal) const { return *this - normal * (2.0f * dot(normal)); }
        Vector2 min(const Vector2& other) const { return {
            std::min(x, other.x),
            std::min(y, other.y),
        }; }
        Vector2 max(const Vector2& other) const { return {
            std::max(x, other.x),
            std::max(y, other.y),
        }; }
        Vector2 abs() const { return {std::abs(x), std::abs(y)}; }
    };

    struct Vector4 {
        float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

        Vector4() = default;
        Vector4(float v) : Vector4(v, v, v, v) {}
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        Vector4 operator+(const Vector4& other) const { return {x+other.x, y+other.y, z+other.z, w+other.w}; }
        Vector4 operator-(const Vector4& other) const { return {x-other.x, y-other.y, z-other.z, w-other.w}; }
        Vector4 operator*(const Vector4& other) const { return {x*other.x, y*other.y, z*other.z, w*other.w}; }
        Vector4 operator/(const Vector4& other) const { return {x/other.x, y/other.y, z/other.z, w/other.w}; }

        bool operator==(const Vector4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        bool operator!=(const Vector4& other) const { return !(*this == other); }

        Vector4 operator-() const { return {-x, -y, -z, -w}; }

        Vector4 operator*(float scalar) const { return {x*scalar, y*scalar, z*scalar, w*scalar}; }
        friend Vector4 operator*(float scalar, const Vector4& v) { return v * scalar; }
        Vector4 operator/(float scalar) const { return {x/scalar, y/scalar, z/scalar, w/scalar}; }

        float lengthSquared() const { return x*x + y*y + z*z + w*w; }
        float length() const { return std::sqrt(lengthSquared()); }
        float dot(const Vector4& other) const { return x*other.x + y*other.y + z*other.z + w*other.w; }

        Vector4 normalized() const { float l = length(); return l > 0 ? *this * (1.0f/l) : Vector4(0); }

        Vector3 xyz() const { return {x, y, z}; }

        Vector3 position() const { return {x / w, y / w, z / w}; }
    };
}
