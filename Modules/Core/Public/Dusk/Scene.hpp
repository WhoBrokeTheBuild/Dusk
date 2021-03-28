#ifndef DUSK_SCENE_HPP
#define DUSK_SCENE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Entity.hpp>
#include <Dusk/System.hpp>

#include <vector>
#include <memory>

namespace Dusk {

class DUSK_CORE_API Scene : public Entity 
{
public:

    DISALLOW_COPY_AND_ASSIGN(Scene)

    Scene() = default;
    
    virtual ~Scene() = default;
    
    System * AddSystem(std::unique_ptr<System> && sys);

    std::vector<System *> GetSystems() const;

private:

    std::vector<std::unique_ptr<System>> _systems;

    std::vector<System *> _systemPtrs;

}; // class Scene

DUSK_CORE_API
void SetCurrentScene(Scene * scene);

DUSK_CORE_API
Scene * GetCurrentScene();

} // namespace Dusk

#endif // DUSK_SCENE_HPP