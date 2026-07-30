// ISC License
//
// Copyright (c) 2026 EblanTechnologies team
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

#include <cstddef>
#include <GLFW/glfw3.h>

#include "../core/ReflectionMacro.hpp"

export module EE.Render;

import std;
import EE.Math.Vector;
import EE.Math.Quaternion;
import EE.Math.Matrix;
import EE.Core.Reflection;

export namespace EE::Render {
    using std::uint32_t;

    struct MeshHandle      { uint32_t id = 0; };
    struct MaterialHandle  { uint32_t id = 0; };
    struct TextureHandle   { uint32_t id = 0; };
    struct ShaderHandle    { uint32_t id = 0; };
    struct PipelineHandle  { uint32_t id = 0; };
    struct BufferHandle    { uint32_t id = 0; };

    export struct DrawCall {
        Matrix4 world = EE::Matrix4::identity();
        MeshHandle mesh;
        MaterialHandle material;
        uint32_t submeshIndex = 0;
        uint32_t layerMask = 0xFFFFFFFF;
    };

    export struct RenderScene {
        std::vector<DrawCall> opaque;
        std::vector<DrawCall> transparent;
        std::vector<DrawCall> shadowCasters;
    };

    export class IDevice {
    public:
        virtual ~IDevice() = default;

        virtual MeshHandle createMesh(const void* vertices, size_t vertexSize, uint32_t vertexCount, const uint32_t* indices, size_t indexCount) = 0 ;
        virtual MaterialHandle createMaterial(ShaderHandle shader) = 0;
        virtual ShaderHandle createShader(const char* vsSource, const char* fsSource) = 0;
        virtual PipelineHandle createPipeline(ShaderHandle shader) = 0;

        virtual void beginFrame() = 0;
        virtual void draw(const RenderScene& scene, const EE::Matrix4& viewProj) = 0;
        virtual void endFrame() = 0;

        virtual void setViewport(int width, int height) = 0;
    };

    export std::unique_ptr<IDevice> createOpenGLDevice(GLFWwindow* window);
}


REFLECT_COMPONENT(EE::Render::MeshHandle,      t.fields = {{"ID", offsetof(EE::Render::MeshHandle,      id), &EE::getTypeInfo<uint32_t>()}};)
REFLECT_COMPONENT(EE::Render::MaterialHandle,  t.fields = {{"ID", offsetof(EE::Render::MaterialHandle,  id), &EE::getTypeInfo<uint32_t>()}};)
REFLECT_COMPONENT(EE::Render::TextureHandle,   t.fields = {{"ID", offsetof(EE::Render::TextureHandle,   id), &EE::getTypeInfo<uint32_t>()}};)
REFLECT_COMPONENT(EE::Render::ShaderHandle,    t.fields = {{"ID", offsetof(EE::Render::ShaderHandle,    id), &EE::getTypeInfo<uint32_t>()}};)
REFLECT_COMPONENT(EE::Render::PipelineHandle,  t.fields = {{"ID", offsetof(EE::Render::PipelineHandle,  id), &EE::getTypeInfo<uint32_t>()}};)
REFLECT_COMPONENT(EE::Render::BufferHandle,    t.fields = {{"ID", offsetof(EE::Render::BufferHandle,    id), &EE::getTypeInfo<uint32_t>()}};)