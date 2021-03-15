#!/usr/bin/env python3

outputDirectory = 'Engine/Assets/Shaders/Dusk/Debug/'

attributes = {
    'AttributeNormal':   'a_Normal',
    'AttributeTangent':  'a_Tangent',
    'AttributeColor':    'a_Color',
    'AttributeTexCoord1': 'vec4(a_TexCoord1, 0, 1)',
    'AttributeTexCoord2': 'vec4(a_TexCoord2, 0, 1)',
}

ATTRIBUTE_VERTEX = '''
#version 450 core

#include <Dusk/Transform.inc.glsl>
#include <Dusk/VertexAttributes.inc.glsl>

layout(location = 0) out vec4 v_Color;

void main() {
    gl_Position = u_MVP * a_Position;
    v_Color = %s;
}
'''

ATTRIBUTE_FRAGMENT = '''
#version 450 core

layout(location = 0) in vec4 v_Color;

layout(location = 0) out vec4 o_Color;

void main() {
    o_Color = v_Color;
}
'''

filename = outputDirectory + 'Attribute.frag.glsl'
with open(filename, 'wt') as file:
    file.write(ATTRIBUTE_FRAGMENT)

for filename, expr in attributes.items():
    filename = outputDirectory + filename + '.vert.glsl'
    with open(filename, 'wt') as file:
        file.write(ATTRIBUTE_VERTEX % (expr,))

materials = {
    'MaterialBaseColorFactor':      'u_BaseColorFactor',
    'MaterialBaseColorMap':         'texture(u_BaseColorMap, v_TexCoord)',
    'MaterialBaseColor':            'texture(u_BaseColorMap, v_TexCoord) * u_BaseColorFactor',
    'MaterialNormalScale':          'vec4(vec3(u_NormalScale), 1)',
    'MaterialNormalMap':            'vec4(texture(u_NormalMap, v_TexCoord).rgb, 1)',
    'MaterialNormal':               'vec4(texture(u_NormalMap, v_TexCoord).rgb * u_NormalScale, 1)',
    'MaterialMetallicFactor':       'vec4(vec3(u_MetallicFactor), 1)',
    'MaterialMetallicMap':          'vec4(vec3(texture(u_MetallicRoughnessMap, v_TexCoord).b), 1)',
    'MaterialMetallic':             'vec4(vec3(texture(u_MetallicRoughnessMap, v_TexCoord).b * u_MetallicFactor), 1)',
    'MaterialRoughnessFactor':      'vec4(vec3(u_RoughnessFactor), 1)',
    'MaterialRoughnessMap':         'vec4(vec3(texture(u_MetallicRoughnessMap, v_TexCoord).g), 1)',
    'MaterialRoughness':            'vec4(vec3(texture(u_MetallicRoughnessMap, v_TexCoord).g * u_RoughnessFactor), 1)',
    'MaterialEmissiveFactor':       'vec4(u_EmissiveFactor, 1)',
    'MaterialEmissiveMap':          'vec4(texture(u_EmissiveMap, v_TexCoord).rgb, 1)',
    'MaterialEmissive':             'vec4(texture(u_EmissiveMap, v_TexCoord).rgb * u_EmissiveFactor, 1)',
    'MaterialOcclusionStrength':    'vec4(vec3(u_OcclusionStrength), 1)',
    'MaterialOcclusionMap':         'vec4(vec3(texture(u_OcclusionMap, v_TexCoord).r), 1)',
    'MaterialOcclusion':            'vec4(vec3(texture(u_OcclusionMap, v_TexCoord).r) * u_OcclusionStrength, 1)',
}

MATERIAL_VERTEX = '''
#version 450 core

#include <Dusk/Transform.inc.glsl>
#include <Dusk/VertexAttributes.inc.glsl>

layout(location = 0) out vec2 v_TexCoord;

void main() {
    gl_Position = u_MVP * a_Position;
    v_TexCoord = a_TexCoord1;
}
'''

MATERIAL_FRAGMENT = '''
#version 450 core

#include <Dusk/Material.inc.glsl>
#include <Dusk/ColorSpace.inc.glsl>

layout(location = 0) in vec2 v_TexCoord;

layout(location = 0) out vec4 o_Color;

void main()
{
    o_Color = %s;
}
'''

filename = outputDirectory + 'Material.vert.glsl'
with open(filename, 'wt') as file:
    file.write(MATERIAL_VERTEX)

for filename, expr in materials.items():
    filename = outputDirectory + filename + '.frag.glsl'
    with open(filename, 'wt') as file:
        file.write(MATERIAL_FRAGMENT % (expr,))
