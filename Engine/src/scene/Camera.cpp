#include "dusk/scene/Camera.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>

namespace dusk {

Camera::Camera()
{ 
    const glm::ivec2& size = App::Inst()->GetWindowSize();
    SetAspect(size);
    SetViewportSize(size);
}

glm::mat4 Camera::GetView() const
{
    return glm::lookAt(GetWorldPosition(), GetWorldPosition() + GetForward(), _up);
}

glm::mat4 Camera::GetProjection() const
{
    if (_mode == Mode::Perspective) {
        return glm::perspective(_fovX, _aspect, _clip[0], _clip[1]);
    }
    else if (_mode == Mode::Orthographic) {
        const auto& view = GetViewport();
        return glm::ortho(view[0], view[1], view[2], view[3], _clip[0], _clip[1]);
    }
    
    return glm::mat4(1.f);
}

void Camera::SetMode(Mode mode) 
{
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

void Camera::SetFOVX(float fovx)
{
    _fovX = fovx;
}

void Camera::SetFOVY(float fovy)
{
    _fovX = 2.f * atanf(tanf(fovy * 0.5f) * _aspect);
}

void Camera::SetViewportScale(float left, float right, float bottom, float top)
{
    _viewportScale = glm::vec4(left, right, bottom, top);
}

void Camera::SetViewportScale(const glm::vec4& viewScale)
{
    _viewportScale = viewScale;
}

void Camera::SetViewportSize(float width, float height)
{
    _viewportSize.x = width;
    _viewportSize.y = height;
}

void Camera::SetViewportSize(const glm::vec2& viewSize)
{
    _viewportSize = viewSize;
}

glm::vec4 Camera::GetViewport() const
{
    glm::vec4 scale = GetViewportScale();
    glm::vec2 size = GetViewportSize();

    if (_aspect > 1.f) {
        size.y /= _aspect;
    } else {
        size.x *= _aspect;
    }

    return glm::vec4(
        size.x * scale[0],
        size.x * scale[1],
        size.y * scale[2],
        size.y * scale[3]);
}

void Camera::SetClip(float near, float far)
{
    _clip.x = near;
    _clip.y = far;
}

void Camera::SetClip(const glm::vec2& clip)
{
    _clip = clip;
}

void Camera::SetUp(const glm::vec3& up)
{
    _up = up;
}

void Camera::SetForward(const glm::vec3& forward)
{
    if ((normalize(forward) + _up) == glm::vec3(0.f)) {
        SetRotation(glm::angleAxis(glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f)));
    } else {
        SetRotation(glm::quatLookAt(glm::normalize(forward), _up));
    }
}

glm::vec3 Camera::GetForward() const
{
    return glm::rotate(GetWorldRotation(), GetWorldForward());
}

void Camera::SetLookAt(const glm::vec3& point)
{
    SetForward(point - GetPosition());
}

void Camera::SetAutoResize(bool autoResize)
{
    _autoResize = autoResize;
}

void Camera::HandleEvent(SDL_Event * evt)
{
    if (_autoResize) {
        if (evt->type == SDL_WINDOWEVENT) {
            if (evt->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                glm::vec2 size(evt->window.data1, evt->window.data2);
                SetAspect(size);
            }
        }
    }
}

void Camera::Print(std::string indent)
{
    Actor::Print(indent);
    DuskLog("%s _mode = %s", indent, (_mode == Mode::Perspective ? "Perspective" : "Orthographic"));
    DuskLog("%s _fovX = %f", indent, _fovX);
    DuskLog("%s _aspect = %f", indent, _aspect);
    DuskLog("%s _clip = %s", indent, glm::to_string(_clip));
    DuskLog("%s _up = %s", indent, glm::to_string(_up));
    DuskLog("%s _viewportSize = %s", indent, glm::to_string(_viewportSize));
    DuskLog("%s _viewportScale = %s", indent, glm::to_string(_viewportScale));
}
} // namespace dusk
