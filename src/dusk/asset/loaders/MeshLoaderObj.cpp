#include "dusk/asset/loaders/MeshLoaderObj.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>

namespace dusk
{

std::vector<std::string> MeshLoaderObj::GetExtensions() const
{
    return {
        "obj",
    };
}

bool MeshLoaderObj::Load(Mesh * mesh, const std::string& filename)
{
    DuskBenchStart();

    bool ret;
    std::string err;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string dirname = GetDirname(filename) + "/";

    DuskLogLoad("Loading model from '%s'", filename.c_str());

    if (filename.empty())
    {
        return false;
    }

    ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), dirname.c_str());
    if (!ret)
    {
        DuskLogError("Failed to load '%s': %s\n", filename.c_str(), err.c_str());
        return false;
    }

    bool hasNorms = !attrib.normals.empty();
    bool hasTxcds = !attrib.texcoords.empty();

    unsigned int offset = 0;
    for (tinyobj::shape_t & shape : shapes)
    {
        Mesh::Data data;

        DuskLogVerbose("Processing shape %s, offset %u", shape.name.c_str(), offset);

        tinyobj::mesh_t & objMesh = shape.mesh;
        std::shared_ptr<Material> material;

        if (!objMesh.material_ids.empty() && objMesh.material_ids[0] >= 0)
        {
            tinyobj::material_t & mat = materials[objMesh.material_ids[0]];

            std::string ambientFilename = (mat.ambient_texname.empty()
                ? std::string()
                : dirname + mat.ambient_texname);
            std::string diffuseFilename = (mat.diffuse_texname.empty()
                ? std::string()
                : dirname + mat.diffuse_texname);
            std::string specularFilename = (mat.specular_texname.empty()
                ? std::string()
                : dirname + mat.specular_texname);
            std::string bumpFilename = (mat.bump_texname.empty()
                ? std::string()
                : dirname + mat.bump_texname);

            data.Material.reset(new Material({
                glm::vec4(mat.ambient[0], mat.ambient[1], mat.ambient[2], 1.0f),
                glm::vec4(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1.0f),
                glm::vec4(mat.specular[0], mat.specular[1], mat.specular[2], 1.0f),
                mat.shininess, mat.dissolve,
                ambientFilename,
                diffuseFilename,
                specularFilename,
                bumpFilename
            }));
        }

        if (objMesh.num_face_vertices.empty())
        {
            // ???
            continue;
        }

        data.Vertices.reserve(objMesh.indices.size());
        if (hasNorms)
        {
            data.Normals.reserve(objMesh.indices.size());
        }
        if (hasTxcds)
        {
            data.TexCoords.reserve(objMesh.indices.size());
        }

        if (objMesh.num_face_vertices.front() == 4)
        {
            // TODO: Cleanup?
            DuskLogError("Loading Mesh with Quads");
            return false;
        }

        size_t index = 0;
        for (size_t f = 0; f < objMesh.num_face_vertices.size(); ++f)
        {
           size_t fv = objMesh.num_face_vertices[f];

           for (size_t v = 0; v < fv; ++v)
           {
               tinyobj::index_t idx = objMesh.indices[index + v];

               data.Vertices.push_back({
                   attrib.vertices[3 * idx.vertex_index + 0],
                   attrib.vertices[3 * idx.vertex_index + 1],
                   attrib.vertices[3 * idx.vertex_index + 2],
               });

               if (hasNorms)
               {
                   data.Normals.push_back({
                       attrib.normals[3 * idx.normal_index + 0],
                       attrib.normals[3 * idx.normal_index + 1],
                       attrib.normals[3 * idx.normal_index + 2],
                   });
               }

               if (hasTxcds)
               {
                   data.TexCoords.push_back({
                       attrib.texcoords[2 * idx.texcoord_index + 0],
                       attrib.texcoords[2 * idx.texcoord_index + 1],
                   });
               }
           }
           index += fv;
        }

        mesh->AddData(data);
    }

    DuskBenchEnd("MeshLoaderObj::Load");
    return mesh->FinishLoad();
}

} // namespace dusk
