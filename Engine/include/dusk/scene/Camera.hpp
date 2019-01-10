#ifndef DUSK_CAMERA_HPP
#define DUSK_CAMERA_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Math.hpp>
#include <dusk/scene/Actor.hpp>

#include <memory>

namespace dusk {

class Camera : public Actor
{
public:

    enum Mode {
        Perspective,
        Orthographic,
    };

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Camera)

    Camera();

    virtual ~Camera() = default;

    /// Methods

    glm::vec3 GetWorldUp() const {
        return glm::vec3(0.f, 1.f, 0.f);
    }

    glm::vec3 GetWorldForward() const {
        return glm::vec3(0.f, 0.f, -1.f);
    }

    glm::mat4 GetView() const;

    glm::mat4 GetProjection() const;

    void SetMode(Mode mode);

    Mode GetMode() const {
        return _mode;
    }

    void SetAspect(float aspect);

    void SetAspect(const glm::vec2& size);

    inline float GetAspect() const {
        return _aspect;
    }

    void SetFOVX(float fovx);

    void SetFOVY(float fovy);

    void SetViewportScale(float left, float right, float bottom, float top);

    void SetViewportScale(const glm::vec4& viewScale);

    glm::vec4 GetViewportScale() const {
        return _viewportScale;
    }

    void SetViewportSize(float width, float height);

    void SetViewportSize(const glm::vec2& viewSize);

    glm::vec2 GetViewportSize() const {
        return _viewportSize;
    }

    glm::vec4 GetViewport() const;

    void SetClip(float near, float far);

    void SetClip(const glm::vec2& clip);

    inline glm::vec2 GetClip() const {
        return _clip;
    }

    void SetUp(const glm::vec3& up);

    glm::vec3 GetUp() const {
        return _up;
    }

    glm::vec3 GetRight() const {
        return glm::normalize(glm::cross(GetForward(), GetUp()));
    }

    void SetForward(const glm::vec3& forward);

    glm::vec3 GetForward() const;

    void SetLookAt(const glm::vec3& point);

    void SetAutoResize(bool autoResize);

    virtual void HandleEvent(SDL_Event * evt) override;

    virtual void Print(std::string indent) override;

private:

    Mode _mode = Mode::Perspective;

    bool _autoResize = true;

    glm::vec2 _clip = glm::vec2(0.1f, 10000.f);

    glm::vec3 _up = glm::vec3(0.f, 1.f, 0.f);

    float _aspect = 16.f / 9.f; // 16:9

    // Perspective

    float _fovX = glm::radians(45.f);

    // Orthographic

    glm::vec2 _viewportSize = glm::vec2(1920.f, 1080.f);
    
    glm::vec4 _viewportScale = glm::vec4(-0.5f, 0.5f, 0.5f, -0.5f);

}; // class Camera

} // namespace dusk

#endif // DUSK_CAMERA_HPP
