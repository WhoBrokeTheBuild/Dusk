#include "dusk/asset/Mesh.hpp"

#include <dusk/core/Benchmark.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/scene/Actor.hpp>
#include <dusk/scene/Camera.hpp>

#include <unordered_map>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>

namespace dusk {

Mesh::Mesh(const std::string& filename, std::unique_ptr<Shader>&& shader /*= nullptr*/)
{
    LoadFromFile(filename, std::move(shader));
}

Mesh::Mesh(Primitive p, std::unique_ptr<Shader>&& shader /*= nullptr*/)
{
    LoadFromData({ p }, std::move(shader));
}

Mesh::Mesh(std::vector<Primitive> primitives, std::unique_ptr<Shader>&& shader /*= nullptr*/)
{
    LoadFromData(primitives);
}

Mesh::~Mesh()
{
    for (auto& p : _primitives) {
        glDeleteVertexArrays(1, &p.VAO);
    }
}

bool Mesh::LoadFromFile(const std::string& filename, std::unique_ptr<Shader>&& shader /*= nullptr*/)
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

        DuskLogVerbose("Checking %s", fullPath);
        if (binary) {
            loaded = loader.LoadBinaryFromFile(&model, &err, &warn, fullPath);
        }
        else {
            loaded = loader.LoadASCIIFromFile(&model, &err, &warn, fullPath);
        }

