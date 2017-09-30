#ifndef DUSK_MODEL_SHADER_HPP
#define DUSK_MODEL_SHADER_HPP

#include <dusk/asset/Shader.hpp>
#include <dusk/asset/Material.hpp>
#include <dusk/scene/Model.hpp>

namespace dusk
{

class ModelShader : public Shader
{
public:

    ModelShader() = default;
    ModelShader(const std::vector<std::string>& filenames)
        : Shader(filenames)
    { }

    void UpdateTransform(Model * model);
    void UpdateMaterial(Material * material);

private:

    struct TransformData
    {
        alignas(64) glm::mat4 Model = glm::mat4(1);
        alignas(64) glm::mat4 View  = glm::mat4(1);
        alignas(64) glm::mat4 Proj  = glm::mat4(1);
        alignas(64) glm::mat4 MVP   = glm::mat4(1);
    };

    struct MaterialData
    {
        alignas(16)  glm::vec4 Ambient  = glm::vec4(0, 0, 0, 1);
        alignas(16)  glm::vec4 Diffuse  = glm::vec4(0, 0, 0, 1);
        alignas(16)  glm::vec4 Specular = glm::vec4(0, 0, 0, 1);

        alignas(4)   GLfloat Shininess = 0.0f;
        alignas(4)   GLfloat Dissolve  = 0.0f;

        alignas(4)   GLuint MapFlags = 0;
    };

    TransformData _transformData;
    MaterialData _materialData;

};

}

#endif // DUSK_MODEL_SHADER_HPP
