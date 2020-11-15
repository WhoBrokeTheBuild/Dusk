#ifndef DUSK_SCENE_HPP
#define DUSK_SCENE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Scene/Entity.hpp>
#include <Dusk/Scene/System.hpp>

#include <vector>
#include <memory>

namespace Dusk {

class DUSK_ENGINE_API Scene : public Entity 
{
public:

    DISALLOW_COPY_AND_ASSIGN(Scene)

    explicit Scene() = default;
    
    virtual ~Scene() = default;
    
    ISystem * AddSystem(std::unique_ptr<ISystem> && sys);

    std::vector<ISystem *> GetSystems() const;

private:

    std::vector<std::unique_ptr<ISystem>> _systems;

    std::vector<ISystem *> _systemPtrs;


}; // class Scene

} // namespace Dusk

#endif // DUSK_SCENE_HPP