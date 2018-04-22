#ifndef DUSK_LIGHT_HPP
#define DUSK_LIGHT_HPP

namespace dusk {

class Light : public Actor
{
public:

    enum class Type {
        POINT,
        DIRECTION,
        SPOT,
    };

    void SetType(Type type);
    inline Type GetType() const { return _type; }

    void SetColor(glm::vec4 color);
    inline glm::vec4 GetColor() const { return _color; }

private:

    Type _type;

    glm::vec4 _color;

}; // class Light

} // namespace dusk

#endif // DUSK_LIGHT_HPP
