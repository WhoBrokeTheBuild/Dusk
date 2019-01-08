#ifndef DUSK_SOUND_COMPONENT
#define DUSK_SOUND_COMPONENT

#include <dusk/Config.hpp>
#include <dusk/asset/Sound.hpp>
#include <dusk/core/OpenAL.hpp>
#include <dusk/scene/ActorComponent.hpp>

#include <memory>

namespace dusk {

class SoundComponent : public ActorComponent
{
public:

    SoundComponent();

    virtual ~SoundComponent();

    virtual void SetSound(std::shared_ptr<Sound> sound);

    virtual void Play();
    
    virtual void Stop();

    virtual void Update(UpdateContext& ctx) override;

private:

    std::shared_ptr<Sound> _sound;

    GLuint _alSource = 0;

}; // class SoundComponent

} // namespace dusk

#endif // DUSK_SOUND_COMPONENT
