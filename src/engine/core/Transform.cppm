module;

#include <cstddef>

#include "ReflectionMacro.hpp"

export module EE.Core.Transform;

import EE.Math.Vector;
import EE.Math.Quaternion;
import EE.Math.Matrix;
import EE.Core.Reflection;

export namespace EE {
    struct Transform {
        Vector3 position{0.0f};
        Quaternion rotation{0.0f, 0.0f, 0.0f, 1.0f};
        Vector3 scale{1.0f};

        mutable Matrix4 worldMatrix = Matrix4::identity();
        mutable bool dirty = true;

        Matrix4 getWorldMatrix() const {
            if (dirty) {
                worldMatrix = Matrix4::trs(position, rotation, scale);
                dirty = false;
            }
            return worldMatrix;
        }

        void setPosition(const Vector3& position) {
            this->position = position;
            dirty = true;
        }

        void setRotation(const Quaternion& rotation) {
            this->rotation = rotation;
            dirty = true;
        }

        void setScale(const Vector3& scale) {
            this->scale = scale;
            dirty = true;
        }

        void setRotationEuler(const Vector3& eulerDeg) {
            rotation = Quaternion::fromEulerDegrees(eulerDeg);
            dirty = true;
        }
    };
}

REFLECT_COMPONENT(EE::Transform, {
    t.fields = {
        {"Position", offsetof(Transform, position), &getTypeInfo<Vector3>()},
        {"Rotation", offsetof(Transform, rotation), &getTypeInfo<Quaternion>()},
        {"Scale",    offsetof(Transform, scale),    &getTypeInfo<Vector3>()},
    };
    t.fields[2].min = 0.001f;
    t.fields[2].max = 1000.0f;
})