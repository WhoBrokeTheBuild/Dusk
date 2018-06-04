#include shaders/data/transform.inc.glsl
#include shaders/data/material.inc.glsl

in vec3 _Position;
in vec3 _Normal;
in vec2 _TexCoord;

out vec4 p_Position;
out vec4 p_Normal;
out vec2 p_TexCoord;

out vec3 p_LightDir;
out vec3 p_ViewDir;

void main()
{
    p_Position = _TransformData.Model * vec4(_Position, 1.0);
    p_Normal   = _TransformData.Model * vec4(_Normal, 1.0);
    p_TexCoord = _TexCoord;

    p_LightDir = vec3(0, 2, 3) - p_Position.xyz;
    p_LightDir = normalize(p_LightDir);

    p_ViewDir = vec3(0, 2, 3) - p_Position.xyz;
    p_ViewDir = normalize(p_ViewDir);

    gl_Position = _TransformData.MVP * vec4(_Position, 1.0);
}
