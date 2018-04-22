#include "dusk/scene/Light.hpp"

namespace dusk {

void Light::SetType(Type type)
{
    _type = type;
}

void Light::SetColor(glm::vec4 color)
{
    _color = color;
}

} // namespace dusk
