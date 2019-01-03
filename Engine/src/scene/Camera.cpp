#include "dusk/scene/Camera.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>

namespace dusk {

Camera::Camera(float fov /*= 45.0f*/, glm::vec3 up /*= glm::vec3(0, 1, 0)*/, glm::vec2 clip /*= glm::vec2(0.1f, 1000.0f)*/)
    : _viewInvalid(true)
    , _projectionInvalid(true)
    , _view(1)
    , _projection(1)
    , _fov(fov)
    , _aspect()
    , _clip(clip)
    , _position(0)
    , _forward(1)
    , _up(up)
{ }

glm::mat4 Camera::GetView()
{
    if (_viewInvalid)
    {
        glm::quat qpitch, qyaw, tmp;
        glm::vec3 pitchAxis;

        pitchAxis = glm::cross(_forward, _up);
        qpitch = glm::angleAxis(_pitch, pitchAxis);
        qyaw = glm::angleAxis(_yaw, _up);

        tmp = glm::cross(qpitch, qyaw);
        tmp = glm::normalize(tmp);

        _forward = glm::rotate(tmp, _forward);

        _pitch = 0;
        _yaw = 0;

        _view = glm::lookAt(_position, _position + _forward, _up);
        _viewInvalid = false;
    }
    return _view;
}

glm::mat4 Camera::GetProjection()
{
    if (_projectionInvalid)
    {
        _projection = glm::perspective(_fov, _aspect, _clip.x, _clip.y);
        _projectionInvalid = false;
    }
    return _projection;
}

void Camera::SetFOV(float fov)
{
    _fov = fov;
    _projectionInvalid = true;
}

void Camera::SetAspect(float aspect)
{
    _aspect = aspect;
    _projectionInvalid = true;
}

void Camera::SetClip(const glm::vec2& clip)
{
    _clip = clip;
    _projectionInvalid = true;
}

void Camera::SetPosition(const glm::vec3& pos)
{
    _position = pos;
    _viewInvalid = true;
}

void Camera::SetForward(const glm::vec3& forward)
{
    _forward = glm::normalize(forward);
    _viewInvalid = true;
}

void Camera::SetUp(const glm::vec3& up)
{
    _up = up;
    _viewInvalid = true;
}

void Camera::ChangePitch(const float& pitch)
{
    _pitch += pitch;
    _viewInvalid = true;
}

void Camera::ChangeYaw(const float& yaw)
{
    _yaw += yaw;
    _viewInvalid = true;
}

} // namespace dusk
