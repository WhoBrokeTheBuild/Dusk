#ifndef DUSK_GLTF2_GLTF2_FILE_HPP
#define DUSK_GLTF2_GLTF2_FILE_HPP

#include <Dusk/GLTF2/Config.hpp>
#include <Dusk/GLTF2/OpenGLStub.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Graphics/Texture.hpp>
#include <Dusk/Graphics/TextureImporter.hpp>
#include <Dusk/Graphics/Material.hpp>

#include <cstdint>
#include <nlohmann/json.hpp>

namespace Dusk::GLTF2 {

using json = nlohmann::json;

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
inline Texture::WrapType GetWrapType(const GLenum& type)
{
    switch (type) {
    case GL_REPEAT:
        return Texture::WrapType::Repeat;
    case GL_MIRRORED_REPEAT:
        return Texture::WrapType::MirroredRepeat;
    case GL_CLAMP_TO_EDGE:
        return Texture::WrapType::ClampToEdge;
    case GL_CLAMP_TO_BORDER:
        return Texture::WrapType::ClampToBorder;
    }

    return Texture::WrapType::Repeat;
}

DUSK_GLTF2_API
inline Texture::FilterType GetFilterType(const GLenum& type)
{
    switch (type) {
    case GL_NEAREST:
        return Texture::FilterType::Nearest;
    case GL_NEAREST_MIPMAP_NEAREST:
        return Texture::FilterType::NearestMipmapNearest;
    case GL_NEAREST_MIPMAP_LINEAR:
        return Texture::FilterType::NearestMipmapLinear;
    case GL_LINEAR:
        return Texture::FilterType::Linear;
    case GL_LINEAR_MIPMAP_NEAREST:
        return Texture::FilterType::LinearMipmapNearest;
    case GL_LINEAR_MIPMAP_LINEAR:
        return Texture::FilterType::LinearMipmapLinear;
    }

    return Texture::FilterType::Nearest;
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
    std::string type;
    GLenum componentType;
    bool normalized;
    // TODO: min, max
};

struct ImageData
{
    std::string uri;
    
    int bufferView;
    std::string mimeType;
};

struct CameraData
{
    std::string name;
    std::string type;

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

class DUSK_GLTF2_API glTF2File 
{
public:

    glTF2File() = default;

    virtual ~glTF2File() = default;

    bool LoadFromFile(const std::string& filename);

private:

    const uint32_t MAGIC = 0x46546C67; // glTF

    enum class ChunkType : uint32_t
    {
        JSON    = 0x4E4F534A, // JSON
        BIN     = 0x004E4942, // BIN
    };

    std::string Filename;

    std::string BaseDir;

    json JSON;

    std::vector<std::vector<uint8_t>> Buffers;

    std::vector<BufferViewData> BufferViews;

    std::vector<AccessorData> Accessors;

    std::vector<ImageData> Images;

    std::vector<Dusk::Texture::Options> Samplers;

    std::vector<std::shared_ptr<Dusk::Texture>> Textures;

    std::vector<std::shared_ptr<Dusk::Material>> Materials;

    std::vector<CameraData> Cameras;

    bool LoadBuffers();

    bool LoadBufferViews();

    bool LoadAccessors();

    bool LoadImages();
    
    bool LoadSamplers();

    bool LoadTextures();

    bool LoadMaterials();
    
    bool LoadCameras();

};

} // namespace Dusk::GLTF2

#endif // DUSK_GLTF2_GLTF2_FILE_HPP