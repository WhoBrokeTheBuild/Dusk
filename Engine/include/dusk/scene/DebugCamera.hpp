#ifndef DUSK_DEBUG_CAMERA_HPP
#define DUSK_DEBUG_CAMERA_HPP

#include <dusk/Config.hpp>
#include <dusk/scene/Camera.hpp>

namespace dusk {

class DebugCamera : public Camera 
{
public:

    virtual void HandleEvent(SDL_Event * evt) override;

    virtual void Update(UpdateContext& ctx) override;

private:

    float _moveSpeed = 0.8f;

    float _zoomSpeed = 0.1f;

    float _rotateSpeed = 0.0025f;

    bool _drag = false;

    glm::vec3 _direction = glm::vec3(0.f);

}; // class DebugCamera

} // namespace dusk

#endif // DUSK_DEBUG_CAMERA_HPP