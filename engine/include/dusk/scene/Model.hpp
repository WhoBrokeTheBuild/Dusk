#ifndef DUSK_MODEL_HPP
#define DUSK_MODEL_HPP

#include <dusk/Config.hpp>

#include <dusk/scene/Actor.hpp>
#include <dusk/asset/Mesh.hpp>
#include <dusk/core/Context.hpp>
#include <memory>

namespace dusk
{

struct TransformData
{
    alignas(64) glm::mat4 Model = glm::mat4(1);
    alignas(64) glm::mat4 View  = glm::mat4(1);
    alignas(64) glm::mat4 Proj  = glm::mat4(1);
    alignas(64) glm::mat4 MVP   = glm::mat4(1);
};

class Model : public Actor
{
public:

    DISALLOW_COPY_AND_ASSIGN(Model)

    Model();
    virtual ~Model() = default;

    virtual void Serialize(nlohmann::json& data) override;
    virtual void Deserialize(nlohmann::json& data) override;

    void AddMesh(std::shared_ptr<Mesh> mesh);
    std::vector<Mesh*> GetMeshes();

    Box GetBounds() const { return _bounds; }

    virtual void Update(const UpdateContext& ctx) override;
    virtual void Render(RenderContext& ctx) override;

private:

    TransformData _shaderData;

    Box _bounds;

    std::vector<std::shared_ptr<Mesh>> _meshes;

}; // class Model

} // namespace dusk

#endif // DUSK_MODEL_HPP
