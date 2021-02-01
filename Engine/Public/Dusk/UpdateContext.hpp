#ifndef DUSK_UPDATE_CONTEXT_HPP
#define DUSK_UPDATE_CONTEXT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Time.hpp>

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
        _expectedFrameDuration = microseconds((int64_t)(1000000.0f / GetTargetFPS()));
    }

    // Ratio between Target FPS and Current FPS
    // > 1.0 means Current is less than Target
    // < 1.0 means Current is greater than Target
    virtual inline float GetFrameSpeedRatio() const {
        return _frameSpeedRatio;
    }

    // Time since program start in Milliseconds
    virtual inline milliseconds GetTotalDuration() const {
        return _totalDuration;
    }

    // Time that a frame should take in Milliseconds
    virtual inline microseconds GetExpectedFrameDuration() const {
        return _expectedFrameDuration;
    }

    // Time the previous frame took in Milliseconds
    virtual inline microseconds GetPreviousFrameDuration() const {
        return _previousFrameDuration;
    }

    virtual inline void ResetTime() {
        _totalDuration = 0ms;
        _previousFrameDuration = 0ms;
        _frameSpeedRatio = 1.0f;
    }

    virtual inline void SetTotalDuration(milliseconds totalFrameDuration) {
        _totalDuration = totalFrameDuration;
    }

    virtual inline void SetPreviousFrameDuration(microseconds previousFrameDuration) {
        _previousFrameDuration = previousFrameDuration;
        _frameSpeedRatio = previousFrameDuration / _expectedFrameDuration;
    }

private:

    float _targetFPS = 0.0f;

    float _frameSpeedRatio = 1.0f;

    milliseconds _totalDuration = 0ms;

    microseconds _expectedFrameDuration = 0ms;

    microseconds _previousFrameDuration = 0ms;

}; // class UpdateContext

} // namespace Dusk

#endif // DUSK_UPDATE_CONTEXT_HPP