#include "dusk/asset/loaders/MeshLoaderFbx.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <fstream>

namespace dusk
{

const std::string FBX_FILE_ID = "Kaydara FBX Binary  ";
const size_t FBX_VERSION_OFF = 23;
const size_t FBX_HEADER_SIZE = 27;

struct FBXProp
{
    uint32_t endOffset;
    uint8_t type;

    struct
    {
        std::string S;
        std::vector<uint8_t> R;

        bool C;
        int16_t Y;
        int32_t I;
        int64_t L;
        float F;
        double D;

        std::vector<bool> b;
        std::vector<unsigned char> c;
        std::vector<int32_t> i;
        std::vector<int64_t> l;
        std::vector<float> f;
        std::vector<double> d;
    } data;

    FBXProp() = default;

    FBXProp(const std::vector<uint8_t> buffer, size_t startOff)
    {
        size_t offset = startOff;
        uint32_t arrayLength = 0;
        uint32_t encoding = UINT32_MAX;
        uint32_t compressedLength = 0;
        size_t elementSize = 0;
        std::vector<uint8_t> decodedBuffer;

        type = buffer[offset];
        offset += sizeof(uint8_t);

        switch (type)
        {
        case 'b':
            elementSize = sizeof(bool);
            break;
        case 'c':
            elementSize = sizeof(unsigned char);
            break;
        case 'i':
            elementSize = sizeof(int32_t);
            break;
        case 'l':
            elementSize = sizeof(int64_t);
            break;
        case 'f':
            elementSize = sizeof(float);
            break;
        case 'd':
            elementSize = sizeof(double);
            break;
        default:;
        }

        switch (type)
        {
        case 'S':
        case 'R':

            memcpy(&arrayLength, buffer.data() + offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);

            break;
        case 'b':
        case 'c':
        case 'i':
        case 'l':
        case 'f':
        case 'd':

            memcpy(&arrayLength, buffer.data() + offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);

            memcpy(&encoding, buffer.data() + offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);

            memcpy(&compressedLength, buffer.data() + offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);

            break;
        default:;
        }

        if (encoding == 1)
        {
            decodedBuffer.resize(arrayLength * elementSize);

            int zstatus = Z_OK;
            z_stream zs;

            memset(&zs, 0, sizeof(zs));
            zs.avail_in = 0;
            zs.next_in = Z_NULL;
            zs.zalloc = Z_NULL;
            zs.zfree = Z_NULL;
            zs.opaque = Z_NULL;

            zstatus = inflateInit(&zs);
            if (zstatus != Z_OK)
            {
                DuskLogError("Failed to init zlib stream: %s", zs.msg);
                inflateEnd(&zs);
                return;
            }

            DuskLogVerbose("Inflating %u bytes to %zu bytes", compressedLength, decodedBuffer.size());

            zs.next_in = (Bytef *)(buffer.data() + offset);
            zs.avail_in = compressedLength;
            zs.next_out = (Bytef *)decodedBuffer.data();
            zs.avail_out = decodedBuffer.size();

            zstatus = inflate(&zs, Z_FINISH);
            inflateEnd(&zs);

            if (zstatus == Z_STREAM_ERROR)
            {
                DuskLogError("Error, zlib stream error");
                return;
            }
            else if (zstatus == Z_DATA_ERROR)
            {
                DuskLogError("Error, compressed data is corrupt");
                return;
            }
            else if (zstatus == Z_MEM_ERROR)
            {
                DuskLogError("Out of memory");
                return;
            }
            //else if (zstatus == Z_STREAM_END)
            //{
            //    DuskLogError("Failed to inflate data: %s", zs.msg);
            //    return;
            //}
        }

        switch (type)
        {
        case 'S':
            data.S.assign(buffer.data() + offset, buffer.data() + offset + arrayLength);
            offset += arrayLength;
            break;
        case 'R':
            data.R.assign(buffer.data() + offset, buffer.data() + offset + arrayLength);
            offset += arrayLength;
            break;
        case 'C':
            data.C = buffer[offset];
            offset += sizeof(uint8_t);
            break;
        case 'Y':
            memcpy(&data.Y, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            break;
        case 'I':
            memcpy(&data.I, buffer.data() + offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);
            break;
        case 'L':
            memcpy(&data.L, buffer.data() + offset, sizeof(uint64_t));
            offset += sizeof(uint64_t);
            break;
        case 'F':
            memcpy(&data.F, buffer.data() + offset, sizeof(float));
            offset += sizeof(float);
            break;
        case 'D':
            memcpy(&data.D, buffer.data() + offset, sizeof(double));
            offset += sizeof(double);
            break;
        case 'b':
            if (encoding == 1)
            {
                data.b.assign(decodedBuffer.begin(), decodedBuffer.end());
            }
            else
            {
                data.b.assign(buffer.data() + offset, buffer.data() + offset + compressedLength);
            }
            offset += compressedLength;
            break;
        case 'c':
            if (encoding == 1)
            {
                data.b.assign(decodedBuffer.begin(), decodedBuffer.end());
            }
            else
            {
                data.c.assign(buffer.data() + offset, buffer.data() + offset + compressedLength);
            }
            offset += compressedLength;
            break;
        case 'i':
            data.i.resize(arrayLength);
            if (encoding == 1)
            {
                memcpy(data.i.data(), decodedBuffer.data(), decodedBuffer.size());
            }
            else
            {
                memcpy(data.i.data(), buffer.data() + offset, compressedLength);
            }
            offset += compressedLength;
            break;
        case 'l':
            data.l.resize(arrayLength);
            if (encoding == 1)
            {
                memcpy(data.l.data(), decodedBuffer.data(), decodedBuffer.size());
            }
            else
            {
                memcpy(data.l.data(), buffer.data() + offset, compressedLength);
            }
            offset += compressedLength;
            break;
        case 'f':
            data.f.resize(arrayLength);
            if (encoding == 1)
            {
                memcpy(data.f.data(), decodedBuffer.data(), decodedBuffer.size());
            }
            else
            {
                memcpy(data.f.data(), buffer.data() + offset, compressedLength);
            }
            offset += compressedLength;
            break;
        case 'd':
            data.d.resize(arrayLength);
            if (encoding == 1)
            {
                memcpy(data.d.data(), decodedBuffer.data(), decodedBuffer.size());
            }
            else
            {
                memcpy(data.d.data(), buffer.data() + offset, compressedLength);
            }
            offset += compressedLength;
            break;
        }

        endOffset = offset;
    }

    void Print(int depth = 0)
    {
        for (int i = 0; i < depth; ++i)
            printf("  ");

        printf("Prop[%c]: ", type);

        switch (type)
        {
        case 'S':
            printf("%zu %s", data.S.size(), data.S.c_str());
            break;
        case 'R':
            printf("%zu", data.R.size());
            break;
        case 'C':
            printf("%s", (data.C ? "true" : "false"));
            break;
        case 'Y':
            printf("%d", data.Y);
            break;
        case 'I':
            printf("%d", data.I);
            break;
        case 'L':
            printf("%ld", data.L);
            break;
        case 'F':
            printf("%f", data.F);
            break;
        case 'D':
            printf("%f", data.D);
            break;
        case 'b':
            for (unsigned int i = 0; i < data.b.size(); ++i)
            {
                printf("%s, ", (data.b[i] ? "true" : "false"));
            }
            break;
        case 'c':
            for (unsigned int i = 0; i < data.c.size(); ++i)
            {
                printf("%u (%c), ", data.c[i], data.c[i]);
            }
            break;
        case 'i':
            for (unsigned int i = 0; i < data.i.size(); ++i)
            {
                printf("%d, ", data.i[i]);
            }
            break;
        case 'l':
            for (unsigned int i = 0; i < data.l.size(); ++i)
            {
                printf("%ld, ", data.l[i]);
            }
            break;
        case 'f':
            for (unsigned int i = 0; i < data.f.size(); ++i)
            {
                printf("%f, ", data.f[i]);
            }
            break;
        case 'd':
            for (unsigned int i = 0; i < data.d.size(); ++i)
            {
                printf("%f, ", data.d[i]);
            }
            break;
        }

        printf("\n");
    }
};

struct FBXNode
{
    uint32_t endOffset;
    std::string name;
    std::vector<FBXProp> props;
    std::vector<FBXNode> nodes;

