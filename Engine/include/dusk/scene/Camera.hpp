#ifndef DUSK_CAMERA_HPP
#define DUSK_CAMERA_HPP

#include <dusk/core/Macros.hpp>
#include <dusk/core/Math.hpp>
#include <dusk/scene/Actor.hpp>

#include <memory>

namespace dusk {

class Camera : public Actor
{
public:

    enum Mode 
    {
        Perspective,
        Orthographic,
    };

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Camera)

    Camera();

    virtual ~Camera() = default;

    /// Methods

    glm::mat4 GetView() const;

    glm::mat4 GetProjection() const;

    void SetMode(Mode);

    Mode GetMode() const {
        return _mode;
    }

    void SetAspect(float);

    void SetAspect(const glm::vec2&);

    inline float GetAspect() const {
        return _aspect;
    }

    void SetFOVX(float);

    void SetFOVY(float);

    void SetViewportScale(float, float, float, float);

    void SetViewportScale(const glm::vec4&);

    glm::vec4 GetViewportScale() const {
        return _viewportScale;
    }

    void SetViewportSize(float, float);

    void SetViewportSize(const glm::vec2&);

    glm::vec2 GetViewportSize() const {
        return _viewportSize;
    }

    glm::vec4 GetViewport() const;

    void SetClip(float, float);

    void SetClip(const glm::vec2&);

    inline glm::vec2 GetClip() const {
        return _clip;
    }

    void SetUp(const glm::vec3&);

    glm::vec3 GetUp() const {
        return _up;
    }

	glm::vec3 GetRight() const;

    void SetForward(const glm::vec3&);

    glm::vec3 GetForward() const;

    void SetLookAt(const glm::vec3&);

    void SetAutoResize(bool);

    void HandleEvent(SDL_Event *) override;

    void Print(std::string indent) override;

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
