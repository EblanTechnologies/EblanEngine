#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include "../../common/CatchStringMakers.hpp"

import EE.Core.Camera;
import EE.Math.Vector;
import EE.Math.Matrix;
import EE.Math.Quaternion;

using namespace Catch::Matchers;

TEST_CASE("Camera - Default state", "[camera]") {
    EE::Camera cam;
    REQUIRE(cam.position() == EE::Vector3(0, 0, 0));
    REQUIRE(cam.pitchDeg() == 0.0f);
    REQUIRE(cam.yawDeg() == 0.0f);
    REQUIRE(cam.fovYDeg() == Catch::Approx(60.0f));
    REQUIRE(cam.nearZ() == Catch::Approx(0.1f));
    REQUIRE(cam.farZ() == Catch::Approx(1000.0f));
}

TEST_CASE("Camera - Forward is -Z at identity (right-handed)", "[camera]") {
    EE::Camera cam;
    const EE::Vector3 f = cam.forward();
    REQUIRE(f.x == Catch::Approx(0.0f).margin(1e-6f));
    REQUIRE(f.y == Catch::Approx(0.0f).margin(1e-6f));
    REQUIRE(f.z == Catch::Approx(-1.0f).margin(1e-6f));

    const EE::Vector3 r = cam.right();
    REQUIRE(r.x == Catch::Approx(1.0f).margin(1e-6f));
    const EE::Vector3 u = cam.up();
    REQUIRE(u.y == Catch::Approx(1.0f).margin(1e-6f));
}

TEST_CASE("Camera - Positive pitch looks up", "[camera]") {
    EE::Camera cam;
    cam.addPitch(45.0f);
    const EE::Vector3 f = cam.forward();
    REQUIRE(f.y > 0.0f);
    REQUIRE(f.length() == Catch::Approx(1.0f).margin(1e-5f));

    cam.addPitch(-90.0f); // back to -45
    REQUIRE(cam.forward().y < 0.0f);
}

TEST_CASE("Camera - Positive yaw turns left (forward -Z -> -X)", "[camera]") {
    EE::Camera cam;
    cam.addYaw(90.0f);
    const EE::Vector3 f = cam.forward();
    REQUIRE(f.x == Catch::Approx(-1.0f).margin(1e-5f));
    REQUIRE(f.z == Catch::Approx(0.0f).margin(1e-5f));

    // facing -X, the right hand side is -Z
    const EE::Vector3 r = cam.right();
    REQUIRE(r.z == Catch::Approx(-1.0f).margin(1e-5f));
    REQUIRE(r.x == Catch::Approx(0.0f).margin(1e-5f));
}

TEST_CASE("Camera - Pitch is clamped to avoid gimbal lock", "[camera]") {
    EE::Camera cam;
    cam.addPitch(1000.0f);
    REQUIRE(cam.pitchDeg() <= 89.9f);
    cam.addPitch(-1000.0f);
    REQUIRE(cam.pitchDeg() >= -89.9f);
}

TEST_CASE("Camera - Yaw wraps around", "[camera]") {
    EE::Camera cam;
    cam.addYaw(179.0f);
    cam.addYaw(179.0f);
    REQUIRE(cam.yawDeg() >= -180.0f);
    REQUIRE(cam.yawDeg() < 180.0f);
    // -179 + (-179) must wrap to +2
    EE::Camera cam2;
    cam2.addYaw(-179.0f);
    cam2.addYaw(-179.0f);
    REQUIRE(cam2.yawDeg() == Catch::Approx(2.0f).margin(1e-3f));
}

TEST_CASE("Camera - Axes are orthonormal", "[camera]") {
    EE::Camera cam{EE::Vector3(3, 4, 5), 17.0f, -63.0f};
    const EE::Vector3 f = cam.forward();
    const EE::Vector3 r = cam.right();
    const EE::Vector3 u = cam.up();

    REQUIRE(f.length() == Catch::Approx(1.0f).margin(1e-5f));
    REQUIRE(r.length() == Catch::Approx(1.0f).margin(1e-5f));
    REQUIRE(u.length() == Catch::Approx(1.0f).margin(1e-5f));
    REQUIRE(f.dot(r) == Catch::Approx(0.0f).margin(1e-5f));
    REQUIRE(f.dot(u) == Catch::Approx(0.0f).margin(1e-5f));
    REQUIRE(r.dot(u) == Catch::Approx(0.0f).margin(1e-5f));
}

