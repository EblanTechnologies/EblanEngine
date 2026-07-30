module;

#include <cmath>
#include <cstring>

module EE.Math.Matrix;

import EE.Math.Matrix;
import EE.Math.Vector;
import EE.Math.Quaternion;
import EE.Math;

namespace EE {
    Matrix4::Matrix4() {
        memset(m, 0, sizeof(m));
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
    }

    Matrix4::Matrix4(const float diagonal) {
        memset(m, 0, sizeof(m));
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = diagonal;
    }

    Matrix4::Matrix4(const float a, const float b, const float c, const float d) {
        memset(m, 0, sizeof(m));
        m[0][0] = a;
        m[1][1] = b;
        m[2][2] = c;
        m[3][3] = d;
    }

    Matrix4::Matrix4(const float data[16]) {
        memcpy(m, data, sizeof(m));
    }

    Matrix4 Matrix4::identity() {
        return Matrix4(1.0);
    }

    Matrix4 Matrix4::translation(const Vector3& translation) {
        Matrix4 result;

        result.m[0][3] = translation.x;
        result.m[1][3] = translation.y;
        result.m[2][3] = translation.z;

        return result;
    }

    Matrix4 Matrix4::rotation(const Quaternion &qUn) {
        Quaternion q = qUn.normalized();
        Matrix4 result;

        const float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z, w2 = q.w;
        const float xx = q.x * x2, xy = q.x * y2, xz = q.x * z2;
        const float yy = q.y * y2, yz = q.y * z2, zz = q.z * z2;
        const float wx = q.w * x2, wy = q.w * y2, wz = q.w * z2;

        result.m[0][0] = 1.0f - yy - zz;
        result.m[0][1] = xy - wz;
        result.m[0][2] = xz + wy;
        result.m[0][3] = 0.0f;

        result.m[1][0] = xy + wz;
        result.m[1][1] = 1.0f - xx - zz;
        result.m[1][2] = yz - wx;
        result.m[1][3] = 0.0f;

        result.m[2][0] = xz - wy;
        result.m[2][1] = yz + wx;
        result.m[2][2] = 1.0f - xx - yy;
        result.m[2][3] = 0.0f;

        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = 0.0f;
        result.m[3][3] = 1.0f;

        return result;
    }

    Matrix4 Matrix4::scaling(const Vector3 &scale) {
        Matrix4 result;
        result.m[0][0] = scale.x,
        result.m[1][1] = scale.y,
        result.m[2][2] = scale.z;
        result.m[3][3] = 1.0f;
        return result;
    }

    Matrix4 Matrix4::perspective(const int fovDeg, const float aspect, const float near, const float far) {
        Matrix4 result(0.0f);

        const float fowRad = EEStd::deg2Rad(static_cast<float>(fovDeg));
        const float tanHalfFov = std::tan(fowRad * 0.5f);
        const float zRange = near - far;

        result.m[0][0] = 1.0f / (aspect * tanHalfFov);
        result.m[1][1] = 1.0f / tanHalfFov;
        result.m[2][2] = (far + near) / zRange;
        result.m[2][3] = (2.0f * far * near) / zRange;
        result.m[3][2] = -1.0f;
        result.m[3][3] = 0.0f;

        return result;
    }

    Matrix4 Matrix4::orthographic(const float left, const float right, const float bottom, const float top, const float near, const float far) {
        Matrix4 result;

        const float width = right - left;
        const float height = top - bottom;
        const float depth = near - far;

        result.m[0][0] = 2.0f / width;
        result.m[1][1] = 2.0f / height;
        result.m[2][2] = -2.0f / depth;
        result.m[3][0] = -(right + left) / width;
        result.m[3][1] = -(top + bottom) / height;
        result.m[3][2] = -(far + near) / depth;
        result.m[3][3] = 1.0f;

        return result;
    }

