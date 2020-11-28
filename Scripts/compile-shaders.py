#!/usr/bin/env python3

import os
import sys
import copy
import json
import ntpath as path
import subprocess
import tempfile

glslc = 'glslc'
if 'GLSLC' in os.environ:
    glslc = os.environ['GLSLC']

if len(sys.argv) < 3:
    exit(1)

configFile = sys.argv[1]
outputDirectory = sys.argv[2]

configFileOut = path.join(outputDirectory, path.basename(configFile))

file = open(configFile)
data = json.load(file)

def getRequiredDefines(defines, uniforms, attributes):
    requiredDefines = []
    for name,define in defines.items():
        required = True
        if 'uniforms' in define:
            for uniform in define['uniforms']:
                if uniform[0] == '!':
                    if uniform[1:] in uniforms.keys():
                        required = False
                        break
                else:
                    if not uniform in uniforms.keys():
                        required = False
                        break
        
        if 'attributes' in define:
            for attribute in define['attributes']:
                if attribute[0] == '!':
                    if attribute[1:] in attributes.keys():
                        required = False
                        break
                else:
                    if not attribute in attributes.keys():
                        required = False
                        break
        
        if not required:
            continue
        
        requiredDefines.append(name)

    return requiredDefines

def checkRequirements(tests, reference):
    for test in tests:
        if test[0] == '!':
            if test[1:] in reference:
                return False
        else:
            if not test in reference:
                return False
    return True


def isValidCombination(combination):
    uniformNames = combination['uniforms'].keys()
    attributeNames = combination['attributes'].keys()

    valid = True
    for uniform in combination['uniforms'].values():
        if 'uniforms' in uniform:
            if not checkRequirements(uniform['uniforms'], uniformNames):
                valid = False
        if 'attributes' in uniform:
            if not checkRequirements(uniform['attributes'], attributeNames):
                valid = False
    
    for attribute in combination['attributes'].values():
        if 'uniforms' in attribute:
            if not checkRequirements(attribute['uniforms'], uniformNames):
                valid = False
        if 'attributes' in attribute:
            if not checkRequirements(attribute['attributes'], attributeNames):
                valid = False

    return valid

def getUniformAttributeCombinations(uniforms, attributes):
    combinations = []

    requiredUniforms = {}
    optionalUniforms = {}
    for name,uniform in uniforms.items():
        if 'optional' in uniform and uniform['optional']:
            optionalUniforms[name] = uniform
        else:
            requiredUniforms[name] = uniform

    requiredAtttributes = {}
    optionalAttributes = {}
    for name,attribute in attributes.items():
        if 'optional' in attribute and attribute['optional']:
            optionalAttributes[name] = attribute
        else:
            requiredAtttributes[name] = attribute

    base = {
        'uniforms': requiredUniforms,
        'attributes': requiredAtttributes,
    }
    combinations.append(base)

    def recurse(base, uniforms, attributes, combinations):
        remainingUniforms = copy.deepcopy(uniforms)
        for uName,uniform in uniforms.items():
            uBase = copy.deepcopy(base)
            uBase['uniforms'][uName] = uniform
            if isValidCombination(uBase):
                combinations.append(uBase)
            del remainingUniforms[uName]

            remainingAttributes = copy.deepcopy(attributes)
            for aName,attribute in attributes.items():
                aBase = copy.deepcopy(uBase)
                aBase['attributes'][aName] = attribute
                if isValidCombination(aBase):
                    combinations.append(aBase)
                del remainingAttributes[aName]

                recurse(aBase, remainingUniforms, remainingAttributes, combinations)

    recurse(base, optionalUniforms, optionalAttributes, combinations)

    return combinations

def parseShader(data, shaderConfigs):
    name = data['name']
    stages = data['stages']
    uniforms = data['uniforms']
    attributes = data['attributes']
    defines = data['defines']

    print('Name: %s' % name)
    print('Stages: %s' % stages)

    combinations = getUniformAttributeCombinations(uniforms, attributes)
    for combination in combinations:
        shaderConfigs.append({
            'stages': stages,
            'uniforms': combination['uniforms'],
            'attributes': combination['attributes'],
            'defines': getRequiredDefines(defines, combination['uniforms'], combination['attributes'])
        })

shaderConfigs = []
parseShader(data, shaderConfigs)

index = 0
for config in shaderConfigs:
    temp = tempfile.mkdtemp()
    os.mkdir(temp + "/Dusk")

    with open(temp + '/Dusk/Attributes.inc.glsl', 'w') as file:
        file.write('#ifndef DUSK_ATTRIBUTES_INC_GLSL\n')
        file.write('#define DUSK_ATTRIBUTES_INC_GLSL\n')
        file.write('\n')

        location = 0
        for name,attribute in config['attributes'].items():
            file.write("layout(location = %d) in %s %s;\n" % (location, attribute['type'], name))
            location += 1
            
        file.write('\n')
        file.write('#endif // DUSK_ATTRIBUTES_INC_GLSL\n')

    with open(temp + '/Dusk/Uniforms.inc.glsl', 'w') as file:
        file.write('#ifndef DUSK_UNIFORMS_INC_GLSL\n')
        file.write('#define DUSK_UNIFORMS_INC_GLSL\n')
        file.write('\n')

        location = 0
        for name,uniform in config['uniforms'].items():
            file.write("layout(location = %d) uniform %s %s;\n" % (location, uniform['type'], name))
            location += 1
            
        file.write('\n')
        file.write('#endif // DUSK_UNIFORMS_INC_GLSL\n')

    commonArgs = [glslc]
    
    commonArgs.append('-I%s' % temp)

    for define in config['defines']:
        commonArgs.append('-D%s' % define)

    for source in config['stages']:
        args = copy.copy(commonArgs)

        parts = source.split('.')
        parts.pop()

        stage = parts.pop()
        args.append('-fshader-stage=%s' % stage)

        output = '.'.join(parts + [stage, 'spv'])

        args.append('--target-env=opengl4.5')

        args.append('-o%s/%d-%s' % (outputDirectory, index, output))
        args.append(source)

        result = subprocess.run(args, capture_output=True)

    index += 1

with open(configFileOut, 'w') as outFile:
    print(configFileOut)
    json.dump(shaderConfigs, outFile, indent=2)
