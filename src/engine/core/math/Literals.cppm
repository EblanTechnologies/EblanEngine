module;

#include <numbers>

export module EE.Math.Literals;

import EE.Math.Angle;

export namespace EE::Literals {
    constexpr Angle operator""_deg(long double value) {
        return Angle(
            static_cast<float>(
                value * std::numbers::pi_v<long double> / 180.0L
            )
        );
    }

    constexpr Angle operator""_rad(long double value) {
        return Angle(static_cast<float>(value));
    }

    constexpr Angle operator""_deg(unsigned long long value) {
        return Angle(
            static_cast<float>(
                value * std::numbers::pi_v<long double> / 180.0L
            )
        );
    }

    constexpr Angle operator""_rad(unsigned long long value) {
        return Angle(static_cast<float>(value));
    }
}