    Matrix4 Matrix4::lookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up) {
        const Vector3 forward = (target - eye).normalized();
        const Vector3 right = forward.cross(up).normalized();
        const Vector3 upLocal = right.cross(forward);

        Matrix4 result;

        result.m[0][0] = right.x;
        result.m[0][1] = right.y;
        result.m[0][2] = right.z;
        result.m[0][3] = -right.dot(eye);

        result.m[1][0] = upLocal.x;
        result.m[1][1] = upLocal.y;
        result.m[1][2] = upLocal.z;
        result.m[1][3] = -upLocal.dot(eye);

        result.m[2][0] = -forward.x;
        result.m[2][1] = -forward.y;
        result.m[2][2] = -forward.z;
        result.m[2][3] = forward.dot(eye);

        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = 0.0f;
        result.m[3][3] = 1.0f;

        return result;
    }

    Matrix4 Matrix4::operator*(const Matrix4& other) const {
        Matrix4 result(0.0f);
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                result.m[i][j] =
                    m[i][0] * other.m[0][j] +
                    m[i][1] * other.m[1][j] +
                    m[i][2] * other.m[2][j] +
                    m[i][3] * other.m[3][j];
            }
        }
        return result;
    }

    Vector3 Matrix4::operator*(const Vector3& v) const {
        const float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
        const float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
        const float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
        float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];
        if (w == 0.0f) w = 1.0f;
        return {x/w, y/w, z/w};
    }

    Vector4 Matrix4::operator*(const Vector4 &v) const {
        return {
            m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w,
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]*v.w,
            m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]*v.w,
            m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3]*v.w
        };
    }

    Matrix4& Matrix4::operator*=(const Matrix4& other) {
        *this = *this * other;
        return *this;
    }

    bool Matrix4::operator==(const Matrix4& other) const {
        return memcmp(m, other.m, sizeof(m)) == 0;
    }

    bool Matrix4::operator!=(const Matrix4& other) const {
        return !(*this == other);
    }

    float& Matrix4::operator()(const int row, const int col) {
        return m[row][col];
    }

    const float& Matrix4::operator()(const int row, const int col) const {
        return m[row][col];
    }

    float Matrix4::minor(const int row, const int col) const {
        float sub[3][3];
        int r = 0, c = 0;

        for(int i = 0; i < 4; i++) {
            if(i == row) continue;
            c = 0;
            for(int j = 0; j < 4; j++) {
                if(j == col) continue;
                sub[r][c] = m[i][j];
                c++;
            }
            r++;
        }

        return sub[0][0] * (sub[1][1]*sub[2][2] - sub[1][2]*sub[2][1])
             - sub[0][1] * (sub[1][0]*sub[2][2] - sub[1][2]*sub[2][0])
             + sub[0][2] * (sub[1][0]*sub[2][1] - sub[1][1]*sub[2][0]);
    }

    float Matrix4::determinant() const {
        const float a = m[0][0], b = m[0][1], c = m[0][2], d = m[0][3];

        const float det =
            a * minor(0, 0) -
            b * minor(0, 1) +
            c * minor(0, 2) -
            d * minor(0, 3);

        return det;
    }

    Matrix4 Matrix4::inverse() const {
        const float det = determinant();
        if(det == 0.0f) return {};

        Matrix4 inv;
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                const float sign = ((i + j) % 2 == 0) ? 1.0f : -1.0f;
                inv.m[j][i] = sign * minor(i, j) / det;
            }
        }

        return inv;
    }

    Matrix4 Matrix4::transpose() const {
        Matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[j][i];
            }
        }
        return result;
    }

    Matrix4 Matrix4::trs(const Vector3& pos, const Quaternion& rot, const Vector3& scale) {
        const Matrix4 trans = translation(pos);
        const Matrix4 rotMat = rotation(rot);
        const Matrix4 scaleMat = scaling(scale);

        return trans * rotMat * scaleMat;
    }

    float* Matrix4::data() {
        return &m[0][0];
    }

    const float* Matrix4::data() const {
        return &m[0][0];
    }
}
