#include <Dusk/DirectX/Shader.hpp>
#include <Dusk/DirectX/GraphicsDriver.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>

#include <fstream>
#include <sstream>

namespace Dusk::DirectX {

DUSK_DIRECTX_API
bool Shader::LoadFromFiles(const std::vector<std::string>& filenames)
{
    DuskBenchmarkStart();

    GraphicsDriver * gfx = (GraphicsDriver *)GetGraphicsDriver();
    ID3D12Device * dev = gfx->GetDevice();

    const auto& shaderPaths = GetShaderPaths();

    for (const auto& filename : filenames) {
        std::ifstream file;

        for (const auto& path : shaderPaths) {
            const std::string& fullPath = path + filename;
            DuskLogVerbose("Checking '%s'", fullPath);

            file.open(fullPath, std::ios::binary);
            if (file.is_open()) {
                break;
            }
        }

        if (!file.is_open()) {
            DuskLogError("Failed to load '%s'", filename);
            return false;
        }

        file.unsetf(std::ios::skipws);

        DuskLogLoad("Loading DirectX shader '%s'", filename);

        std::vector<uint8_t> data(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );

        size_t pivot = filename.find_last_of('.');
        if (pivot == std::string::npos) {
            DuskLogWarn("Failed to find shader type extension for '%s'", filename.c_str());
            continue;
        }
        
        const std::string& ext = GetExtension(filename.substr(0, pivot));
        if (ext == "vert" || ext == "vertex") {
            dev->CreateVertexShader(data.data(), data.size(), );
        }
        else if (ext == "frag" || ext == "fragment") {
            // dev->CreateFragmentShader();
        }
        else {

        }

    }

    DuskBenchmarkEnd("DirectX::Shader::LoadFromFiles");
    return true;
}

DUSK_DIRECTX_API
void Shader::Bind()
{

}

} // namespace Dusk::DirectX