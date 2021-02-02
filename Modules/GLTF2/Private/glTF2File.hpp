#ifndef DUSK_GLTF2_GLTF2_FILE_HPP
#define DUSK_GLTF2_GLTF2_FILE_HPP

#include <Dusk/GLTF2/GLTF2Config.hpp>

#include <Dusk/Math.hpp>
#include <Dusk/JSON.hpp>
#include <Dusk/Texture.hpp>
#include <Dusk/TextureImporter.hpp>
#include <Dusk/Material.hpp>

#include "OpenGLStub.hpp"

#include <cstdint>

namespace Dusk::GLTF2 {

DUSK_GLTF2_API
inline vec2 ParseVec2(const json& value, vec2 def)
{
    if (value.is_array() && value.size() == 2) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec2(v.data());
    }
    return def;
}

DUSK_GLTF2_API
inline vec3 ParseVec3(const json& value, vec3 def)
{
    if (value.is_array() && value.size() == 3) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec3(v.data());
    }
    return def;
}

DUSK_GLTF2_API
inline vec4 ParseVec4(const json& value, vec4 def)
{
    if (value.is_array() && value.size() == 4) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec4(v.data());
    }
    return def;
}

DUSK_GLTF2_API
inline quat ParseQuat(const json& value, quat def)
{
    if (value.is_array() && value.size() == 4) {
        const auto& v = value.get<std::vector<float>>();
        return glm::quat(v[3], v[0], v[1], v[2]);
    }
    return def;
}

DUSK_GLTF2_API
inline TextureWrapType GetWrapType(const GLenum& type)
{
    switch (type) {
    case GL_REPEAT:
        return TextureWrapType::Repeat;
    case GL_MIRRORED_REPEAT:
        return TextureWrapType::MirroredRepeat;
    case GL_CLAMP_TO_EDGE:
        return TextureWrapType::ClampToEdge;
    case GL_CLAMP_TO_BORDER:
        return TextureWrapType::ClampToBorder;
    }

    return TextureWrapType::Repeat;
}

DUSK_GLTF2_API
inline TextureFilterType GetFilterType(const GLenum& type)
{
    switch (type) {
    case GL_NEAREST:
        return TextureFilterType::Nearest;
    case GL_NEAREST_MIPMAP_NEAREST:
        return TextureFilterType::NearestMipmapNearest;
    case GL_NEAREST_MIPMAP_LINEAR:
        return TextureFilterType::NearestMipmapLinear;
    case GL_LINEAR:
        return TextureFilterType::Linear;
    case GL_LINEAR_MIPMAP_NEAREST:
        return TextureFilterType::LinearMipmapNearest;
    case GL_LINEAR_MIPMAP_LINEAR:
        return TextureFilterType::LinearMipmapLinear;
    }

    return TextureFilterType::Nearest;
}

struct BufferViewData
{
    int buffer;
    size_t byteLength;
    size_t byteOffset;
    size_t byteStride;
    GLenum target;
};

struct AccessorData
{
    int bufferView;
    size_t byteOffset;
    size_t count;
    string type;
    GLenum componentType;
    bool normalized;
    // TODO: min, max
};

struct ImageData
{
    string uri;
    
    int bufferView;
    string mimeType;
};

struct CameraData
{
    string name;
    string type;

    // Perspective
    float aspectRatio;
    float yfov;

    // Orthographic
    float xmag;
    float ymag;

    // Both
    float zfar;
    float znear;
};

struct PrimitiveData
{
    std::vector<std::unique_ptr<PrimitiveData>> Primitives;
};

class DUSK_GLTF2_API glTF2File : public Object
{
public:

    glTF2File() = default;

    virtual ~glTF2File() = default;

    bool LoadFromFile(const string& filename);

    const uint32_t MAGIC = 0x46546C67; // glTF

    enum class ChunkType : uint32_t
    {
        JSON    = 0x4E4F534A, // JSON
        BIN     = 0x004E4942, // BIN
    };

    string Filename;

    string BaseDir;

    json JSON;

    std::vector<std::vector<uint8_t>> Buffers;

    std::vector<BufferViewData> BufferViews;

    std::vector<AccessorData> Accessors;

    std::vector<ImageData> Images;

    std::vector<Dusk::Texture::Options> Samplers;

    std::vector<std::shared_ptr<Dusk::Texture>> Textures;

    std::vector<std::shared_ptr<Dusk::Material>> Materials;

    std::vector<CameraData> Cameras;
    
    // std::vector<>

    bool LoadBuffers();

    bool LoadBufferViews();

    bool LoadAccessors();

    bool LoadImages();
    
    bool LoadSamplers();

    bool LoadTextures();

    bool LoadMaterials();
    
    bool LoadCameras();

    bool LoadMeshes();

};

} // namespace Dusk::GLTF2

#endif // DUSK_GLTF2_GLTF2_FILE_HPP