#include "dusk/scene/SoundComponent.hpp"

#include <dusk/scene/Actor.hpp>

namespace dusk {

SoundComponent::SoundComponent()
{
    alGenSources(1, &_alSource);
}

SoundComponent::~SoundComponent()
{
    alDeleteSources(1, &_alSource);
}

void SoundComponent::SetSound(std::shared_ptr<Sound> sound)
{
    _sound = sound;
    alSourcei(_alSource, AL_BUFFER, _sound->GetALBufferID());
}

void SoundComponent::SceneStart()
{
    if (_playing) {
        alSourcePlay(_alSource);
    }
}

void SoundComponent::SceneStop()
{
    if (_playing) {
        alSourcePause(_alSource);
    }
}

void SoundComponent::Play()
{
    _playing = true;
    alSourcePlay(_alSource);
}

void SoundComponent::Pause()
{
    _playing = false;
    alSourcePause(_alSource);
}

void SoundComponent::Stop()
{
    _playing = false;
    alSourceStop(_alSource);
}

void SoundComponent::Update(UpdateContext& ctx)
{
    if (!GetActor()) {
        return;
    }

    const auto& pos = GetActor()->GetPosition();
    alSource3f(_alSource, AL_POSITION, pos.x, pos.y, pos.z);
}

} // namespace dusk
