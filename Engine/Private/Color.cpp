#include <Dusk/Color.hpp>

namespace dusk {

namespace Color {

// https://en.wikipedia.org/wiki/SRGB

DUSK_API
vec4 ToSRGB(const vec4& color)
{
    return vec4(
        (color.r < 0.0031308f ? 12.92f * color.r : 1.055f * pow(color.r, 1.0f / 2.4f) - 0.055f),
        (color.g < 0.0031308f ? 12.92f * color.g : 1.055f * pow(color.g, 1.0f / 2.4f) - 0.055f),
        (color.b < 0.0031308f ? 12.92f * color.b : 1.055f * pow(color.b, 1.0f / 2.4f) - 0.055f),
        color.a
    );
}

DUSK_API
vec4 ToLinear(const vec4& color)
{
    return vec4(
        (color.r < 0.04045f ? (1.0f / 12.92f) * color.r : pow((color.r + 0.055f) * (1.0f / 1.055f), 2.4f)),
        (color.g < 0.04045f ? (1.0f / 12.92f) * color.g : pow((color.g + 0.055f) * (1.0f / 1.055f), 2.4f)),
        (color.b < 0.04045f ? (1.0f / 12.92f) * color.b : pow((color.b + 0.055f) * (1.0f / 1.055f), 2.4f)),
        color.a
    );
}

} // namespace Color

} // namespace dusk
