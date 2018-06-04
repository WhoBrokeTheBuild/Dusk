#include shaders/data/transform.inc.glsl
#include shaders/data/material.inc.glsl

in vec4 p_Position;
in vec4 p_Normal;
in vec2 p_TexCoord;

in vec3 p_LightDir;
in vec3 p_ViewDir;

out vec4 o_Color;

void main()
{
    vec4 normal = normalize(p_Normal);
    vec4 normalMap = texture(_NormalMap, p_TexCoord);
    if ((_MaterialData.MapFlags & NormalMapFlag) > 0u)
    {
        normal = _TransformData.Model * (normalMap.rgba * 2.0 - 1.0);
    }

    vec3 ambient = _MaterialData.Ambient.rgb;
    vec4 ambientMap = texture(_AmbientMap, p_TexCoord);
    if ((_MaterialData.MapFlags & AmbientMapFlag) > 0u)
    {
        ambient = ambientMap.rgb;
    }

    float diff = max(0.0, dot(normal.xyz, p_LightDir));
    vec4 diffuse = diff * _MaterialData.Diffuse.rgba;
    vec4 diffuseMap = texture(_DiffuseMap, p_TexCoord);

    diffuse.a = _MaterialData.Diffuse.a;
    if ((_MaterialData.MapFlags & DiffuseMapFlag) > 0u)
    {
        diffuse = diff * diffuseMap.rgba;
        diffuse.a = diffuseMap.a;
    }

    vec3  halfwayDir = normalize(p_LightDir + p_ViewDir);
    float spec       = pow(max(0.0, dot(normal.xyz, halfwayDir)), 32.0);

    vec3 specular = vec3(spec);
    vec4 specularMap = texture(_SpecularMap, p_TexCoord);
    if ((_MaterialData.MapFlags & SpecularMapFlag) > 0u)
    {
        specular = spec * specularMap.rgb;
    }

    o_Color = vec4(ambient + diffuse.rgb + specular, diffuse.a);
}