TEST_CASE("Camera - View matrix translates world by -position", "[camera]") {
    EE::Camera cam;
    cam.setPosition(EE::Vector3(10, 0, 0));
    const EE::Matrix4 view = cam.view();
    const EE::Vector3 p = view * EE::Vector3(10, 0, 0);
    REQUIRE(p == EE::Vector3(0, 0, 0));
}

TEST_CASE("Camera - View matrix rotates with yaw", "[camera]") {
    EE::Camera cam;
    cam.addYaw(90.0f); // looking along -X now
    const EE::Matrix4 view = cam.view();
    // a point straight ahead (towards -X) must appear in front of the camera
    const EE::Vector3 ahead = view * EE::Vector3(-5, 0, 0);
    REQUIRE(ahead.z == Catch::Approx(-5.0f).margin(1e-4f));
    REQUIRE(std::abs(ahead.x) < 1e-4f);
}

TEST_CASE("Camera - Projection matches Matrix4::perspective", "[camera]") {
    EE::Camera cam;
    cam.setFovYDeg(70.0f);
    cam.setClipPlanes(0.5f, 200.0f);
    const EE::Matrix4 proj = cam.projection(16.0f / 9.0f);
    const EE::Matrix4 expected = EE::Matrix4::perspective(70.0f, 16.0f / 9.0f, 0.5f, 200.0f);
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            REQUIRE(proj.m[r][c] == Catch::Approx(expected.m[r][c]).margin(1e-6f));
}

TEST_CASE("Camera - ViewProjection sees the object it looks at", "[camera]") {
    // Camera above and behind the origin looking slightly down.
    EE::Camera cam{EE::Vector3(0, 2, 5), -20.0f, 0.0f};
    const EE::Matrix4 vp = cam.viewProjection(1280.0f / 720.0f);

    const EE::Vector4 clip = vp * EE::Vector4(0, 0, 0, 1.0f);
    REQUIRE(clip.w > 0.0f); // in front of the camera, not clipped

    const float ndcX = clip.x / clip.w;
    const float ndcY = clip.y / clip.w;
    const float ndcZ = clip.z / clip.w;
    REQUIRE(std::abs(ndcX) < 1.0f);
    REQUIRE(std::abs(ndcY) < 1.0f);
    REQUIRE(ndcZ >= -1.0f);
    REQUIRE(ndcZ <= 1.0f);
}

TEST_CASE("Camera - Points behind camera fall outside clip volume", "[camera]") {
    EE::Camera cam;
    const EE::Matrix4 vp = cam.viewProjection(1.0f);

    const EE::Vector4 clip = vp * EE::Vector4(0, 0, 5, 1.0f); // behind (camera looks -Z)
    REQUIRE(clip.w < 0.0f);
}

TEST_CASE("Camera - Wider fov projects more of the scene", "[camera]") {
    EE::Camera cam{EE::Vector3(0, 0, 5), 0.0f, 0.0f};

    // a point slightly off-axis: visible with a wide fov, invisible with a narrow one
    const EE::Vector4 point(0, 3.5f, 0, 1.0f);

    cam.setFovYDeg(110.0f);
    const EE::Vector4 wide = cam.viewProjection(1.0f) * point;
    REQUIRE(std::abs(wide.y / wide.w) < 1.0f);

    cam.setFovYDeg(20.0f);
    const EE::Vector4 narrow = cam.viewProjection(1.0f) * point;
    REQUIRE(std::abs(narrow.y / narrow.w) > 1.0f);
}

TEST_CASE("Camera - Move accumulates position", "[camera]") {
    EE::Camera cam;
    cam.setPosition(EE::Vector3(1, 2, 3));
    cam.move(EE::Vector3(0, 0, -4));
    REQUIRE(cam.position() == EE::Vector3(1, 2, -1));
}
