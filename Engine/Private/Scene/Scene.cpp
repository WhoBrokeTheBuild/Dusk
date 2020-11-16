#include <Dusk/Scene/Scene.hpp>

namespace Dusk {

DUSK_ENGINE_API
System * Scene::AddSystem(std::unique_ptr<System> && sys)
{
	_systemPtrs.push_back(sys.get());
	_systems.push_back(std::move(sys));

	return _systemPtrs.back();
}

DUSK_ENGINE_API
std::vector<System *> Scene::GetSystems() const
{
	return _systemPtrs;
}

}