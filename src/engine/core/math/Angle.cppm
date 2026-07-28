module;

#include <numbers>

export module EE.Math.Angle;

export namespace EE {
    struct Angle {
        constexpr explicit Angle(float radians) : m_radians(radians) {}

        [[nodiscard]]
        constexpr float radians() const {
            return m_radians;
        }

        [[nodiscard]]
        constexpr float degrees() const {
            return m_radians * 180.0f / std::numbers::pi_v<float>;
        }

        static constexpr Angle fromRadians(const float value) {
            return Angle(value);
        }

        static constexpr Angle fromDegrees(const float value) {
            return Angle(value * std::numbers::pi_v<float> / 180.0f);
        }

    private:
        float m_radians;
    };
}