#include "glTF2File.hpp"

#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>

#include <Base64.h>
#include <fstream>

namespace Dusk::GLTF2 {

bool glTF2File::LoadFromFile(const std::string& filename)
{
    DuskBenchmarkStart();

    Filename = filename;
    BaseDir = GetDirname(Filename);

    std::ifstream file;
    file.open(Filename, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        DuskLogError("Failed to load glTF2, '%s'", Filename);
        return false;
    }

    const auto& ext = GetExtension(Filename);
    
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

        if (!LoadBuffers()) {
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

    if (!LoadSamplers()) {
        return false;
    }

    if (!LoadTextures()) {
        return false;
    }

    if (!LoadCameras()) {
        return false;
    }

    if (!LoadMeshes()) {
        return false;
    }

    DuskLogLoad("glTF2 file '%s'", Filename);

    DuskBenchmarkEnd("glTF2File::LoadFromFile");
    return true;
}

bool glTF2File::LoadBuffers()
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

                std::string path = BaseDir + DUSK_PATH_SLASH + uri;
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
        for (const auto& object : JSON[IMAGES_PATH]) {
            Images.push_back(ImageData{
                object.value("uri", ""),
                object.value("bufferView", -1),
                object.value("mimeType", ""),
            });
        }
    }

    DuskLogVerbose("Loaded %d Image(s)", Images.size());

    return true;
}

bool glTF2File::LoadSamplers()
{
    const auto SAMPLERS_PATH = json::json_pointer("/samplers");

    if (JSON.contains(SAMPLERS_PATH)) {
        for (const auto& object : JSON[SAMPLERS_PATH]) {
            Dusk::Texture::Options options;
            options.WrapS = GetWrapType(object.value("wrapS", 0));
            options.WrapT = GetWrapType(object.value("wrapT", 0));
            options.MagFilter = GetFilterType(object.value("magFilter", 0));
            options.MinFilter = GetFilterType(object.value("minFilter", 0));
            options.GenerateMipmaps = false;
            Samplers.push_back(options);
        }
    }

    DuskLogVerbose("Loaded %d Samplers(s)", Samplers.size());

    return true;
}

bool glTF2File::LoadTextures()
{
    const auto TEXTURES_PATH = json::json_pointer("/textures");

    GraphicsDriver * gfx = Dusk::GetGraphicsDriver();
    if (!gfx) {
        DuskLogError("Unable to load textures from a glTF2 file without a graphics driver");
        return false;
    }

    if (JSON.contains(TEXTURES_PATH)) {
        for (const auto& object : JSON[TEXTURES_PATH]) {
            int sampler = object.value("sampler", -1);
            int source = object.value("source", -1);

            if (source < 0 || source >= Images.size()) {
                DuskLogError("Invalid glTF2 sourceimage: %d\n", source);
                return false;
            }

            Dusk::Texture::Options opts;
            if (sampler >= 0 && sampler < Samplers.size()) {
                opts = Samplers[sampler];
            }

            const auto& image = Images[source];
            if (image.bufferView >= 0) {
                DuskLogVerbose("Loading glTF2 texture from bufferView");

                const auto& bufferView = BufferViews[image.bufferView];
                const auto& buffer = Buffers[bufferView.buffer];

                if (bufferView.byteStride > 0) {
                    DuskLogError("Unsupported glTF2 bufferView, byteStride != 0");
                    return false;
                }

                const uint8_t * ptr = buffer.data() + bufferView.byteOffset;

                auto texture = LoadTextureFromMemory(ptr, bufferView.byteLength, opts);
                if (!texture) {
                    return false;
                }

                Textures.push_back(texture);
            }
            else if (image.uri.rfind("data:", 0) == 0) {
                DuskLogVerbose("Loading glTF2 texture from base64 encoded uri");
                
                size_t pivot = image.uri.find(',');
                const auto& data = macaron::Base64::Decode(image.uri.substr(pivot + 1));

                auto texture = LoadTextureFromMemory(data.data(), data.size(), opts);
                if (!texture) {
                    return false;
                }

                Textures.push_back(texture);
            }
            else {
                std::string path = BaseDir + DUSK_PATH_SLASH + image.uri;
                DuskLogVerbose("Loading glTF2 texture from file: '%s'", path);
                
                auto texture = LoadTextureFromFile(path, opts);
                if (!texture) {
                    return false;
                }

                Textures.push_back(texture);
            }
        }
    }

    DuskLogVerbose("Loaded %d Texture(s)", Textures.size());

    return true;
}

