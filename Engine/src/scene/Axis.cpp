#include "dusk/scene/Axis.hpp"

#include <dusk/scene/Actor.hpp>

namespace dusk {

Axis::Axis() 
{
    if (!_Mesh) {
        Mesh::Primitive p;

        glGenVertexArrays(1, &p.VAO);
        glBindVertexArray(p.VAO);

        GLuint indices[] = {
            0, 1, 
            2, 3, 
            4, 5,
        };

        GLfloat position[] = { 
            0.f, 0.f, 0.f,
            1.f, 0.f, 0.f,
            0.f, 0.f, 0.f,
            0.f, 1.f, 0.f,
            0.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 
        };

        GLfloat color[] = { 
            1.f, 0.f, 0.f,
            1.f, 0.f, 0.f,
            0.f, 1.f, 0.f,
            0.f, 1.f, 0.f,
            0.f, 0.f, 1.f,
            0.f, 0.f, 1.f,
        };

        GLuint vbos[3];
        glGenBuffers(3, vbos);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(Mesh::AttributeID::POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(Mesh::AttributeID::COLOR);

        glBindVertexArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteBuffers(3, vbos);

        p.Mode = GL_LINES;
        p.Count = sizeof(indices) / sizeof(indices[0]);
        p.Type = GL_UNSIGNED_INT;
        p.Offset = 0;

        _Mesh = std::make_shared<Mesh>(p, 
            std::unique_ptr<Shader>(new Shader({ 
            "shaders/default/axis.vs.glsl", 
            "shaders/default/axis.fs.glsl",
            }))
        );
    }
}

void Axis::Render(RenderContext& ctx)
{
    if (ctx.CurrentPass != 1) {
        return;
    }

    auto transform = GetActor()->GetWorldTransform();
    transform = glm::scale(transform, 1.f / GetActor()->GetWorldScale());
    _Mesh->Render(ctx, transform);
}

} // namespace dusk