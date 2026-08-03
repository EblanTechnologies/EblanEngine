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

export module EE.Render.RendererWorld;

import std;
import EE.Core.Entity;
import EE.Core.EntityRegistry;
import EE.Core.Transform;
import EE.Render;
import EE.Render.MeshRenderer;
import EE.Math.Matrix;

export namespace EE::Render {
    using std::uint32_t;
    class RenderWorld {
        RenderScene m_scene;

    public:
        void extract(EntityRegistry& registry) {
            m_scene.opaque.clear();
            m_scene.transparent.clear();
            m_scene.shadowCasters.clear();

            auto view = registry.view<Transform, MeshRenderer>();
            for (auto [entity, transform, meshRenderer] : view) {
                if (!meshRenderer || !transform) continue;
                if (!meshRenderer->enabled) continue;

                const Matrix4& world = transform->getWorldMatrix();
                for (uint32_t i = 0; i < meshRenderer->materials.size(); ++i) {
                    DrawCall dCall;
                    dCall.world = world;
                    dCall.mesh = meshRenderer->mesh;
                    dCall.material = meshRenderer->materials[i];
                    dCall.submeshIndex = i;
                    dCall.layerMask = meshRenderer->layerMask;

                    m_scene.opaque.push_back(dCall);
                    if (meshRenderer->castShadows) m_scene.shadowCasters.push_back(dCall);
                }
            }
        }

        [[nodiscard]] const RenderScene& getScene() const { return m_scene; }
    };
}