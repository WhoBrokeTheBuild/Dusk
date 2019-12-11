#ifndef DUSK_SHADER_LOADER_HPP
#define DUSK_SHADER_LOADER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>

#include <unordered_map>

namespace Dusk {

struct DUSK_CORE_API ShaderData
{
    DISALLOW_COPY_AND_ASSIGN(ShaderData)

    explicit ShaderData() = default;

    ShaderData(ShaderData&& rhs) {
        std::swap(Files, rhs.Files);
    }

    std::unordered_map<std::string, std::string> Files;
};

class IShaderImporter 
{
public:

    DISALLOW_COPY_AND_ASSIGN(IShaderImporter)

    explicit IShaderImporter() = default;

    virtual ~IShaderImporter() = default;

    virtual ShaderData && LoadFromFile(std::string filename) = 0;

}; // class IShaderImporter

DUSK_CORE_API
void SetShaderImporter(IShaderImporter * loader);

DUSK_CORE_API
IShaderImporter * GetShaderImporter();

} // namepsace Dusk

#endif // DUSK_SHADER_LOADER_HPP
