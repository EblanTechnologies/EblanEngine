// ISC License
//
// Copyright (c) "2026" EblanTechnologies team
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

export module EE.Math;

import std;

export namespace EEStd {
    constexpr float PI_F = 3.14159265359f;
    constexpr double PI = 3.14159265358979323846;

    template<std::floating_point T>
    constexpr T deg2Rad(T deg) {
        return deg * PI_F / 180.0f;
    }

    template<std::floating_point T>
    constexpr T rad2Deg(T rad) {
        return rad * 180.0f / PI_F;
    }

    template<typename T>
    constexpr T clamp(T& value, T& min, T& max) {
        return value < min ? min : (value > max ? max : value);
    }

    template<typename T>
    constexpr T lerp(const T& a, const T& b, float t) {
        return a + (b - a) * t;
    }

    template<std::floating_point T>
    constexpr T smoothstep(T edge0, T edge1, T x) {
        const T t = clamp((x - edge0) / (edge1 - edge0), T(0), T(1));
        return t * t * (T(3) - T(2) * t);
    }

    template<typename T>
    constexpr T remap(const T& value, const T& from1, const T& to1, const T& from2, const T& to2) {
        return from2 + (value - from1) * (to2 - from2) / (to1 - from1);
    }

    template<std::integral T>
    constexpr bool isPowerOfTwo(T value) {
        return value > 0 && (value & (value - 1)) == 0;
    }

    template<std::integral T>
    constexpr T alignUp(T value, T alignment) {
        return (value + alignment - 1) & ~(alignment - 1);
    }

    template<std::integral T>
    constexpr T alignDown(T value, T alignment) {
        return value & ~(alignment - 1);
    }
}