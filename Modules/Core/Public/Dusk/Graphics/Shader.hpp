#ifndef DUSK_SHADER_HPP
#define DUSK_SHADER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>

#include <string>
#include <vector>

namespace Dusk {

class DUSK_CORE_API IShader 
{
public:

    DISALLOW_COPY_AND_ASSIGN(IShader)

    explicit IShader() = default;

    virtual ~IShader() = default;

    virtual bool LoadFromFiles(const std::vector<std::string>& filenames) = 0;

    virtual void Bind() = 0;

};

DUSK_CORE_API
std::vector<std::string> GetShaderPaths();

} // namespace Dusk

#endif // DUSK_SHADER_HPP
