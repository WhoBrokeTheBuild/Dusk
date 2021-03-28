#ifndef DUSK_JSON_HPP
#define DUSK_JSON_HPP

#include <Dusk/Config.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Math.hpp>

#include <nlohmann/json.hpp>

namespace Dusk {

typedef nlohmann::basic_json<
    std::map,
    std::vector,
    string,
    bool,
    int,
    unsigned,
    float,
    std::allocator> json;

inline vec2 ExtractVec2(const json& value, vec2 def)
{
    if (value.is_array() && value.size() == 2) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec2(v.data());
    }

    return def;
}

inline vec3 ExtractVec3(const json& value, vec3 def)
{
    if (value.is_array() && value.size() == 3) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec3(v.data());
    }

    return def;
}

inline vec4 ExtractVec4(const json& value, vec4 def)
{
    if (value.is_array() && value.size() == 4) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec4(v.data());
    }

    return def;
}

inline quat ExtractQuat(const json& value, quat def)
{
    if (value.is_array() && value.size() == 4) {
        const auto& v = value.get<std::vector<float>>();
        return glm::quat(v[3], v[0], v[1], v[2]);
    }

    return def;
}

} // namespace Dusk

#endif // DUSK_JSON_HPP