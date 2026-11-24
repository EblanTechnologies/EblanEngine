#include <catch2/catch_test_macros.hpp>
#include <cstddef>

import EE.Core.Entity;
import EE.Core.EntityRegistry;
import EE.Core.Transform;
import EE.Render.MeshRenderer;

namespace {
    // Internal-linkage component type: instantiating imported registry
    // templates with it exercises the same code path engine users would take.
    struct Health {
        float value = 100.0f;
    };
}

TEST_CASE("componentTypeKey - stable, distinct, non-zero", "[ecs]") {
    const std::size_t a1 = EE::componentTypeKey<EE::Transform>();
    const std::size_t a2 = EE::componentTypeKey<EE::Transform>();
    const std::size_t b = EE::componentTypeKey<EE::MeshRenderer>();
    const std::size_t c = EE::componentTypeKey<Health>();

    REQUIRE(a1 == a2);
    REQUIRE(a1 != b);
    REQUIRE(a1 != c);
    REQUIRE(b != c);
    REQUIRE(a1 != 0);
}

TEST_CASE("EntityRegistry - create/alive/destroy lifecycle", "[ecs]") {
    EE::EntityRegistry registry;
    const EE::EntityId e = registry.create();
    REQUIRE(registry.alive(e));
    REQUIRE(e != EE::NULL_ENTITY);

    registry.destroy(e);
    REQUIRE_FALSE(registry.alive(e));
}

TEST_CASE("EntityRegistry - add/get/has/remove component", "[ecs]") {
    EE::EntityRegistry registry;
    const EE::EntityId e = registry.create();
    const EE::EntityId other = registry.create();

    REQUIRE_FALSE(registry.has<EE::Transform>(e));

    auto& written = registry.add<EE::Transform>(e);
    written.setPosition({1.0f, 2.0f, 3.0f});

    REQUIRE(registry.has<EE::Transform>(e));
    REQUIRE_FALSE(registry.has<EE::Transform>(other));

    const auto* read = registry.get<EE::Transform>(e);
    REQUIRE(read != nullptr);
    REQUIRE(read->position.x == 1.0f);
    REQUIRE(read->position.y == 2.0f);
    REQUIRE(read->position.z == 3.0f);

    REQUIRE(registry.get<EE::Transform>(other) == nullptr);

    registry.remove<EE::Transform>(e);
    REQUIRE_FALSE(registry.has<EE::Transform>(e));
    REQUIRE(registry.get<EE::Transform>(e) == nullptr);
}

TEST_CASE("EntityRegistry - getOrAdd returns existing component", "[ecs]") {
    EE::EntityRegistry registry;
    const EE::EntityId e = registry.create();

    auto& first = registry.add<EE::Transform>(e);
    first.setPosition({5.0f, 0.0f, 0.0f});

    auto& second = registry.getOrAdd<EE::Transform>(e);
    REQUIRE(&first == &second);
    REQUIRE(second.position.x == 5.0f);
}

TEST_CASE("EntityRegistry - view iterates the first component's array", "[ecs]") {
    // Documented engine semantics (not classic sparse-set intersection):
    // View<A, B, ...> walks the array of the FIRST component and yields
    // nullptr for the remaining components when an entity lacks them.
    // RenderWorld::extract and HierarchySystem::update rely on this by
    // null-checking components.
    //
    // This is also exactly the instantiation pattern that broke the updated
    // MSVC: View<Transform, MeshRenderer>::Iterator::operator* ->
    // EntityRegistry::get<T>. The registry templates are instantiated HERE,
    // in this translation unit, and must not require <typeinfo> to do so.
    EE::EntityRegistry registry;

    const EE::EntityId a = registry.create();
    registry.add<EE::Transform>(a);
    registry.add<EE::MeshRenderer>(a);

    const EE::EntityId b = registry.create();
    registry.add<EE::Transform>(b);
    registry.add<EE::MeshRenderer>(b);

    const EE::EntityId c = registry.create();
    registry.add<EE::Transform>(c); // no MeshRenderer -> renderer == nullptr

    std::size_t count = 0;
    bool sawA = false, sawB = false, sawC = false;
    for (auto [entity, transform, renderer] : registry.view<EE::Transform, EE::MeshRenderer>()) {
        ++count;
        sawA = sawA || (entity == a);
        sawB = sawB || (entity == b);
        sawC = sawC || (entity == c);

        REQUIRE(transform != nullptr); // first component: always present

        if (entity == c) {
            REQUIRE(renderer == nullptr);
        } else {
            REQUIRE(renderer != nullptr);
        }
    }

    REQUIRE(count == 3);
    REQUIRE(sawA);
    REQUIRE(sawB);
    REQUIRE(sawC);
}

TEST_CASE("EntityRegistry - destroy removes components from recycled slot", "[ecs]") {
    EE::EntityRegistry registry;
    const EE::EntityId e = registry.create();
    registry.add<EE::Transform>(e);
    registry.add<EE::MeshRenderer>(e);

    registry.destroy(e);

    const EE::EntityId recycled = registry.create(); // index slot is reused
    REQUIRE(recycled != e);                          // but the generation differs
    REQUIRE_FALSE(registry.has<EE::Transform>(recycled));
    REQUIRE_FALSE(registry.has<EE::MeshRenderer>(recycled));
}

TEST_CASE("EntityRegistry - view on empty registry yields nothing", "[ecs]") {
    EE::EntityRegistry registry;
    std::size_t count = 0;
    for (auto [entity, transform, renderer] : registry.view<EE::Transform, EE::MeshRenderer>()) {
        (void)entity;
        (void)transform;
        (void)renderer;
        ++count;
    }
    REQUIRE(count == 0);
}
