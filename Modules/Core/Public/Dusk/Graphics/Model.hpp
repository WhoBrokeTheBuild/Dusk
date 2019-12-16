#ifndef DUSK_MODEL_HPP
#define DUSK_MODEL_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Graphics/Mesh.hpp>

namespace Dusk {

class DUSK_CORE_API Model 
{
public:

    DISALLOW_COPY_AND_ASSIGN(Model)

    explicit Model() = default;

    virtual ~Model() = default;

    virtual void Render();

    virtual bool LoadFromFile(const std::string& filename);

private:

    std::vector<std::unique_ptr<IMesh>> _meshes;

}; // class Model

} // namespace Dusk

#endif // DUSK_MODEL_HPP