#ifndef DUSK_CAMERA_HPP
#define DUSK_CAMERA_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Entity.hpp>
#include <Dusk/String.hpp>

namespace Dusk {

enum class CameraMode 
{
    Perspective,
    Orthographic,

}; // enum class CameraMode

class DUSK_ENGINE_API Camera : public Entity
{
public:

    DISALLOW_COPY_AND_ASSIGN(Camera)

    Camera();

    virtual ~Camera();

    mat4 GetView() const;

    mat4 GetProjection() const;

    void SetMode(CameraMode mode);

    inline CameraMode GetMode() const {
        return _mode;
    }

    void SetClip(const vec2& clip);

    inline vec2 GetClip() const {
        return _clip;
    }

    void SetUp(const vec3& up);

    inline vec3 GetUp() const {
        return _up;
    }

    void SetForward(const vec3& forward);

    vec3 GetForward() const;

    vec3 GetRight() const;

    void SetAspect(float aspect);

    void SetAspect(const vec2& aspect);

    inline float GetAspect() const {
        return _aspect;
    }

    // Perspective

    void SetFOVX(float fovX);

    inline float GetFOVX() const {
        return _fovX;
    }

    void SetFOVY(float fovY);

    void SetLookAt(const vec3& point);

    // Orthographic

    void SetViewportSize(const vec2& size);

    inline vec2 GetViewportSize() const {
        return _viewportSize;
    }

    void SetViewportScale(const vec4& scale);

    inline vec4 GetViewportScale() const {
        return _viewportScale;
    }

    vec4 GetViewport() const;

private:

    CameraMode _mode = CameraMode::Perspective;

    bool _flipY = false;

    vec2 _clip = { 0.1f, 10000.f };

    vec3 _up = GetWorldUp();

    float _aspect = 16.f / 9.f; // 16:9

    unsigned _windowResizedEventHandlerID;

    // Perspective

    float _fovX = glm::radians(45.f);

    // Orthographic

    vec2 _viewportSize = { 1920.f, 1080.f };

    vec4 _viewportScale = { -.5f, .5f, .5f, -.5f };

};

inline string CameraModeToString(CameraMode cameraMode)
{
    switch (cameraMode) {
        case CameraMode::Perspective:
            return "Perspective";
        case CameraMode::Orthographic:
            return "Orthographic";
    }

    return "Unknown";
}

} // namespace Dusk

#endif // DUSK_CAMERA_HPP