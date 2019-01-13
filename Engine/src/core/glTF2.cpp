#include "dusk/core/glTF2.hpp"

#include <dusk/core/Benchmark.hpp>
#include <dusk/core/Util.hpp>
#include <dusk/core/JSON.hpp>
#include <dusk/asset/Material.hpp>
#include <dusk/asset/Mesh.hpp>
#include <dusk/asset/Texture.hpp>
#include <dusk/scene/Actor.hpp>
#include <dusk/scene/MeshComponent.hpp>

#include <stb/stb_image.h>

#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>

namespace dusk {

namespace glTF2 {

std::vector<std::vector<uint8_t>> loadBuffers(const json& data, const std::string& dir)
{
    std::vector<std::vector<uint8_t>> buffers;
    
    auto it = data.find("buffers");
    if (it != data.end()) {
        if (it.value().is_array()) {
            auto& array = it.value();
            for (auto& object : array) {
                if (object.is_object()) {
                    buffers.push_back(std::vector<uint8_t>());
                    auto& buffer = buffers.back();

                    size_t byteLength = object.value<size_t>("byteLength", 0);
                    const auto& uri = object.value("uri", "");

                    DuskLogVerbose("glTF buffer %zu, %s", byteLength, uri);

                    std::ifstream bufferFile(dir + "/" + uri, std::ios::in | std::ios::binary);
                    if (!bufferFile.is_open()) {
                        DuskLogError("Failed to open glTF data file '%s'", uri);
                        continue;
                    }

                    buffer.reserve(byteLength);
                    buffer.assign(
                        (std::istreambuf_iterator<char>(bufferFile)),
                        std::istreambuf_iterator<char>()
                    );
                    bufferFile.close();

                    DuskLogLoad("glTF data file '%s'", uri);
                    
                    if (buffers.back().size() != byteLength) {
                        DuskLogWarn("Buffer size mismatch %zu != %zu", buffers.back().size(), byteLength);
                    }
                }
            }
        }
    }

    return buffers;
}

struct bufferView_t {
    int buffer;
    size_t byteLength;
    size_t byteOffset;
    size_t byteStride;
    GLenum target;
};

std::vector<bufferView_t> loadBufferViews(const json& data)
{
    std::vector<bufferView_t> bufferViews;

    const auto it = data.find("bufferViews");
    if (it != data.cend()) {
        if (it.value().is_array()) {
            const auto& array = it.value();
            for (const auto& object : array) {
                if (object.is_object()) {
                    bufferViews.push_back(bufferView_t{
                        object.value("buffer", -1),
                        object.value<size_t>("byteLength", 0),
                        object.value<size_t>("byteOffset", 0),
                        object.value<size_t>("byteStride", 0),
                        object.value<GLenum>("target", 0),
                    });

                    DuskLogVerbose("BufferView %zu, %zu, %zu, %zu", 
                        bufferViews.back().buffer, 
                        bufferViews.back().byteLength, 
                        bufferViews.back().byteOffset, 
                        bufferViews.back().target);
                }
            }
        }
    }

    return bufferViews;
}

struct accessor_t {
    int bufferView;
    size_t byteOffset;
    GLenum componentType;
    size_t count;
    // TODO: min, max
    std::string type;
};

std::vector<accessor_t> loadAccessors(const json& data)
{
    std::vector<accessor_t> accessors;

    const auto& it = data.find("accessors");
    if (it != data.cend()) {
        if (it.value().is_array()) {
            const auto& array = it.value();
            for (const auto& object : array) {
                if (object.is_object()) {
                    accessors.push_back(accessor_t{
                        object.value("bufferView", -1),
                        object.value<size_t>("byteOffset", 0),
                        object.value<GLenum>("componentType", 0),
                        object.value<size_t>("count", 0),
                        // TODO: min, max
                        object.value("type", ""),
                    });
                }
            }
        }
    }

    return accessors;
}

struct image_t {
    glm::ivec2 size;
    int components;
    std::unique_ptr<uint8_t> data;
};

std::vector<image_t> loadImages(
    const json& data, 
    const std::string& dir, 
    const std::vector<bufferView_t>& bufferViews, 
    const std::vector<std::vector<uint8_t>>& buffers)
{
    std::vector<image_t> images;

    const auto it = data.find("images");
    if (it != data.cend()) {
        if (it.value().is_array()) {
            const auto& array = it.value();
            for (const auto& object : array) {
                if (object.is_object()) {
                    images.push_back(image_t{});
                    auto& image = images.back();

                    const auto& uri = object.value("uri", "");
                    if (!uri.empty()) {
                        const auto& imageFilename = dir + "/" + uri;
                        image.data.reset(stbi_load(
                            imageFilename.c_str(), 
                            &image.size.x, 
                            &image.size.y, 
                            &image.components, 
                            STBI_rgb_alpha)
                        );

                        if (!image.data) {
                            DuskLogError("Failed to load glTF image file '%s'", imageFilename);
                            continue;
                        }

                        DuskLogLoad("glTF image file '%s'", uri);
                    } else {
                        int bufferViewIndex = object.value("bufferView", -1);
                        //const auto& mimeType = object.value("mimeType", "");

                        if (bufferViewIndex < 0 || bufferViewIndex > (int)bufferViews.size()) {
                            DuskLogError("Invalid glTF bufferView index %d", bufferViewIndex);
                            continue;
                        }

                        const auto& bufferView = bufferViews[bufferViewIndex];
                        
                        if (bufferView.buffer < 0 || bufferView.buffer >(int)buffers.size()) {
                            DuskLogError("Invalid glTF buffer index %d", bufferView.buffer);
                            continue;
                        }

                        const auto& buffer = buffers[bufferView.buffer];

                        image.data.reset(stbi_load_from_memory(
                            buffer.data() + bufferView.byteOffset, 
                            bufferView.byteLength, 
                            &image.size.x, 
                            &image.size.y, 
                            &image.components, 
                            STBI_rgb_alpha)
                        );
                    }
                }
            }
        }
    }

    return images;
}

std::vector<Texture::Options> loadSamplers(const json& data)
{
    std::vector<Texture::Options> samplers;

    const auto it = data.find("samplers");
    if (it != data.cend()) {
        if (it.value().is_array()) {
            const auto& array = it.value();
            for (const auto& object : array) {
                if (object.is_object()) {
                    samplers.push_back(Texture::Options{});
                    auto& sampler = samplers.back();
                    
                    sampler.MagFilter = object.value<GLenum>("magFilter", sampler.MagFilter);
                    sampler.MinFilter = object.value<GLenum>("minFilter", sampler.MinFilter);
                    sampler.WrapS = object.value<GLenum>("wrapS", sampler.WrapS);
                    sampler.WrapT = object.value<GLenum>("wrapT", sampler.WrapT);
                }
            }
        }
    }

    return samplers;
}

std::vector<std::shared_ptr<Texture>> loadTextures(
    const json& data, 
    const std::vector<image_t>& images,
    const std::vector<Texture::Options> samplers)
{
    std::vector<std::shared_ptr<Texture>> textures;

    const auto it = data.find("textures");
    if (it != data.cend()) {
        if (it.value().is_array()) {
            const auto& array = it.value();
            for (const auto& object : array) {
                if (object.is_object()) {
                    int sampler = object.value("sampler", -1);
                    int source = object.value("source", -1);

                    if (source < 0) {
                        DuskLogError("Invalid glTF texture source %d", source);
                        continue;
                    }

                    const auto& image = images[source];
                    textures.push_back(std::make_shared<Texture>(
                        image.data.get(), 
                        image.size,
                        image.components,
                        (sampler >= 0 ? samplers[sampler] : Texture::Options())
                    ));

                    DuskLogVerbose("Texture %zu, %zu", sampler, source);
                }
            }
        }
    }

    return textures;
}

std::vector<std::shared_ptr<Material>> loadMaterials(
    const json& data, 
    const std::vector<std::shared_ptr<Texture>>& textures)
{
    std::vector<std::shared_ptr<Material>> materials;

    auto parseVec3 = [](const json& value, glm::vec3 def) -> glm::vec3 {
        if (value.is_array()) {
            const auto& v = value.get<std::vector<float>>();
            return glm::make_vec3(v.data());
        }
        return def;
    };

    auto parseVec4 = [](const json& value, glm::vec4 def) -> glm::vec4 {
        if (value.is_array()) {
            const auto& v = value.get<std::vector<float>>();
            return glm::make_vec4(v.data());
        }
        return def;
    };

    auto parseTexture = [&textures](const json& value) -> std::shared_ptr<Texture> {
        if (value.is_object()) {
            int index = value.value("index", -1);
            int texCoord = value.value("texCoord", 0);

            if (index < 0 || index >(int)textures.size()) {
                DuskLogError("Invalid glTF texture index %d", index);
                return nullptr;
            }

            if (texCoord > 0) {
                DuskLogWarn("Multiple TEXCOORDs not supported");
            }

            return textures[index];
        }
        return nullptr;
    };

    const auto it = data.find("materials");
    if (it != data.cend()) {
        const auto& array = it.value();
        for (const auto& object : array) {
            if (object.is_object()) {
                materials.push_back(std::make_shared<Material>());
                auto& material = materials.back();

                DuskLogVerbose("glTF material %s", object.value("name", ""));

                auto valIt = object.find("normalTexture");
                if (valIt != object.end()) {
                    const auto& value = valIt.value();
                    material->NormalMap = parseTexture(value);
                    if (value.is_object()) {
                        material->NormalScale = value.value("scale", 1.0f);
                    }
                }

                valIt = object.find("emissiveFactor");
                if (valIt != object.end()) {
                    const auto& value = valIt.value();
                    material->EmissiveFactor = parseVec3(value, material->EmissiveFactor);
                }

                valIt = object.find("emissiveTexture");
                if (valIt != object.end()) {
                    const auto& value = valIt.value();
                    material->EmissiveMap = parseTexture(value);
                }

                valIt = object.find("occlusionTexture");
                if (valIt != object.end()) {
                    const auto& value = valIt.value();
                    material->OcclusionMap = parseTexture(value);
                    if (value.is_object()) {
                        material->OcclusionStrength = value.value("strength", 1.0f);
                    }
                }

                const auto groupIt = object.find("pbrMetallicRoughness");
                if (groupIt != object.cend()) {
                    const auto& group = groupIt.value();
                    if (group.is_object()) {
                        valIt = group.find("baseColorFactor");
                        if (valIt != group.end()) {
                            const auto& value = valIt.value();
                            material->BaseColorFactor = parseVec4(value, material->BaseColorFactor);
                        }

                        valIt = group.find("baseColorTexture");
                        if (valIt != group.end()) {
                            const auto& value = valIt.value();
                            material->BaseColorMap = parseTexture(value);
                        }

                        valIt = group.find("metallicFactor");
                        if (valIt != group.end()) {
                            material->MetallicFactor = valIt.value().get<float>();
                        }

                        valIt = group.find("roughnessFactor");
                        if (valIt != group.end()) {
                            material->MetallicFactor = valIt.value().get<float>();
                        }

                        valIt = group.find("metallicRoughnessTexture");
                        if (valIt != group.end()) {
                            const auto& value = valIt.value();
                            material->MetallicRoughnessMap = parseTexture(value);
                        }
                    }
                }
            }
        }
    }

    return materials;
}

std::vector<std::shared_ptr<Mesh>> loadMeshes(
    const json& data,
    const std::vector<bufferView_t>& bufferViews, 
    const std::vector<std::vector<uint8_t>>& buffers,
    const std::vector<accessor_t>& accessors,
    const std::vector<std::shared_ptr<Material>>& materials)
{
    std::vector<std::shared_ptr<Mesh>> meshes;
    
	std::vector<GLuint> vbos;

    const auto it = data.find("meshes");
    if (it != data.cend()) {
        const auto& array = it.value();
        for (const auto& object : array) {
            if (object.is_object()) {
                DuskLogVerbose("glTF mesh %s", object.value("name", ""));

                std::vector<Mesh::Primitive> primitives;

                const auto& primIt = object.find("primitives");
                if (primIt != object.end()) {
                    const auto& primArray = primIt.value();
                    if (primArray.is_array()) {
                    for (const auto& primitive : primArray) {
                        if (primitive.is_object()) {
                            GLuint vao;
                            glGenVertexArrays(1, &vao);
                            glBindVertexArray(vao);

                            int indices = primitive.value("indices", -1);
                            if (indices < 0) {
                                DuskLogError("glDrawArrays not supported");
                                continue;
                            }

                            const auto& indexAccessor = accessors[indices];

                            {
                                const auto& bufferView = bufferViews[indexAccessor.bufferView];
                                const auto& buffer = buffers[bufferView.buffer];

                                GLuint vbo;
                                glGenBuffers(1, &vbo);
                                vbos.push_back(vbo);

                                glBindBuffer(bufferView.target, vbo);
                                glBufferData(bufferView.target, bufferView.byteLength,
                                    buffer.data() + bufferView.byteOffset, GL_STATIC_DRAW);
                            }

                            const auto& attrIt = primitive.find("attributes");
                            if (attrIt != primitive.end()) {
                                const auto& attribs = attrIt.value();
                                if (attribs.is_object()) {
                                    for (const auto& [attrib, accessorIndex] : attribs.items()) {
                                        auto& accessor = accessors[accessorIndex];
                                        auto& bufferView = bufferViews[accessor.bufferView];
                                        auto& buffer = buffers[bufferView.buffer];
                                        int byteStride = bufferView.byteStride;

                                        DuskLogVerbose("glTF attribute %s", attrib);

                                        GLuint vbo;
                                        glGenBuffers(1, &vbo);
                                        vbos.push_back(vbo);

                                        glBindBuffer(bufferView.target, vbo);
                                        glBufferData(bufferView.target, bufferView.byteLength,
                                            buffer.data() + bufferView.byteOffset, GL_STATIC_DRAW);

                                        GLint size = -1;
                                        if (accessor.type == "SCALAR") {
                                            size = 1;
                                        } else if (accessor.type == "VEC2") {
                                            size = 2;
                                        } else if (accessor.type == "VEC3") {
                                            size = 3;
                                        } else if (accessor.type == "VEC4") {
                                            size = 4;
                                        }

                                        GLint vaa = -1;
                                        if (attrib == "POSITION") {
                                            vaa = Mesh::AttributeID::POSITION;
                                        } else if (attrib == "NORMAL") {
                                            vaa = Mesh::AttributeID::NORMAL;
                                        } else if (attrib == "TEXCOORD_0") {
                                            vaa = Mesh::AttributeID::TEXCOORD;
                                        } else if (attrib == "TANGENT") {
                                            vaa = Mesh::AttributeID::TANGENT;
                                        }

                                        if (vaa > -1) {
                                            glEnableVertexAttribArray(vaa);
                                            glVertexAttribPointer(vaa, size, accessor.componentType, 
                                                accessor.componentType == GL_FLOAT ? GL_FALSE : GL_TRUE, byteStride,
                                                (void*)accessor.byteOffset);
                                        } else {
                                            DuskLogWarn("Ignoring glTF attribute %s", attrib);
                                        }
                                    }
                                }
                            }

                            GLenum mode = primitive.value<GLenum>("mode", 0);
                            int materialIndex = primitive.value("material", -1);

                            std::shared_ptr<Material> material;
                            if (materialIndex >= 0) {
                                material = materials[materialIndex];
                            }

                            primitives.push_back({
                                vao,
                                mode,
                                (GLsizei)indexAccessor.count,
                                (GLenum)indexAccessor.componentType,
                                (GLsizei)indexAccessor.byteOffset,
                                Box(),
                                material,
                            });
                        }
                    }

                    meshes.push_back(std::make_shared<Mesh>(primitives));

                    }
                }
            }
        }
    }

    return meshes;
}

bool loadFromASCII(const std::string& filename, std::vector<std::shared_ptr<Mesh>> * outMeshes, std::vector<std::unique_ptr<Actor>> * outActors)
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

