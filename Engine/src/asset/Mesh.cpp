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
    swap(_loaded, rhs._loaded);
    swap(_glVAO, rhs._glVAO);
}

Mesh::Mesh(const string& filename)
{
    LoadFromFile(filename);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &_glVAO);
}

bool Mesh::LoadFromFile(const std::string& filename)
{
    DuskBenchStart();

    using namespace tinygltf;

    _filename = filename;

    std::string fullPath;
    const auto& paths = GetAssetPaths();

    tinygltf::Model model;
    TinyGLTF loader;
    string err;
    string warn;

    bool loaded = false;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath.c_str());
        loaded = loader.LoadBinaryFromFile(&model, &err, &warn, fullPath.c_str());

        if (loaded) break;
    }

	if (!loaded)
    {
        DuskLogError("Failed to load model, '%s'", filename.c_str());
        return false;
    }

	DuskLogVerbose("Model Generator %s", model.asset.generator.c_str());

	glGenVertexArrays(1, &_glVAO);
	glBindVertexArray(_glVAO);

    unordered_map<int, GLuint> vbos;
    for (size_t i = 0; i < model.bufferViews.size(); ++i) {
        auto& bufferView = model.bufferViews[i];
        auto& buffer = model.buffers[bufferView.buffer];

        GLuint vbo;
        glGenBuffers(1, &vbo);
        vbos[(int)i] = vbo;

        glBindBuffer(bufferView.target, vbo);
        glBufferData(bufferView.target, bufferView.byteLength, 
            buffer.data.data() + bufferView.byteOffset, GL_STATIC_DRAW);
    }

    for (auto& material : model.materials) {
        Material::Data data;
        auto& vals := material.values;
        auto& it = vals.end();

		it = vals.find("baseColorFactor");
		if (it != vals.end()) {
			auto& arr = it->second.number_array;
			data.Diffuse = vec3(arr[0], arr[1], arr[2]);
		}
        
        it = vals.find("baseColorTexture");
        if (it != vals.end()) {
            
        }

        for (auto& val : material.values) {
            DuskLogInfo("%s", val.first.c_str());
        }

        _materials.push_back(make_unique<Material>(data));
    }
    if (_materials.empty()) {
        DuskLogWarn("No Materials found, adding default");
        _materials.push_back(make_unique<Material>(Material::Data{}));
    }

    const auto& scene = model.scenes[model.defaultScene];
	for (int id : scene.nodes) {
        auto& node = model.nodes[id];
        auto& mesh = model.meshes[node.mesh];
        for (size_t i = 0; i < mesh.primitives.size(); ++i) {
            auto& primitive = mesh.primitives[i];
            auto& indexAccessor = model.accessors[primitive.indices];

            for (auto& attrib : primitive.attributes) {
                auto& accessor = model.accessors[attrib.second];
                int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);

                glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

                GLint size = (accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type);

                GLint vaa = -1;
                if (attrib.first.compare("POSITION") == 0) {
                    vaa = AttributeID::POSITION;
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

			_primitives.push_back({
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
    if (!ctx.CurrentShader) return;

    ctx.CurrentShader->Bind();

    glBindVertexArray(_glVAO);

	for (auto& p : _primitives) {
        if (p.material >= 0) {
            auto& mat = _materials[p.material];
            mat->Bind(ctx.CurrentShader);
        }

		glDrawElements(p.mode, p.count, p.type, (char*)0 + p.offset);
	}

    glBindVertexArray(0);
}

Box Mesh::ComputeBounds(const std::vector<glm::vec3>& verts)
{
    Box bounds;

    for (const glm::vec3& v : verts)
    {
        bounds.Min = glm::min(bounds.Min, v);
        bounds.Max = glm::max(bounds.Max, v);
    }

    return bounds;
}

} // namespace dusk
