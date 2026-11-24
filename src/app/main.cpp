

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

import EE.Core.Entity;
import EE.Core.EntityRegistry;
import EE.Core.Transform;
import EE.Core.Camera;
import EE.Render;
import EE.Render.MeshRenderer;
import EE.Render.RendererWorld;
import EE.Core.Hierarchy;
import EE.Math.Matrix;
import EE.Math.Vector;

constexpr int W_WIDTH = 1280, W_HEIGHT = 720;

struct Vertex { float pos[3]; float normal[3]; float uv[2]; };

int main() {
    glfwSetErrorCallback([](int code, const char* description) {
        fprintf(stderr, "GLFW error %d: %s\n", code, description);
    });

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }

    GLFWwindow* window = nullptr;
    for (int minor : {6, 5, 4, 3, 2, 1, 0}) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "EblanEngine test render", nullptr, nullptr);
        if (window) break;
    }

    if (!window) {
        fprintf(stderr, "Failed to create OpenGL window\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    const auto device = EE::Render::createOpenGLDevice(window);
    device->setViewport(W_WIDTH, W_HEIGHT);

    EE::EntityRegistry registry;
    EE::Render::RenderWorld renderWorld;

    const Vertex cubeVertices[] = {
        {{-0.5f, -0.5f,  0.5f}, {0, 0, 1}, {0, 0}},
        {{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}, {1, 0}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}, {1, 1}},
        {{-0.5f,  0.5f,  0.5f}, {0, 0, 1}, {0, 1}},

        {{ 0.5f, -0.5f, -0.5f}, {0, 0,-1}, {0, 0}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0,-1}, {1, 0}},
        {{-0.5f,  0.5f, -0.5f}, {0, 0,-1}, {1, 1}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0,-1}, {0, 1}},

        {{-0.5f,  0.5f,  0.5f}, {0, 1, 0}, {0, 0}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 1, 0}, {1, 0}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 1, 0}, {1, 1}},
        {{-0.5f,  0.5f, -0.5f}, {0, 1, 0}, {0, 1}},

        {{-0.5f, -0.5f, -0.5f}, {0,-1, 0}, {0, 0}},
        {{ 0.5f, -0.5f, -0.5f}, {0,-1, 0}, {1, 0}},
        {{ 0.5f, -0.5f,  0.5f}, {0,-1, 0}, {1, 1}},
        {{-0.5f, -0.5f,  0.5f}, {0,-1, 0}, {0, 1}},

        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}, {0, 0}},
        {{ 0.5f, -0.5f, -0.5f}, {1, 0, 0}, {1, 0}},
        {{ 0.5f,  0.5f, -0.5f}, {1, 0, 0}, {1, 1}},
        {{ 0.5f,  0.5f,  0.5f}, {1, 0, 0}, {0, 1}},

        {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0, 0}},
        {{-0.5f, -0.5f,  0.5f}, {-1, 0, 0}, {1, 0}},
        {{-0.5f,  0.5f,  0.5f}, {-1, 0, 0}, {1, 1}},
        {{-0.5f,  0.5f, -0.5f}, {-1, 0, 0}, {0, 1}}
    };

    const uint32_t cubeIndices[] = {
        0, 1, 2,   2, 3, 0,
        4, 5, 6,   6, 7, 4,
        8, 9, 10,  10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    EE::Render::MeshHandle cubeMesh = device->createMesh(cubeVertices, sizeof(Vertex), sizeof(cubeVertices) / sizeof(Vertex), cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));

    const int glMajor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    const int glMinor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    char versionLine[32];
    snprintf(versionLine, sizeof(versionLine), "#version %d%02d core\n", glMajor, glMinor * 10);

    constexpr auto vsBody = R"(
        layout(location=0) in vec3 aPos;
        layout(location=1) in vec3 aNormal;

        out vec3 vNormal;

        uniform mat4 uWorld;
        uniform mat4 uViewProj;

        void main() {
            vNormal = mat3(uWorld) * aNormal;
            gl_Position = uViewProj * uWorld * vec4(aPos, 1.0);
        }
    )";

    constexpr auto fsBody = R"(
        in vec3 vNormal;
        out vec4 FragColor;

        void main() {
            vec3 norm = normalize(vNormal);
            vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));

            float diff = max(dot(norm, lightDir), 0.2);

            FragColor = vec4(vec3(1.0, 0.5, 0.2) * diff, 1.0);
        }
    )";

    const std::string vs = std::string(versionLine) + vsBody;
    const std::string fs = std::string(versionLine) + fsBody;

    const EE::Render::ShaderHandle shader = device->createShader(vs.c_str(), fs.c_str());
    const EE::Render::MaterialHandle material = device->createMaterial(shader);

    const auto parent = registry.create();
    registry.add<EE::Transform>(parent).setPosition({15, 0, -6});
    registry.add<EE::MeshRenderer>(parent, cubeMesh, std::vector{ material });

    const auto child = registry.create();
    registry.add<EE::Transform>(child).setPosition({-5, 0, 0});
    registry.add<EE::MeshRenderer>(child, cubeMesh, std::vector{ material });
    registry.add<EE::Parent>(child, parent);

    EE::HierarchySystem::onParentChanged(registry, child, EE::NULL_ENTITY, parent);

    std::vector<EE::EntityId> entities;
    for (int index = 0; index < 3; index++) {
        float x = -2.0f + static_cast<float>(index) * 2.0f;

        const auto e = registry.create();

        auto& transform = registry.add<EE::Transform>(e);

        transform.setPosition({ x, 0, 0 });
        transform.setScale({ 1.0f, 1.0f, 1.0f });

        registry.add<EE::MeshRenderer>(e, cubeMesh, std::vector{ material });

        entities.push_back(e);
    }

    // --- Camera setup ---
    // Right-handed world: +X right, +Y up, cubes near the origin.
    // Start behind the cubes, looking slightly down at them.
    EE::Camera camera{EE::Vector3(0.0f, 2.0f, 5.0f), -20.0f, 0.0f};
    camera.setClipPlanes(0.1f, 100.0f);
    camera.setFovYDeg(60.0f);

    float aspect = static_cast<float>(W_WIDTH) / static_cast<float>(W_HEIGHT);

    glfwSetWindowUserPointer(window, &camera);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        if (width > 0 && height > 0) glViewport(0, 0, width, height);
    });
    glfwSetScrollCallback(window, [](GLFWwindow* win, double, double yOffset) {
        auto* cam = static_cast<EE::Camera*>(glfwGetWindowUserPointer(win));
        cam->setFovYDeg(std::clamp(cam->fovYDeg() + static_cast<float>(-yOffset) * 2.0f, 20.0f, 120.0f));
    });

    double lastTime = glfwGetTime();
    double lastMouseX = 0.0, lastMouseY = 0.0;
    bool lookActive = false;
    constexpr float MOVE_SPEED = 4.0f;      // units per second
    constexpr float LOOK_SPEED = 0.25f;     // degrees per pixel

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        const double now = glfwGetTime();
        const float dt = static_cast<float>(now - lastTime);
        lastTime = now;

        // --- camera input ---
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);

        const bool rightPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
        double mouseX = 0.0, mouseY = 0.0;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        if (rightPressed && lookActive) {
            camera.addYaw(-static_cast<float>(mouseX - lastMouseX) * LOOK_SPEED);
            camera.addPitch(-static_cast<float>(mouseY - lastMouseY) * LOOK_SPEED);
        }
        lookActive = rightPressed;
        lastMouseX = mouseX;
        lastMouseY = mouseY;

        const EE::Vector3 flatForward = camera.forward().normalized();
        const EE::Vector3 flatRight = camera.right().normalized();
        EE::Vector3 velocity{0.0f, 0.0f, 0.0f};
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) velocity += flatForward;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) velocity -= flatForward;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) velocity += flatRight;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) velocity -= flatRight;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) velocity += EE::Vector3(0.0f, 1.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) velocity -= EE::Vector3(0.0f, 1.0f, 0.0f);
        camera.move(velocity * (MOVE_SPEED * dt));

        const auto time = static_cast<float>(now);
        const auto scaleFactor = 1.15f + std::sin(time * 7.0f) * 0.15f;
        const auto transformFactor = 0.25f + std::sin(time * 7.0f) * 0.25f;

        static float angle = 0.0f;
        angle += 30.0f * dt;
        for (size_t i = 0; i < entities.size(); i++) {
            auto* transform = registry.get<EE::Transform>(entities[i]);
            if (!transform) continue;

            switch (i) {
                case 0:
                    transform->setPosition(transform->position.updateZ(transformFactor));
                    break;
                case 1:
                    transform->setRotationEuler({angle, angle * 0.7f, angle * 0.3f});
                    break;
                case 2:
                    transform->setScale({scaleFactor, scaleFactor, scaleFactor});
                    break;
            }
        }

        auto* parentTransform = registry.get<EE::Transform>(parent);
        parentTransform->setPosition(parentTransform->position.updateX(transformFactor * 2.0f));
        parentTransform->setRotationEuler({angle, angle * 0.7f, angle * 0.3f});

        EE::HierarchySystem::update(registry);

        int fbWidth = 0, fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        if (fbWidth > 0 && fbHeight > 0) aspect = static_cast<float>(fbWidth) / static_cast<float>(fbHeight);

        const EE::Matrix4 viewProj = camera.viewProjection(aspect);

        renderWorld.extract(registry);
        device->beginFrame();
        device->draw(renderWorld.getScene(), viewProj);
        device->endFrame();
    }

    glfwTerminate();
    return 0;
}
