#include <Dusk/Scene/Scene.hpp>

namespace Dusk {

DUSK_ENGINE_API
ISystem * Scene::AddSystem(std::unique_ptr<ISystem> && sys)
{
	_systemPtrs.push_back(sys.get());
	_systems.push_back(std::move(sys));

	return _systemPtrs.back();
}

DUSK_ENGINE_API
std::vector<ISystem *> Scene::GetSystems() const
{
	return _systemPtrs;
}

}