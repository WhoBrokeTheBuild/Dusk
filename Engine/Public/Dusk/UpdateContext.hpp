#ifndef DUSK_UPDATE_CONTEXT_HPP
#define DUSK_UPDATE_CONTEXT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>

#include <cstdint>

namespace Dusk {

class DUSK_ENGINE_API UpdateContext : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(UpdateContext)

    UpdateContext() = default;

    virtual ~UpdateContext() = default;

    // Target Frames/Second
    virtual inline float GetTargetFPS() const {
        return _targetFPS;
    }

    virtual inline void SetTargetFPS(float fps) {
        _targetFPS = fps;
    }

    // Current Frames/Second
    virtual inline float GetCurrentFPS() const {
        return _currentFPS;
    }

    virtual inline void SetCurrentFPS(float fps) {
        _currentFPS = fps;
        _fpsRatio = _targetFPS / _currentFPS;
    }

    // Ratio between Target FPS and Current FPS
    // > 1.0 means Current is less than Target
    // < 1.0 means Current is greater than Target
    virtual inline float GetFPSRatio() const {
        return _fpsRatio;
    }

    // Time since last update in Milliseconds
    virtual inline long long GetElapsedTime() const {
        return _elapsedTime;
    }

    // Time since program start in Milliseconds
    virtual inline long long GetTotalTime() const {
        return _totalTime;
    }

    virtual inline void AddElapsedTime(long long time) {
        _elapsedTime = time;
        _totalTime += time;
    }

private:

    float _targetFPS = 0.f;

    float _currentFPS = 0.f;

    float _fpsRatio = 0.f;

    uintmax_t _elapsedTime = 0;

    uintmax_t _totalTime = 0;

}; // class UpdateContext

} // namespace Dusk

#endif // DUSK_UPDATE_CONTEXT_HPP