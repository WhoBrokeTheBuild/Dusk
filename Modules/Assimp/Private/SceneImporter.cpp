#include <Dusk/Assimp/SceneImporter.hpp>

#include <Dusk/Benchmark.hpp>
#include <Dusk/Graphics/Mesh.hpp>
#include <Dusk/Graphics/MeshComponent.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Dusk::Assimp {

void ProcessNode(const aiScene * scene, Entity * parentEntity, aiNode * parentNode)
{
    auto gfx = GetGraphicsDriver();
    if (!gfx) {
        DuskLogError("Unable to load mesh, no graphics driver found");
        return;
    }

    for (unsigned c = 0; c < parentNode->mNumChildren; ++c) {
        const auto& node = parentNode->mChildren[c];
        auto entity = std::make_unique<Entity>();

        aiVector3D scale, position;
        aiQuaternion orient;
        node->mTransformation.Decompose(scale, orient, position);

        entity->SetName(node->mName.data);
        DuskLogLoad("Loading %s", entity->GetName());

        entity->SetPosition({ position.x, position.y, position.z });
        entity->SetScale({ scale.x, scale.y, scale.z });
        entity->SetOrientation({ orient.w, orient.x, orient.y, orient.z });

        if (node->mNumMeshes > 0) {
            auto meshComp = std::make_unique<MeshComponent>();

            for (unsigned m = 0; m < node->mNumMeshes; ++m) {
                const auto& nodeMesh = scene->mMeshes[node->mMeshes[m]];
                MeshData data;

                bool hasNormals = nodeMesh->HasNormals();
                bool hasUVs = nodeMesh->HasTextureCoords(0);
                bool hasColors = nodeMesh->HasVertexColors(0);
                bool hasTangentsAndBitangents = nodeMesh->HasTangentsAndBitangents();

                for (unsigned f = 0; f < nodeMesh->mNumFaces; ++f) {
                    const auto& face = nodeMesh->mFaces[f];
                    for (unsigned i = 0; i < face.mNumIndices; ++i) {
                        const auto& index = face.mIndices[i];

                        const auto& vertex = nodeMesh->mVertices[index];
                        data.Vertices.push_back(vertex.x);
                        data.Vertices.push_back(vertex.y);
                        data.Vertices.push_back(vertex.z);

                        if (hasNormals) {
                            const auto& normal = nodeMesh->mNormals[index];
                            data.Normals.push_back(normal.x);
                            data.Normals.push_back(normal.y);
                            data.Normals.push_back(normal.z);
                        }

                        if (hasUVs) {
                            const auto& uv = nodeMesh->mTextureCoords[0][index];
                            data.UVs.push_back(uv.x);
                            data.UVs.push_back(uv.y);
                        }

                        if (hasColors) {
                            const auto& color = nodeMesh->mColors[0][index];
                            data.Colors.push_back(color.r);
                            data.Colors.push_back(color.g);
                            data.Colors.push_back(color.b);
                            data.Colors.push_back(color.a);
                        }
                        
                        if (hasTangentsAndBitangents) {
                            const auto& tangent = nodeMesh->mTangents[index];
                            data.Tangents.push_back(tangent.x);
                            data.Tangents.push_back(tangent.y);
                            data.Tangents.push_back(tangent.z);

                            const auto& bitangent = nodeMesh->mBitangents[index];
                            data.Bitangents.push_back(bitangent.x);
                            data.Bitangents.push_back(bitangent.y);
                            data.Bitangents.push_back(bitangent.z);
                        }
                    }
                }
                
                auto mesh = gfx->CreateMesh();
                mesh->Load(&data);
                meshComp->AddMesh(std::move(mesh));
            }

            entity->AddComponent(std::move(meshComp));
        }

        parentEntity->AddChild(std::move(entity));
    }
}

DUSK_ASSIMP_API
bool SceneImporter::LoadFromFile(Entity * root, const std::string& filename)
{
    DuskBenchmarkStart();

    DuskLogLoad("Loading %s", filename);

    ::Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);

    if (!scene) {
        return false;
    }

    ProcessNode(scene, root, scene->mRootNode);

    DuskBenchmarkEnd("Assimp::SceneImporter::LoadFromFile");
    return true;
}

} // namespace Dusk::Assimp
