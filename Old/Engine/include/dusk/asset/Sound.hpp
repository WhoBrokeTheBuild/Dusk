#ifndef DUSK_SOUND_HPP
#define DUSK_SOUND_HPP

#include <dusk/asset/ILoadable.hpp>
#include <dusk/core/OpenAL.hpp>

#include <string>

namespace dusk {

class Sound : public ILoadable
{
public:

    Sound() = default;

    Sound(const std::string&);

    virtual ~Sound();

    bool LoadFromFile(const std::string&);

    ALuint GetALBufferID() const {
        return _alID;
    }

private:

    ALuint _alID = 0;

}; // class Sound

} // namespace dusk

#endif // DUSK_SOUND_HPP
