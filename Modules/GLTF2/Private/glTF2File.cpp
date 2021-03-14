#include "glTF2File.hpp"

#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/GLTF2/GLTF2PrimitiveData.hpp>

#include <Base64.h>
#include <fstream>

namespace Dusk::GLTF2 {

bool glTF2File::IsValidTexture(int index, int texCoord)
{
    if (index < 0 || index > static_cast<int>(Textures.size())) {
        DuskLogError("Invalid glTF2 Texture Index: %d", index);
        return false;
    }

    if (texCoord > 0) {
        DuskLogWarn("Multiple glTF2 TEXCOORDs not supported");
        return false;
    }

    return true;
}

bool glTF2File::LoadFromFile(const string& filename)
{
    DuskBenchmarkStart();

    Filename = filename;
    BaseDir = GetDirname(Filename);

    std::ifstream file;
    file.open(Filename, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
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

        try {
            JSON = json::parse(jsonChunk.data());
        }
        catch (...) {
            return false;
        }

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
        try {
            file >> JSON;
        }
        catch (...) {
            return false;
        }

        if (!LoadBuffers()) {
            return false;
        }
    }

    const auto VERSION_PATH = json::json_pointer("/asset/version");

    if (JSON.contains(VERSION_PATH)) {
        auto version = JSON[VERSION_PATH].get<string>();
        DuskLogVerbose("glTF2 Version: %s", version);
    }

    const auto GENERATOR_PATH = json::json_pointer("/asset/generator");

    if (JSON.contains(GENERATOR_PATH)) {
        auto generator = JSON[GENERATOR_PATH].get<string>();
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

    if (!LoadMaterials()) {
        return false;
    }

    if (!LoadCameras()) {
        return false;
    }

    DuskBenchmarkEnd();
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

                string path = BaseDir + DUSK_PATH_SLASH + uri;
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
                object.value<string>("type", ""),
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
                string path = BaseDir + DUSK_PATH_SLASH + image.uri;
                DuskLogVerbose("Loading glTF2 texture from file: '%s'", path);
                
                auto texture = LoadTextureFromFile(path, false, opts);
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

    GraphicsDriver * gfx = Dusk::GetGraphicsDriver();
    if (!gfx) {
        DuskLogError("Unable to load textures from a glTF2 file without a graphics driver");
        return false;
    }

    if (JSON.contains(MATERIALS_PATH)) {
        for (const auto& object : JSON[MATERIALS_PATH]) {
            auto material = gfx->CreateMaterial();
            material->Initialize();

            auto it = object.find("normalTexture");
            if (it != object.end()) {
                const auto& value = it.value();
                if (value.is_object()) {
                    int index = value.value("index", -1);
                    int texCoord = value.value("texCoord", 0);

                    if (IsValidTexture(index, texCoord)) {
                        material->SetNormalMap(Textures[index]);
                    }

                    material->SetNormalScale(value.value("scale", material->GetNormalScale()));
                }
                else {
                    DuskLogWarn("Malformed glTF2 normalTexture");
                }
            }

            it = object.find("emissiveTexture");
            if (it != object.end()) {
                const auto& value = it.value();
                if (value.is_object()) {
                    int index = value.value("index", -1);
                    int texCoord = value.value("texCoord", 0);

                    if (IsValidTexture(index, texCoord)) {
                        material->SetEmissiveMap(Textures[index]);
                        
                        // TODO: Improve, this is a fix for (map * factor) == 0
                        material->SetEmissiveFactor(vec3(1.0f));
                    }
                }
                else {
                    DuskLogWarn("Malformed glTF2 emissiveTexture");
                }
            }

            it = object.find("emissiveFactor");
            if (it != object.end()) {
                material->SetEmissiveFactor(ExtractVec3(it.value(), material->GetEmissiveFactor()));
            }

            it = object.find("occlusionTexture");
            if (it != object.end()) {
                const auto& value = it.value();
                if (value.is_object()) {
                    int index = value.value("index", -1);
                    int texCoord = value.value("texCoord", 0);

                    if (IsValidTexture(index, texCoord)) {
                        material->SetOcclusionMap(Textures[index]);
                    }

                    material->SetOcclusionStrength(value.value("strength", material->GetOcclusionStrength()));
                }
                else {
                    DuskLogWarn("Malformed glTF2 occlusionTexture");
                }
            }

            it = object.find("pbrMetallicRoughness");
            if (it != object.end()) {
                const auto& group = it.value();
                if (group.is_object()) {
                    it = group.find("baseColorFactor");
                    if (it != group.end()) {
                        material->SetBaseColorFactor(ExtractVec4(it.value(), material->GetBaseColorFactor()));
                    }

                    it = group.find("baseColorTexture");
                    if (it != group.end()) {
                        const auto& value = it.value();
                        if (value.is_object()) {
                            int index = value.value("index", -1);
                            int texCoord = value.value("texCoord", 0);

                            if (IsValidTexture(index, texCoord)) {
                                material->SetBaseColorMap(Textures[index]);
                            }
                        }
                        else {
                            DuskLogWarn("Malformed glTF2 baseColorTexture");
                        }
                    }

                    material->SetMetallicFactor(group.value("metallicFactor", material->GetMetallicFactor()));

                    material->SetRoughnessFactor(group.value("roughnessFactor", material->GetRoughnessFactor()));

                    it = group.find("metallicRoughnessTexture");
                    if (it != group.end()) {
                        const auto& value = it.value();
                        if (value.is_object()) {
                            int index = value.value("index", -1);
                            int texCoord = value.value("texCoord", 0);

                            if (IsValidTexture(index, texCoord)) {
                                material->SetMetallicRoughnessMap(Textures[index]);
                            }
                        }
                        else {
                            DuskLogWarn("Malformed glTF2 metallicRoughnessTexture");
                        }
                    }
                }
            }

            // it = object.find("pbrSpecularGlossiness");
            // if (it != object.end()) {
            // }

            // extensions
            /// KHR_materials_transmission
            /// KHR_materials_clearcoat
            /// KHR_materials_pbrSpecularGlossiness
            /// KHR_materials_variants
            /// KHR_techniques_webgl

            // name

            // [alphaMode] ?
            /// OPAQUE, MASK, BLEND

            // [doubleSided]

            Materials.push_back(material);
        }
    }

    DuskLogVerbose("Loaded %d Material(s)", Materials.size());

    return true;
}

bool glTF2File::LoadCameras()
{
    const auto CAMERAS_PATH = json::json_pointer("/cameras");

    if (JSON.contains(CAMERAS_PATH)) {
        for (const auto& object : JSON[CAMERAS_PATH]) {
            Cameras.push_back(CameraData{
                object.value<string>("name", ""),
                object.value<string>("type", ""),
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

std::vector<std::unique_ptr<PrimitiveData>> glTF2File::LoadMesh()
{
    const auto MESHES_PATH = json::json_pointer("/meshes");

    if (JSON.contains(MESHES_PATH)) {
        std::vector<std::unique_ptr<PrimitiveData>> primitiveDataList;
        for (const auto& object : JSON[MESHES_PATH]) {

            auto iter = object.find("primitives");
            if (iter != object.end()) {
                const auto& primitives = iter.value();
                if (!primitives.is_array()) {
                    DuskLogError("Invalid glTF2 mesh, missing primitives array");
                    continue;
                }

                for (const auto& primitive : primitives) {
                    // TODO: Possibly change
                    GLTF2PrimitiveData * primitiveData = New GLTF2PrimitiveData();
                    primitiveDataList.push_back(std::unique_ptr<PrimitiveData>(primitiveData));

                    int materialIndex = primitive.value("material", -1);
                    if (materialIndex >= 0) {
                        primitiveData->material = Materials[materialIndex];
                    }

                    int indicesIndex = primitive.value("indices", -1);
                    GLenum primitiveType = primitive.value<GLenum>("mode", GL_TRIANGLES);

                    if (primitiveType == GL_LINE_LOOP) {
                        DuskLogWarn("Unsupported glTF2 primitive mode: GL_LINE_LOOP");
                        continue;
                    }
                    else if (primitiveType == GL_TRIANGLE_FAN) {
                        DuskLogWarn("Unsupported glTF2 primitive mode: GL_TRIANGLE_FAN");
                        continue;
                    }

                    switch (primitiveType) {
                    case GL_POINTS:
                        primitiveData->topology = PrimitiveTopology::PointList;
                        break;
                    case GL_LINES:
                        primitiveData->topology = PrimitiveTopology::LineList;
                        break;
                    case GL_LINE_STRIP:
                        primitiveData->topology = PrimitiveTopology::LineStrip;
                        break;
                    case GL_TRIANGLES:
                        primitiveData->topology = PrimitiveTopology::TriangleList;
                        break;
                    case GL_TRIANGLE_STRIP:
                        primitiveData->topology = PrimitiveTopology::TriangleStrip;
                        break;
                    }
                    
                    if (indicesIndex >= 0) {
                        const auto& accessor = Accessors[indicesIndex];
                        primitiveData->indexList.resize(accessor.count);

                        AccessorIterator iterIndex(this, indicesIndex);
                        for (auto& index : primitiveData->indexList) {
                            index = iterIndex.getInteger();
                            ++iterIndex;
                        }
                    }

                    iter = primitive.find("attributes");
                    if (iter != primitive.end()) {
                        const auto& attributes = iter.value();
                        if (!attributes.is_object()) {
                            DuskLogError("Invalid glTF2 primitive, missing attributes dictionary");
                            continue;
                        }

                        int positionIndex = attributes.value("POSITION", -1);
                        if (positionIndex < 0) {
                            DuskLogError("Invalid glTF2 primitive, missing POSITION");
                            continue;
                        }

                        int tangentIndex = attributes.value("TANGENT", -1);

                        const auto& positionAccessor = Accessors[positionIndex];
                        primitiveData->vertexList.resize(positionAccessor.count);

                        AccessorIterator iterPosition (this, positionIndex);
                        AccessorIterator iterNormal   (this, attributes.value("NORMAL", -1));
                        AccessorIterator iterTangent  (this, tangentIndex);
                        AccessorIterator iterTexCoord1(this, attributes.value("TEXCOORD_0", -1));
                        AccessorIterator iterTexCoord2(this, attributes.value("TEXCOORD_1", -1));
                        AccessorIterator iterColor    (this, attributes.value("COLOR_0", -1));
                        AccessorIterator iterJoints   (this, attributes.value("JOINTS_0", -1));
                        AccessorIterator iterWeights  (this, attributes.value("WEIGHTS_0", -1));

                        for (auto& vertex : primitiveData->vertexList) {
                            vertex.Position = iterPosition.getVec4({ 0.0f, 0.0f, 0.0f, 1.0f });
                            ++iterPosition;

                            vertex.Normal = iterNormal.getVec4({ 0.0f, 0.0f, 0.0f, 1.0f });
                            ++iterNormal;

                            vertex.Tangent = iterTangent.getVec4();
                            ++iterTangent;

                            vertex.TexCoord1 = iterTexCoord1.getVec2();
                            ++iterTexCoord1;

                            vertex.TexCoord2 = iterTexCoord2.getVec2();
                            ++iterTexCoord2;

                            vertex.Color = iterColor.getVec4({ 0.0f, 0.0f, 0.0f, 1.0f });
                            ++iterColor;

                            vertex.Joints = iterJoints.getUVec4();
                            ++iterJoints;

                            vertex.Weights = iterWeights.getVec4();
                            ++iterWeights;
                        }

                        if (tangentIndex < 0) {
                            primitiveData->CalculateTBN();
                        }
                    }
                }
            }
        }

        return primitiveDataList;
    }

    return { };
}

} // namespace Dusk::GLTF2