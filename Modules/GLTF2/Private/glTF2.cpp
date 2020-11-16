#include <Dusk/GLTF2/glTF2File.hpp>

#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>

#include <Base64.h>
#include <fstream>

namespace Dusk::GLTF2 {

bool glTF2File::LoadFromFile(const std::string& filename)
{
    DuskBenchmarkStart();

    std::ifstream file;
    file.open(filename, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        DuskLogError("Failed to load glTF2, '%s'", filename);
        return false;
    }

    const auto& dir = GetDirname(filename);
    const auto& ext = GetExtension(filename);
    
    if (ext == "glb") {
        // Header
        uint32_t magic;
        file.read(reinterpret_cast<char *>(&magic), sizeof(magic));
        if (magic != MAGIC) {
            DuskLogError("Invalid binary glTF2 file header");
            return false;
        }

        uint32_t version;
        file.read(reinterpret_cast<char *>(&version), sizeof(version));
        if (version != 2) {
            DuskLogError("Invalid binary glTF2 container version, %u", version);
            return false;
        }

        uint32_t length;
        file.read(reinterpret_cast<char *>(&length), sizeof(length));

        // JSON Chunk

        uint32_t jsonChunkLength;
        file.read(reinterpret_cast<char *>(&jsonChunkLength), sizeof(jsonChunkLength));

        ChunkType jsonChunkType;
        file.read(reinterpret_cast<char *>(&jsonChunkType), sizeof(jsonChunkType));

        if (jsonChunkType != ChunkType::JSON) {
            DuskLogError("Invalid binary glTF2 file. The first chunk must be JSON, found '%08x'", jsonChunkType);
            return false;
        }

        std::vector<char> jsonChunk(jsonChunkLength + 1);
        file.read(jsonChunk.data(), jsonChunkLength);
        jsonChunk.back() = '\0';

        JSON = json::parse(jsonChunk.data());

        // Buffer Chunk (Optional)

        if (file.tellg() < length) {
            uint32_t bufferChunkLength;
            file.read(reinterpret_cast<char *>(&bufferChunkLength), sizeof(bufferChunkLength));

            ChunkType bufferChunkType;
            file.read(reinterpret_cast<char *>(&bufferChunkType), sizeof(bufferChunkType));

            if (bufferChunkType != ChunkType::BIN) {
                DuskLogError("Invalid binary glTF2 file. The second chunk must be BIN, found '%08x'", bufferChunkType);
                return false;
            }

            Buffers.push_back(std::vector<uint8_t>(bufferChunkLength));
            file.read(reinterpret_cast<char *>(Buffers.back().data()), bufferChunkLength);
        }
    }
    else {
        file >> JSON;

        if (!LoadBuffers(dir)) {
            return false;
        }
    }

    const auto VERSION_PATH = json::json_pointer("/asset/version");

    if (JSON.contains(VERSION_PATH)) {
        auto version = JSON[VERSION_PATH].get<std::string>();
        DuskLogVerbose("glTF2 Version: %s", version);
    }

    const auto GENERATOR_PATH = json::json_pointer("/asset/generator");

    if (JSON.contains(GENERATOR_PATH)) {
        auto generator = JSON[GENERATOR_PATH].get<std::string>();
        DuskLogVerbose("glTF2 Generator: %s", generator);
    }

    if (!LoadBufferViews()) {
        return false;
    }

    if (!LoadAccessors()) {
        return false;
    }

    if (!LoadImages()) {
        return false;
    }

    if (!LoadCameras()) {
        return false;
    }

    DuskLogLoad("glTF2 file '%s'", filename);

    DuskBenchmarkEnd("glTF2File::LoadFromFile");
    return true;
}

bool glTF2File::LoadBuffers(const std::string& dir)
{
    const auto BUFFERS_PATH = json::json_pointer("/buffers");

    if (JSON.contains(BUFFERS_PATH)) {
        for (const auto& object : JSON[BUFFERS_PATH]) {
            const auto& byteLength = object.value<size_t>("byteLength", 0);
            const auto& uri = object.value("uri", "");

            if (uri.rfind("data:", 0) == 0) {
                size_t pivot = uri.find(',');
                Buffers.push_back(macaron::Base64::Decode(uri.substr(pivot + 1)));
            }
            else {
                DuskLogVerbose("Loading glTF2 buffer from '%s'", uri);

                std::string path = dir + DUSK_PATH_SEPARATOR + uri;
                std::ifstream file(path, std::ios::in | std::ios::binary);
                if (!file.is_open()) {
                    DuskLogError("Failed to read glTF2 buffer from '%s'", uri);
                    return false;
                }

                Buffers.push_back(std::vector<uint8_t>(byteLength));
                file.read(reinterpret_cast<char *>(Buffers.back().data()), byteLength);

                DuskLogLoad("glTF2 buffer '%s'", path);
            }
        }
    }
    else {
        return false;
    }

    DuskLogVerbose("Loaded %d Buffer(s)", Buffers.size());

    return true;
}

bool glTF2File::LoadBufferViews()
{
    const auto BUFFER_VIEWS_PATH = json::json_pointer("/bufferViews");

    if (JSON.contains(BUFFER_VIEWS_PATH)) {
        for (const auto& object : JSON[BUFFER_VIEWS_PATH]) {
            BufferViews.push_back(BufferViewData{
                object.value<int>("buffer", -1),
                object.value<size_t>("byteLength", 0),
                object.value<size_t>("byteOffset", 0),
                object.value<size_t>("byteStride", 0),
                object.value<GLenum>("target", GL_INVALID_ENUM),
            });
        }
    }
    else {
        return false;
    }

    DuskLogVerbose("Loaded %d Buffer View(s)", BufferViews.size());

    return true;
}

bool glTF2File::LoadAccessors()
{
    const auto ACCESSORS_PATH = json::json_pointer("/accessors");

    if (JSON.contains(ACCESSORS_PATH)) {
        for (const auto& object : JSON[ACCESSORS_PATH]) {
            Accessors.push_back(AccessorData{
                object.value<int>("bufferView", -1),
                object.value<size_t>("byteOffset", 0),
                object.value<size_t>("count", 0),
                object.value<std::string>("type", ""),
                object.value<GLenum>("componentType", GL_INVALID_ENUM),
                object.value<bool>("normalized", false),
            });
        }
    }
    else {
        return false;
    }

    DuskLogVerbose("Loaded %d Accessor(s)", Accessors.size());

    return true;
}

bool glTF2File::LoadImages()
{
    const auto IMAGES_PATH = json::json_pointer("/images");

    if (JSON.contains(IMAGES_PATH)) {
        for (const auto& image : JSON[IMAGES_PATH]) {
            Images.push_back(ImageData{

            });
        }
    }

    DuskLogVerbose("Loaded %d Image(s)", Images.size());

    return true;
}

bool glTF2File::LoadCameras()
{
    const auto CAMERAS_PATH = json::json_pointer("/cameras");

    if (JSON.contains(CAMERAS_PATH)) {
        for (const auto& object : JSON[CAMERAS_PATH]) {
            Cameras.push_back(CameraData{
                object.value<std::string>("name", ""),
                object.value<std::string>("type", ""),
            });
            auto& camera = Cameras.back();

            if (camera.type == "perspective") {
                if (auto perspective = object.find("perspective"); perspective != object.end()) {
                }
            }
            else if (camera.type == "orthographic") {
                if (auto orthographic = object.find("orthographic"); orthographic != object.end()) {
                }
            }
            else {
                DuskLogWarn("Unknown camera type: '%s'", camera.type);
            }
        }
    }

    DuskLogVerbose("Loaded %d Camera(s)", Cameras.size());

    return true;
}

} // namespace Dusk::GLTF2