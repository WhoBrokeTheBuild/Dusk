#include "dusk/asset/Mesh.hpp"

#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>

namespace dusk {

std::shared_ptr<Mesh> Mesh::Create()
{
    return std::make_shared<Mesh>();
}

std::shared_ptr<Mesh> Mesh::Create(const std::string& filename)
{
    return std::make_shared<Mesh>(filename);
}

std::shared_ptr<Mesh> Mesh::Create(const Data& data)
{
    return std::make_shared<Mesh>(data);
}

std::shared_ptr<Mesh> Mesh::Create(const std::vector<Data>& datum)
{
    return std::make_shared<Mesh>(datum);
}

Mesh::~Mesh()
{
    glDeleteBuffers(sizeof(_glVBOs) / sizeof(GLuint), _glVBOs);
    glDeleteVertexArrays(1, &_glVAO);
}

void Mesh::Serialize(nlohmann::json& data)
{

}

void Mesh::Deserialize(nlohmann::json& data)
{

}

bool Mesh::LoadFromFile(const std::string& filename)
{
    _filename = filename;

    const unsigned int flags = aiProcess_Triangulate |
                               aiProcess_FlipUVs;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, flags);

	if (!scene)
    {
        DuskLogError("Failed to load model, '%s'", filename.c_str());
        return false;
    }

	std::string dirname = GetDirname(filename) + "/";

    auto toVec2 = [](const aiVector3D& v) -> glm::vec2 {
        return { v.x, v.y };
    };

    auto toVec3 = [](const aiVector3D& v) -> glm::vec3 {
        return { v.x, v.y, v.z };
    };

    auto toVec4 = [](const aiColor4D& c) -> glm::vec4 {
        return { c.r, c.g, c.b, 1.0f };
    };

    for (unsigned int m = 0; m < scene->mNumMeshes; ++m)
    {
        aiMesh * aiMesh = scene->mMeshes[m];
        Data data;

        data.Vertices.reserve(aiMesh->mNumFaces * 3);

        if (aiMesh->HasNormals())
        {
            data.Normals.reserve(aiMesh->mNumFaces * 3);
        }

        if (aiMesh->HasTextureCoords(0))
        {
            data.TexCoords.reserve(aiMesh->mNumFaces * 3);
        }

        for (unsigned int f = 0; f < aiMesh->mNumFaces; ++f)
        {
            aiFace& aiFace = aiMesh->mFaces[f];
            assert(aiFace.mNumIndices == 3);

            for (int i = 0; i < 3; ++i) {
                unsigned int index = aiFace.mIndices[i];

                data.Vertices.push_back(toVec3(aiMesh->mVertices[index]));

                if (aiMesh->HasNormals())
                {
                    data.Normals.push_back(toVec3(aiMesh->mNormals[index]));
                }

                if (aiMesh->HasTextureCoords(0))
                {
                    data.TexCoords.push_back(toVec2(aiMesh->mTextureCoords[0][index]));
                }
            }
        }

        if (scene->HasMaterials())
        {
            aiMaterial * aiMat = scene->mMaterials[aiMesh->mMaterialIndex];
            aiString path;

            std::string ambientTex;
            if (aiMat->GetTextureCount(aiTextureType_AMBIENT) > 0)
            {
                aiMat->GetTexture(aiTextureType_AMBIENT, 0, &path);
                ambientTex = dirname + path.C_Str();
            }

            std::string diffuseTex;
            if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
                diffuseTex = dirname + path.C_Str();
            }

            std::string specularTex;
            if (aiMat->GetTextureCount(aiTextureType_SPECULAR) > 0)
            {
                aiMat->GetTexture(aiTextureType_SPECULAR, 0, &path);
                specularTex = dirname + path.C_Str();
            }

            std::string normalTex;
            if (aiMat->GetTextureCount(aiTextureType_NORMALS) > 0)
            {
                aiMat->GetTexture(aiTextureType_NORMALS, 0, &path);
                normalTex = dirname + path.C_Str();
            }

			aiColor4D aiAmbient;
			aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_AMBIENT, &aiAmbient);

