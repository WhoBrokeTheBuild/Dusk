#ifndef DUSK_MODEL_HPP
#define DUSK_MODEL_HPP

#include <dusk/Config.hpp>

#include <dusk/asset/Mesh.hpp>

#include <initializer_list>
#include <memory>
#include <vector>

namespace dusk {

class Model
{
public:

    DISALLOW_COPY_AND_ASSIGN(Model)

    Model() = default;

    Model(std::shared_ptr<Mesh> mesh);

    Model(std::initializer_list<std::shared_ptr<Mesh>> mesh);

    Model(std::vector<std::shared_ptr<Mesh>> mesh);

    virtual ~Model() = default;

    void AddMesh(std::shared_ptr<Mesh> mesh);

    void SetPosition(const glm::vec3& pos);
    
    inline glm::vec3 GetPosition() const { 
        return _position; 
    }

    void SetRotation(const glm::vec3& rot);
    
    inline glm::vec3 GetRotation() const { 
        return _rotation; 
    }

    void SetScale(const glm::vec3& scale);

    inline glm::vec3 GetScale() const { 
        return _scale; 
    }

    glm::mat4 GetTransform();

    Box GetBounds() const;

    void Render(RenderContext& ctx);

private:

    glm::mat4 _transform = glm::mat4(1.0f);
    glm::vec3 _position  = glm::vec3(0.0f);
    glm::vec3 _rotation  = glm::vec3(0.0f);
    glm::vec3 _scale     = glm::vec3(1.0f);

    std::vector<std::shared_ptr<Mesh>> _meshes;

}; // class Model

} // namespace dusk

#endif // DUSK_MODEL_HPP
