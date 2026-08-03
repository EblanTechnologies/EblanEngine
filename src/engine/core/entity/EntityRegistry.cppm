module;

#include <cassert>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

export module EE.Core.EntityRegistry;

import EE.Core.Entity;

export namespace EE {
    class EntityRegistry {
        uint32_t m_nextIndex = 1;
        std::vector<uint32_t> m_generations;
        std::unordered_map<size_t, std::unique_ptr<IComponentArray>> m_components;

    public:
        EntityRegistry() { m_generations.push_back(0); }

        EntityId create() {
            const uint32_t index = m_nextIndex++;
            if (index >= m_generations.size()) m_generations.push_back(0);
            const uint32_t gen = m_generations[index];
            return MakeEntityId(index,gen);
        }

        void destroy(EntityId id) {
            const uint32_t index = EntityIndex(id);
            if (index >= m_generations.size()) return;
            if (EntityGeneration(id) != m_generations[index]) return;

            for (auto& [_, arr] : m_components) arr -> remove(id);

            m_generations[index]++;
        }

        [[nodiscard]] bool alive(const EntityId id) const {
            const uint32_t index = EntityIndex(id);
            return index < m_generations.size() && EntityGeneration(id) == m_generations[index];
        }

        template <typename T, typename... Args>
        T& add(EntityId id, Args&&... args) {
            assert(alive(id));
            auto& arr = getOrCreateArray<T>();
            T comp(std::forward<Args>(args)...);
            arr.add(id, std::move(comp));
            return *arr.get(id);
        }

        template<typename T>
        void remove(const EntityId id) {
            if (const auto it = m_components.find(typeid(T).hash_code()); it != m_components.end()) it->second->remove(id);
        }

        template<typename T>
        T* get(EntityId id) {
            const auto it = m_components.find(typeid(T).hash_code());
            if (it == m_components.end()) return nullptr;
            return static_cast<ComponentArray<T>*>(it->second.get())->get(id);
        }

        template<typename T>
        const T* get(EntityId id) const {
            const auto it = m_components.find(typeid(T).hash_code());
            if (it == m_components.end()) return nullptr;
            return static_cast<const ComponentArray<T>*>(it->second.get())->get(id);
        }

        template<typename T, typename... Args>
        T& getOrAdd(EntityId id, Args&&... args) {
            if (auto* ptr = get<T>(id)) return *ptr;
            return add<T>(id, std::forward<Args>(args)...);
        }

        template<typename T>
        [[nodiscard]] bool has(const EntityId id) const {
            const auto it = m_components.find(typeid(T).hash_code());
            return it != m_components.end() && it->second->has(id);
        }

        template<typename... Components>
        class View {
            EntityRegistry* m_registry;
            ComponentArray<std::tuple_element_t<0, std::tuple<Components...>>>* m_first = nullptr;

        public:
            explicit View(EntityRegistry* registry) : m_registry(registry) {
                m_first = m_registry->template getArray<std::tuple_element_t<0, std::tuple<Components...>>>();
            }

            struct Iterator {
                ComponentArray<std::tuple_element_t<0, std::tuple<Components...>>>* arr;
                size_t index = 0;
                EntityRegistry* registry{};

                bool operator!=(const Iterator& other) const { return index != other.index; }
                void operator++() { ++index; }

                auto operator*() const {
                    EntityId entity = arr->entity[index];
                    return std::tuple<EntityId, Components*...>(entity, registry->get<Components>(entity)...);
                }
            };

            Iterator begin() { return {.arr = m_first, .index = 0, .registry = m_registry}; }
            Iterator end() { return {.arr = m_first, .index = m_first ? m_first->size() : 0, .registry = m_registry}; }

            Iterator begin() const { return {.arr = m_first, .index = 0, .registry = m_registry}; }
            Iterator end() const { return {.arr = m_first, .index = m_first ? m_first->size() : 0, .registry = m_registry}; }
        };

        template<typename... Components>
        View<Components...> view() { return View<Components...>(this); }

    private:
        template<typename T>
        ComponentArray<T>* getArray() {
            const auto it = m_components.find(typeid(T).hash_code());
            return it != m_components.end() ? static_cast<ComponentArray<T>*>(it->second.get()) : nullptr;
        }

        template<typename T>
        ComponentArray<T>& getOrCreateArray() {
            size_t key = typeid(T).hash_code();
            if (const auto it = m_components.find(key); it != m_components.end()) return *static_cast<ComponentArray<T>*>(it->second.get());

            auto arr = std::make_unique<ComponentArray<T>>();
            auto* ptr = arr.get();
            m_components.emplace(key, std::move(arr));
            return *ptr;
        }
    };

    inline EntityRegistry* getEntityRegistry() {
        static auto* g_registry = new EntityRegistry();
        return g_registry;
    }
}
