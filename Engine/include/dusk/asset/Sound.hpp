#ifndef DUSK_SOUND_HPP
#define DUSK_SOUND_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/ILoadable.hpp>

#include <string>

namespace dusk {

class Sound : public ILoadable
{
public:

    Sound() = default;

    Sound(const std::string& filename);

    virtual ~Sound() = default;

    bool LoadFromFile(const std::string& filename);

private:

}; // class Sound

} // namespace dusk

#endif // DUSK_SOUND_HPP
