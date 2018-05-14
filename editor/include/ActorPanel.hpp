#ifndef DUSK_ACTOR_WINDOW_HPP
#define DUSK_ACTOR_WINDOW_HPP

#include "Config.hpp"
#include "EditorPanel.hpp"

class ActorPanel : public EditorPanel
{
public:

    ActorPanel(Editor * editor);

    void SetActor(dusk::Actor * actor);

protected:

    virtual void DoReset() override;
    virtual void DoApply() override;
    virtual void DoRender() override;

private:

    dusk::Actor * _actor = nullptr;

    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;

    std::array<char, 512> _tags;

}; // class ActorPanel

#endif // DUSK_ACTOR_WINDOW_HPP
