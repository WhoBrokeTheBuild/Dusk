#ifndef DUSK_COLOR_HPP
#define DUSK_COLOR_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Array.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/String.hpp>

namespace dusk {

namespace Color {

    // Convert a color from Linear to sRGB Color Space
    DUSK_API
    vec4 ToSRGB(const vec4& linearColor);

    // Convert a color from sRGB to Linear Color Space
    DUSK_API
    vec4 ToLinear(const vec4& srgbColor);

    constexpr inline vec4 FromInts(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
    {
        return {
            red   / 255.0f,
            green / 255.0f,
            blue  / 255.0f,
            alpha / 255.0f
        };
    }

    constexpr inline uvec4 ToInts(const vec4& color)
    {
        return uvec4(
            color.r * 255,
            color.g * 255,
            color.b * 255,
            color.a * 255
        );
    }

    constexpr inline vec4 FromHex(uint32_t color)
    {
        return Color::FromInts(
            ((color & 0xFF000000) >> 24),
            ((color & 0x00FF0000) >> 16),
            ((color & 0x0000FF00) >> 8),
            ((color & 0x000000FF))
        );
    }

    constexpr inline uint32_t ToHex(const vec4& color)
    {
        return (
            ((uint32_t)(color.r * 255) << 24)
            | ((uint32_t)(color.g * 255) << 16)
            | ((uint32_t)(color.b * 255) << 8)
            |  (uint32_t)(color.a * 255)
        );
    }

    constexpr inline Array<float, 4> ToArray(const vec4& color)
    {
        return Array<float, 4>{ color.r, color.g, color.b, color.a };
    }

    constexpr vec4 Transparent    = FromInts(0,   0,   0,   0);
    constexpr vec4 Black          = FromInts(0,   0,   0  );
    constexpr vec4 White          = FromInts(255, 255, 255);
    constexpr vec4 Red            = FromInts(255, 0,   0  );
    constexpr vec4 Green          = FromInts(0,   255, 0  );
    constexpr vec4 Blue           = FromInts(0,   0,   255);
    constexpr vec4 Yellow         = FromInts(255, 255, 0  );
    constexpr vec4 Magenta        = FromInts(255, 0,   255);
    constexpr vec4 Cyan           = FromInts(0,   255, 255);
    constexpr vec4 CornflowerBlue = FromInts(100, 149, 237); // https://en.wikipedia.org/wiki/Cornflower_blue

} // namespace Color

} // namespace dusk

#endif // DUSK_COLOR_HPP