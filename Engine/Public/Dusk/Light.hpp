#ifndef DUSK_LIGHT_HPP
#define DUSK_LIGHT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Entity.hpp>
#include <Dusk/ShaderScene.hpp>

namespace Dusk {

enum class LightType
{
    Point = 0,
    Spot = 1,
    Directional = 2,

}; // enum class LightType

class DUSK_ENGINE_API Light : public Entity
{
public:

    DISALLOW_COPY_AND_ASSIGN(Light)

    Light() = default;

    virtual ~Light();

    void SetType(LightType type);

    inline LightType GetType() const {
        return _type;
    }

    void FillLightData(LightData * data);

    void SetColor(vec3 color);

    inline vec3 GetColor() const {
        return _color;
    }

    // Spot

    void SetInnerCutOff(float cutOff);

    inline float GetInnerCutOff() const {
        return _innerCutOff;
    }

    void SetOuterCutOff(float cutOff);

    inline float GetOuterCutOff() const {
        return _outerCutOff;
    }

private:

    LightType _type;

    vec3 _color;

    // Spot

    float _innerCutOff;

    float _outerCutOff;

};

} // namespace Dusk

#endif // DUSK_LIGHT_HPP