    FBXNode() = default;

    FBXNode * GetNode(const std::string& name)
    {
        for (FBXNode& node : nodes)
        {
            if (node.name == name)
            {
                return &node;
            }
        }

        return nullptr;
    }

    std::vector<FBXNode *> GetNodes(const std::string& name)
    {
        std::vector<FBXNode *> tmp;
        for (FBXNode& node : nodes)
        {
            if (node.name == name)
            {
                tmp.push_back(&node);
            }
        }
        return tmp;
    }

    FBXNode(const std::vector<uint8_t> buffer, size_t startOff)
    {
        size_t offset = startOff;
        uint32_t numProps = 0;
        uint32_t propListLen = 0;
        uint8_t nameLen = 0;

        memcpy(&endOffset, buffer.data() + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        // EOF Node
        if (endOffset == 0)
            return;

        memcpy(&numProps, buffer.data() + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&propListLen, buffer.data() + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&nameLen, buffer.data() + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        name.assign(buffer.data() + offset, buffer.data() + offset + nameLen);
        offset += nameLen;

        for (unsigned int i = 0; i < numProps; ++i)
        {
            FBXProp prop(buffer, offset);
            offset = prop.endOffset;
            props.push_back(prop);
        }

        while (offset < endOffset)
        {
            FBXNode node(buffer, offset);

            if (node.endOffset == 0)
                break;

            offset = node.endOffset;
            nodes.push_back(node);
        }
    }

    void Print(int depth = 0)
    {
        auto printTabs = [&depth]() {
            for (int i = 0; i < depth; ++i)
                printf("  ");
        };

        printTabs();
        printf("%s\n", name.c_str());

        printTabs();
        printf("  End Offset: %u\n", endOffset);

        for (unsigned int i = 0; i < props.size(); ++i)
        {
            props[i].Print(depth + 1);
        }

        for (unsigned int i = 0; i < nodes.size(); ++i)
        {
            nodes[i].Print(depth + 1);
        }
    }
};

std::vector<std::string> MeshLoaderFbx::GetExtensions() const
{
    return {
        "fbx",
    };
}

bool MeshLoaderFbx::Load(Mesh * mesh, const std::string& filename)
{
    DuskBenchStart();

    std::vector<unsigned char> buffer;
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file)
    {
        DuskLogError("Failed to open '%s'", filename.c_str());
        return false;
    }

    // Don't skip "whitespace" in a binary file
    file.unsetf(std::ios::skipws);

    buffer.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    buffer.insert(buffer.begin(),
                  (std::istream_iterator<unsigned char>(file)),
                  std::istream_iterator<unsigned char>());

    std::string fileId;
    fileId.assign(buffer.data(), buffer.data() + FBX_FILE_ID.size());

    if (fileId != FBX_FILE_ID)
    {
        DuskLogError("Invalid FBX Header");
        return false;
    }

    uint32_t version = 0;
    memcpy(&version, buffer.data() + FBX_VERSION_OFF, sizeof(uint32_t));
    DuskLogVerbose("FBX Version %d.%d", version / 1000, version % 1000);

    FBXNode root;
    root.name = "Root";

    for (size_t offset = FBX_HEADER_SIZE; offset < buffer.size();)
    {
        FBXNode node(buffer, offset);

        if (node.endOffset == 0)
            break;

        root.nodes.push_back(node);
        offset = node.endOffset;
    }

    FBXNode * objects = root.GetNode("Objects");
    if (!objects)
    {
        return false;
    }

    objects->Print();

    const std::vector<FBXNode *> textureNodes = objects->GetNodes("Texture");
    for (FBXNode * texNode : textureNodes)
    {
        FBXNode * filenameNode = texNode->GetNode("RelativeFilename");

        std::string filename = filenameNode->props[0].data.S;
    }

    const std::vector<FBXNode *> geometryNodes = objects->GetNodes("Geometry");
    for (FBXNode * geomNode : geometryNodes)
    {
        Mesh::Data data;

        bool hasNorms = false;
        bool hasTxcds = false;

        FBXNode * vertNode = geomNode->GetNode("Vertices");
        FBXNode * faceNode = geomNode->GetNode("PolygonVertexIndex");

        FBXNode * normLayerNode = geomNode->GetNode("LayerElementNormal");
        FBXNode * normNode = nullptr;
        if (normLayerNode)
        {
            hasNorms = true;
            normNode = normLayerNode->GetNode("Normals");
        }

        FBXNode * uvLayerNode = geomNode->GetNode("LayerElementUV");
        FBXNode * txcdNode = nullptr;
        FBXNode * txcdIndNode = nullptr;
        if (uvLayerNode)
        {
            hasTxcds = true;
            txcdNode = uvLayerNode->GetNode("UV");
            txcdIndNode = uvLayerNode->GetNode("UVIndex");
        }

        DuskLogVerbose("Model %s", geomNode->props[1].data.S.c_str());

        if (!vertNode || !faceNode) continue;

        const std::vector<double>& nodeVerts = vertNode->props[0].data.d;
        const std::vector<int32_t>& nodeInds = faceNode->props[0].data.i;

        const std::vector<double>& nodeNorms = (hasNorms ? normNode->props[0].data.d : std::vector<double>());

        const std::vector<int32_t>& txcds = (hasTxcds ? txcdIndNode->props[0].data.i : std::vector<int32_t>());
        const std::vector<double>& txcdInds = (hasTxcds ? txcdNode->props[0].data.d : std::vector<double>());

        // Calculate total verticies needed
        unsigned int totalVerts = 0;
        for (int i = 0, c = 0; i < nodeInds.size(); ++i)
        {
            ++c;
            if (nodeInds[i] < 0)
            {
                totalVerts += 3 * (c - 2);
                c = 0;
            }
        }

        // Pre-allocate space for them
        data.Vertices.reserve(totalVerts);
        if (hasNorms)
        {
            data.Normals.reserve(totalVerts);
        }
        if (hasTxcds)
        {
            data.TexCoords.reserve(totalVerts);
        }

        std::vector<int> indices;
        for (unsigned int i = 0; i < nodeInds.size(); ++i)
        {
            indices.push_back(nodeInds[i]);
            // A negative index indicates a complete polygon
            if (indices.back() < 0)
            {
                // Minimum size of 3 indices per polygon
                std::vector<int> tmp;
                tmp.push_back(indices[0]);
                tmp.push_back(indices[1]);
                tmp.push_back(indices[2]);

                // Triangulate remaining indices (pray it's convex)
                // 0, 1, 2
                // 0, 2, 3
                // 0, 3, 4
                // etc..
                for (int k = 3; k < indices.size(); ++k)
                {
                    tmp.push_back(indices[0]);
                    tmp.push_back(indices[k - 1]);
                    tmp.push_back(indices[k]);
                }

                for (int k = 0; k < tmp.size(); ++k)
                {
                    // Convert to usable index
                    int index = tmp[k];
                    if (index < 0)
                    {
                        index *= -1;
                        --index;
                    }

                    data.Vertices.push_back({
                        nodeVerts[3 * index + 0],
                        nodeVerts[3 * index + 1],
                        nodeVerts[3 * index + 2],
                    });

                    if (hasNorms)
                    {
                        data.Normals.push_back({
                            nodeNorms[3 * index + 0],
                            nodeNorms[3 * index + 1],
                            nodeNorms[3 * index + 2],
                        });
                    }
                }
                indices.clear();
            }
        }

        mesh->AddData(data);
    }

    DuskBenchEnd("Mesh::LoadFromFBX");
    return mesh->FinishLoad();
}

} // namespace dusk
