#include <Dusk/Scene/Camera.hpp>

#include <Dusk/Drivers/GraphicsDriver.hpp>

namespace Dusk {

DUSK_CORE_API
Camera::Camera()
{
    IGraphicsDriver * gfx = GetGraphicsDriver();
    if (gfx) {
        const ivec2& size = gfx->GetWindowSize();
        SetAspect(size);
        SetViewportSize(size);
    }
}

DUSK_CORE_API
mat4 Camera::GetView() const
{
    return glm::lookAt(GetWorldPosition(), GetWorldPosition() + GetForward(), GetUp());
}

DUSK_CORE_API
mat4 Camera::GetProjection() const 
{
    if (_mode == Mode::Perspective) {
        return glm::perspective(_fovX, GetAspect(), _clip[0], _clip[1]);
    }
    else if (_mode == Mode::Orthographic) {
        const auto& view = GetViewport();
        return glm::ortho(view[0], view[1], view[2], view[3], _clip[0], _clip[1]);
    }

    return mat4(1.f);
}

DUSK_CORE_API
void Camera::SetMode(Mode mode)
{
    _mode = mode;
}

DUSK_CORE_API
void Camera::SetClip(const vec2& clip)
{
    _clip = clip;
}

DUSK_CORE_API
void Camera::SetUp(const vec3& up)
{
    _up = up;
}

DUSK_CORE_API
void Camera::SetForward(const vec3& forward)
{
    if ((glm::normalize(forward) + _up) == vec3(0.f)) {
        SetOrientation(glm::angleAxis(glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f)));
    }
    else {
        SetOrientation(glm::quatLookAt(glm::normalize(forward), _up));
    }
}

DUSK_CORE_API
vec3 Camera::GetForward() const
{
    return glm::rotate(GetWorldOrientation(), GetWorldForward());
}

DUSK_CORE_API
vec3 Camera::GetRight() const
{
    return glm::normalize(glm::cross(GetForward(), GetUp()));
}

DUSK_CORE_API
void Camera::SetAspect(float aspect)
{
    _aspect = aspect;
}

DUSK_CORE_API
void Camera::SetAspect(const vec2& size)
{
    _aspect = size.x / size.y;
}

DUSK_CORE_API
void Camera::SetFOVX(float fovX)
{
    _fovX = fovX;
}

DUSK_CORE_API
void Camera::SetFOVY(float fovY)
{
    _fovX = 2.f * atanf(tanf(fovY * .5f) * _aspect);
}

DUSK_CORE_API
void Camera::SetLookAt(const vec3& point)
{
    SetForward(point - GetPosition());
}

DUSK_CORE_API
void Camera::SetViewportSize(const vec2& size) 
{
    _viewportSize = size;
}

DUSK_CORE_API
void Camera::SetViewportScale(const vec4& scale) 
{
    _viewportScale = scale;
}

DUSK_CORE_API
vec4 Camera::GetViewport() const
{
    const vec4& scale = GetViewportScale();
    vec2 size = GetViewportSize();

    if (_aspect > 1.f) {
        size.y /= _aspect;
    } else {
        size.x *= _aspect;
    }

    return {
        size.x * scale[0],
        size.x * scale[1],
        size.y * scale[2],
        size.y * scale[3],
    };
}

}