    json data;
    file >> data;

    if (auto it = data.find("asset"); it != data.end()) {
        const auto& object = it.value();
        if (object.is_object()) {

            const auto& version = object.value("version", "");

            DuskLogVerbose("glTF Generator %s", object.value("generator", ""));
            DuskLogVerbose("glTF Version %s", object.value("version", ""));
            
            if (version != "2.0") {
                DuskLogError("only glTF 2.0 is supported");
				return false;
            }
        } else {
            DuskLogError("glTF missing required asset entry");
			return false;
        }
    }

    if (auto it = data.find("extensionsRequired"); it != data.end()) {
        const auto& array = it.value();
        if (array.is_array()) {
            for (const auto& ext : array) {
                DuskLogError("Missing glTF required extension '%s'", ext);
            }
        }
    }

    if (auto it = data.find("extensionsUsed"); it != data.end()) {
        const auto& array = it.value();
        if (array.is_array()) {
            for (const auto& ext : array) {
                DuskLogWarn("Missing glTF extension '%s'", ext);
            }
        }
    }

    const auto& buffers = loadBuffers(data, dir);
    const auto& bufferViews = loadBufferViews(data);
    const auto& accessors = loadAccessors(data);
    const auto& images = loadImages(data, dir, bufferViews, buffers);
    const auto& samplers = loadSamplers(data);
    const auto& textures = loadTextures(data, images, samplers);
    const auto& materials = loadMaterials(data, textures);
    const auto& meshes = loadMeshes(data, bufferViews, buffers, accessors, materials);

