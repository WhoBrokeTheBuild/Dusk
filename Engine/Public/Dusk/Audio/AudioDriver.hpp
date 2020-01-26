#ifndef DUSK_AUDIO_DRIVER_HPP
#define DUSK_AUDIO_DRIVER_HPP

#include <Dusk/Macros.hpp>

namespace Dusk {

class AudioDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(AudioDriver)

    explicit AudioDriver() = default;

    virtual ~AudioDriver() = default;

}; // class AudioDriver

void SetAudioDriver(AudioDriver * driver);

AudioDriver * GetAudioDriver();

} // namespace Dusk

#endif // DUSK_AUDIO_DRIVER_HPP
