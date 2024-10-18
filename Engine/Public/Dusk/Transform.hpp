#ifndef DUSK_TRANSFORM_HPP
#define DUSK_TRANSFORM_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>

namespace dusk {

struct DUSK_API Transform
{
    vec3 Position;

    quat Orientation = { };

    vec3 Scale = { 1, 1, 1 };

    inline mat4 ToMatrix() const {
        mat4 matrix(1.0);
        matrix = glm::translate(matrix, Position);
        matrix *= glm::mat4_cast(Orientation);
        matrix = glm::scale(matrix, Scale);
        return matrix;
    }

    constexpr Transform& operator+=(const Transform& rhs)
    {
        Position += rhs.Position;
        Orientation *= rhs.Orientation;
        Scale *= rhs.Scale;
        return *this;
    }
};

constexpr Transform operator+(const Transform& lhs, const Transform& rhs)
{
    return Transform{
        lhs.Position + rhs.Position,
        lhs.Orientation * rhs.Orientation,
        lhs.Scale * rhs.Scale
    };
}

} // namespace dusk

#endif // DUSK_TRANSFORM_HPP