#include "dusk/asset/Mesh.hpp"

#include <dusk/core/Benchmark.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/core/glTF2.hpp>
#include <dusk/scene/Actor.hpp>
#include <dusk/scene/Camera.hpp>

#include <unordered_map>

namespace dusk {

Mesh::Mesh(const std::string& filename)
{
    LoadFromFile(filename);
}

Mesh::Mesh(std::vector<Primitive>&& primitives)
{
    DuskLogVerbose("Loading Mesh from %zu Primitives", primitives.size());
    LoadFromData(std::move(primitives));
}

Mesh::~Mesh()
{
    for (auto& p : _primitives) {
        glDeleteVertexArrays(1, &p.VAO);
    }
}

bool Mesh::LoadFromFile(const std::string& filename)
{
	return LoadFromData(glTF2::LoadPrimitivesFromFile(filename));
}

bool Mesh::LoadFromData(std::vector<Primitive>&& primitives)
{
    for (auto& p : primitives) {
        if (!p.Shader) {
            std::unordered_map<std::string, std::string> defines;
            glBindVertexArray(p.VAO);

            std::unordered_map<GLint, std::string> attributes = {
                { POSITION, "HAS_POSITION", },
                { NORMAL, "HAS_NORMAL", },
                { UV, "HAS_UV", },
                { COLOR, "HAS_COLOR", },
                { TANGENT, "HAS_TANGENT", },
                { BITANGENT, "HAS_BITANGENT", },
            };

            GLint enabled;
            for (const auto& [id, def] : attributes) {
                glGetVertexAttribiv(id, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
                if (enabled) {
					defines[def] = std::string();
                }
            }

			if (p.Material) {
				const auto& materialDefines = p.Material->GetDefines();
				for (const auto& [name, value] : materialDefines) {
					defines[name] = value;
				}
			}

            glBindVertexArray(0);

            p.Shader.reset(new Shader({ 
                "shaders/default/default.vs.glsl", 
                "shaders/default/default.fs.glsl",
            }, defines));
        }
    }

    for (auto&& p : primitives) {
        _primitives.push_back(std::move(p));
    }
    
    SetLoaded(true);
    
    return true;
}

void Mesh::Render(RenderContext& ctx, glm::mat4 transform /*= glm::mat4(1.f)*/)
{
    if (!ctx.CurrentCamera) {
        return;
    }

    glm::mat4 model = transform;
    glm::mat4 view = ctx.CurrentCamera->GetView();
    glm::mat4 proj = ctx.CurrentCamera->GetProjection();
    glm::mat4 mvp = proj * view * model;
    glm::mat4 normal = glm::transpose(glm::inverse(glm::mat3(model)));

    for (auto& p : _primitives) {
        p.Shader->Bind();

        p.Shader->SetUniformMatrix("u_ModelMatrix", model);
        p.Shader->SetUniformMatrix("u_NormalMatrix", normal);
        p.Shader->SetUniformMatrix("u_ViewMatrix", view);
        p.Shader->SetUniformMatrix("u_ProjMatrix", proj);
        p.Shader->SetUniformMatrix("u_MVPMatrix", mvp);

        p.Shader->SetUniform("u_Camera", ctx.CurrentCamera->GetPosition());
        
        // TODO:
        p.Shader->SetUniform("u_LightDirection", glm::vec3(0.0f));

        if (p.Material) {
            p.Material->Bind(p.Shader.get());
        }

        glBindVertexArray(p.VAO);
        glDrawElements(p.Mode, p.Count, p.Type, (char*)0 + p.Offset);
        glBindVertexArray(0);
    }
}

} // namespace dusk
