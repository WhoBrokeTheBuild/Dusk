#include "dusk/scene/Model.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/scene/Camera.hpp>

namespace dusk {

Model::Model(std::shared_ptr<Mesh> mesh)
{
    AddMesh(mesh);
}

Model::Model(std::initializer_list<std::shared_ptr<Mesh>> meshes)
{
    _meshes.insert(_meshes.end(), meshes.begin(), meshes.end());
}

Model::Model(std::vector<std::shared_ptr<Mesh>> meshes)
{
    _meshes.insert(_meshes.end(), meshes.begin(), meshes.end());
}

void Model::AddMesh(std::shared_ptr<Mesh> mesh)
{
    _meshes.push_back(mesh);
}

void Model::SetPosition(const glm::vec3& pos)
{
    _position = pos;
}

void Model::SetRotation(const glm::vec3& rot)
{
    _rotation = rot;
}

void Model::SetScale(const glm::vec3& scale)
{
    _scale = scale;
}

glm::mat4 Model::GetTransform()
{
    _transform = glm::mat4(1.0f);
    _transform = glm::translate(_transform, _position);
    _transform = glm::rotate(_transform, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    _transform = glm::rotate(_transform, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    _transform = glm::rotate(_transform, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    _transform = glm::scale(_transform, _scale);

    return _transform;
}

Box Model::GetBounds() const
{
    Box bounds;

    for (auto& mesh : _meshes) {
        bounds += mesh->GetBounds();
    }

    return bounds;
}

void Model::Render(RenderContext& ctx)
{
    auto sp = ctx.CurrentShader;
    sp->Bind();

    glm::mat4 model = GetTransform();
    glm::mat4 view = ctx.CurrentCamera->GetView();
    glm::mat4 proj = ctx.CurrentCamera->GetProjection();
    glm::mat4 mvp = proj * view * model;
    glm::mat4 normal = glm::transpose(glm::inverse(glm::mat3(model)));

    sp->SetUniformMatrix("u_ModelMatrix", model);
    sp->SetUniformMatrix("u_NormalMatrix", normal);
    sp->SetUniformMatrix("u_ViewMatrix", view);
    sp->SetUniformMatrix("u_ProjMatrix", proj);
    sp->SetUniformMatrix("u_MVPMatrix", mvp);

    sp->SetUniform("u_Camera", ctx.CurrentCamera->GetPosition());
    sp->SetUniform("u_LightDirection", glm::vec3(0.0f));

    for (auto& mesh : _meshes) {
        mesh->Render(ctx);
    }
}

}
