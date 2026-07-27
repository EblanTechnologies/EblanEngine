import EE.Math.Quaternion;
import EE.Math.Vector;

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>

using namespace Catch::Matchers;

TEST_CASE("Quaternion - Constructors", "[quaternion]") {
    EE::Quaternion q1;
    REQUIRE(q1.x == 0.0f);
    REQUIRE(q1.y == 0.0f);
    REQUIRE(q1.z == 0.0f);
    REQUIRE(q1.w == 1.0f);

    EE::Quaternion q2(1, 2, 3, 4);
    REQUIRE(q2.x == 1.0f);
    REQUIRE(q2.y == 2.0f);
    REQUIRE(q2.z == 3.0f);
    REQUIRE(q2.w == 4.0f);
}

TEST_CASE("Quaternion - Identity", "[quaternion]") {
    auto q = EE::Quaternion::Identity;
    REQUIRE(q.x == 0.0f);
    REQUIRE(q.y == 0.0f);
    REQUIRE(q.z == 0.0f);
    REQUIRE(q.w == 1.0f);
}

TEST_CASE("Quaternion - FromAxisAngle", "[quaternion]") {
    EE::Vector3 axis(0, 0, 1);
    float angle = 3.141592f * 0.5f;

    auto q = EE::Quaternion::fromAxisAngle(axis, angle);

    REQUIRE(q.x == Catch::Approx(0.0f));
    REQUIRE(q.y == Catch::Approx(0.0f));
    REQUIRE(q.z == Catch::Approx(0.7071f));
    REQUIRE(q.w == Catch::Approx(0.7071f));
}

TEST_CASE("Quaternion - FromEuler", "[quaternion]") {
    auto q = EE::Quaternion::fromEuler(0, 0, 3.141592f / 2.0f);

    REQUIRE(q.x == Catch::Approx(0.0f));
    REQUIRE(q.y == Catch::Approx(0.0f));
    REQUIRE(q.z == Catch::Approx(0.7071f));
    REQUIRE(q.w == Catch::Approx(0.7071f));
}

TEST_CASE("Quaternion - Multiplication", "[quaternion]") {
    EE::Quaternion q1(1, 0, 0, 0);
    EE::Quaternion q2(0, 1, 0, 0);

    auto q3 = q1 * q2;
    REQUIRE(q3.x == 0.0f);
    REQUIRE(q3.y == 0.0f);
    REQUIRE(q3.z == 1.0f);
    REQUIRE(q3.w == 0.0f);
}

TEST_CASE("Quaternion - Length", "[quaternion]") {
    EE::Quaternion q(1, 2, 3, 4);
    REQUIRE(q.lengthSquared() == 30.0f);
    REQUIRE(q.length() == Catch::Approx(5.4772f));
}

TEST_CASE("Quaternion - Normalize", "[quaternion]") {
    EE::Quaternion q(1, 2, 3, 4);
    auto n = q.normalized();
    REQUIRE(n.length() == Catch::Approx(1.0f));
}

TEST_CASE("Quaternion - Conjugate", "[quaternion]") {
    EE::Quaternion q(1, 2, 3, 4);
    auto c = q.conjugate();
    REQUIRE(c.x == -1.0f);
    REQUIRE(c.y == -2.0f);
    REQUIRE(c.z == -3.0f);
    REQUIRE(c.w == 4.0f);
}

TEST_CASE("Quaternion - Inverse", "[quaternion]") {
    EE::Quaternion q(1, 2, 3, 4);
    auto inv = q.inverse();
    auto result = q * inv;

    REQUIRE(result.x == Catch::Approx(0.0f));
    REQUIRE(result.y == Catch::Approx(0.0f));
    REQUIRE(result.z == Catch::Approx(0.0f));
    REQUIRE(result.w == Catch::Approx(1.0f));
}

TEST_CASE("Quaternion - Vector Rotation", "[quaternion]") {
    EE::Quaternion q = EE::Quaternion::fromAxisAngle(
        EE::Vector3(0, 0, 1), 3.141592f / 2.0f
    );

    EE::Vector3 v(1, 0, 0);
    auto result = q * v;

    REQUIRE(result.x == Catch::Approx(0.0f).margin(1e-5f));
    REQUIRE(result.y == Catch::Approx(1.0f).margin(1e-5f));
    REQUIRE(result.z == Catch::Approx(0.0f).margin(1e-5f));
}