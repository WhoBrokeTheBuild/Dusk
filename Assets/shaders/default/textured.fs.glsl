#include shaders/data/transform.inc.glsl
#include shaders/data/material.inc.glsl

in vec4 p_Position;
in vec4 p_Normal;
in vec2 p_TexCoord;

in vec4 p_LightDir;
in vec4 p_ViewDir;

out vec4 _Color;

void main() {
    vec4 normal = normalize(p_Normal);
    //if (HasNormalMap()) {
    //    normal = texture(u_NormalMap, p_TexCoord);
    //}
    
    float alpha = u_Dissolve;
    vec3 diffuse = u_Diffuse;
    if (HasDiffuseMap()) {
        diffuse = texture(u_DiffuseMap, p_TexCoord).rgb;
        alpha = texture(u_DiffuseMap, p_TexCoord).a;
    }

    float diff = max(0.0, dot(normal.xyz, p_LightDir.xyz));
    diffuse = diff * diffuse;

    vec3 ambient = u_Ambient.rgb;
    if (HasAmbientMap()) {
        ambient = texture(u_AmbientMap, p_TexCoord).rgb;
    }
    ambient *= 0.1;

    vec3 halfwayDir = normalize(p_LightDir.xyz + p_ViewDir.xyz);
    float spec = pow(max(0.0, dot(normal.xyz, halfwayDir)), 32);

    vec3 specular = u_Specular.rgb;
    if (HasSpecularMap()) {
        specular = texture(u_SpecularMap, p_TexCoord).rgb;
    }
    specular *= spec;

    _Color = vec4(diffuse + ambient + specular, alpha);
}
