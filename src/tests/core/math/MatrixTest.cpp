#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include "../../common/CatchStringMakers.hpp"

import EE.Math.Matrix;
import EE.Math.Quaternion;
import EE.Math.Vector;
import EE.Math.Angle;
import EE.Math.Literals;

using namespace Catch::Matchers;
using namespace EE::Literals;

namespace {
    bool allFinite(const EE::Matrix4& m) {
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                if (!std::isfinite(m.m[r][c])) return false;
        return true;
    }
}

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

TEST_CASE("Matrix4 - transformDirection ignores translation", "[matrix]") {
    EE::Matrix4 m = EE::Matrix4::trs(EE::Vector3(10, 0, 0), EE::Quaternion(), EE::Vector3(2, 2, 2));
    auto dir = m.transformDirection(EE::Vector3(1, 0, 0));
    REQUIRE(dir.x == Catch::Approx(2.0f));
    REQUIRE(dir.y == Catch::Approx(0.0f));
    REQUIRE(dir.z == Catch::Approx(0.0f));
}

TEST_CASE("Matrix4 - transformPoint applies translation", "[matrix]") {
    EE::Matrix4 m = EE::Matrix4::trs(EE::Vector3(10, 0, 0), EE::Quaternion(), EE::Vector3(2, 2, 2));
    auto p = m.transformPoint(EE::Vector3(1, 0, 0));
    REQUIRE(p.x == Catch::Approx(12.0f));
    REQUIRE(p.y == Catch::Approx(0.0f));
    REQUIRE(p.z == Catch::Approx(0.0f));
}

TEST_CASE("Matrix4 - Perspective maps near/far planes to -1/+1", "[matrix][projection]") {
    const EE::Matrix4 p = EE::Matrix4::perspective(60.0f, 1.0f, 0.1f, 100.0f);

    // Points on the view axis (negative Z is forward in view space).
    EE::Vector4 near(0, 0, -0.1f, 1);
    EE::Vector4 far(0, 0, -100.0f, 1);
    EE::Vector4 behind(0, 0, +1.0f, 1);

    const auto cn = p * near;
    const auto cf = p * far;
    const auto cb = p * behind;

    REQUIRE(cn.z / cn.w == Catch::Approx(-1.0f).margin(1e-5f));
    REQUIRE(cf.z / cf.w == Catch::Approx(1.0f).margin(1e-4f));
    // W clip: -z, so points behind the camera get negative w (clipped out).
    REQUIRE(cb.w < 0.0f);
}

TEST_CASE("Matrix4 - Perspective aspect narrows horizontal FOV", "[matrix][projection]") {
    const EE::Matrix4 p = EE::Matrix4::perspective(90.0f, 2.0f, 1.0f, 10.0f);
    // m[1][1] == 1/tan(45deg) == 1; m[0][0] == m[1][1] / aspect
    REQUIRE(p.m[1][1] == Catch::Approx(1.0f).margin(1e-5f));
    REQUIRE(p.m[0][0] == Catch::Approx(0.5f).margin(1e-5f));
}

