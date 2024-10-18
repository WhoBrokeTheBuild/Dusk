#ifndef DUSK_GLTF_HPP
#define DUSK_GLTF_HPP

#include <Dusk/List.hpp>
#include <Dusk/String.hpp>
#include <Dusk/VulkanTexture.hpp>
#include <Dusk/Material.hpp>
#include <Dusk/Mesh.hpp>

#include "OpenGLStub.hpp"

namespace dusk {
namespace glTF {

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
    String type;
    GLenum componentType;
    bool normalized;
    // TODO: min, max
};

struct ImageData
{
    String uri;
    int bufferView;
    String mimeType;
};

struct SamplerData
{
    GLenum wrapS;
    GLenum wrapT;
    GLenum magFilter;
    GLenum minFilter;
};

struct CameraData
{
    String name;
    String type;

    // perspective
    float aspectRatio;
    float yfov;

    // orthographics
    float xmag;
    float ymag;

    // both
    float zfar;
    float znear;
};

class Loader
{
public:

    static constexpr uint32_t MAGIC      = 0x46546C67; // "glTF"
    static constexpr uint32_t CHUNK_JSON = 0x4E4F534A; // "JSON"
    static constexpr uint32_t CHUNK_BIN  = 0x004E4942; // "BIN\0"

    bool LoadFromFile(const Path& path);

    List<List<uint8_t>> BufferList;

    List<BufferViewData> BufferViewList;

    List<AccessorData> AccessorList;

    List<ImageData> ImageList;

    List<SamplerData> SamplerList;

    List<VulkanTexture::Pointer> TextureList;

    List<Material::Pointer> MaterialList;

    List<CameraData> CameraList;

    List<Mesh::Pointer> MeshList;

}; // class Loader

class AccessorIterator
{
public:

    AccessorIterator(Loader * gltf, int accessorIndex);

    inline size_t GetCount() {
        if (_accessor) {
            return _accessor->count;
        }
        return 0;
    }

    float GetFloat(size_t offset = 0, float defaultValue = 0.0f);

    uint32_t GetUInt32(size_t offset = 0, uint32_t defaultValue = 0);

    inline vec2 GetVec2(vec2 defaultValue = vec2(0.0f)) {
        return vec2(
            GetFloat(0, defaultValue[0]),
            GetFloat(1, defaultValue[1])
        );
    }

    inline vec3 GetVec3(vec3 defaultValue = vec3(0.0f)) {
        return vec3(
            GetFloat(0, defaultValue[0]),
            GetFloat(1, defaultValue[1]),
            GetFloat(2, defaultValue[2])
        );
    }

    inline vec4 GetVec4(vec4 defaultValue = vec4(0.0f)) {
        return vec4(
            GetFloat(0, defaultValue[0]),
            GetFloat(1, defaultValue[1]),
            GetFloat(2, defaultValue[2]),
            GetFloat(3, defaultValue[3])
        );
    }

    inline uvec4 GetUVec4(uvec4 defaultValue = uvec4(0.0f)) {
        return uvec4(
            GetUInt32(0, defaultValue[0]),
            GetUInt32(1, defaultValue[1]),
            GetUInt32(2, defaultValue[2]),
            GetUInt32(3, defaultValue[3])
        );
    }

    AccessorIterator& operator++();

private:

    template <typename T>
    inline T getValueAs(size_t offset) {
        return *reinterpret_cast<T *>(_buffer.data() + (offset * sizeof(T)));
    }

    Span<uint8_t> _buffer;

    size_t _stride = 0;

    size_t _componentCount = 1;

    AccessorData * _accessor = nullptr;

    BufferViewData * _bufferView = nullptr;

}; // class AccessorIterator

} // namespace glTF
} // namespace dusk

#endif // DUSK_GLTF_HPP