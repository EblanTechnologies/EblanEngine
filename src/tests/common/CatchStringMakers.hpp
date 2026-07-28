#pragma once

#include <catch2/catch_tostring.hpp>
#include <string>
#include <format>

import EE.Math.Vector;

namespace Catch {
    template<>
    struct StringMaker<EE::Vector3> {
        static std::string convert(const EE::Vector3& v) {
            return std::format("({}, {}, {})", v.x, v.y, v.z);
        }
    };

}