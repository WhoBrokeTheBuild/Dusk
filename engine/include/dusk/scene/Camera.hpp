#ifndef DUSK_CAMERA_HPP
#define DUSK_CAMERA_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Event.hpp>
#include <dusk/scene/Scene.hpp>

#include <memory>

namespace dusk {

class Camera : public ICallbackHost
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Camera)

    Camera(std::string id, Scene * scene, float fov = 45.0f, glm::vec3 up = glm::vec3(0, 1, 0), glm::vec2 clip = glm::vec2(0.1f, 1000.0f));
    virtual ~Camera() = default;

    void Serialize(nlohmann::json& data);
    void Deserialize(nlohmann::json& data);

    /// Methods

    std::string GetId() const { return _id; }

    glm::mat4 GetView();
    glm::mat4 GetProjection();

    void SetFOV(float fov);
    float GetFOV() const { return _fov; }

    void SetAspect(float aspect);
    void SetAspect(float width, float height) { SetAspect(width / height); }
    void SetAspect(const glm::vec2& size) { SetAspect(size.x / size.y); }
    float GetAspect() const { return _aspect; }

    void SetClip(const glm::vec2& clip);
    void SetClip(float znear, float zfar) { SetClip(glm::vec2(znear, zfar)); };
    glm::vec2 GetClip() const { return _clip; }

    void SetPosition(const glm::vec3& pos);
    glm::vec3 GetPosition() const { return _position; }

    void SetForward(const glm::vec3& forward);
    glm::vec3 GetForward() const { return _forward; }

    void SetLookAt(const glm::vec3& lookAt) { SetForward(lookAt - GetPosition()); };

    void SetUp(const glm::vec3& up);
    glm::vec3 GetUp() const { return _up; }

    void ChangePitch(const float& pitch);
    void ChangeYaw(const float& yaw);

private:

    std::string _id;

    Scene * _scene;

    bool _viewInvalid;
    bool _projectionInvalid;

    glm::mat4 _view;
    glm::mat4 _projection;

    float _fov;
    float _aspect;
    glm::vec2 _clip;

    glm::vec3 _position;
    glm::vec3 _forward;
    glm::vec3 _up;

    float _pitch = 0.0f;
    float _yaw   = 0.0f;
    float _roll   = 0.0f;

}; // class Camera

} // namespace dusk

#endif // DUSK_CAMERA_HPP