TEST_CASE("Matrix4 - Orthographic centers and scales correctly", "[matrix][projection]") {
    // NOTE: regression test - the translation used to live in the last ROW,
    // which silently broke every orthographic projection.
    const EE::Matrix4 o = EE::Matrix4::orthographic(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

    const auto center = o * EE::Vector4(0, 0, -50.05f, 1);   // middle of the frustum depth
    REQUIRE(center.x == Catch::Approx(0.0f).margin(1e-5f));
    REQUIRE(center.y == Catch::Approx(0.0f).margin(1e-5f));
    REQUIRE(center.z == Catch::Approx(0.0f).margin(1e-4f));

    const auto left = o * EE::Vector4(-10, 0, -50.05f, 1);
    REQUIRE(left.x == Catch::Approx(-1.0f).margin(1e-5f));

    const auto top = o * EE::Vector4(0, 10, -50.05f, 1);
    REQUIRE(top.y == Catch::Approx(1.0f).margin(1e-5f));

    const auto nearP = o * EE::Vector4(0, 0, -0.1f, 1);
    const auto farP = o * EE::Vector4(0, 0, -100.0f, 1);
    REQUIRE(nearP.z == Catch::Approx(-1.0f).margin(1e-4f));
    REQUIRE(farP.z == Catch::Approx(1.0f).margin(1e-4f));
}

TEST_CASE("Matrix4 - LookAt places target in front of camera", "[matrix][view]") {
    // Camera at (0,2,5) looking at the origin: origin must land on the view axis.
    const EE::Matrix4 view = EE::Matrix4::lookAt({0, 2, 5}, {0, 0, 0}, {0, 1, 0});
    const EE::Vector3 v = view * EE::Vector3(0, 0, 0);
    REQUIRE(v.z == Catch::Approx(-5.385f).margin(1e-2f));
    REQUIRE(std::abs(v.x) < 0.25f);
    REQUIRE(std::abs(v.y) < 0.25f);

    // A point behind the camera must map behind (-z > 0 ... i.e. positive z).
    const EE::Vector3 behind = view * EE::Vector3(0, 2, 6);
    REQUIRE(behind.z > 0.0f);
}

TEST_CASE("Matrix4 - LookAt straight down does not produce NaN", "[matrix][view]") {
    // Regression: forward parallel to up used to degenerate the cross products.
    const EE::Matrix4 view = EE::Matrix4::lookAt({0, 10, 0}, {0, 0, 0}, {0, 1, 0});
    REQUIRE(allFinite(view));
    const EE::Vector3 v = view * EE::Vector3(0, 0, 0);
    REQUIRE(v.z == Catch::Approx(-10.0f).margin(1e-2f));
}

TEST_CASE("Matrix4 - LookAt straight up does not produce NaN", "[matrix][view]") {
    const EE::Matrix4 view = EE::Matrix4::lookAt({0, -10, 0}, {0, 0, 0}, {0, 1, 0});
    REQUIRE(allFinite(view));
    const EE::Vector3 v = view * EE::Vector3(0, 0, 0);
    REQUIRE(v.z == Catch::Approx(-10.0f).margin(1e-2f));
}

TEST_CASE("Matrix4 - trs composes T * R * S", "[matrix]") {
    EE::Quaternion q = EE::Quaternion::fromEuler(0_deg, 90_deg, 0_deg);
    EE::Matrix4 m = EE::Matrix4::trs(EE::Vector3(10, 0, 0), q, EE::Vector3(2, 2, 2));

    // scale(1,0,0)*2 = (2,0,0) -> rotate yaw 90: (2,0,0) -> (0,0,-2) -> translate: (10,0,-2)
    EE::Vector3 r = m * EE::Vector3(1, 0, 0);
    REQUIRE(r.x == Catch::Approx(10.0f).margin(1e-4f));
    REQUIRE(r.y == Catch::Approx(0.0f).margin(1e-4f));
    REQUIRE(r.z == Catch::Approx(-2.0f).margin(1e-4f));
}

TEST_CASE("Matrix4 - Inverse roundtrip yields identity", "[matrix]") {
    EE::Quaternion q = EE::Quaternion::fromEuler(15_deg, 40_deg, EE::Angle::fromDegrees(-25.0f));
    EE::Matrix4 m = EE::Matrix4::trs(EE::Vector3(3, -2, 5), q, EE::Vector3(1, 2, 3));

    EE::Matrix4 inv = m.inverse();
    EE::Matrix4 product = m * inv;

    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            const float expected = (r == c) ? 1.0f : 0.0f;
            REQUIRE(product.m[r][c] == Catch::Approx(expected).margin(1e-3f));
        }
    }
}

TEST_CASE("Matrix4 - Transpose", "[matrix]") {
    EE::Matrix4 m = EE::Matrix4::translation(EE::Vector3(1, 2, 3));
    EE::Matrix4 t = m.transpose();
    REQUIRE(t.m[3][0] == 1.0f);
    REQUIRE(t.m[3][1] == 2.0f);
    REQUIRE(t.m[3][2] == 3.0f);
    // double transpose = original
    REQUIRE(t.transpose() == m);
}

TEST_CASE("Matrix4 - Determinant", "[matrix]") {
    REQUIRE(EE::Matrix4::identity().determinant() == Catch::Approx(1.0f));
    EE::Matrix4 scaled = EE::Matrix4::scaling(EE::Vector3(2, 3, 4));
    REQUIRE(scaled.determinant() == Catch::Approx(24.0f).margin(1e-4f));
}

TEST_CASE("Matrix4 - ViewProj keeps visible points in clip volume", "[matrix][camera]") {
    // Full pipeline smoke test: camera + projection + world matrix.
    EE::Quaternion q = EE::Quaternion::fromEuler(-20_deg, 0_deg, 0_deg);
    const EE::Vector3 eye(0, 2, 5);
    EE::Matrix4 view = EE::Matrix4::lookAt(eye, eye + q * EE::Vector3(0, 0, -1), EE::Vector3(0, 1, 0));
    EE::Matrix4 proj = EE::Matrix4::perspective(60.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
    EE::Matrix4 world = EE::Matrix4::trs(EE::Vector3(0, 0, 0), EE::Quaternion(), EE::Vector3(1, 1, 1));
    EE::Matrix4 viewProj = proj * view;

    // All 8 cube corners must land inside the NDC cube.
    for (int ix = -1; ix <= 1; ix += 2)
        for (int iy = -1; iy <= 1; iy += 2)
            for (int iz = -1; iz <= 1; iz += 2) {
                EE::Vector4 corner(ix * 0.5f, iy * 0.5f, iz * 0.5f, 1.0f);
                EE::Vector4 clip = viewProj * (world * corner);
                REQUIRE(clip.w > 0.0f);
                const float ndcX = clip.x / clip.w, ndcY = clip.y / clip.w, ndcZ = clip.z / clip.w;
                REQUIRE(ndcX >= -1.0f);
                REQUIRE(ndcX <= 1.0f);
                REQUIRE(ndcY >= -1.0f);
                REQUIRE(ndcY <= 1.0f);
                REQUIRE(ndcZ >= -1.0f);
                REQUIRE(ndcZ <= 1.0f);
            }
}
