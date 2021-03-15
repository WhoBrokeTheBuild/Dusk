#include <Dusk/TinyOBJ/TinyOBJMeshImporter.hpp>

#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/TinyOBJ/TinyOBJPrimitiveData.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Dusk::TinyOBJ {

DUSK_TINYOBJ_API
std::vector<std::unique_ptr<PrimitiveData>> TinyOBJMeshImporter::LoadFromFile(const string& filename, bool useAssetPath /*= true*/)
{
    DuskBenchmarkStart();

    auto gfx = GetGraphicsDriver();

    std::vector<std::shared_ptr<Material>> materialList;
    std::vector<std::unique_ptr<PrimitiveData>> primitiveList;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapeList;
    std::vector<tinyobj::material_t> mtlList;

    Path modelDirectory;
    string warn, err;
    bool result = false;

    if (useAssetPath) {
        const auto& assetPathList = GetAssetPathList();

        for (const auto& path : assetPathList) {
            Path fullPath = path / "Models" / filename;
            DuskLogVerbose("Checking '%s'", fullPath);

            modelDirectory = fullPath.GetParentPath();

            warn = "";
            err = "";

            result = tinyobj::LoadObj(&attrib, &shapeList, &mtlList, &warn, &err, fullPath.ToCString(), modelDirectory.ToCString());

            // If the error isn't 'Cannot open file', the .obj file is probably
            // broken, and we should fail
            if (!err.empty() && err.rfind("Cannot open file", 0) != 0) {
                break;
            }

            if (result) {
                break;
            }
        }
    }
    else {
        modelDirectory = Path(filename).GetParentPath();
        result = tinyobj::LoadObj(&attrib, &shapeList, &mtlList, &warn, &err, filename.c_str(), modelDirectory.ToCString());
    }

    if (!warn.empty()) {
        DuskLogWarn("tinyobj::LoadObj(): %s", warn);
    }

    if (!err.empty()) {
        DuskLogError("tinyobj::LoadObj(): %s", err);
    }

    if (!result) {
        return { };
    }

    for (const auto& mtl : mtlList) {
        auto material = gfx->CreateMaterial();
        material->Initialize();

        material->SetBaseColorFactor(vec4(glm::make_vec3(mtl.diffuse), 1.0f));
        if (!mtl.diffuse_texname.empty()) {
            Path path = modelDirectory / mtl.diffuse_texname;
            material->SetBaseColorMap(LoadTextureFromFile(path, false));

            if (!material->GetBaseColorMap()) {
                DuskLogWarn("Failed to load diffuse map '%s'", path);
            }
        }

        if (!mtl.bump_texname.empty()) {
            Path path = modelDirectory / mtl.bump_texname;
            material->SetNormalMap(LoadTextureFromFile(path, false));

            if (!material->GetNormalMap()) {
                DuskLogWarn("Failed to load (bump) normal map '%s'", path);
            }
        }

        // If map_Bump is not defined, attempt to use map_Ns
        if (!material->GetNormalMap()) {
            if (!mtl.normal_texname.empty()) {
                Path path = modelDirectory / mtl.normal_texname;
                material->SetNormalMap(LoadTextureFromFile(path, false));

                if (!material->GetNormalMap()) {
                    DuskLogWarn("Failed to load normal map '%s'", path);
                }
            }
        }

        if (mtl.metallic > 0) {
            material->SetMetallicFactor(mtl.metallic);
        }

        if (mtl.roughness > 0) {
            material->SetRoughnessFactor(mtl.roughness);
        }
        
        // There is no "metallicRoughness_texname", so verify both component maps are the same (empty or both set to the same texture)
        if (mtl.metallic_texname != mtl.roughness_texname) {
            DuskLogWarn("Failed to load metallic roughness map, map_Pm != map_Pr, '%s' != '%s'",
                mtl.metallic_texname, mtl.roughness_texname);
        }
        else {
            // At this point, either metallic_texname or roughness_texname would work
            if (!mtl.metallic_texname.empty()) {
                Path path = modelDirectory / mtl.metallic_texname;
                material->SetMetallicRoughnessMap(LoadTextureFromFile(path, false));

                if (!material->GetMetallicRoughnessMap()) {
                    DuskLogWarn("Failed to load metallic roughness map '%s'", path);
                }
            }
        }

        material->SetEmissiveFactor(glm::make_vec3(mtl.emission));
        if (!mtl.emissive_texname.empty()) {
            Path path = modelDirectory / mtl.emissive_texname;
            material->SetEmissiveMap(LoadTextureFromFile(path, false));

            // Emissive factor needs to be (1,1,1) if you have an emissive texture
            material->SetEmissiveFactor({ 1.0f, 1.0f, 1.0f });

            if (!material->GetNormalMap()) {
                DuskLogWarn("Failed to load (bump) normal map '%s'", path);
            }
        }

        DuskLogLoad("Found material '%s'", mtl.name);
        materialList.push_back(material);
    }
    
    bool hasNormals = (!attrib.normals.empty());
    bool hasTexCoords = (!attrib.texcoords.empty());
    bool hasColors = (!attrib.colors.empty());

    for (auto& shape : shapeList) {
        TinyOBJPrimitiveData * primitiveData = New TinyOBJPrimitiveData();

        if (!shape.mesh.material_ids.empty()) {
            int materialIndex = shape.mesh.material_ids[0];
            if (materialIndex >= 0 && materialIndex < materialList.size()) {
                primitiveData->material = materialList[materialIndex];
            }
        }
        
        primitiveData->vertexList.resize(shape.mesh.indices.size());

        for (size_t i = 0; i < shape.mesh.indices.size(); ++i) {
            auto index = shape.mesh.indices[i];
            auto& vertex = primitiveData->vertexList[i];

            vertex.Position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2],
                1.0f,
            };

            if (hasNormals) {
                vertex.Normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                    1.0f,
                };
            }
            
            if (hasColors) {
                vertex.Color = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2],
                    1.0f,
                };
            }
            
            if (hasTexCoords) {
                vertex.TexCoord1 = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    // TODO: Investigate why we need to flip them here
                    (1.0f - attrib.texcoords[2 * index.texcoord_index + 1]),
                };
            }
        }

        primitiveData->CalculateTangents();
        primitiveList.push_back(std::unique_ptr<PrimitiveData>(primitiveData));
    }

    DuskBenchmarkEnd();
    return primitiveList;
}

} // namespace Dusk::TinyOBJ