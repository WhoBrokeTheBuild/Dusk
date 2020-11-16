#ifndef DUSK_GLTF2_GLTF2_FILE_HPP
#define DUSK_GLTF2_GLTF2_FILE_HPP

#include <Dusk/GLTF2/Config.hpp>
#include <Dusk/Math.hpp>

#include <cstdint>
#include <nlohmann/json.hpp>

namespace Dusk::GLTF2 {

using json = nlohmann::json;

typedef uint32_t GLenum;

#define GL_INVALID_ENUM 0x0500

inline vec2 ParseVec2(const json& value, vec2 def)
{
    if (value.is_array() && value.size() == 2) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec2(v.data());
    }
    return def;
}

inline vec3 ParseVec3(const json& value, vec3 def)
{
    if (value.is_array() && value.size() == 3) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec3(v.data());
    }
    return def;
}

inline vec4 ParseVec4(const json& value, vec4 def)
{
    if (value.is_array() && value.size() == 4) {
        const auto& v = value.get<std::vector<float>>();
        return glm::make_vec4(v.data());
    }
    return def;
}

inline quat ParseQuat(const json& value, quat def)
{
    if (value.is_array() && value.size() == 4) {
        const auto& v = value.get<std::vector<float>>();
        return glm::quat(v[3], v[0], v[1], v[2]);
    }
    return def;
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
    ivec2 size;
    int components;
    std::unique_ptr<uint8_t> data;
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

    explicit glTF2File() = default;

    virtual ~glTF2File() = default;

    bool LoadFromFile(const std::string& filename);

private:

    const uint32_t MAGIC = 0x46546C67; // glTF

    enum class ChunkType : uint32_t
    {
        JSON    = 0x4E4F534A, // JSON
        BIN     = 0x004E4942, // BIN
    };

    json JSON;

    std::vector<std::vector<uint8_t>> Buffers;

    std::vector<BufferViewData> BufferViews;

    std::vector<AccessorData> Accessors;

    std::vector<ImageData> Images;

    std::vector<CameraData> Cameras;

    bool LoadBuffers(const std::string& dir);

    bool LoadBufferViews();

    bool LoadAccessors();

    bool LoadImages();
    
    bool LoadCameras();

};

} // namespace Dusk::GLTF2

#endif // DUSK_GLTF2_GLTF2_FILE_HPP