        if (loaded) break;
    }

    if (!loaded)
    {
        DuskLogError("Failed to load model, '%s'", filename);
        return false;
    }

    if (shader) {
        _shader = std::move(shader);
    } else {
        _shader.reset(new Shader({ 
            "shaders/default/default.vs.glsl", 
            "shaders/default/default.fs.glsl",
        }));
    }

    DuskLogLoad("Loading Mesh from %s", fullPath);
    DuskLogVerbose("Model Generator %s", model.asset.generator);

    std::vector<std::shared_ptr<Texture>> textures;
    for (auto& texture : model.textures) {
        tinygltf::Image &image = model.images[texture.source];
        if (texture.sampler <= 0) {
			DuskLogVerbose("Loading Texture from buffer (%d, %d, %d)", image.width, image.height, image.component);
            textures.push_back(std::make_shared<Texture>(image.image.data(), glm::ivec2(image.width, image.height), image.component));
        }
        else {
            textures.push_back(std::make_shared<Texture>(std::move((GLuint)texture.sampler), glm::ivec2(image.width, image.height)));
        }
    }

    std::vector<std::shared_ptr<Material>> materials;
    for (auto& material : model.materials) {
        auto mat = std::make_shared<Material>();

        auto& vals = material.values;
        auto it = vals.end();

        auto& addVals = material.additionalValues;
        auto addIt = addVals.end();

        it = vals.find("baseColorFactor");
        if (it != vals.end() && !it->second.number_array.empty()) {
            mat->BaseColorFactor = glm::make_vec4(it->second.ColorFactor().data());
        }
        
        it = vals.find("baseColorTexture");
        if (it != vals.end()) {
            mat->BaseColorMap = textures[it->second.TextureIndex()];
        }

		it = vals.find("metallicFactor");
		if (it != vals.end()) {
			mat->Metallic = (float)it->second.Factor();
		}

		it = vals.find("roughnessFactor");
		if (it != vals.end()) {
			mat->Roughness = (float)it->second.Factor();
		}

        it = vals.find("metallicRoughnessTexture");
        if (it != vals.end()) {
            mat->MetallicRoughnessMap = textures[it->second.TextureIndex()];
        }

        addIt = addVals.find("normalTexture");
        if (addIt != addVals.end()) {
            mat->NormalMap = textures[addIt->second.TextureIndex()];
            mat->NormalScale = (float)addIt->second.Factor();
        }

        addIt = addVals.find("emissiveFactor");
        if (it != vals.end() && !it->second.number_array.empty()) {
            mat->EmissiveFactor = glm::make_vec3(it->second.ColorFactor().data());
        }

        addIt = addVals.find("emissiveTexture");
        if (addIt != addVals.end()) {
            mat->EmissiveMap = textures[addIt->second.TextureIndex()];
        }

        addIt = addVals.find("occlusionTexture");
        if (addIt != addVals.end()) {
            mat->OcclusionMap = textures[addIt->second.TextureIndex()];
            mat->OcclusionStrength = (float)addIt->second.Factor();
        }

        for (const auto& val : material.values) {
            DuskLogVerbose("Material value %s", val.first);
        }

		for (const auto& val : material.additionalValues) {
			DuskLogVerbose("Material additional value %s", val.first);
		}

        materials.push_back(std::move(mat));
    }

    if (materials.empty()) {
        DuskLogWarn("No Materials found, adding default");
        materials.push_back(std::make_shared<Material>());
    }

	std::vector<GLuint> vbos;

    const auto& scene = model.scenes[model.defaultScene];
    for (int id : scene.nodes) {
        auto& node = model.nodes[id];
		if (node.mesh < 0) {
			continue;
		}

        auto& mesh = model.meshes[node.mesh];
        for (size_t pInd = 0; pInd < mesh.primitives.size(); ++pInd) {
            auto& primitive = mesh.primitives[pInd];
			auto& indexAccessor = model.accessors[primitive.indices];

            Box bounds;

			GLuint vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			{
				auto& bufferView = model.bufferViews[indexAccessor.bufferView];
				auto& buffer = model.buffers[bufferView.buffer];

				GLuint vbo;
				glGenBuffers(1, &vbo);
				vbos.push_back(vbo);

				glBindBuffer(bufferView.target, vbo);
				glBufferData(bufferView.target, bufferView.byteLength,
					buffer.data.data() + bufferView.byteOffset, GL_STATIC_DRAW);
			}

            for (auto& attrib : primitive.attributes) {
                auto& accessor = model.accessors[attrib.second];
                auto& bufferView = model.bufferViews[accessor.bufferView];
                auto& buffer = model.buffers[bufferView.buffer];
                int byteStride = accessor.ByteStride(bufferView);

				GLuint vbo;
				glGenBuffers(1, &vbo);
				vbos.push_back(vbo);

				glBindBuffer(bufferView.target, vbo);
				glBufferData(bufferView.target, bufferView.byteLength,
					buffer.data.data() + bufferView.byteOffset, GL_STATIC_DRAW);

                GLint size = (accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type);

                GLint vaa = -1;
                if (attrib.first.compare("POSITION") == 0) {
                    vaa = AttributeID::POSITION;
                    if (accessor.minValues.size() == 3) {
                        auto& val = accessor.minValues;
                        bounds.Min = glm::vec3(val[0], val[1], val[2]);
                    }
                    if (accessor.maxValues.size() == 3) {
                        auto& val = accessor.maxValues;
                        bounds.Max = glm::vec3(val[0], val[1], val[2]);
                    }
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
                    glEnableVertexAttribArray(vaa);
                    glVertexAttribPointer(vaa, size, accessor.componentType, 
                        accessor.normalized ? GL_TRUE : GL_FALSE, byteStride,
                        (void*)accessor.byteOffset);
                } else {
                    DuskLogWarn("Ignoring attribute %s", attrib.first);
                }
            }
            
            /* Bitangent Generation
			auto& norm = primitive.attributes.find("NORMAL");
			auto& tang = primitive.attributes.find("TANGENT");
            if (norm != primitive.attributes.end() && tang != primitive.attributes.end()) {
				auto& nAccessor = model.accessors[norm->second];
				auto& nBufferView = model.bufferViews[nAccessor.bufferView];
				auto& nBuffer = model.buffers[nBufferView.buffer];
				glm::vec3 * nData = (glm::vec3 *)(nBuffer.data.data() + nBufferView.byteOffset);

				auto& tAccessor = model.accessors[tang->second];
				auto& tBufferView = model.bufferViews[tAccessor.bufferView];
				auto& tBuffer = model.buffers[tBufferView.buffer];
				glm::vec4 * tData = (glm::vec4 *)(tBuffer.data.data() + tBufferView.byteOffset);

				if (nAccessor.count != tAccessor.count) {
					DuskLogWarn("Normal and Tangent data mismatch");
				}
				else {
					using namespace glm;

					size_t len = nAccessor.count;

					std::vector<vec4> bitangents;
					bitangents.reserve(len);

					for (size_t i = 0; i < len; ++i) {
						vec3& N = nData[i];
						vec4& T = tData[i];
						vec3 B = cross(N, vec3(T)) * T.w;
						bitangents.push_back(vec4(B, 1.0f));
					}

					GLuint vbo;
					glGenBuffers(1, &vbo);
					vbos.push_back(vbo);

					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(vec3),
						bitangents.data(), GL_STATIC_DRAW);

					glEnableVertexAttribArray(AttributeID::BITANGENT);
					glVertexAttribPointer(AttributeID::BITANGENT, 3, GL_FLOAT,
						GL_FALSE, sizeof(vec3), 0);
				}
            }
            */

            _primitives.push_back({
                vao,
                (GLenum)primitive.mode,
                (GLsizei)indexAccessor.count,
                (GLenum)indexAccessor.componentType,
                (GLsizei)indexAccessor.byteOffset,
                bounds,
                materials[(primitive.material < 0 ? 0 : primitive.material)],
            });
        }
    }

    glBindVertexArray(0);

	for (auto vbo : vbos) {
		glDeleteBuffers(1, &vbo);
	}

    DuskBenchEnd("Mesh::LoadFromFile");

    _loaded = true;
    return _loaded;
}

