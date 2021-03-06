#ifndef DUSK_GLTF2_GLTF2_FILE_HPP
#define DUSK_GLTF2_GLTF2_FILE_HPP

#include <Dusk/GLTF2/GLTF2Config.hpp>

#include <Dusk/Math.hpp>
#include <Dusk/JSON.hpp>
#include <Dusk/Texture.hpp>
#include <Dusk/TextureImporter.hpp>
#include <Dusk/Material.hpp>
#include <Dusk/PrimitiveData.hpp>
#include <Dusk/Path.hpp>

#include "OpenGLStub.hpp"

#include <cstdint>
#include <iterator>

namespace Dusk::GLTF2 {

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

class DUSK_GLTF2_API glTF2File : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(glTF2File);

    glTF2File() = default;

    virtual ~glTF2File() = default;

    void LoadFromFile(const Path& path);

    const uint32_t MAGIC = 0x46546C67; // glTF

    enum class ChunkType : uint32_t
    {
        JSON    = 0x4E4F534A, // JSON
        BIN     = 0x004E4942, // BIN
    };

    Path Directory;

    json JSON;

    std::vector<std::vector<uint8_t>> Buffers;

    std::vector<BufferViewData> BufferViews;

    std::vector<AccessorData> Accessors;

    std::vector<ImageData> Images;

    std::vector<Texture::Options> Samplers;

    std::vector<std::shared_ptr<Texture>> Textures;

    std::vector<std::shared_ptr<Material>> Materials;

    std::vector<CameraData> Cameras;
    
    // std::vector<>

    bool IsValidTexture(int index, int texCoord);

    void LoadBuffers();

    void LoadBufferViews();

    void LoadAccessors();

    void LoadImages();
    
    void LoadSamplers();

    void LoadTextures();

    void LoadMaterials();
    
    void LoadCameras();

    std::vector<std::unique_ptr<PrimitiveData>> LoadMesh();

};

class AccessorIterator
{
public:

    AccessorIterator(glTF2File * file, int index)
    {
        if (index < 0) {
            _ptr = nullptr;
            return;
        }

        _accessor = &file->Accessors[index];
        _bufferView = &file->BufferViews[_accessor->bufferView];
        _buffer = &file->Buffers[_bufferView->buffer];

        _ptr = _buffer->data() + _accessor->byteOffset + _bufferView->byteOffset;

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

        _stride = _bufferView->byteStride;
        if (_stride == 0) {
            switch (_accessor->componentType) {
            case GL_UNSIGNED_BYTE:
                _stride = _componentCount * sizeof(uint8_t);
                break;
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
    }

    inline float getFloat(size_t index = 0, float def = 0.0f)
    {
        if (_ptr && index < _componentCount) {
            switch (_accessor->componentType) {
            case GL_UNSIGNED_BYTE:
                return NormalizeInteger(getAs<uint8_t>(index));
            case GL_UNSIGNED_SHORT:
                return NormalizeInteger(getAs<uint16_t>(index));
            case GL_UNSIGNED_INT:
                return NormalizeInteger(getAs<uint32_t>(index));
            case GL_FLOAT:
                return getAs<float>(index);
            }
        }

        return def;
    }

    inline uint32_t getInteger(size_t index = 0, unsigned def = 0)
    {
        if (_ptr && index < _componentCount) {
            switch (_accessor->componentType) {
            case GL_UNSIGNED_BYTE:
                return getAs<uint8_t>(index);
            case GL_UNSIGNED_SHORT:
                return getAs<uint16_t>(index);
            case GL_UNSIGNED_INT:
                return getAs<uint32_t>(index);
            case GL_FLOAT:
                return getAs<float>(index);
            }
        }

        return def;
    }

    inline vec2 getVec2(vec2 def = vec2()) {
        return vec2(
            getFloat(0, def[0]),
            getFloat(1, def[1])
        );
    }

    inline vec4 getVec4(vec4 def = vec4()) {
        return vec4(
            getFloat(0, def[0]),
            getFloat(1, def[1]),
            getFloat(2, def[2]),
            getFloat(3, def[3])
        );
    }

    inline uvec4 getUVec4(uvec4 def = uvec4()) {
        return vec4(
            getInteger(0, def[0]),
            getInteger(1, def[1]),
            getInteger(2, def[2]),
            getInteger(3, def[3])
        );
    }

    inline AccessorIterator& operator++() {
        if (_ptr) {
            _ptr += _stride;
            if (_ptr >= _buffer->data() + _buffer->size()) {
                _ptr = nullptr;
            }
        }

        return *this;
    }

private: 

    template <typename T>
    inline T getAs(size_t index) {
        return *reinterpret_cast<T *>(_ptr + (sizeof(T) * index));
    }

    uint8_t * _ptr;

    size_t _stride;

    size_t _componentCount;
    
    AccessorData * _accessor = nullptr;

    BufferViewData * _bufferView = nullptr;

    std::vector<uint8_t> * _buffer = nullptr;

}; // class AccessorIterator

} // namespace Dusk::GLTF2

#endif // DUSK_GLTF2_GLTF2_FILE_HPP