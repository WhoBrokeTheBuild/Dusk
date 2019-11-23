#ifndef DUSK_SOUND_COMPONENT
#define DUSK_SOUND_COMPONENT

#include <dusk/asset/Sound.hpp>
#include <dusk/core/Macros.hpp>
#include <dusk/core/OpenAL.hpp>
#include <dusk/scene/ActorComponent.hpp>

#include <memory>

namespace dusk {

class SoundComponent : public ActorComponent
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(SoundComponent)

    SoundComponent();

    virtual ~SoundComponent();

    /// Methods

    virtual void SetSound(std::shared_ptr<Sound>);

    virtual void Play();

    virtual void Pause();

    virtual void Stop();

    //virtual void SetLooping(bool looping);

    virtual void SceneStart() override;

    virtual void SceneStop() override;

    virtual void Update(UpdateContext&) override;

private:

    std::shared_ptr<Sound> _sound;

    GLuint _alSource = 0;

    bool _playing = true;

}; // class SoundComponent

} // namespace dusk

#endif // DUSK_SOUND_COMPONENT
