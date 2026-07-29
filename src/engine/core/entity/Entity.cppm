module;

#include <cstdint>;
#include <vector>;
#include <unordered_map>;
#include <cassert>;

export module EE.Entity;

import EE.Math.Vector;

export namespace EE {
    using EntityId = uint64_t;
    constexpr EntityId NULL_ENTITY = 0;

    constexpr EntityId MakeEntityId(const uint32_t index, const uint32_t generation) {
        return (static_cast<EntityId>(generation) << 32) | index;
    }
    constexpr uint32_t EntityIndex(const EntityId id) { return static_cast<uint32_t>(id); }
    constexpr uint32_t EntityGeneration(const EntityId id) { return static_cast<uint32_t>(id >> 32); }

    struct IComponentArray {
        virtual ~IComponentArray() = default;
        virtual void remove(EntityId ) = 0;
        [[nodiscard]] virtual bool has(EntityId) const = 0;
        [[nodiscard]] virtual size_t size() const = 0;
        virtual void* getPtr(EntityId) = 0;
        [[nodiscard]] virtual const void* getPtr(EntityId) const = 0;
    };

    template<typename T>
    struct ComponentArray : IComponentArray {
        std::vector<T> dense;
        std::vector<uint32_t> sparce;
        std::vector<EntityId> entity;

        void resizeSparce(const uint32_t newSize) {
            if (sparce.size() <= newSize) sparce.resize(newSize + 1, UINT32_MAX);
        }

        void add(const EntityId id, T&& component) {
            const uint32_t index = EntityIndex(id);
            resizeSparce(index);
            assert(sparce[index] == UINT32_MAX && "Component already exists");
            sparce[index] = static_cast<uint32_t>(dense.size());
            dense.push_back(std::move(component));
            entity.push_back(id);
        }

        void remove(EntityId entityId) override {
            const uint32_t index = EntityIndex(entityId);
            if (index >= sparce.size() || sparce[index] == UINT32_MAX) return;
            uint32_t denseIndex = sparce[index];
            uint32_t lastIndex = static_cast<uint32_t>(dense.size()) - 1;
            const EntityId lastEntity = entity[lastIndex];

            dense[denseIndex] = std::move(dense[lastIndex]);
            entity[denseIndex] = lastEntity;
            sparce[EntityIndex(lastEntity)] = denseIndex;

            dense.pop_back();
            entity.pop_back();
            sparce[index] = UINT32_MAX;
        }

        [[nodiscard]] bool has(const EntityId entityId) const override {
            const uint32_t index = EntityIndex(entityId);
            return index < sparce.size() && sparce[index] != UINT32_MAX;
        }

        [[nodiscard]] size_t size() const override { return dense.size(); }

        T* get(EntityId entityId) {
            const uint32_t index = EntityIndex(entityId);
            if (index >= sparce.size() || sparce[index] == UINT32_MAX) return nullptr;
            return &dense[sparce[index]];
        }

        void* getPtr(const EntityId entityId) override { return get(entityId); }
        [[nodiscard]] const void* getPtr(EntityId entityId) const override { return get(entityId); }
    };
}
