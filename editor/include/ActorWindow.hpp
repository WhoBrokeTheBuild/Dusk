#ifndef DUSK_ACTOR_WINDOW_HPP
#define DUSK_ACTOR_WINDOW_HPP

#include "Config.hpp"
#include "PopupWindow.hpp"

class ActorWindow : public PopupWindow
{
public:

    ActorWindow(dusk::Actor * actor, EditorWindow * parent);

protected:

    virtual void DoReset() override;
    virtual void DoApply() override;
    virtual void DoRender() override;

private:

    dusk::Actor * _actor;

    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;

}; // class ActorWindow

#endif // DUSK_ACTOR_WINDOW_HPP
