#ifndef DUSK_MATH_HPP
#define DUSK_MATH_HPP

#include <Dusk/ThirdParty/glm.hpp>

#include <limits>

namespace dusk {

typedef uint32_t uint;

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::ivec2;
using glm::ivec3;
using glm::ivec4;

using glm::uvec2;
using glm::uvec3;
using glm::uvec4;

using glm::mat2;
using glm::mat3;
using glm::mat4;

using glm::quat;

template <typename T>
inline constexpr T ToRadians(T degrees)
{
    return glm::radians(degrees);
}

template <typename T>
inline constexpr T ToDegrees(T radians)
{
    return glm::degrees(radians);
}

template <typename T>
constexpr float NormalizeInteger(T value)
{
    return (
        value < 0
        ? -static_cast<float>(value) / static_cast<float>(std::numeric_limits<T>::min())
        :  static_cast<float>(value) / static_cast<float>(std::numeric_limits<T>::min())
    );
}

} // namespace dusk

#endif // DUSK_MATH_HPP