bool Mesh::LoadFromData(std::vector<Primitive> primitives, std::unique_ptr<Shader>&& shader /*= nullptr*/)
{
    if (shader) {
        _shader = std::move(shader);
    } else {
        _shader.reset(new Shader({ 
            "shaders/default/default.vs.glsl", 
            "shaders/default/default.fs.glsl",
        }));
    }

    _primitives.insert(_primitives.end(), primitives.begin(), primitives.end());
    
    _loaded = true;
    return true;
}

void Mesh::SetShader(std::unique_ptr<Shader>&& shader)
{
    _shader = std::move(shader);
}

void Mesh::Render(RenderContext& ctx, glm::mat4 transform /*= glm::mat4(1.f)*/)
{
    if (!_shader || !ctx.CurrentCamera) {
        return;
    }

    _shader->Bind();

    glm::mat4 model = transform;
    glm::mat4 view = ctx.CurrentCamera->GetView();
    glm::mat4 proj = ctx.CurrentCamera->GetProjection();
    glm::mat4 mvp = proj * view * model;
    glm::mat4 normal = glm::transpose(glm::inverse(glm::mat3(model)));

    _shader->SetUniformMatrix("u_ModelMatrix", model);
    _shader->SetUniformMatrix("u_NormalMatrix", normal);
    _shader->SetUniformMatrix("u_ViewMatrix", view);
    _shader->SetUniformMatrix("u_ProjMatrix", proj);
    _shader->SetUniformMatrix("u_MVPMatrix", mvp);

    _shader->SetUniform("u_Camera", ctx.CurrentCamera->GetPosition());
    
    // TODO:
    _shader->SetUniform("u_LightDirection", glm::vec3(0.0f));

    for (auto& p : _primitives) {
        if (p.Mat) {
            p.Mat->Bind(_shader.get());
        }

        glBindVertexArray(p.VAO);
        glDrawElements(p.Mode, p.Count, p.Type, (char*)0 + p.Offset);
        glBindVertexArray(0);
    }
}

} // namespace dusk
