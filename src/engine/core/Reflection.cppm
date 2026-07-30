module;

#include <cfloat>
#include <vector>

export module EE.Core.Reflection;

export namespace EE {
    struct TypeInfo {
        const char* name;
        size_t size;
        size_t alignment;
        struct Field {
            const char* name;
            size_t offset;
            const TypeInfo* type;
            float min = -FLT_MAX, max = FLT_MAX;
            const char* tooltip = "";
        };
        std::vector<Field> fields;
        void* (*create)() = nullptr;
        void (*destroy)(void*) = nullptr;
    };

    template<typename T>
    inline const TypeInfo& getTypeInfo() {
        static TypeInfo info = {
            .name = typeid(T).name(),
            .size = sizeof(T),
            .alignment = alignof(T),
            .fields = {},
            .create = []() -> void* {
                if constexpr (std::is_default_constructible_v<T>) {
                    return new T();
                } else {
                    return nullptr;
                }
            },
            .destroy = [](void* p) {
                if constexpr (std::is_destructible_v<T>) {
                    delete static_cast<T*>(p);
                }
            }
        };
        return info;
    }
}
