#include "dusk/core/glTF2.hpp"

#include <dusk/core/Benchmark.hpp>
#include <dusk/core/Util.hpp>
#include <dusk/core/JSON.hpp>
#include <dusk/asset/Texture.hpp>

#include <fstream>
#include <string>

namespace dusk {

namespace glTF2 {

bool loadBinary(const std::string& filename) 
{
    const auto& paths = GetAssetPaths();

    std::ifstream file;
    std::string fullPath;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath);
        
        file.open(fullPath.c_str(), std::ios::in | std::ios::binary);
        if (file.is_open()) {
            break;
        }
    }

    if (!file.is_open())
    {
        DuskLogError("Failed to load glTF, '%s'", filename);
        return false;
    }

    return true;
}

bool loadASCII(const std::string& filename)
{
    const auto& paths = GetAssetPaths();

    std::ifstream file;
    std::string fullPath;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath);
        
        file.open(fullPath.c_str(), std::ios::in);
        if (file.is_open()) {
            break;
        }
    }

    if (!file.is_open())
    {
        DuskLogError("Failed to load glTF, '%s'", filename);
        return false;
    }

    std::string dir = GetDirname(fullPath);

    nlohmann::json data;
    file >> data;

    if (auto it = data.find("asset"); it != data.end()) {
        auto& object = it.value();

        const auto& version = object.value("version", "");

        DuskLogVerbose("glTF Generator %s", object.value("generator", ""));
        DuskLogVerbose("glTF Version %s", object.value("version", ""));
        
        if (version != "2.0") {
            DuskLogError("only glTF 2.0 is supported");
            return false;
        }
    }

    std::vector<std::vector<uint8_t>> buffers;
    if (auto it = data.find("buffers"); it != data.end()) {
        auto& array = it.value();
        for (auto object : array) {
            size_t byteLength = object.value<size_t>("byteLength", 0);
            const auto& uri = object.value("uri", "");

            DuskLogVerbose("Buffer %zu, %s", byteLength, uri);

            std::ifstream bufferFile(dir + "/" + uri);
            if (!bufferFile.is_open()) {
                DuskLogError("Failed to open glTF data file '%s'", uri);
                continue;
            }

            buffers.push_back(std::vector<uint8_t>(
                (std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>())
            );

            DuskLogLoad("glTF data file '%s'", uri);
            
            bufferFile.close();
        }
    }

    struct bufferView {
        size_t buffer;
        size_t byteLength;
        size_t byteOffset;
        GLenum target;
    };
    std::vector<bufferView> bufferViews;
    if (auto it = data.find("bufferViews"); it != data.end()) {
        auto& array = it.value();
        for (auto object : array) {
            bufferViews.push_back(bufferView{
                object.value<size_t>("buffer", 0),
                object.value<size_t>("byteLength", 0),
                object.value<size_t>("byteOffset", 0),
                object.value<GLenum>("target", 0),
            });

            auto& b = bufferViews.back();
            DuskLogVerbose("BufferView %zu, %zu, %zu, %zu", b.buffer, b.byteLength, b.byteOffset, b.target);
        }
    }

    std::vector<std::shared_ptr<Texture>> textures;
    if (auto it = data.find("textures"); it != data.end()) {
        auto& array = it.value();
        for (auto object : array) {
            size_t sampler = object.value<size_t>("sampler", 0);
            size_t source = object.value<size_t>("source", 0);

            DuskLogVerbose("Texture %zu, %zu", sampler, source);
        }
    }

    return true;
}

bool LoadSceneFromFile(const std::string& filename)
{
    DuskBenchStart();

    const std::string& ext = GetExtension(filename);
    bool binary = (ext == "glb");

    bool loaded = (binary ? loadBinary(filename) : loadASCII(filename));

    DuskBenchEnd("glTF2::LoadSceneFromFile");
    return loaded;
}

bool LoadModelFromFile(const std::string& filename)
{
    DuskBenchStart();


    DuskBenchEnd("glTF2::LoadModelFromFile");
    return false;
}

} // namespace glTF2

} // namespace dusk