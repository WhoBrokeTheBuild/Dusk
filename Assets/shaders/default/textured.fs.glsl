#include shaders/data/transform.inc.glsl
#include shaders/data/material.inc.glsl

in vec4 p_Position;
in vec4 p_Normal;
in vec2 p_TexCoord;

in vec3 p_LightDir;
in vec3 p_ViewDir;

out vec4 _Color;

void main() {
    vec3 normal = normalize(p_Normal).xyz;
    if (HasNormalMap()) {
        normal = normalize(texture(u_NormalMap, p_TexCoord).xyz * 2.0 - 1.0);
    }
    
    vec3 emissive = u_Emissive.rgb;
    if (HasEmissiveMap()) {
        emissive = texture(u_EmissiveMap, p_TexCoord).rgb;
    }

    float alpha = u_Dissolve;
    vec3 diffuse = u_Diffuse;
    if (HasDiffuseMap()) {
        diffuse = texture(u_DiffuseMap, p_TexCoord).rgb;
        alpha = texture(u_DiffuseMap, p_TexCoord).a;
    }

    float diff = max(0.0, dot(normal, p_LightDir));
    diffuse = diff * diffuse;

    vec3 ambient = u_Ambient.rgb;
    if (HasAmbientMap()) {
        ambient = texture(u_AmbientMap, p_TexCoord).rgb;
    }
    ambient *= 0.1;

    vec3 halfwayDir = normalize(p_LightDir + p_ViewDir);
    float spec = pow(max(0.0, dot(normal, halfwayDir)), 32.0);

    vec3 specular = u_Specular.rgb;
    if (HasSpecularMap()) {
        specular = texture(u_SpecularMap, p_TexCoord).rgb;
    }
    specular *= spec;

    _Color = vec4(emissive + diffuse + ambient + specular, alpha);
}
