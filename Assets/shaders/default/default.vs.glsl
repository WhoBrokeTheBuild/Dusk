#include shaders/data/attributes.inc.glsl
#include shaders/data/transform.inc.glsl

out vec3 v_Position;
out vec2 v_UV;
out mat3 v_TBN;

void main()
{
    vec4 pos = u_ModelMatrix * a_Position;
    v_Position = vec3(pos.xyz) / pos.w;

    vec3 normalW = normalize(vec3(u_NormalMatrix * vec4(a_Normal.xyz, 0.0)));
    vec3 tangentW = normalize(vec3(u_NormalMatrix * vec4(a_Tangent.xyz, 0.0)));
    vec3 bitangentW = cross(normalW, tangentW) * a_Tangent.w;
    v_TBN = mat3(tangentW, bitangentW, normalW);

    v_UV = a_UV;

    gl_Position = u_MVPMatrix * a_Position;
}
