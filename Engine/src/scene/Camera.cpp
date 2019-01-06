#include "dusk/scene/Camera.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>

namespace dusk {

Camera::Camera()
{ 
    const glm::ivec2& size = App::Inst()->GetWindowSize();
    SetAspect(size);
    SetViewport(0.f, (float)size.x, (float)size.y, 0.f);
}

glm::mat4 Camera::GetView() const
{
    if (_mode == Mode::Perspective) {
        glm::vec3 forward = glm::rotate(GetWorldRotation(), glm::vec3(0.f, 0.f, -1.f));
        return glm::lookAt(GetWorldPosition(), GetWorldPosition() + forward, _up);
    }
    else if (_mode == Mode::Orthographic) {
        return glm::mat4(1.f);
    }
    
    return glm::mat4(1.f);
}

glm::mat4 Camera::GetProjection() const
{
    if (_mode == Mode::Perspective) {
        return glm::perspective(_fov, _aspect, _clip.x, _clip.y);
    }
    else if (_mode == Mode::Orthographic) {
        return glm::ortho(_viewport[0], _viewport[1], _viewport[2], _viewport[3], _clip.x, _clip.y);
    }
    
    return glm::mat4(1.f);
}

void Camera::SetMode(Mode mode) {
    _mode = mode;
}

void Camera::SetAspect(float aspect)
{
    _aspect = aspect;
}

void Camera::SetAspect(const glm::vec2& size)
{
    _aspect = size.x / size.y;
}

void Camera::SetViewport(float left, float right, float bottom, float top)
{
    _viewport = glm::vec4(left, right, bottom, top);
}

void Camera::SetViewport(const glm::vec4& viewport)
{
    _viewport = viewport;
}

void Camera::SetClip(const glm::vec2& clip)
{
    _clip = clip;
}

void Camera::SetForward(const glm::vec3& forward)
{
    SetRotation(glm::quatLookAt(glm::normalize(forward), _up));
}

void Camera::SetLookAt(const glm::vec3& point)
{
    SetForward(point - GetPosition());
}

void Camera::Update(UpdateContext& ctx)
{

}

void Camera::Print(std::string indent)
{
    Actor::Print(indent);
    DuskLog("%s _mode = %s", indent, (_mode == Mode::Perspective ? "Perspective" : "Orthographic"));
    DuskLog("%s _fov = %f", indent, _fov);
    DuskLog("%s _aspect = %f", indent, _aspect);
    DuskLog("%s _clip = %s", indent, glm::to_string(_clip));
    DuskLog("%s _up = %s", indent, glm::to_string(_up));
    DuskLog("%s _viewport = %s", indent, glm::to_string(_viewport));
}
} // namespace dusk
