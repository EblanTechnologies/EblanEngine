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
    const TypeInfo& getTypeInfo();
}
