#include shaders/data/attributes.inc.glsl
#include shaders/data/transform.inc.glsl

out vec4 p_Position;
out vec4 p_Normal;
out vec2 p_TexCoord;

out vec4 p_LightDir;
out vec4 p_ViewDir;

void main()
{
    p_Position = u_Model * vec4(_Position, 1.0);
    p_Normal   = vec4(mat3(transpose(inverse(u_Model))) * _Normal, 1.0);
    p_TexCoord = _TexCoord;

	p_LightDir = normalize(vec4(0.2, 1.0, 0.3, 0.0));
	p_ViewDir = normalize(-(u_View * p_Position));

    gl_Position = u_MVP * vec4(_Position, 1.0);
}
