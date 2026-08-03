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
#include "ReflectionMacro.hpp"

export module EE.Core.Hierarchy;

import std;
import EE.Core.Entity;
import EE.Core.EntityRegistry;
import EE.Core.Reflection;
import EE.Core.Transform;
import EE.Math.Matrix;

export namespace EE {
    struct Parent {
        EntityId entity = NULL_ENTITY;
    };
    REFLECT_COMPONENT(Parent, t.fields = {{"Entity", offsetof(Parent, entity), &EE::getTypeInfo<EntityId>()}};)

    struct Children {
        std::vector<EntityId> entities;
    };
    REFLECT_COMPONENT(Children, t.fields = {{"Entities", offsetof(Children, entities), &EE::getTypeInfo<std::vector<EntityId>>()}};)

    class HierarchySystem {
    public:
        static void update(EntityRegistry& registry) {
            auto view = registry.view<Transform, Parent>();
            for (auto [entity, transform, parent] : view) {
                if (parent && parent->entity != NULL_ENTITY && registry.alive(parent->entity)) {
                    continue;
                }
                updateRecursive(registry, entity, Matrix4::identity());
            }

            auto rootsNoParent = registry.view<Transform>();
            for (auto [entity, transform] : rootsNoParent) {
                if (!registry.has<Parent>(entity)) {
                    updateRecursive(registry, entity, Matrix4::identity());
                }
            }
        }

        static void onParentChanged(EntityRegistry& registry, const EntityId child, const EntityId oldParent, const EntityId newParent) {
            if (oldParent != NULL_ENTITY && registry.alive(oldParent)) {
                if (auto* children = registry.get<Children>(oldParent)) {
                    auto& vec = children->entities;
                    std::erase(vec, child);
                }
            }

            if (newParent != NULL_ENTITY && registry.alive(newParent)) {
                auto& children = registry.getOrAdd<Children>(newParent);
                children.entities.push_back(child);
            }
            markSubtreeDirty(registry, child);
        }

        static void onTransformChanged(EntityRegistry& registry, const EntityId entity) {
            markSubtreeDirty(registry, entity);
        }
    private:
        static void updateRecursive(EntityRegistry& registry, const EntityId entity, const Matrix4& parentWorld) {
            const auto* transform = registry.get<Transform>(entity);
            if (!transform) return;

            const Matrix4 local = Matrix4::trs(transform->position, transform->rotation, transform->scale);
            transform->worldMatrix = parentWorld * local;
            transform->dirty = false;

            if (const auto* children = registry.get<Children>(entity)) {
                for (const EntityId child : children->entities) {
                    if (registry.alive(child)) {
                        updateRecursive(registry, child, transform->getWorldMatrix());
                    }
                }
            }
        }

        static void markSubtreeDirty(EntityRegistry& registry, const EntityId entity) {
            if (const auto* transform = registry.get<Transform>(entity)) {
                transform->dirty = true;
            }

            if (const auto* children = registry.get<Children>(entity)) {
                for (const EntityId child : children->entities) {
                    if (registry.alive(child)) markSubtreeDirty(registry, child);
                }
            }
        }
    };
}