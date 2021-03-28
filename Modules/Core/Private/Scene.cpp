#include <Dusk/Scene.hpp>

namespace Dusk {

static Dusk::Scene * _CurrentScene = nullptr;

DUSK_CORE_API
System * Scene::AddSystem(std::unique_ptr<System> && sys)
{
	_systemPtrs.push_back(sys.get());
	_systems.push_back(std::move(sys));

	return _systemPtrs.back();
}

DUSK_CORE_API
std::vector<System *> Scene::GetSystems() const
{
	return _systemPtrs;
}

DUSK_CORE_API
void SetCurrentScene(Scene * scene)
{
	_CurrentScene = scene;
}

DUSK_CORE_API
Scene * GetCurrentScene()
{
	return _CurrentScene;
}

} // namespace Dusk