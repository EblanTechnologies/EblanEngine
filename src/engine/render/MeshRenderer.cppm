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

#include <cstddef>
#include "../core/ReflectionMacro.hpp"

export module EE.Render.MeshRenderer;

import std;
import EE.Render;
import EE.Core.Reflection;

export namespace EE {
    using std::uint32_t;
    struct MeshRenderer {
        EE::Render::MeshHandle mesh = {0};
        std::vector<EE::Render::MaterialHandle> materials;
        uint32_t layerMask = 0xFFFFFFFF;
        bool castShadows = true;
        bool enabled = true;
    };
}

REFLECT_COMPONENT(EE::MeshRenderer,
    t.fields = {
        {"Mesh",       offsetof(EE::MeshRenderer, mesh),       &EE::getTypeInfo<EE::Render::MeshHandle>()},
        {"Materials",  offsetof(EE::MeshRenderer, materials),  &EE::getTypeInfo<std::vector<EE::Render::MaterialHandle>>()},
        {"Layer Mask", offsetof(EE::MeshRenderer, layerMask),  &EE::getTypeInfo<std::uint32_t>()},
        {"Cast Shadows", offsetof(EE::MeshRenderer, castShadows), &EE::getTypeInfo<bool>()},
        {"Enabled",    offsetof(EE::MeshRenderer, enabled),    &EE::getTypeInfo<bool>()},
    };
)