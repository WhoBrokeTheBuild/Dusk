#include <Dusk/Assimp/SceneImporter.hpp>

#include <Dusk/Benchmark.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Dusk::Assimp {

void ProcessNode(const aiScene * scene, Entity * parent, aiNode * node)
{
    for (unsigned i = 0; i < node->mNumChildren; ++i) {
        const auto& childNode = node->mChildren[i];
        auto childEntity = std::make_unique<Entity>();

        aiVector3D scale, position;
        aiQuaternion orient;
        childNode->mTransformation.Decompose(scale, orient, position);

        childEntity->SetName(childNode->mName.data);
        childEntity->SetPosition({ position.x, position.y, position.z });
        childEntity->SetScale({ scale.x, scale.y, scale.z });
        childEntity->SetOrientation({ orient.w, orient.x, orient.y, orient.z });

        DuskLogLoad("Loading %s", childEntity->GetName());

        parent->AddChild(std::move(childEntity));
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
