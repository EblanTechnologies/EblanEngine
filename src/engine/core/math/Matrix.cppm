export module EE.Math.Matrix;

import EE.Math.Vector;
import EE.Math.Quaternion;

export namespace EE {
    struct Matrix4 {
        float m[4][4];

        Matrix4();
        Matrix4(float diagonal);
        Matrix4(float a, float b, float c, float d);
        Matrix4(const float data[16]);

        static Matrix4 identity();
        static Matrix4 translation(const Vector3& pos);
        static Matrix4 rotation(const Quaternion& q);
        static Matrix4 scaling(const Vector3& scale);
        static Matrix4 perspective(float fov, float aspect, float near, float far);
        static Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far);
        static Matrix4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

        Matrix4 operator*(const Matrix4& other) const;
        Vector3 operator*(const Vector3& v) const;
        Vector4 operator*(const Vector4& v) const;

        Matrix4& operator*=(const Matrix4& other);

        bool operator==(const Matrix4& other) const;
        bool operator!=(const Matrix4& other) const;

        float minor(int row, int col) const;
        float determinant() const;
        Matrix4 inverse() const;
        Matrix4 transpose() const;
        Matrix4 trs(const Vector3& pos, const Quaternion& rot, const Vector3& scale) const;

        float& operator()(int row, int col);
        const float& operator()(int row, int col) const;

        float* data();
        const float* data() const;
    };
}