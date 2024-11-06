#ifndef DUSK_BOUNDING_BOX_HPP
#define DUSK_BOUNDING_BOX_HPP

#include <Dusk/Math.hpp>

namespace dusk {

class BoundingBox
{
public:

    vec3 Lower = vec3(std::numeric_limits<float>::max());

    vec3 Upper = vec3(std::numeric_limits<float>::min());

    inline vec3 GetSize() {
        return {
            Lower.x - Upper.x,
            Lower.y - Upper.y,
            Lower.z - Upper.z,
        };
    }

    inline vec3 GetCenter() {
        return {
            (Lower.x + Upper.x) * 0.5f,
            (Lower.y + Upper.y) * 0.5f,
            (Lower.z + Upper.z) * 0.5f,
        };
    }

    inline void Extend(const vec3& point) {
        Upper = glm::max(Upper, point);
        Lower = glm::min(Lower, point);
    }

    inline void Extend(const BoundingBox& box) {
        Upper = glm::max(Upper, box.Upper);
        Lower = glm::min(Lower, box.Lower);
    }

}; // class BoundingBox

} // namespace dusk

#endif // DUSK_BOUNDING_BOX_HPP