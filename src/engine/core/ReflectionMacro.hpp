#pragma once

#ifndef EBLANENGINE_REFLECTIONMACRO_HPP
#define EBLANENGINE_REFLECTIONMACRO_HPP

#define REFLECT_COMPONENT(T, ...) \
    template<> const EE::TypeInfo& EE::getTypeInfo<T>() { \
        static EE::TypeInfo info = []{ \
            EE::TypeInfo t; \
            t.name = #T; \
            t.size = sizeof(T); \
            t.alignment = alignof(T); \
            t.create = []() -> void* { return new T(); }; \
            t.destroy = [](void* p){ delete static_cast<T*>(p); }; \
            __VA_ARGS__ \
            return t; \
        }(); \
        return info; \
    }

#endif //EBLANENGINE_REFLECTIONMACRO_HPP
