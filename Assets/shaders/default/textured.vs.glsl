#include shaders/data/attributes.inc.glsl
#include shaders/data/transform.inc.glsl

uniform vec3 u_EyePos;

out vec4 p_Position;
out vec4 p_Normal;
out vec2 p_TexCoord;

out vec3 p_LightDir;
out vec3 p_ViewDir;

void main()
{
    p_Position = u_Model * vec4(_Position, 1.0);
    p_Normal   = vec4(mat3(transpose(inverse(u_Model))) * _Normal, 1.0);
    p_TexCoord = _TexCoord;
    
    mat3 model = transpose(inverse(mat3(u_Model)));
	vec3 t = normalize(model * _Tangent.xyz);
    vec3 n = normalize(model * _Normal.xyz);
    t = normalize(t - dot(t, n) * n);
	vec3 b = cross(n, t);
    mat3 TBN = transpose(mat3(t, b, n));

    vec3 tanLightPos = TBN * vec3(100, 100, 100); // normally a lightPos
    vec3 tanViewPos  = TBN * u_EyePos;
    vec3 tanFragPos  = TBN * p_Position.xyz;
    
    p_LightDir = normalize(tanLightPos - tanFragPos);
    p_ViewDir = normalize(tanViewPos - tanFragPos);
    
    gl_Position = u_MVP * vec4(_Position, 1.0);
}
