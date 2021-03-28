#ifndef DUSK_AUDIO_DRIVER_HPP
#define DUSK_AUDIO_DRIVER_HPP

#include <Dusk/Config.hpp>

namespace Dusk {

class DUSK_CORE_API AudioDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(AudioDriver)

    AudioDriver() = default;

    virtual ~AudioDriver() = default;

}; // class AudioDriver

void SetAudioDriver(AudioDriver * driver);

AudioDriver * GetAudioDriver();

} // namespace Dusk

#endif // DUSK_AUDIO_DRIVER_HPP
