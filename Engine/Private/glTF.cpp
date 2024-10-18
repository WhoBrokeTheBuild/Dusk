#include "glTF.hpp"
#include <Dusk/Base64.hpp>
#include <Dusk/File.hpp>
#include <Dusk/String.hpp>
#include <Dusk/ThirdParty/vulkan.hpp>
#include <Dusk/Tuple.hpp>
#include <Dusk/VulkanTexture.hpp>
#include <Dusk/Material.hpp>
#include <Dusk/JSON.hpp>
#include <Dusk/Log.hpp>

namespace dusk {
namespace glTF {

Optional<vk::SamplerAddressMode> mapTextureWrapToVulkan(GLenum textureWrap)
{
    switch (textureWrap) {
    case GL_REPEAT:
        return vk::SamplerAddressMode::eRepeat;
    case GL_MIRRORED_REPEAT:
        return vk::SamplerAddressMode::eMirroredRepeat;
    case GL_CLAMP_TO_EDGE:
        return vk::SamplerAddressMode::eClampToEdge;
    case GL_CLAMP_TO_BORDER:
        return vk::SamplerAddressMode::eClampToBorder;
    }

    return nullopt;
}

Tuple<Optional<vk::Filter>, Optional<vk::SamplerMipmapMode>> mapTextureFilterToVulkan(GLenum textureFilter)
{
    switch (textureFilter) {
    case GL_NEAREST:
        return { vk::Filter::eNearest, nullopt };
    case GL_NEAREST_MIPMAP_NEAREST:
        return { vk::Filter::eNearest, vk::SamplerMipmapMode::eNearest };
    case GL_NEAREST_MIPMAP_LINEAR:
        return { vk::Filter::eNearest, vk::SamplerMipmapMode::eLinear };
    case GL_LINEAR:
        return { vk::Filter::eLinear, nullopt };
    case GL_LINEAR_MIPMAP_NEAREST:
        return { vk::Filter::eLinear, vk::SamplerMipmapMode::eNearest };
    case GL_LINEAR_MIPMAP_LINEAR:
        return { vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear };
    }

    return { nullopt, nullopt };
}

Optional<vk::PrimitiveTopology> mapTopologyToVulkan(GLenum topology)
{
    switch (topology) {
    case GL_POINTS:
        return vk::PrimitiveTopology::ePointList;
    case GL_LINES:
        return vk::PrimitiveTopology::eLineList;
    case GL_LINE_LOOP:
        return vk::PrimitiveTopology::eLineStrip;
    case GL_TRIANGLES:
        return vk::PrimitiveTopology::eTriangleList;
    case GL_TRIANGLE_STRIP:
        return vk::PrimitiveTopology::eTriangleStrip;
    case GL_TRIANGLE_FAN:
        return vk::PrimitiveTopology::eTriangleFan;
    }

    return nullopt;
}

bool Loader::LoadFromFile(const Path& path)
{
    File file(path, File::Mode::Read);
    if (!file) {
        return false;
    }

    JSON::Object json;

    Path directory = path.parent_path();
    auto extension = path.extension();
    if (extension == ".glb") {
        uint32_t magic = file.ReadOne<uint32_t>().value();
        if (magic != MAGIC) {
            Log(DUSK_ANCHOR, "Invalid binary glTF file magic");
            return false;
        }

        uint32_t version = file.ReadOne<uint32_t>().value();
        if (version != 2) {
            Log(DUSK_ANCHOR, "Unsupported binary glTF container version, {}", version);
            return false;
        }

        uint32_t totalLength = file.ReadOne<uint32_t>().value();

        // JSON Chunk

        uint32_t jsonChunkLength = file.ReadOne<uint32_t>().value();
        uint32_t jsonChunkType = file.ReadOne<uint32_t>().value();
        if (jsonChunkType != CHUNK_JSON) {
            char * c = reinterpret_cast<char *>(jsonChunkType);
            Log(DUSK_ANCHOR, "Invalid glTF file, the first chunk must be JSON, found '{}{}{}{}'", c[0], c[1], c[2], c[3]);
            return false;
        }

        List<char> jsonChunk(jsonChunkLength + 1);
        if (not file.ReadInto(jsonChunk.data(), jsonChunkLength)) {
            Log(DUSK_ANCHOR, "Invalid glTF file, failed to read entire JSON chunk");
            return false;
        }

        jsonChunk.back() = '\0';
        json = JSON::Object::parse(jsonChunk.data());

        // BIN Chunk (Optional)
        
        if (file.GetPosition() < totalLength) {
            uint32_t binChunkLength = file.ReadOne<uint32_t>().value();
            uint32_t binChunkType = file.ReadOne<uint32_t>().value();
            if (binChunkType != CHUNK_BIN) {
                char * c = reinterpret_cast<char *>(binChunkType);
                Log(DUSK_ANCHOR, "Invalid glTF file, the second chunk must be BIN, found '{}{}{}{}'", c[0], c[1], c[2], c[3]);
                return false;
            }

            BufferList.push_back(file.ReadSome(binChunkLength).value());
        }

    }
    else if (extension == ".gltf") {
        file.GetStream() >> json;

        const auto BUFFERS_PATH = JSON::Pointer("/buffers");
        if (json.contains(BUFFERS_PATH)) {
            for (const auto& object : json[BUFFERS_PATH]) {
                // size_t byteLength = object.value<size_t>("byteLength", 0);
                const String& uri = object.value("uri", String());

                if (uri.find("data:") == 0) {
                    size_t pivot = uri.find(',');

                    auto buffer = Base64::Decode(uri.substr(pivot + 1));
                    if (not buffer) {
                        Log(DUSK_ANCHOR, "Failed to decode base64 buffer string '{}'", uri.substr(pivot + 1));
                    }

                    BufferList.push_back(buffer.value());
                }
                else {
                    auto bufferPath = directory / uri;
                    Log(DUSK_ANCHOR, "Loading glTF buffer from '{}'", bufferPath.string());

                    File bufferFile(bufferPath, File::Mode::Read);
                    if (not bufferFile) {
                        Log(DUSK_ANCHOR, "Failed to load glTF buffer file '{}'", bufferPath.string());
                        continue;
                    }

                    BufferList.push_back(bufferFile.ReadAll().value());
                }
            }
        }
    }
    else {
        return false;
    }

    Log(DUSK_ANCHOR, "Loaded {} Buffer(s)", BufferList.size());


    const auto BUFFER_VIEWS_PATH = JSON::Pointer("/bufferViews");
    if (json.contains(BUFFER_VIEWS_PATH)) {
        for (const auto& object : json[BUFFER_VIEWS_PATH]) {
            BufferViewList.push_back(BufferViewData{
                .buffer     = object.value<int>("buffer", -1),
                .byteLength = object.value<size_t>("byteLength", 0),
                .byteOffset = object.value<size_t>("byteOffset", 0),
                .byteStride = object.value<size_t>("byteStride", 0),
                .target     = object.value<GLenum>("target", GL_INVALID_ENUM),
            });

            auto& bufferView = BufferViewList.back();
            Log(DUSK_ANCHOR, "Added buffer view: buffer={} len={} off={} stride={} target={}",
                bufferView.buffer,
                bufferView.byteLength,
                bufferView.byteOffset,
                bufferView.byteStride,
                bufferView.target
            );
        }
    }

    Log(DUSK_ANCHOR, "Loaded {} BufferView(s)", BufferViewList.size());

    const auto ACCESSORS_PATH = JSON::Pointer("/accessors");
    if (json.contains(ACCESSORS_PATH)) {
        for (const auto& object : json[ACCESSORS_PATH]) {
            AccessorList.push_back(AccessorData{
                .bufferView    = object.value<int>("bufferView", -1),
                .byteOffset    = object.value<size_t>("byteOffset", 0),
                .count         = object.value<size_t>("count", 0),
                .type          = object.value<String>("type", ""),
                .componentType = object.value<GLenum>("componentType", GL_INVALID_ENUM),
                .normalized    = object.value<bool>("normalized", false),
            });

            auto& accessor = AccessorList.back();
            Log(DUSK_ANCHOR, "Added accessor: view={} off={} count={} type={} comp={} norm={}",
                accessor.bufferView,
                accessor.byteOffset,
                accessor.count,
                accessor.type,
                accessor.componentType,
                accessor.normalized
            );
        }
    }

    Log(DUSK_ANCHOR, "Loaded {} Accessor(s)", AccessorList.size());

    const auto IMAGES_PATH = JSON::Pointer("/images");
    if (json.contains(IMAGES_PATH)) {
        for (const auto& object : json[IMAGES_PATH]) {
            ImageList.push_back(ImageData{
                .uri        = object.value<String>("uri", ""),
                .bufferView = object.value<int>("bufferView", -1),
                .mimeType   = object.value<String>("mimeType", ""),
            });

            auto& image = ImageList.back();
            Log(DUSK_ANCHOR, "Added image: uri={} view={} mime={}",
                image.uri,
                image.bufferView,
                image.mimeType
            );
        }
    }

    Log(DUSK_ANCHOR, "Loaded {} Image(s)", ImageList.size());

    const auto SAMPLERS_PATH = JSON::Pointer("/samplers");
    if (json.contains(SAMPLERS_PATH)) {
        for (const auto& object : json[SAMPLERS_PATH]) {
            SamplerList.push_back(SamplerData{
                .wrapS           = object.value<GLenum>("wrapS", GL_INVALID_ENUM),
                .wrapT           = object.value<GLenum>("wrapT", GL_INVALID_ENUM),
                .magFilter       = object.value<GLenum>("magFilter", GL_INVALID_ENUM),
                .minFilter       = object.value<GLenum>("minFilter", GL_INVALID_ENUM),
            });

            auto& sampler = SamplerList.back();
            Log(DUSK_ANCHOR, "Added sampler: S={} T={} mag={} min={}",
                sampler.wrapS,
                sampler.wrapT,
                sampler.magFilter,
                sampler.minFilter
            );
        }
    }

    Log(DUSK_ANCHOR, "Loaded {} Sampler(s)", SamplerList.size());

    const auto TEXTURES_PATH = JSON::Pointer("/textures");
    if (json.contains(TEXTURES_PATH)) {
        for (const auto& object : json[TEXTURES_PATH]) {
            int sampler = object.value("sampler", -1);
            int source = object.value("source", -1);

            if (source < 0 or source >= ImageList.size()) {
                Log(DUSK_ANCHOR, "Invalid glTF file, Texture.source {} out of range [0, {}]", source, ImageList.size());
                return false;
            }

            vk::SamplerCreateInfo samplerCreateInfo;
            if (sampler >= 0 and sampler < SamplerList.size()) {
                const auto& samplerData = SamplerList[sampler];

                auto addressModeU = mapTextureWrapToVulkan(samplerData.wrapS);
                if (addressModeU) {
                    samplerCreateInfo.setAddressModeU(addressModeU.value());
                }

                auto addressModeV = mapTextureWrapToVulkan(samplerData.wrapT);
                if (addressModeU) {
                    samplerCreateInfo.setAddressModeV(addressModeV.value());
                }
                
                auto[magFilter, magMipmapMode] = mapTextureFilterToVulkan(samplerData.magFilter);
                if (magFilter) {
                    samplerCreateInfo.setMagFilter(magFilter.value());
                }

                auto[minFilter, minMipmapMode] = mapTextureFilterToVulkan(samplerData.minFilter);
                if (minFilter) {
                    samplerCreateInfo.setMinFilter(minFilter.value());
                }

                // TODO: Improve
                if (magMipmapMode) {
                    samplerCreateInfo.setMipmapMode(magMipmapMode.value());
                }
                else if (minMipmapMode) {
                    samplerCreateInfo.setMipmapMode(minMipmapMode.value());
                }
            }

            VulkanTexture::Pointer texture(new VulkanTexture());

            bool loaded = false;
            const auto& image = ImageList[source];
            if (image.bufferView >= 0) {
                Log(DUSK_ANCHOR, "Loading glTF texture from image[{}] -> bufferView[{}]", source, image.bufferView);

                const auto& bufferView = BufferViewList[image.bufferView];
                const auto& buffer = BufferList[bufferView.buffer];

                if (bufferView.byteStride > 0) {
                    Log(DUSK_ANCHOR, "Invalid BufferView.byteStride > 0 for Texture");
                    return false;
                }

                loaded = texture->LoadFromBuffer(
                    buffer.data() + bufferView.byteOffset,
                    bufferView.byteLength,
                    samplerCreateInfo
                );
            }
            else if (image.uri.find("data:", 0) == 0) {
                Log(DUSK_ANCHOR, "Loading glTF texture from base64 encoded uri");

                size_t pivot = image.uri.find(',');
                const auto& data = Base64::Decode(image.uri.substr(pivot + 1));
                if (not data) {
                    Log(DUSK_ANCHOR, "Invalid base64 encoded uri");
                }

                loaded = texture->LoadFromBuffer(*data, samplerCreateInfo);
            }
            else {
                Path texturePath = directory / image.uri;
                Log(DUSK_ANCHOR, "Loading glTF texture from '{}'", texturePath.string());

                loaded = texture->LoadFromFile(texturePath, samplerCreateInfo);
            }

            if (not loaded) {
                Log(DUSK_ANCHOR, "Invalid glTF file, failed to load texture");
                return false;
            }

            TextureList.push_back(std::move(texture));
        }
    }

    Log(DUSK_ANCHOR, "Loaded {} Texture(s)", TextureList.size());

    const auto MATERIALS_PATH = JSON::Pointer("/materials");
    if (json.contains(MATERIALS_PATH)) {
        for (const auto& object : json[MATERIALS_PATH]) {
            
            Material::Pointer material(new Material());

            auto it = object.find("normalTexture");
            if (it != object.end()) {
                const auto& value = it.value();
                if (value.is_object()) {
                    int index = value.value("index", -1);
                    int texCoord = value.value("texCoord", 0);

                    if (index > TextureList.size()) {
                        Log(DUSK_ANCHOR, "Invalid glTF file, failed to load material");
                        return false;
                    }

                    if (texCoord > 0) {
                        Log(DUSK_ANCHOR, "Invalid glTF file, multiple TEXCOORDs are not supported");
                        return false;
                    }

                    if (index >= 0) {
                        material->SetTexture(Material::NormalMapName, TextureList[index]);
                    }

                    material->SetFloatParameter(Material::NormalScaleName, value.value("scale", 1.0f));
                }
            }
 
            it = object.find("emissiveTexture");
            if (it != object.end()) {
                const auto& value = it.value();
                if (value.is_object()) {
                    int index = value.value("index", -1);
                    int texCoord = value.value("texCoord", 0);

                    if (index > TextureList.size()) {
                        Log(DUSK_ANCHOR, "Invalid glTF file, failed to load material");
                        return false;
                    }

                    if (texCoord > 0) {
                        Log(DUSK_ANCHOR, "Invalid glTF file, multiple TEXCOORDs are not supported");
                        return false;
                    }

                    if (index >= 0) {
                        material->SetTexture(Material::EmissiveMapName, TextureList[index]);

                        // TODO: Improve
                        material->SetVec3Parameter(Material::EmissiveFactorName, vec3(1.0f));
                    }
                }
            }

            it = object.find("emissiveFactor");
            if (it != object.end()) {
                material->SetVec3Parameter(Material::EmissiveFactorName, JSON::ToVec3(*it, vec3(0.0f)));
            }
 
            it = object.find("occlusionTexture");
            if (it != object.end()) {
                const auto& value = it.value();
                if (value.is_object()) {
                    int index = value.value("index", -1);
                    int texCoord = value.value("texCoord", 0);

                    if (index > TextureList.size()) {
                        Log(DUSK_ANCHOR, "Invalid glTF file, failed to load material");
                        return false;
                    }

                    if (texCoord > 0) {
                        Log(DUSK_ANCHOR, "Invalid glTF file, multiple TEXCOORDs are not supported");
                        return false;
                    }

                    if (index >= 0) {
                        material->SetTexture(Material::OcclusionMapName, TextureList[index]);
                    }

                    material->SetFloatParameter(Material::OcclusionStrengthName, value.value("strength", 1.0f));
                }
            }
 
            it = object.find("pbrMetallicRoughness");
            if (it != object.end()) {
                const auto& group = it.value();
                if (group.is_object()) {
                    it = group.find("baseColorFactor");
                    if (it != group.end()) {
                        material->SetVec3Parameter(Material::BaseColorFactorName, JSON::ToVec3(*it, vec3(1.0f)));
                    }

                    it = group.find("baseColorTexture");
                    if (it != group.end()) {
                        const auto& value = it.value();

                        int index = value.value("index", -1);
                        int texCoord = value.value("texCoord", 0);

                        if (index > TextureList.size()) {
                            Log(DUSK_ANCHOR, "Invalid glTF file, failed to load material");
                            return false;
                        }

                        if (texCoord > 0) {
                            Log(DUSK_ANCHOR, "Invalid glTF file, multiple TEXCOORDs are not supported");
                            return false;
                        }

                        if (index >= 0) {
                            material->SetTexture(Material::BaseColorMapName, TextureList[index]);
                        }
                    }

                    material->SetFloatParameter(Material::MetallicFactorName, group.value("metallicFactor", 1.0f));
                    material->SetFloatParameter(Material::RoughnessFactorName, group.value("roughnessFactor", 1.0f));

                    it = group.find("metallicRoughnessTexture");
                    if (it != group.end()) {
                        const auto& value = it.value();

                        int index = value.value("index", -1);
                        int texCoord = value.value("texCoord", 0);

                        if (index > TextureList.size()) {
                            Log(DUSK_ANCHOR, "Invalid glTF file, failed to load material");
                            return false;
                        }

                        if (texCoord > 0) {
                            Log(DUSK_ANCHOR, "Invalid glTF file, multiple TEXCOORDs are not supported");
                            return false;
                        }

                        if (index >= 0) {
                            material->SetTexture(Material::MetallicRoughnessMapName, TextureList[index]);
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
            }

            MaterialList.push_back(std::move(material));
        }
    }

    Log(DUSK_ANCHOR, "Loaded {} Material(s)", MaterialList.size());

    const auto CAMERAS_PATH = JSON::Pointer("/cameras");
    if (json.contains(CAMERAS_PATH)) {
        for (const auto& object : json[CAMERAS_PATH]) {
            CameraList.push_back(CameraData{
                .name = object.value("name", ""),
                .type = object.value("type", ""),
            });
            auto& camera = CameraList.back();

            if (camera.type == "perspective") {
                auto it = object.find("perspective");
                if (it != object.end()) {
                    Log(DUSK_ANCHOR, "Invalid glTF file, missing perspective data for camera");
                    return false;
                }

                const auto& perspective = it.value();

                camera.aspectRatio = perspective.value("aspectRatio", 0.0f); // TODO: Find sensible default
                camera.yfov = perspective.value("yfov", 0.0f); // TODO: Find sensible default
                camera.zfar = perspective.value("zfar", 1000.0f);
                camera.znear = perspective.value("znear", 0.0001f);
            }
            else if (camera.type == "orthographic") {
                auto it = object.find("orthographic");
                if (it != object.end()) {
                    Log(DUSK_ANCHOR, "Invalid glTF file, missing orthographic data for camera");
                    return false;
                }

                const auto& orthographic = it.value();

                camera.xmag = orthographic.value("xmag", 0.0f);
                camera.ymag = orthographic.value("ymag", 0.0f);
                camera.zfar = orthographic.value("zfar", 1000.0f);
                camera.znear = orthographic.value("znear", 0.0001f);
            }
            else {
                Log(DUSK_ANCHOR, "Unknown glTF camera type '{}' for camera '{}'", camera.type, camera.name);
            }
        }
    }

    Log(DUSK_ANCHOR, "Loaded {} Camera(s)", CameraList.size());

    const auto MESHES_PATH = JSON::Pointer("/meshes");
    if (json.contains(MESHES_PATH)) {
        for (const auto& object : json[MESHES_PATH]) {

            auto it = object.find("primitives");
            if (it == object.end()) {
                Log(DUSK_ANCHOR, "Invalid glTF file, missing primitives array");
                return false;
            }

            MeshList.emplace_back(new Mesh());
            auto& mesh = MeshList.back();

            const auto& primitives = it.value();
            for (const auto& primitive : primitives) {

                VulkanPrimitive::Pointer vulkanPrimitive(new VulkanPrimitive());

                GLenum mode = primitive.value("mode", GL_TRIANGLES);
                auto primitiveTopology = mapTopologyToVulkan(mode);
                if (not primitiveTopology) {
                    Log(DUSK_ANCHOR, "Invalid glTF file, unsupported primitive mode 0x{:04X}", mode);
                    return false;
                }
                
                int material = primitive.value("material", -1);
                if (material >= 0) {
                    vulkanPrimitive->SetMaterial(MaterialList[material]);
                }

                AccessorIterator indexIter(this, primitive.value("indices", -1));
                List<uint32_t> indexList(indexIter.GetCount());
                for (auto& index : indexList) {
                    index = indexIter.GetUInt32();
                    ++indexIter;
                }

                it = primitive.find("attributes");
                if (it == primitive.end()) {
                    Log(DUSK_ANCHOR, "Invalid glTF file, missing primitive attributes");
                    return false;
                }

                const auto& attributes = it.value();

                AccessorIterator positionIter(this, attributes.value("POSITION", -1));
                AccessorIterator normalIter(this, attributes.value("NORMAL", -1));
                AccessorIterator tangentIter(this, attributes.value("TANGENT", -1));
                AccessorIterator texCoordIter(this, attributes.value("TEXCOORD_0", -1));
                AccessorIterator colorIter(this, attributes.value("COLOR_0", -1));
                AccessorIterator jointsIter(this, attributes.value("JOINTS_0", -1));
                AccessorIterator weightsIter(this, attributes.value("WEIGHTS_0", -1));

                List<PrimitiveVertex> vertexList(positionIter.GetCount());
                if (vertexList.empty()) {
                    Log(DUSK_ANCHOR, "Invalid glTF file, missing POSITION attribute");
                    return false;
                }

                for (auto& vertex : vertexList) {
                    vertex.Position = positionIter.GetVec4({ 0.0f, 0.0f, 0.0f, 1.0f });
                    ++positionIter;

                    vertex.Normal = normalIter.GetVec4({ 0.0f, 0.0f, 0.0f, 1.0f });
                    ++normalIter;

                    vertex.Tangent = tangentIter.GetVec4({ 0.0f, 0.0f, 0.0f, 1.0f });
                    ++tangentIter;

                    vertex.TexCoord = texCoordIter.GetVec2({ 0.0f, 0.0f });
                    ++texCoordIter;

                    vertex.Color = colorIter.GetVec4({ 1.0f, 1.0f, 1.0f, 1.0f });
                    ++colorIter;

                    vertex.Joints = jointsIter.GetUVec4({ 0, 0, 0, 0 });
                    ++jointsIter;

                    vertex.Weights = weightsIter.GetVec4({ 0.0f, 0.0f, 0.0f, 0.0f });
                    ++weightsIter;
                }

                bool haveTangents = (tangentIter.GetCount() > 0);
                vulkanPrimitive->Create(indexList, vertexList, primitiveTopology.value(), haveTangents);

                mesh->AddPrimitive(std::move(vulkanPrimitive));
            }

        }
    }

    Log(DUSK_ANCHOR, "Loaded {} Mesh(es)", MeshList.size());

    Log(DUSK_ANCHOR, "Loaded {}", path.string());

    return true;
}

AccessorIterator::AccessorIterator(Loader * gltf, int accessorIndex)
{
    if (accessorIndex < 0) {
        return;
    }

    _accessor = &gltf->AccessorList[accessorIndex];
    _bufferView = &gltf->BufferViewList[_accessor->bufferView];
    _buffer = Span<uint8_t>(
        gltf->BufferList[_bufferView->buffer].data() + _bufferView->byteOffset + _accessor->byteOffset,
        _bufferView->byteLength
    );

    // _ptr = _buffer.data() + _bufferView->byteOffset + _accessor->byteOffset;

    if (_accessor->type == "SCALAR") {
        _componentCount = 1;
    }
    else if (_accessor->type == "VEC2") {
        _componentCount = 2;
    }
    else if (_accessor->type == "VEC3") {
        _componentCount = 3;
    }
    else if (_accessor->type == "VEC4") {
        _componentCount = 4;
    }

    if (_bufferView->byteStride == 0) {
        // If byteStride is not defined, the elements are tightly packed
        switch (_accessor->componentType) {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            _stride = _componentCount * sizeof(uint8_t);
            break;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
            _stride = _componentCount * sizeof(uint16_t);
            break;
        case GL_UNSIGNED_INT:
            _stride = _componentCount * sizeof(uint32_t);
            break;
        case GL_FLOAT:
            _stride = _componentCount * sizeof(float);
            break;
        }
    }
    else {
        _stride = _bufferView->byteStride;
    }
}

float AccessorIterator::GetFloat(size_t offset /*= 0*/, float defaultValue /*= 0.0f*/)
{
    if (not _buffer.empty() and offset < _componentCount) {
        if (_accessor->normalized) {
            switch (_accessor->componentType) {
            case GL_BYTE:
                return NormalizeInteger(getValueAs<int8_t>(offset));
            case GL_UNSIGNED_BYTE:
                return NormalizeInteger(getValueAs<uint8_t>(offset));
            case GL_SHORT:
                return NormalizeInteger(getValueAs<int16_t>(offset));
            case GL_UNSIGNED_SHORT:
                return NormalizeInteger(getValueAs<uint16_t>(offset));
            }
        }
        else {
            switch (_accessor->componentType) {
            case GL_BYTE:
                return getValueAs<int8_t>(offset);
            case GL_UNSIGNED_BYTE:
                return getValueAs<uint8_t>(offset);
            case GL_SHORT:
                return getValueAs<int16_t>(offset);
            case GL_UNSIGNED_SHORT:
                return getValueAs<uint16_t>(offset);
            case GL_FLOAT:
                return getValueAs<float>(offset);
            }
        }
    }

    return defaultValue;
}

uint32_t AccessorIterator::GetUInt32(size_t offset /*= 0*/, uint32_t defaultValue /*= 0*/)
{
    if (not _buffer.empty() and offset < _componentCount) {
        switch (_accessor->componentType) {
        case GL_BYTE:
            return getValueAs<int8_t>(offset);
        case GL_UNSIGNED_BYTE:
            return getValueAs<uint8_t>(offset);
        case GL_SHORT:
            return getValueAs<int16_t>(offset);
        case GL_UNSIGNED_SHORT:
            return getValueAs<uint16_t>(offset);
        case GL_UNSIGNED_INT:
            return getValueAs<uint32_t>(offset);
        }
    }

    return defaultValue;
}

AccessorIterator& AccessorIterator::operator++()
{
    if (not _buffer.empty()) {
        _buffer = _buffer.subspan(_stride);
    }

    return *this;
}

} // namespace glTF
} // namespace dusk