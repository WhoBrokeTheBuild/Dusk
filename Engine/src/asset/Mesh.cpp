#include "dusk/asset/Mesh.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/scene/Actor.hpp>
#include <dusk/scene/Camera.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace dusk {

Mesh::Mesh(Mesh&& rhs)
{
    swap(_loaded, rhs._loaded);
    swap(_glVAO, rhs._glVAO);
    memcpy(_glVBOs, rhs._glVBOs, sizeof(_glVBOs));
    memset(rhs._glVBOs, 0, sizeof(rhs._glVBOs));
    _renderGroups = move(rhs._renderGroups);
}

Mesh::Mesh(const string& filename)
{
    LoadFromFile(filename);
}

Mesh::Mesh(const Data& data)
{
    AddData(data);
    FinishLoad();
}

Mesh::Mesh(const vector<Data>& datum)
{
    for (const auto& data : datum) {
        AddData(data);
    }
    FinishLoad();
}

Mesh::~Mesh()
{
    glDeleteBuffers(sizeof(_glVBOs) / sizeof(GLuint), _glVBOs);
    glDeleteVertexArrays(1, &_glVAO);
}

bool Mesh::LoadFromFile(const std::string& filename)
{
    _filename = filename;

    const unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs;

    Assimp::Importer importer;
    std::string fullPath;
    const auto& paths = GetAssetPaths();
    const aiScene* scene = nullptr;

    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath.c_str());
        scene = importer.ReadFile(fullPath, flags);

        if (scene) break;
    }

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
            Material::Data matData;
            aiString path;

            if (aiMat->GetTextureCount(aiTextureType_AMBIENT) > 0) {
                aiMat->GetTexture(aiTextureType_AMBIENT, 0, &path);
                matData.AmbientMap = dirname + path.C_Str();
            }

            if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
                matData.DiffuseMap = dirname + path.C_Str();
            }

            if (aiMat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
                aiMat->GetTexture(aiTextureType_SPECULAR, 0, &path);
                matData.SpecularMap = dirname + path.C_Str();
            }

            if (aiMat->GetTextureCount(aiTextureType_NORMALS) > 0) {
                aiMat->GetTexture(aiTextureType_NORMALS, 0, &path);
                matData.NormalMap = dirname + path.C_Str();
            }

            if (aiMat->GetTextureCount(aiTextureType_OPACITY) > 0) {
                aiMat->GetTexture(aiTextureType_OPACITY, 0, &path);
                matData.AlphaMap = dirname + path.C_Str();
            }

            if (aiMat->GetTextureCount(aiTextureType_DISPLACEMENT ) > 0) {
                aiMat->GetTexture(aiTextureType_DISPLACEMENT , 0, &path);
                matData.DisplacementMap = dirname + path.C_Str();
            }

            //if (aiMat->GetTextureCount(aiTextureType_OPACITY) > 0) {
            //    aiMat->GetTexture(aiTextureType_OPACITY, 0, &path);
            //    matData.RoughnessMap = dirname + path.C_Str();
            //}

            //if (aiMat->GetTextureCount(aiTextureType_OPACITY) > 0) {
            //    aiMat->GetTexture(aiTextureType_OPACITY, 0, &path);
            //    matData.MetallicMap = dirname + path.C_Str();
            //}

            //if (aiMat->GetTextureCount(aiTextureType_SHININESS ) > 0) {
            //    aiMat->GetTexture(aiTextureType_SHININESS , 0, &path);
            //    matData.SheenMap = dirname + path.C_Str();
            //}

            if (aiMat->GetTextureCount(aiTextureType_EMISSIVE) > 0) {
                aiMat->GetTexture(aiTextureType_EMISSIVE, 0, &path);
                matData.EmissiveMap = dirname + path.C_Str();
            }

			aiColor3D aiAmbient;
			aiMat->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbient);

			aiColor3D aiDiffuse;
			aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse);

			aiColor3D aiSpecular;
			aiMat->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecular);
            
			aiColor3D aiEmissive;
			aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, aiEmissive);

            data.Material.reset(new Material(matData));
        }

        AddData(data);
    }

    FinishLoad();

    DuskLogLoad("Finished loading Mesh from '%s'", fullPath.c_str());

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

    DuskLogVerbose("Uploading Mesh Data to OpenGL");

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
