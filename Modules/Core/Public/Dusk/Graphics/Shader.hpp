#ifndef DUSK_SHADER_HPP
#define DUSK_SHADER_HPP

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

class DUSK_CORE_API IShader 
{
public:

    DISALLOW_COPY_AND_ASSIGN(IShader)

    explicit IShader() = default;

    virtual ~IShader() = default;

    virtual void Bind() = 0;

};

} // namespace Dusk

#endif // DUSK_SHADER_HPP
