#ifndef DUSK_SCENE_IMPORTER_HPP
#define DUSK_SCENE_IMPORTER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>

namespace Dusk {

class DUSK_CORE_API ISceneLoader 
{
public:

    virtual bool LoadFromFile(const std::string& filename) = 0;

}; // class ISceneLoader

} // namespace Dusk

#endif // DUSK_SCENE_IMPORTER_HPP