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

module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

module EE.Render;

import std;
import EE.Math.Matrix;
import EE.Math.Vector;

#define BUFFER_OFFSET(i) ((void*)(uintptr_t)(i))

namespace EE::Render {
    static GLuint compileShader(const GLenum shaderType, const char* shaderSource) {
        const GLuint id = glCreateShader(shaderType);
        glShaderSource(id, 1, &shaderSource, nullptr);
        glCompileShader(id);
        GLint ok;
        glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char log[1024];
            glGetShaderInfoLog(id, 1024, nullptr, log);
            std::cerr << "Shader compilation error: " << log << '\n' << std::endl;
            glDeleteShader(id);
            return 0;
        }
        return id;
    }

    static GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId) {
        const GLuint id = glCreateProgram();
        glAttachShader(id, vertexShaderId);
        glAttachShader(id, fragmentShaderId);
        glLinkProgram(id);
        GLint ok;
        glGetProgramiv(id, GL_LINK_STATUS, &ok);
        if (!ok) {
            char log[1024];
            glGetProgramInfoLog(id, 1024, nullptr, log);
            std::cerr << "Program linking error: " << log << '\n' << std::endl;
            glDeleteProgram(id);
            return 0;
        }
        return id;
    }

    class OpenGLDevice final : public IDevice {
        GLFWwindow* m_window = nullptr;
        GLuint currentProgram = 0;

        struct MeshGL { GLuint VAO = 0; GLuint VBO = 0; GLuint EBO = 0; uint32_t indexCount = 0; };
        struct MaterialGL { GLuint program = 0; };
        std::vector<MeshGL> m_meshes;
        std::vector<MaterialGL> m_materials;

        template<typename T>
        uint32_t allocHandle(std::vector<T>& vec) {
            vec.emplace_back();
            return static_cast<uint32_t>(vec.size());
        }

    public:
        explicit OpenGLDevice(GLFWwindow* window) : m_window(window) {
            gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }

        MeshHandle createMesh(const void* vertices, const size_t vertexSize, const uint32_t vertexCount,
            const uint32_t* indices, const size_t indexCount) override {
            MeshHandle handle(allocHandle(m_meshes));
            MeshGL& mesh = m_meshes[handle.id - 1];
            mesh.indexCount = indexCount;

            glGenVertexArrays(1, &mesh.VAO);
            glGenBuffers(1, &mesh.VBO);
            glGenBuffers(1, &mesh.EBO);

            glBindVertexArray(mesh.VAO);

            glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
            glBufferData(GL_ARRAY_BUFFER, vertexSize * vertexCount, vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexCount, indices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(vertexSize), BUFFER_OFFSET(0));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(vertexSize), BUFFER_OFFSET(12));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(vertexSize), BUFFER_OFFSET(24));

            glBindVertexArray(0);
            return handle;
        }

        ShaderHandle createShader(const char* vsSource, const char* fsSource) override {
            const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
            const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);
            const GLuint program = linkProgram(vertexShader, fragmentShader);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            const ShaderHandle handle{allocHandle(m_materials)};
            m_materials[handle.id - 1].program = program;
            return handle;
        }

        MaterialHandle createMaterial(const ShaderHandle shader) override {
            return MaterialHandle{shader.id}; // (Temporary): Material is shader.
        }

        PipelineHandle createPipeline(ShaderHandle shader) override {
            return PipelineHandle{0}; // Not required for OpenGL.
        }

        void beginFrame() override {
            glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void draw(const RenderScene& scene, const EE::Matrix4& viewProj) override {
            GLuint lastProgram = 0;

            for (const auto& dc : scene.opaque) {
                if (dc.mesh.id == 0 || dc.mesh.id > m_meshes.size()) continue;
                if (dc.material.id == 0 || dc.material.id > m_materials.size()) continue;

                const MeshGL& mesh = m_meshes[dc.mesh.id - 1];
                const MaterialGL& mat = m_materials[dc.material.id - 1];

                if (mat.program == 0) continue;

                if (mat.program != lastProgram) {
                    glUseProgram(mat.program);
                    lastProgram = mat.program;

                    const GLint uViewProj = glGetUniformLocation(mat.program, "uViewProj");
                    if (uViewProj >= 0) {
                        glUniformMatrix4fv(uViewProj, 1, GL_TRUE, viewProj.data());
                    }
                }

                const GLint uWorld = glGetUniformLocation(mat.program, "uWorld");
                if (uWorld >= 0) {
                    glUniformMatrix4fv(uWorld, 1, GL_TRUE, dc.world.data());
                }

                glBindVertexArray(mesh.VAO);
                glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, nullptr);
            }

            glBindVertexArray(0);
        }

        void endFrame() override {
            glfwSwapBuffers(m_window);
        }

        void setViewport(const int width, const int height) override {
            glViewport(0, 0, width, height);
        }
    };

    std::unique_ptr<IDevice> createOpenGLDevice(GLFWwindow* window) {
        return std::make_unique<OpenGLDevice>(window);
    }
}