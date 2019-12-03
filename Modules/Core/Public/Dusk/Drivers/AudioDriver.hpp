#ifndef DUSK_AUDIO_DRIVER_HPP
#define DUSK_AUDIO_DRIVER_HPP

#include <Dusk/Macros.hpp>

namespace Dusk {

class IAudioDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(IAudioDriver)

    explicit IAudioDriver() = default;

    virtual ~IAudioDriver() = default;

}; // class IAudioDriver

void SetAudioDriver(IAudioDriver * driver);

IAudioDriver * GetAudioDriver();

} // namespace Dusk

#endif // DUSK_AUDIO_DRIVER_HPP
