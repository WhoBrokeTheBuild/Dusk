#include <Dusk/TinyOBJ/TinyOBJMeshImporter.hpp>

#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/TinyOBJ/TinyOBJPrimitiveData.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Dusk::TinyOBJ {

DUSK_TINYOBJ_API
std::vector<std::unique_ptr<PrimitiveData>> TinyOBJMeshImporter::LoadFromFile(const std::string& filename)
{
    DuskBenchmarkStart();

    std::vector<std::unique_ptr<PrimitiveData>> primitiveList;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapeList;
    std::vector<tinyobj::material_t> materialList;

    std::string warn, err;
    bool result = false;

    const auto& assetPathList = GetAssetPathList();

    for (const auto& path : assetPathList) {
        std::string fullPath = path + "Models/" + filename;
        DuskLogVerbose("Checking '%s'", fullPath);

        std::string dir = GetDirname(fullPath);

        warn = "";
        err = "";

        result = tinyobj::LoadObj(&attrib, &shapeList, &materialList, &warn, &err, fullPath.c_str(), dir.c_str());

        // If the error isn't 'Cannot open file', the .obj file is probably
        // broken, and we should fail
        if (!err.empty() && err.rfind("Cannot open file", 0) != 0) {
            break;
        }

        if (result) {
            break;
        }
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
    
    bool hasNormals = (!attrib.normals.empty());
    bool hasTexCoords = (!attrib.texcoords.empty());
    bool hasColors = (!attrib.colors.empty());

    for (auto& shape : shapeList) {
        TinyOBJPrimitiveData * primitiveData = New TinyOBJPrimitiveData();
        
        primitiveData->VertexList.resize(shape.mesh.indices.size());

        for (size_t i = 0; i < shape.mesh.indices.size(); ++i) {
            auto index = shape.mesh.indices[i];
            auto& vertex = primitiveData->VertexList[i];

            index.vertex_index -= 1;
            vertex.Position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2],
                1.0f,
            };

            if (hasNormals) {
                index.normal_index -= 1;
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
                index.texcoord_index -= 1;
                vertex.TexCoord1 = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1],
                };
            }
        }

        primitiveData->CalculateTBN();
        primitiveList.push_back(std::unique_ptr<PrimitiveData>(primitiveData));
    }

    DuskBenchmarkEnd("TinyOBJMeshImporter::LoadFromFile");
    return primitiveList;
}

} // namespace Dusk::TinyOBJ