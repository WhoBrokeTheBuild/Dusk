#include "dusk/scene/Scene.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/core/Util.hpp>
#include <dusk/scene/Axis.hpp>
#include <dusk/scene/Camera.hpp>
#include <dusk/scene/MeshComponent.hpp>
#include <dusk/asset/Mesh.hpp>
#include <dusk/asset/Material.hpp>
#include <dusk/asset/Texture.hpp>

#include <fstream>
#include <functional>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>

namespace dusk {

bool Scene::LoadFromFile(const std::string& filename)
{
    DuskBenchStart();

    using namespace tinygltf;

    std::string ext = GetExtension(filename);
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

    DuskLogLoad("Loading Scene from [%s]", fullPath);
    DuskLogVerbose("Model Generator [%s]", model.asset.generator);

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

    Camera * defaultCamera = nullptr;

    std::function<std::unique_ptr<Actor>(tinygltf::Node&)> processNode;
    processNode = [&](tinygltf::Node& node) -> std::unique_ptr<Actor> {
        Actor * actor = nullptr;
        
        glm::vec3 position = glm::vec3(0.f);
        if (node.translation.size() == 3) {
            position = glm::make_vec3(node.translation.data());
        }

        glm::quat rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
        if (node.rotation.size() == 4) {
            glm::vec4 data = glm::make_vec4(node.rotation.data());
            rotation = glm::quat(data[3], data[0], data[1], data[2]);
        }

        glm::vec3 scale = glm::vec3(1.f);
        if (node.scale.size() == 3) {
            scale = glm::make_vec3(node.scale.data());
        }

		if (node.camera >= 0) {
            Camera * camera = new Camera();
            actor = camera;
            
            auto& data = model.cameras[node.camera];
        
            if (data.type == "perspective") {
                camera->SetMode(Camera::Mode::Perspective);
                camera->SetClip(glm::vec2(data.perspective.znear, data.perspective.zfar));

                camera->SetPosition(position);
                camera->SetRotation(rotation);
                camera->SetScale(scale);

                if (!defaultCamera) { 
                    defaultCamera = camera;
                }

            }
            else if (data.type == "orthographic") {
                camera->SetMode(Camera::Mode::Orthographic);
            }
		}
        else {
            actor = new Actor();

            actor->SetPosition(position);
            actor->SetRotation(rotation);
            actor->SetScale(scale);

            if (node.mesh >= 0) {
                auto& mesh = model.meshes[node.mesh];

                std::vector<Mesh::Primitive> primitives;
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

                        DuskLogVerbose("Attribute %s", attrib.first);

                        GLuint vbo;
                        glGenBuffers(1, &vbo);
                        vbos.push_back(vbo);

                        glBindBuffer(bufferView.target, vbo);
                        glBufferData(bufferView.target, bufferView.byteLength,
                            buffer.data.data() + bufferView.byteOffset, GL_STATIC_DRAW);

                        GLint size = (accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type);

                        GLint vaa = -1;
                        if (attrib.first.compare("POSITION") == 0) {
                            vaa = Mesh::AttributeID::POSITION;
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
                            vaa = Mesh::AttributeID::NORMAL;
                        }
                        if (attrib.first.compare("TEXCOORD_0") == 0) {
                            vaa = Mesh::AttributeID::TEXCOORD;
                        }
                        if (attrib.first.compare("TANGENT") == 0) {
                            vaa = Mesh::AttributeID::TANGENT;
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

                    primitives.push_back({
                        vao,
                        (GLenum)primitive.mode,
                        (GLsizei)indexAccessor.count,
                        (GLenum)indexAccessor.componentType,
                        (GLsizei)indexAccessor.byteOffset,
                        bounds,
                        materials[(primitive.material < 0 ? 0 : primitive.material)],
                    });
                }
                
                auto comp = std::make_unique<MeshComponent>();
                comp->AddMesh(std::make_unique<Mesh>(primitives));
                actor->AddComponent(std::make_unique<Axis>());
                actor->AddComponent(std::move(comp));
            }
        }

        for (int id : node.children) {
            auto& child = model.nodes[id];
            actor->AddActor(processNode(child));
        }

        return std::unique_ptr<Actor>(actor);
    };

    const auto& scene = model.scenes[model.defaultScene];
    for (int id : scene.nodes) {
        auto& node = model.nodes[id];
        AddActor(processNode(node));
    }

    glBindVertexArray(0);

	for (auto vbo : vbos) {
		glDeleteBuffers(1, &vbo);
	}

    if (!defaultCamera) {
        auto camera = new Camera();
        camera->SetPosition(glm::vec3(10.0f));
        camera->SetLookAt(glm::vec3(0.f, 3.0f, 0.f));
        
        defaultCamera = camera;
        AddActor(std::unique_ptr<Camera>(camera));
    }

    App::Inst()->GetRenderContext().CurrentCamera = defaultCamera;

    DuskBenchEnd("Scene::LoadFromFile");
    return true;
}

void Scene::Start()
{ }

void Scene::Stop()
{ }

void Scene::AddActor(std::unique_ptr<Actor>&& actor)
{
    actor->SetScene(this);
    _actors.push_back(std::move(actor));
}

void Scene::HandleEvent(SDL_Event * evt)
{
    for (auto& actor : _actors) {
        actor->HandleEvent(evt);
    }
}

void Scene::Update(UpdateContext& ctx)
{
    for (auto& actor : _actors) {
        actor->Update(ctx);
    }
}

void Scene::Render(RenderContext& ctx)
{
    for (auto& actor : _actors) {
        actor->Render(ctx);
    }
}

void Scene::Print()
{
    DuskLog("_actors.size = %zu", _actors.size());
    DuskLog("_actors {", _actors.size());
    for (auto& actor : _actors) {
        actor->Print("  ");
    }
    DuskLog("}");
}

} // namespace dusk
