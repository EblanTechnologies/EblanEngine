

#include <GLFW/glfw3.h>

import std;
import EE.Core.Entity;
import EE.Core.EntityRegistry;
import EE.Core.Transform;
import EE.Render;
import EE.Render.MeshRenderer;
import EE.Render.RendererWorld;
import EE.Math.Matrix;

constexpr int W_WIDTH = 1280, W_HEIGHT = 720;

struct Vertex { float pos[3]; float normal[3]; float uv[2]; };

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "EblanEngine test render", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    const auto device = EE::Render::createOpenGLDevice(window);
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

    constexpr auto vs = R"(
        #version 460 core
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

    constexpr auto fs = R"(
        #version 460 core
        in vec3 vNormal;
        out vec4 FragColor;

        void main() {
            vec3 norm = normalize(vNormal);
            vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
        
            float diff = max(dot(norm, lightDir), 0.2);
        
            FragColor = vec4(vec3(1.0, 0.5, 0.2) * diff, 1.0);
        }
    )";

    const EE::Render::ShaderHandle shader = device->createShader(vs, fs);
    EE::Render::MaterialHandle material = device->createMaterial(shader);

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

    const EE::Matrix4 proj = EE::Matrix4::perspective(60, static_cast<float>(W_WIDTH) / static_cast<float>(W_HEIGHT), 0.1f, 100.0f);
    const EE::Matrix4 view = EE::Matrix4::lookAt({ 0.0f, 2.0f, 5.0f }, { 0, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
    const EE::Matrix4 viewProj = proj * view;

    float angle = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        const auto time = static_cast<float>(glfwGetTime());
        const auto scaleFactor = 1.15f + std::sin(time * 7.0f) * 0.15f;
        const auto transformFactor = 0.25f + std::sin(time * 7.0f) * 0.25f;

        angle += 0.5f;
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

        renderWorld.extract(registry);
        device->beginFrame();
        device->draw(renderWorld.getScene(), viewProj);
        device->endFrame();
    }

    glfwTerminate();
    return 0;
}