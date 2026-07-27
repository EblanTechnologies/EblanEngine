import EE.Math.Vector;
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>

using namespace Catch::Matchers;

TEST_CASE("Vector3 - Default Constructor") {
    EE::Vector3 v;
    REQUIRE(v.x == 0.0f);
    REQUIRE(v.y == 0.0f);
    REQUIRE(v.z == 0.0f);
}

TEST_CASE("Vector3 - Constructor with one value") {
    EE::Vector3 v(5.0f);
    REQUIRE(v.x == 5.0f);
    REQUIRE(v.y == 5.0f);
    REQUIRE(v.z == 5.0f);
}

TEST_CASE("Vector3 - Constructor with three values") {
    EE::Vector3 v(1.0f, 2.0f, 3.0f);
    CHECK(v.x == 1.0f);
    CHECK(v.y == 2.0f);
    CHECK(v.z == 3.0f);
}

TEST_CASE("Vector3 - Addition Operator") {
    EE::Vector3 a(1, 2, 3);
    EE::Vector3 b(4, 5, 6);
    auto c = a + b;

    REQUIRE(c.x == 5.0f);
    REQUIRE(c.y == 7.0f);
    REQUIRE(c.z == 9.0f);
}

TEST_CASE("Vector3 - Subtraction operator") {
    EE::Vector3 a(5, 6, 7);
    EE::Vector3 b(1, 2, 3);
    auto c = a - b;

    REQUIRE(c.x == 4.0f);
    REQUIRE(c.y == 4.0f);
    REQUIRE(c.z == 4.0f);
}

TEST_CASE("Vector3 - Multiplication operator (component-wise)") {
    EE::Vector3 a(2, 3, 4);
    EE::Vector3 b(5, 6, 7);
    auto c = a * b;

    REQUIRE(c.x == 10.0f);
    REQUIRE(c.y == 18.0f);
    REQUIRE(c.z == 28.0f);
}

TEST_CASE("Vector3 - Scalar multiplication operator") {
    EE::Vector3 a(1, 2, 3);
    auto c = a * 2.0f;

    REQUIRE(c.x == 2.0f);
    REQUIRE(c.y == 4.0f);
    REQUIRE(c.z == 6.0f);
}

TEST_CASE("Vector3 - Scalar * Vector") {
    EE::Vector3 a(1, 2, 3);
    auto c = 2.0f * a;

    REQUIRE(c.x == 2.0f);
    REQUIRE(c.y == 4.0f);
    REQUIRE(c.z == 6.0f);
}

TEST_CASE("Vector3 - Squality operator") {
    EE::Vector3 a(1, 2, 3);
    EE::Vector3 b(1, 2, 3);
    EE::Vector3 c(4, 5, 6);

    REQUIRE(a == b);
    REQUIRE(a != c);
}

TEST_CASE("Vector3 - Unary minus") {
    EE::Vector3 a(1, -2, 3);
    auto b = -a;

    REQUIRE(b.x == -1.0f);
    REQUIRE(b.y == 2.0f);
    REQUIRE(b.z == -3.0f);
}

TEST_CASE("Vector3 - lengthSquared") {
    EE::Vector3 a(1, 2, 3);
    REQUIRE(a.lengthSquared() == 14.0f);
}

TEST_CASE("Vector3 - length") {
    EE::Vector3 a(1, 2, 3);
    REQUIRE_THAT(a.length(), WithinAbs(3.741657f, 0.001f));
}

TEST_CASE("Vector3 - dot") {
    EE::Vector3 a(1, 2, 3);
    EE::Vector3 b(4, 5, 6);
    CHECK(a.dot(b) == 32.0f);
}

TEST_CASE("Vector3 - cross") {
    EE::Vector3 a(1, 2, 3);
    EE::Vector3 b(4, 5, 6);
    auto c = a.cross(b);

    REQUIRE(c.x == -3.0f);
    REQUIRE(c.y == 6.0f);
    REQUIRE(c.z == -3.0f);
}

TEST_CASE("Vector3 - normalized") {
    EE::Vector3 a(3, 0, 4);
    auto n = a.normalized();

    REQUIRE_THAT(n.x, WithinAbs(0.6f, 0.001f));
    REQUIRE_THAT(n.y, WithinAbs(0.0f, 0.001f));
    REQUIRE_THAT(n.z, WithinAbs(0.8f, 0.001f));
    REQUIRE_THAT(n.length(), WithinAbs(1.0f, 0.001f));
}

TEST_CASE("Vector3 - distance") {
    EE::Vector3 a(1, 2, 3);
    EE::Vector3 b(4, 5, 6);
    REQUIRE_THAT(a.distance(b), WithinAbs(5.196152, 0.001f));
}

TEST_CASE("Vector3 - angle") {
    EE::Vector3 a(1, 0, 0);
    EE::Vector3 b(0, 1, 0);
    REQUIRE_THAT(a.angle(b), WithinAbs(3.141592f / 2.0f, 0.001f));
}

TEST_CASE("Vector3 - Static constants") {
    REQUIRE(EE::Vector3::Zero == EE::Vector3(0, 0, 0));
    REQUIRE(EE::Vector3::One == EE::Vector3(1, 1, 1));
    REQUIRE(EE::Vector3::Up == EE::Vector3(0, 1, 0));
    REQUIRE(EE::Vector3::Right == EE::Vector3(1, 0, 0));
    REQUIRE(EE::Vector3::Forward == EE::Vector3(0, 0, 1));
}

TEST_CASE("Vector3 - lerp") {
    EE::Vector3 a(0, 0, 0);
    EE::Vector3 b(10, 10, 10);

    auto mid = a.lerp(b, 0.5f);
    REQUIRE(mid.x == 5.0f);
    REQUIRE(mid.y == 5.0f);
    REQUIRE(mid.z == 5.0f);
}

TEST_CASE("Vector3 - min") {
    EE::Vector3 a(1, 5, 3);
    EE::Vector3 b(4, 2, 6);
    auto c = a.min(b);

    REQUIRE(c.x == 1.0f);
    REQUIRE(c.y == 2.0f);
    REQUIRE(c.z == 3.0f);
}

TEST_CASE("Vector3 - max") {
    EE::Vector3 a(1, 5, 3);
    EE::Vector3 b(4, 2, 6);
    auto c = a.max(b);

    REQUIRE(c.x == 4.0f);
    REQUIRE(c.y == 5.0f);
    REQUIRE(c.z == 6.0f);
}

TEST_CASE("Vector3 - abs") {
    EE::Vector3 a(-1, 2, -3);
    auto c = a.abs();

    REQUIRE(c.x == 1.0f);
    REQUIRE(c.y == 2.0f);
    REQUIRE(c.z == 3.0f);
}

TEST_CASE("Vector3 - projectOn") {
    EE::Vector3 a(1, 1, 0);
    EE::Vector3 b(2, 0, 0);
    auto proj = a.projectOn(b);

    REQUIRE(proj.x == 1.0f);
    REQUIRE(proj.y == 0.0f);
    REQUIRE(proj.z == 0.0f);
}

TEST_CASE("Vector3 - reflect") {
    EE::Vector3 a(1, -1, 0);
    EE::Vector3 normal(0, 1, 0);
    auto reflected = a.reflect(normal);

    REQUIRE(reflected.x == 1.0f);
    REQUIRE(reflected.y == 1.0f);
    REQUIRE(reflected.z == 0.0f);
}