#include "dusk/scene/Component.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/scene/Actor.hpp>

namespace dusk {

Component::Component()
    : _actor(nullptr)
{
}

Component::~Component()
{
}

void Component::SetActor(Actor * actor)
{
    _actor = actor;
}

std::unique_ptr<ModelComponent> ModelComponent::Create(std::unique_ptr<Model> model)
{
    return std::make_unique<ModelComponent>(std::move(model));
}

ModelComponent::ModelComponent(std::unique_ptr<Model> model)
    : Component()
    , _model(std::move(model))
{
}

ModelComponent::~ModelComponent()
{
    if (GetActor())
    {
        //GetActor()->EvtUpdate.RemoveListener(updateBindId);
        //GetActor()->EvtRender.RemoveListener(renderBindId);
    }
}

void ModelComponent::SetActor(Actor * actor)
{
    if (GetActor())
    {
        //GetActor()->EvtUpdate.RemoveListener(updateBindId);
        //GetActor()->EvtRender.RemoveListener(renderBindId);
    }

    Component::SetActor(actor);

    //updateBindId = GetActor()->EvtUpdate.AddMember(this, &ModelComponent::OnUpdate);
    //renderBindId = GetActor()->EvtRender.AddMember(this, &ModelComponent::OnRender);
}

void ModelComponent::OnUpdate(UpdateContext& ctx)
{
    _model->SetBaseTransform(GetActor()->GetTransform());
    _model->Update(ctx);
}

void ModelComponent::OnRender(RenderContext& ctx)
{
    _model->Render(ctx);
}

std::unique_ptr<CameraComponent> CameraComponent::Create(std::unique_ptr<Camera> camera)
{
    return std::make_unique<CameraComponent>(std::move(camera));
}

CameraComponent::CameraComponent(std::unique_ptr<Camera> camera)
    : Component()
    , _camera(std::move(camera))
{
}

CameraComponent::~CameraComponent()
{
    if (GetActor())
    {
        //GetActor()->RemoveEventListener((EventID)Actor::Events::UPDATE, this, &CameraComponent::OnUpdate);
    }
}

void CameraComponent::SetActor(Actor * actor)
{
    if (GetActor())
    {
        //GetActor()->RemoveEventListener((EventID)Actor::Events::UPDATE, this, &CameraComponent::OnUpdate);
    }

    Component::SetActor(actor);

    //GetActor()->AddEventListener((EventID)Actor::Events::UPDATE, this, &CameraComponent::OnUpdate);
}

/*
ScriptComponent::ScriptComponent(const std::string& filename)
    : Component()
    , _scriptHost()
    , _filename(filename)
{
    lua_State * L = _scriptHost.GetLuaState();

    lua_pushinteger(L, (ptrdiff_t)this);
    lua_setglobal(L, "dusk_current_ScriptComponent");

    // Wait until we have an actor to run the script
}

std::unique_ptr<Component> ScriptComponent::Clone()
{
    ScriptComponent * component = new ScriptComponent(_filename);

    component->SetActor(GetActor());

    return std::unique_ptr<Component>(component);
}

void ScriptComponent::SetActor(Actor * actor)
{
    if (GetActor() && !IsTemplate())
    {
        // TODO: Cleanup script host?
    }

    Component::SetActor(actor);

    if (!IsTemplate())
    {
        // TODO: Fix
        _scriptHost.RunFile(_filename);
    }
}
*/

} // namespace dusk