bool glTF2File::LoadMaterials()
{
    const auto MATERIALS_PATH = json::json_pointer("/materials");

    // if (JSON.contains(MATERIALS_PATH)) {
    //     for (const auto& object : JSON[MATERIALS_PATH]) {

            // extensions
            /// KHR_materials_transmission
            /// KHR_materials_clearcoat
            /// KHR_materials_pbrSpecularGlossiness
            /// KHR_materials_variants
            /// KHR_techniques_webgl

            // each texture
            /// [scale] = 1
            /// index
            /// [texCoord] = 0

            // name

            // [alphaMode] ?
            /// OPAQUE, MASK, BLEND

            // [doubleSided]

            // [normal]
            // [occlusionTexture]
            // [emissiveTexture]
            // [emissiveFactor]

            // [pbrMetallicRoughness]
            /// [baseColorFactor]
            /// [baseColorTexture]
            /// [metallicFactor]
            /// [roughnessFactor]
            /// [metallicRoughnessTexture]

    //     }
    // }

    DuskLogVerbose("Loaded %d Material(s)", Materials.size());

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
                auto it = object.find("perspective");
                if (it == object.end()) {
                    DuskLogError("Invalid glTF2 perspective camera");
                    return false;
                }

                const auto& perspective = it.value();
                camera.aspectRatio = perspective.value<float>("aspectRatio", 0.0f);
                camera.yfov = perspective.value<float>("yfov", 0.0f);
                camera.zfar = perspective.value<float>("zfar", 0.0f);
                camera.znear = perspective.value<float>("znear", 0.0f);
            }
            else if (camera.type == "orthographic") {
                auto it = object.find("orthographic");
                if (it == object.end()) {
                    DuskLogError("Invalid glTF2 orthographic camera");
                    return false;
                }

                const auto& orthographic = it.value();
                camera.xmag = orthographic.value("xmag", 0.0f);
                camera.ymag = orthographic.value("ymag", 0.0f);
                camera.zfar = orthographic.value("zfar", 0.0f);
                camera.znear = orthographic.value("znear", 0.0f);
            }
            else {
                DuskLogWarn("Unknown glTF2 camera type: '%s'", camera.type);
            }
        }
    }

    DuskLogVerbose("Loaded %d Camera(s)", Cameras.size());

    return true;
}

// gltf2 Node
    // camera (Camera Component?)
    // children[]
    // skin (needs mesh)
    // matrix (rip apart into TRS? Error out instead)
    // mesh (Mesh Component)
    // translate
    // rotation
    // scale
    // weights (needs mesh)

// gltf2 

// Entity -> glft2 Node
// Mesh -> gltf2 Primitive
// MeshComponent -> gltf2 Mesh

bool glTF2File::LoadMeshes()
{
    const auto MESHES_PATH = json::json_pointer("/meshes");

    if (JSON.contains(MESHES_PATH)) {
        for (const auto& object : JSON[MESHES_PATH]) {
            // std::vector<GLTF2PrimitiveData>

            auto it = object.find("primitives");
            if (it != object.end()) {
                const auto& primitives = it.value();
                if (!primitives.is_array()) {
                    DuskLogError("Invalid glTF2 mesh, missing primitives array");
                    return false;
                }

                for (const auto& primitive : primitives) {
                    int indicesIndex = primitives.value("indices", -1);
                    
                    if (indicesIndex >= 0) {
                        // const auto& indexAccessor = Accessors[indicesIndex];
                        // const auto& indexBufferView = BufferViews[indexAccessor.bufferView];
                        // const auto& indexBuffer = Buffers[indexBufferView.buffer];


                    }

                    it = primitive.find("attributes");
                    if (it != object.end()) {
                        const auto& attributes = it.value();
                        if (!attributes.is_object()) {
                            DuskLogError("Invalid glTF2 primitive, missing attributes dictionary");
                            return false;
                        }

                        // for (const auto& [name, index] : attributes.items()) {

                        // }
                    }
                }

                if (auto it = object.find("attributes"); it != primitives.end()) {
                    // const auto& attributes = it.value();

                }




                // attributes
                // material
                // mode
                // targets (animation)
            }

            // Meshes.push_back()
        }
    }

    return true;
}

} // namespace Dusk::GLTF2