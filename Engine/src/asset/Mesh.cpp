#include "dusk/asset/Mesh.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/scene/Actor.hpp>
#include <dusk/scene/Camera.hpp>

#include <unordered_map>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>

namespace dusk {

Mesh::Mesh(Mesh&& rhs)
{
    std::swap(_loaded, rhs._loaded);
    std::swap(_primitives, rhs._primitives);
}

Mesh::Mesh(const std::string& filename)
{
    LoadFromFile(filename);
}

Mesh::~Mesh()
{
	for (auto& p : _primitives) {
    	glDeleteVertexArrays(1, &p.vao);
	}
}

bool Mesh::LoadFromFile(const std::string& filename)
{
    DuskBenchStart();

    using namespace tinygltf;

    _filename = filename;

	std::string ext = GetExtension(_filename);
	bool binary = (ext == "glb");

    std::string fullPath;
    const auto& paths = GetAssetPaths();

    tinygltf::Model model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    bool loaded = false;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath.c_str());
		if (binary) {
			loaded = loader.LoadBinaryFromFile(&model, &err, &warn, fullPath.c_str());
		}
		else {
			loaded = loader.LoadASCIIFromFile(&model, &err, &warn, fullPath.c_str());
		}

        if (loaded) break;
    }

	if (!loaded)
    {
        DuskLogError("Failed to load model, '%s'", filename.c_str());
        return false;
    }

	DuskLogLoad("Loading Mesh from %s", fullPath.c_str());

	DuskLogVerbose("Model Generator %s", model.asset.generator.c_str());
    DuskLogVerbose("%zu accessors", model.accessors.size());
    DuskLogVerbose("%zu animations", model.animations.size());
    DuskLogVerbose("%zu buffers", model.buffers.size());
    DuskLogVerbose("%zu bufferViews", model.bufferViews.size());
    DuskLogVerbose("%zu materials", model.materials.size());
    DuskLogVerbose("%zu meshes", model.meshes.size());
    DuskLogVerbose("%zu nodes", model.nodes.size());
    DuskLogVerbose("%zu textures", model.textures.size());
    DuskLogVerbose("%zu images", model.images.size());
    DuskLogVerbose("%zu skins", model.skins.size());
    DuskLogVerbose("%zu samplers", model.samplers.size());
    DuskLogVerbose("%zu cameras", model.cameras.size());
    DuskLogVerbose("%zu scenes", model.scenes.size());
	DuskLogVerbose("%zu lights", model.lights.size());

    std::unordered_map<int, GLuint> vbos;

	std::vector<std::shared_ptr<Texture>> textures;
	for (auto& texture : model.textures) {
		tinygltf::Image &image = model.images[texture.source];
		if (texture.sampler <= 0) {
			textures.push_back(std::make_shared<Texture>(image.image.data(), glm::ivec2(image.width, image.height), image.component));
		}
		else {
			textures.push_back(std::make_shared<Texture>((GLuint)texture.sampler, glm::ivec2(image.width, image.height)));
		}
	}

    for (auto& material : model.materials) {
        auto mat = std::make_unique<Material>();

        auto& vals = material.values;
        auto it = vals.end();

		auto& addVals = material.additionalValues;
		auto addIt = addVals.end();

		it = vals.find("baseColorFactor");
		if (it != vals.end()) {
			auto& arr = it->second.number_array;
			mat->Diffuse = glm::vec3(arr[0], arr[1], arr[2]);
		}
        
        it = vals.find("baseColorTexture");
        if (it != vals.end()) {
			mat->DiffuseMap = textures[it->second.TextureIndex()];
        }

		it = vals.find("metallicRoughnessTexture");
		if (it != vals.end()) {
			mat->RoughnessMap = textures[it->second.TextureIndex()];
		}

		addIt = addVals.find("normalTexture");
		if (addIt != addVals.end()) {
			mat->NormalMap = textures[addIt->second.TextureIndex()];
		}

        for (auto& val : material.values) {
            DuskLogInfo("%s", val.first.c_str());
        }

        _materials.push_back(std::move(mat));
    }

    if (_materials.empty()) {
        DuskLogWarn("No Materials found, adding default");
        _materials.push_back(std::make_unique<Material>());
    }

    const auto& scene = model.scenes[model.defaultScene];
	for (int id : scene.nodes) {
        auto& node = model.nodes[id];
        auto& mesh = model.meshes[node.mesh];
		DuskLogVerbose("%zu primitives", mesh.primitives.size());
		
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

        for (size_t i = 0; i < mesh.primitives.size(); ++i) {
			for (size_t j = 0; j < model.bufferViews.size(); ++j) {
				auto& bufferView = model.bufferViews[j];
				auto& buffer = model.buffers[bufferView.buffer];
				if (bufferView.target == 0) {
					DuskLogWarn("Target is 0");
				}

				GLuint vbo;
				glGenBuffers(1, &vbo);
				vbos[(int)j] = vbo;

				glBindBuffer(bufferView.target, vbo);
				glBufferData(bufferView.target, bufferView.byteLength,
					buffer.data.data() + bufferView.byteOffset, GL_STATIC_DRAW);
			}

            auto& primitive = mesh.primitives[i];
            auto& indexAccessor = model.accessors[primitive.indices];

            for (auto& attrib : primitive.attributes) {
                auto& accessor = model.accessors[attrib.second];
				auto& bufferView = model.bufferViews[accessor.bufferView];
				auto& buffer = model.buffers[bufferView.buffer];
                int byteStride = accessor.ByteStride(bufferView);

                glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

                GLint size = (accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type);

                GLint vaa = -1;
                if (attrib.first.compare("POSITION") == 0) {
                    vaa = AttributeID::POSITION;
					
					glm::vec3 * data = (glm::vec3 *)(&buffer.data.at(0) + bufferView.byteOffset);
					ComputeBounds(data, bufferView.byteLength / sizeof(glm::vec3));
                }
                if (attrib.first.compare("NORMAL") == 0) {
                    vaa = AttributeID::NORMAL;
                }
                if (attrib.first.compare("TEXCOORD_0") == 0) {
                    vaa = AttributeID::TEXCOORD;
                }
				if (attrib.first.compare("TANGENT") == 0) {
					vaa = AttributeID::TANGENT;
				}

                if (vaa > -1) {
					DuskLogVerbose("Binding VAA %d", vaa);
                    glEnableVertexAttribArray(vaa);
                    glVertexAttribPointer(vaa, size, accessor.componentType, 
                        accessor.normalized ? GL_TRUE : GL_FALSE, byteStride,
                        (char*)0 + accessor.byteOffset);
                } else {
					DuskLogWarn("Ignoring VAA %s", attrib.first.c_str());
				}
            }

			if (auto norm = primitive.attributes.find("NORMAL"); norm != primitive.attributes.end()) {
				if (auto tang = primitive.attributes.find("TANGENT"); tang != primitive.attributes.end()) {
					// TODO: Compute Bitangent
				}
			}

			_primitives.push_back({
				vao,
				(GLenum)primitive.mode,
				(GLsizei)indexAccessor.count,
				(GLenum)indexAccessor.componentType,
				(GLsizei)indexAccessor.byteOffset,
                (primitive.material < 0 ? 0 : primitive.material),
			});
        }
    }

	glBindVertexArray(0);

	for (auto vbo : vbos) {
		glDeleteBuffers(1, &vbo.second);
	}

    _loaded = true;
    DuskBenchEnd("Mesh::LoadFromFile");
    return false;
}


void Mesh::Render(RenderContext& ctx)
{
	for (auto& p : _primitives) {
    	glBindVertexArray(p.vao);

        if (p.material >= 0) {
            auto& mat = _materials[p.material];
            mat->Bind(ctx.CurrentShader);
        }

		glDrawElements(p.mode, p.count, p.type, (char*)0 + p.offset);

		glBindVertexArray(0);
	}
}

void Mesh::ComputeBounds(const glm::vec3* data, size_t length)
{
	for (size_t i = 0; i < length; ++i)
	{
		_bounds.Min = glm::min(_bounds.Min, data[i]);
		_bounds.Max = glm::max(_bounds.Max, data[i]);
	}
}

} // namespace dusk
