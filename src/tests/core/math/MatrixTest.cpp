#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>
#include "../../common/CatchStringMakers.hpp"

import EE.Math.Matrix;
import EE.Math.Quaternion;
import EE.Math.Vector;
import EE.Math.Literals;

using namespace Catch::Matchers;
using namespace EE::Literals;

TEST_CASE("Matrix4 - Multiplication", "[matrix]") {
    EE::Matrix4 a;
    EE::Matrix4 b;
    auto c = a * b;
    REQUIRE(c == EE::Matrix4());
}

TEST_CASE("Matrix4 - Vector rotation", "[matrix]") {
    EE::Quaternion q = EE::Quaternion::fromEuler(0_deg, 90_deg, 0_deg);
    EE::Matrix4 rot = EE::Matrix4::rotation(q);
    EE::Vector3 v(1, 0, 0);
    auto result = rot * v;
    REQUIRE(result.x == Catch::Approx(0.0f).margin(1e-5f));
    REQUIRE(result.y == Catch::Approx(0.0f));
    REQUIRE(result.z == Catch::Approx(-1.0f));
}

TEST_CASE("Matrix4 - Translation", "[matrix]") {
    EE::Matrix4 trans = EE::Matrix4::translation(EE::Vector3(5, 0, 0));
    EE::Vector3 v(1, 2, 3);
    auto result = trans * v;
    REQUIRE(result == EE::Vector3(6, 2, 3));
}