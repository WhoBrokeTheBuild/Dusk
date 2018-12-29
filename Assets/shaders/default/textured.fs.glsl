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
    //    normal = texture(_NormalMap, p_TexCoord);
    //}
    
    float alpha = _MaterialData.Dissolve;
    vec3 diffuse = _MaterialData.Diffuse;
    if (HasDiffuseMap()) {
        diffuse = texture(_DiffuseMap, p_TexCoord).rgb;
        alpha = texture(_DiffuseMap, p_TexCoord).a;
    }

    float diff = max(0.0, dot(normal.xyz, p_LightDir.xyz));
    diffuse = diff * diffuse;

    vec3 ambient = _MaterialData.Ambient.rgb;
    if (HasAmbientMap()) {
        ambient = texture(_AmbientMap, p_TexCoord).rgb;
    }
    ambient *= 0.1;

    vec3 halfwayDir = normalize(p_LightDir.xyz + p_ViewDir.xyz);
    float spec = pow(max(0.0, dot(normal.xyz, halfwayDir)), 32);

    vec3 specular = _MaterialData.Specular.rgb;
    if (HasSpecularMap()) {
        specular = texture(_SpecularMap, p_TexCoord).rgb;
    }
    specular *= spec;

    _Color = vec4(diffuse + ambient + specular, alpha);
}
