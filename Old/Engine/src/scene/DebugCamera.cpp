#include "dusk/scene/DebugCamera.hpp"

#include <dusk/core/Util.hpp>

namespace dusk {

void DebugCamera::HandleEvent(SDL_Event * evt)
{
    Camera::HandleEvent(evt);

    if (evt->type == SDL_MOUSEMOTION) {
        if (_drag) {
            glm::vec2 delta(evt->motion.xrel, evt->motion.yrel);
            delta *= -1.f * _rotateSpeed;

            glm::vec3 forward = GetForward();
            glm::vec3 right = glm::normalize(glm::cross(GetWorldUp(), forward));
            SetUp(glm::cross(forward, right));
            
            glm::quat rotation = GetRotation();
            rotation = glm::angleAxis(delta.x, GetUp()) * rotation;
            rotation = glm::angleAxis(delta.y, GetRight()) * rotation;
            SetRotation(rotation);
        }
    }
    else if (evt->type == SDL_MOUSEBUTTONDOWN) {
        if (evt->button.button == SDL_BUTTON_MIDDLE) {
            _drag = true;
        }
        else if (evt->button.button == SDL_BUTTON_LEFT) {
            if ((SDL_GetModState() & KMOD_CTRL) > 0) {
                _drag = true;
            }
        }
    } 
    else if (evt->type == SDL_MOUSEBUTTONUP) {
        if (evt->button.button == SDL_BUTTON_MIDDLE ||
            evt->button.button == SDL_BUTTON_LEFT) {
            _drag = false;
        }
    }
    else if (evt->type == SDL_KEYDOWN) {
        auto key = evt->key.keysym.sym;

        if (key == SDLK_a) {
            _direction.x = -1.f;
        }
        else if (key == SDLK_d) {
            _direction.x = 1.f;
        }
        else if (key == SDLK_e) {
            _direction.y = 1.f;
        }
        else if (key == SDLK_q) {
            _direction.y = -1.f;
        }
        else if (key == SDLK_s) {
            _direction.z = -1.f;
        }
        else if (key == SDLK_w) {
            _direction.z = 1.f;
        }
    }
    else if (evt->type == SDL_KEYUP) {
        auto key = evt->key.keysym.sym;

        if (key == SDLK_a && _direction.x < 0.f) {
            _direction.x = 0.f;
        }
        if (key == SDLK_d && _direction.x > 0.f) {
            _direction.x = 0.f;
        }
        if (key == SDLK_e && _direction.y > 0.f) {
            _direction.y = 0.f;
        }
        if (key == SDLK_q && _direction.y < 0.f) {
            _direction.y = 0.f;
        }
        if (key == SDLK_s && _direction.z < 0.f) {
            _direction.z = 0.f;
        }
        if (key == SDLK_w && _direction.z > 0.f) {
            _direction.z = 0.f;
        }
    }
    else if (evt->type == SDL_MOUSEWHEEL) {
        glm::vec3 delta = glm::vec3(0.f);
        delta += _zoomSpeed * (float)evt->wheel.x * GetRight() * -1.f;
        delta += _zoomSpeed * (float)evt->wheel.y * GetForward();
        SetPosition(GetPosition() + delta);
    }
}

void DebugCamera::Update(UpdateContext& ctx)
{
    Camera::Update(ctx);

    glm::vec3 delta = glm::vec3(0.f);
    delta += _moveSpeed * _direction.x * GetRight();
    delta += _moveSpeed * _direction.y * GetUp();
    delta += _moveSpeed * _direction.z * GetForward();
    SetPosition(GetPosition() + delta * ctx.DeltaTime);
}

} // namespace dusk
