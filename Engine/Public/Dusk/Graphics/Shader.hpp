#ifndef DUSK_SHADER_HPP
#define DUSK_SHADER_HPP

#include <Dusk/Config.hpp>

#include <string>
#include <vector>

namespace Dusk {

class DUSK_ENGINE_API Shader 
{
public:

    DISALLOW_COPY_AND_ASSIGN(Shader)

    Shader() = default;

    virtual ~Shader() = default;

    virtual bool LoadFromFiles(const std::vector<std::string>& filenames) = 0;

    virtual void Bind() = 0;

    // GetSize()
    // SetPreprocessorDefines()
    // GetPreprocessorDefines()

};

} // namespace Dusk

#endif // DUSK_SHADER_HPP
