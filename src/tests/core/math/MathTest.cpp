#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

import EE.Math;

TEST_CASE("Math - deg2Rad / rad2Deg roundtrip", "[math]") {
    REQUIRE(EEStd::deg2Rad(180.0f) == Catch::Approx(3.14159265f).margin(1e-6f));
    REQUIRE(EEStd::deg2Rad(0.0f) == 0.0f);
    REQUIRE(EEStd::rad2Deg(3.14159265f) == Catch::Approx(180.0f).margin(1e-4f));

    const float rad = 0.7f;
    REQUIRE(EEStd::rad2Deg(EEStd::deg2Rad(rad)) == Catch::Approx(rad).margin(1e-6f));
}

TEST_CASE("Math - clamp accepts temporaries (rvalues)", "[math]") {
    // Regression: clamp used to take T& and could not be called with temporaries.
    REQUIRE(EEStd::clamp(5.0f, 0.0f, 3.0f) == 3.0f);
    REQUIRE(EEStd::clamp(-1.0f, 0.0f, 3.0f) == 0.0f);
    REQUIRE(EEStd::clamp(1.5f, 0.0f, 3.0f) == 1.5f);
}

TEST_CASE("Math - smoothstep endpoints and clamping", "[math]") {
    REQUIRE(EEStd::smoothstep(0.0f, 1.0f, 0.0f) == Catch::Approx(0.0f).margin(1e-6f));
    REQUIRE(EEStd::smoothstep(0.0f, 1.0f, 1.0f) == Catch::Approx(1.0f).margin(1e-6f));
    REQUIRE(EEStd::smoothstep(0.0f, 1.0f, 0.5f) == Catch::Approx(0.5f).margin(1e-6f));
    // below/above the edges it must clamp, not extrapolate
    REQUIRE(EEStd::smoothstep(0.0f, 1.0f, -5.0f) == 0.0f);
    REQUIRE(EEStd::smoothstep(0.0f, 1.0f, 5.0f) == 1.0f);
}

TEST_CASE("Math - lerp", "[math]") {
    REQUIRE(EEStd::lerp(0.0f, 10.0f, 0.0f) == 0.0f);
    REQUIRE(EEStd::lerp(0.0f, 10.0f, 1.0f) == 10.0f);
    REQUIRE(EEStd::lerp(0.0f, 10.0f, 0.25f) == Catch::Approx(2.5f).margin(1e-6f));
}

TEST_CASE("Math - isPowerOfTwo / alignment", "[math]") {
    REQUIRE(EEStd::isPowerOfTwo(1));
    REQUIRE(EEStd::isPowerOfTwo(256));
    REQUIRE_FALSE(EEStd::isPowerOfTwo(0));
    REQUIRE_FALSE(EEStd::isPowerOfTwo(255));

    REQUIRE(EEStd::alignUp(13, 16) == 16);
    REQUIRE(EEStd::alignUp(16, 16) == 16);
    REQUIRE(EEStd::alignDown(29, 16) == 16);
}

TEST_CASE("Math - remap", "[math]") {
    REQUIRE(EEStd::remap(5.0f, 0.0f, 10.0f, 100.0f, 200.0f) == Catch::Approx(150.0f).margin(1e-5f));
}
