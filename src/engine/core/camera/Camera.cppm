// ISC License
//
// Copyright (c) 2026 EblanTechnologies team
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

module;

#include <cmath>

export module EE.Core.Camera;

import EE.Math.Vector;
import EE.Math.Quaternion;
import EE.Math.Angle;
import EE.Math.Matrix;
import EE.Math;

export namespace EE {
    /// Free 3D camera.
    ///
    /// The engine is right-handed (OpenGL-style): with pitch = yaw = roll = 0
    /// the camera sits at `position` and looks along **-Z**, +X is right and
    /// +Y is up. Positive `pitchDeg` looks up, positive `yawDeg` turns left
    /// (counter-clockwise seen from above, standard RH rotation around +Y).
    class Camera {
        Vector3 m_position{0.0f, 0.0f, 0.0f};
        float m_pitchDeg = 0.0f;
        float m_yawDeg = 0.0f;
        float m_rollDeg = 0.0f;

        float m_fovYDeg = 60.0f;
        float m_nearZ = 0.1f;
        float m_farZ = 1000.0f;

        static constexpr float MAX_PITCH_DEG = 89.9f;

    public:
        Camera() = default;
        Camera(const Vector3& position, const float pitchDeg, const float yawDeg)
            : m_position(position), m_pitchDeg(pitchDeg), m_yawDeg(yawDeg) {}

        [[nodiscard]] const Vector3& position() const { return m_position; }
        void setPosition(const Vector3& position) { m_position = position; }

        /// Moves the camera in world space.
        void move(const Vector3& offset) { m_position += offset; }

        [[nodiscard]] float pitchDeg() const { return m_pitchDeg; }
        [[nodiscard]] float yawDeg() const { return m_yawDeg; }
        [[nodiscard]] float rollDeg() const { return m_rollDeg; }

        /// Rotates the camera. Positive pitch looks up, positive yaw turns left.
        void setEuler(const float pitchDeg, const float yawDeg, const float rollDeg = 0.0f) {
            m_pitchDeg = pitchDeg;
            m_yawDeg = yawDeg;
            m_rollDeg = rollDeg;
        }

        void addPitch(const float deltaDeg) {
            m_pitchDeg = EEStd::clamp(m_pitchDeg + deltaDeg, -MAX_PITCH_DEG, MAX_PITCH_DEG);
        }

        void addYaw(const float deltaDeg) {
            m_yawDeg += deltaDeg;
            // keep the angle in [-180, 180) so it never grows unbounded
            if (m_yawDeg >= 180.0f) m_yawDeg -= 360.0f;
            if (m_yawDeg < -180.0f) m_yawDeg += 360.0f;
        }

        [[nodiscard]] float fovYDeg() const { return m_fovYDeg; }
        void setFovYDeg(const float fovYDeg) { m_fovYDeg = fovYDeg; }

        [[nodiscard]] float nearZ() const { return m_nearZ; }
        [[nodiscard]] float farZ() const { return m_farZ; }
        void setClipPlanes(const float nearZ, const float farZ) {
            m_nearZ = nearZ;
            m_farZ = farZ;
        }

        /// Rotation quaternion built from pitch/yaw/roll (same convention as Transform).
        [[nodiscard]] Quaternion rotation() const {
            return Quaternion::fromEuler(Angle::fromDegrees(m_pitchDeg),
                                         Angle::fromDegrees(m_yawDeg),
                                         Angle::fromDegrees(m_rollDeg));
        }

        /// World-space direction the camera looks along (normalized).
        [[nodiscard]] Vector3 forward() const {
            return rotation() * Vector3(0.0f, 0.0f, -1.0f);
        }

        /// World-space right axis (normalized).
        [[nodiscard]] Vector3 right() const {
            return rotation() * Vector3(1.0f, 0.0f, 0.0f);
        }

        /// World-space up axis (normalized).
        [[nodiscard]] Vector3 up() const {
            return rotation() * Vector3(0.0f, 1.0f, 0.0f);
        }

        /// View matrix (world -> view space).
        [[nodiscard]] Matrix4 view() const {
            return Matrix4::lookAt(m_position, m_position + forward(), Vector3(0.0f, 1.0f, 0.0f));
        }

        /// Perspective projection matrix. `aspect` is width / height.
        [[nodiscard]] Matrix4 projection(const float aspect) const {
            return Matrix4::perspective(m_fovYDeg, aspect, m_nearZ, m_farZ);
        }

        /// Ready-to-upload combined matrix (proj * view, column-vector order).
        [[nodiscard]] Matrix4 viewProjection(const float aspect) const {
            return projection(aspect) * view();
        }
    };
}