	if (outActors) {
		auto actor = std::make_unique<Actor>();
		auto comp = std::make_unique<MeshComponent>();
		for (auto& mesh : meshes) {
			comp->AddMesh(mesh);
		}
		actor->AddComponent(std::move(comp));
		outActors->push_back(std::move(actor));
	}

	return true;
}

bool loadFromBinary(const std::string& filename, std::vector<std::shared_ptr<Mesh>> * outMeshes, std::vector<std::unique_ptr<Actor>> * outActors)
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

bool LoadSceneFromFile(const std::string& filename, Scene * scene)
{
    DuskBenchStart();

    const std::string& ext = GetExtension(filename);
    bool binary = (ext == "glb");

	std::vector<std::unique_ptr<Actor>> actors;

    bool loaded = (binary ? loadFromBinary(filename, nullptr, &actors) : loadFromASCII(filename, nullptr, &actors));

	for (auto&& actor : actors) {
		scene->AddActor(std::move(actor));
	}

    DuskBenchEnd("glTF2::LoadSceneFromFile");
	return loaded;
}

std::vector<std::shared_ptr<Mesh>> LoadMeshesFromFile(const std::string& filename)
{
    DuskBenchStart();

    const std::string& ext = GetExtension(filename);
    bool binary = (ext == "glb");

	std::vector<std::shared_ptr<Mesh>> meshes;

    bool loaded = (binary ? loadFromBinary(filename, &meshes, nullptr) : loadFromASCII(filename, &meshes, nullptr));

    DuskBenchEnd("glTF2::LoadMeshFromFile");
    return meshes;
}

} // namespace glTF2

} // namespace dusk