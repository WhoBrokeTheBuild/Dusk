#ifndef DUSK_JSON_HPP
#define DUSK_JSON_HPP

#include <Dusk/Math.hpp>
#include <Dusk/String.hpp>

#include <Dusk/ThirdParty/nlohmann_json.hpp>

namespace dusk {

namespace JSON {

using Object = nlohmann::json;

using Pointer = nlohmann::json_pointer<String>;

inline vec2 ToVec2(const Object& value, vec2 def)
{
    if (value.is_array() && value.size() == 2) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec2(v.data());
    }

    return def;
}

inline vec3 ToVec3(const Object& value, vec3 def)
{
    if (value.is_array() && value.size() == 3) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec3(v.data());
    }

    return def;
}

inline vec4 ToVec4(const Object& value, vec4 def)
{
    if (value.is_array() && value.size() == 4) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec4(v.data());
    }

    return def;
}

inline quat ToQuat(const Object& value, quat def)
{
    if (value.is_array() && value.size() == 4) {
        const auto& v = value.get<std::vector<float>>();
        return glm::quat(v[3], v[0], v[1], v[2]);
    }

    return def;
}

} // namespace JSON

} // namespace dusk

#endif // DUSK_JSON_HPP