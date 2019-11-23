#ifndef DUSK_DEBUG_CAMERA_HPP
#define DUSK_DEBUG_CAMERA_HPP

#include <dusk/core/Macros.hpp>
#include <dusk/scene/Camera.hpp>

namespace dusk {

class DebugCamera : public Camera 
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(DebugCamera)

    DebugCamera() = default;

    virtual ~DebugCamera() = default;

    /// Methods

    void HandleEvent(SDL_Event *) override;

    void Update(UpdateContext&) override;

private:

    float _moveSpeed = 0.8f;

    float _zoomSpeed = 0.1f;

    float _rotateSpeed = 0.0025f;

    bool _drag = false;

    glm::vec3 _direction = glm::vec3(0.f);

}; // class DebugCamera

} // namespace dusk

#endif // DUSK_DEBUG_CAMERA_HPP