			aiColor4D aiDiffuse;
			aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, &aiDiffuse);

			aiColor4D aiSpecular;
			aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_SPECULAR, &aiSpecular);

            data.Material.reset(new Material({
                toVec4(aiAmbient), toVec4(aiDiffuse), toVec4(aiSpecular),
                ambientTex, diffuseTex, specularTex, normalTex
            }));
        }

        AddData(data);
    }

    FinishLoad();

    return false;
}

void Mesh::AddData(const Data& data)
{
    _data.push_back(data);
}

bool Mesh::FinishLoad()
{
    DuskBenchStart();
    _loaded = false;

    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> norms;
    std::vector<glm::vec2> txcds;

    for (auto& data : _data)
    {
        _renderGroups.push_back({
            data.DrawMode,
            data.Material,
            verts.size(),
            data.Vertices.size()
        });

        verts.reserve(verts.size() + data.Vertices.size());
        verts.insert(verts.end(), data.Vertices.begin(), data.Vertices.end());

        norms.reserve(norms.size() + data.Normals.size());
        norms.insert(norms.end(), data.Normals.begin(), data.Normals.end());

        txcds.reserve(txcds.size() + data.TexCoords.size());
        txcds.insert(txcds.end(), data.TexCoords.begin(), data.TexCoords.end());
    }

    if (verts.empty())
    {
        DuskLogWarn("Cannot create an empty mesh");
        return false;
    }

    _bounds = ComputeBounds(verts);

    DuskLogLoad("Loading Mesh to OpenGL");

    glGenVertexArrays(1, &_glVAO);
    glBindVertexArray(_glVAO);

    glGenBuffers(sizeof(_glVBOs) / sizeof(GLuint), _glVBOs);

    glBindBuffer(GL_ARRAY_BUFFER, _glVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts.size(), (GLfloat *) verts.data(), GL_STATIC_DRAW);

    if (norms.empty())
    {
        glDeleteBuffers(1, &_glVBOs[1]);
        _glVBOs[1] = 0;
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, _glVBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * norms.size(), (GLfloat *) norms.data(), GL_STATIC_DRAW);
    }

    if (txcds.empty())
    {
        glDeleteBuffers(1, &_glVBOs[2]);
        _glVBOs[2] = 0;
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, _glVBOs[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * txcds.size(), (GLfloat *) txcds.data(), GL_STATIC_DRAW);
    }

    DuskLogVerbose("Bound mesh to VAO %u", _glVAO);

    // TODO: Colors?

    DuskBenchEnd("Mesh::FinishLoad");
    _loaded = true;
    return true;
}

void Mesh::Render(RenderContext& ctx)
{
    if (!ctx.CurrentShader) return;

    ctx.CurrentShader->Bind();

    glBindVertexArray(_glVAO);

    glBindBuffer(GL_ARRAY_BUFFER, _glVBOs[0]);
    glVertexAttribPointer(ctx.CurrentShader->GetAttributeLocation("_Position"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(ctx.CurrentShader->GetAttributeLocation("_Position"));

    if (_glVBOs[1] > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _glVBOs[1]);
        glVertexAttribPointer(ctx.CurrentShader->GetAttributeLocation("_Normal"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(ctx.CurrentShader->GetAttributeLocation("_Normal"));
    }

    if (_glVBOs[2] > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _glVBOs[2]);
        glVertexAttribPointer(ctx.CurrentShader->GetAttributeLocation("_TexCoord"), 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(ctx.CurrentShader->GetAttributeLocation("_TexCoord"));
    }

    for (const RenderGroup& group : _renderGroups)
    {
        if (group.material)
        {
            group.material->Bind(ctx.CurrentShader);
        }

        glDrawArrays(group.drawMode, group.start, group.count);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Box Mesh::ComputeBounds(const std::vector<glm::vec3>& verts)
{
    Box bounds;

    for (const glm::vec3& v : verts)
    {
        bounds.Min = glm::min(bounds.Min, v);
        bounds.Max = glm::max(bounds.Max, v);
    }

    return bounds;
}

} // namespace dusk
