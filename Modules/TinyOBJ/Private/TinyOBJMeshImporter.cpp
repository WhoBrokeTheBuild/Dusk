#include <Dusk/TinyOBJ/TinyOBJMeshImporter.hpp>

#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Util.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Dusk {

DUSK_TINYOBJ_API
std::vector<std::unique_ptr<MeshData>> TinyOBJMeshImporter::LoadFromFile(const std::string& filename)
{
    DuskBenchmarkStart();

    const std::string& dir = GetDirname(filename);
    std::vector<std::unique_ptr<MeshData>> meshes;

    DuskLogInfo("%s %s", dir, filename);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn, err;

    bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), dir.c_str());

    if (!warn.empty()) {
        DuskLogWarn("tinyobj: %s", warn);
    }

    if (!err.empty()) {
        DuskLogError("tinyobj: %s", err);
    }

    if (!result) {
        return { };
    }
    
    bool hasNormals = (!attrib.normals.empty());
    bool hasUVs = (!attrib.texcoords.empty());
    bool hasColors = (!attrib.colors.empty());

    for (auto& shape : shapes) {
        TinyOBJMeshData * meshData = New TinyOBJMeshData();
        
        meshData->Vertices.reserve(shape.mesh.indices.size());
        
        if (hasNormals) {
            meshData->Normals.reserve(shape.mesh.indices.size());
        }

        if (hasUVs) {
            meshData->UVs.reserve(shape.mesh.indices.size());
        }

        if (hasColors) {
            meshData->Colors.reserve(shape.mesh.indices.size());
        }

        for (const auto& i : shape.mesh.indices) {
            meshData->Vertices.push_back(attrib.vertices[3 * i.vertex_index + 0]);
            meshData->Vertices.push_back(attrib.vertices[3 * i.vertex_index + 1]);
            meshData->Vertices.push_back(attrib.vertices[3 * i.vertex_index + 2]);
            meshData->Vertices.push_back(1.0f);
            
            if (hasNormals) {
                meshData->Normals.push_back(attrib.normals[3 * i.normal_index + 0]);
                meshData->Normals.push_back(attrib.normals[3 * i.normal_index + 1]);
                meshData->Normals.push_back(attrib.normals[3 * i.normal_index + 2]);
                meshData->Normals.push_back(1.0f);
            }
            
            if (hasUVs) {
                meshData->UVs.push_back(attrib.texcoords[2 * i.texcoord_index + 0]);
                meshData->UVs.push_back(attrib.texcoords[2 * i.texcoord_index + 1]);
            }

            if (hasColors) {
                meshData->Colors.push_back(attrib.colors[3 * i.vertex_index + 0]);
                meshData->Colors.push_back(attrib.colors[3 * i.vertex_index + 1]);
                meshData->Colors.push_back(attrib.colors[3 * i.vertex_index + 2]);
                meshData->Colors.push_back(1.0f);
            }
        }

        meshes.push_back(std::unique_ptr<MeshData>(meshData));
    }

    DuskBenchmarkEnd("TinyOBJMeshImporter::LoadFromFile");
    return meshes;
}

} // namespace Dusk