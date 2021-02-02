#ifndef DUSK_SHADER_HPP
#define DUSK_SHADER_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/String.hpp>

#include <vector>

namespace Dusk {

enum class ShaderStage
{
    Vertex,
    Fragment,
    Geometry,
    Compute,
    TessControl,
    TessEvaluation,

}; // enum class ShaderStage

class DUSK_ENGINE_API Shader : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Shader)

    Shader() = default;

    virtual ~Shader() = default;

    virtual void Terminate() { }

    virtual bool LoadFromFiles(const std::vector<string>& filenames, bool useAssetPath = true) = 0;

    // GetSize()
    // SetPreprocessorDefines()
    // GetPreprocessorDefines()

};

inline string ShaderStageToString(ShaderStage shaderStage)
{
    switch (shaderStage) {
        case ShaderStage::Vertex:
            return "Vertex";
        case ShaderStage::Fragment:
            return "Fragment";
        case ShaderStage::Geometry:
            return "Geometry";
        case ShaderStage::Compute:
            return "Compute";
        case ShaderStage::TessControl:
            return "TessControl";
        case ShaderStage::TessEvaluation:
            return "TessEvaluation";
    }

    return "Unknown";
}

} // namespace Dusk

#endif // DUSK_SHADER_HPP
