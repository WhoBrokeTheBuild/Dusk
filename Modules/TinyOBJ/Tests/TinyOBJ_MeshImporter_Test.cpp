#include <Dusk/TinyOBJ/TinyOBJMeshImporter.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/Log.hpp>

#include <gtest/gtest.h>

static Dusk::TinyOBJ::TinyOBJMeshImporter _MeshImporter;

TEST(LoadFromFile, Cube) {
    auto data = _MeshImporter.LoadFromFile("test_cube.obj");
    ASSERT_EQ(data.size(), 1);

    const auto& cube = data[0];
    EXPECT_EQ(cube->GetTopology(), Dusk::PrimitiveTopology::TriangleList);
    EXPECT_EQ(cube->GetVertexList().size(), 36);
    EXPECT_TRUE(cube->GetIndexList().empty());

    // TODO: Spot check vertices
    // TODO: Material
}

TEST(LoadFromFile, Scene) {
    auto data = _MeshImporter.LoadFromFile("test_scene.obj");
    ASSERT_EQ(data.size(), 2);

    const auto& icosphere = data[0];
    const auto& cylinder = data[1];

    EXPECT_EQ(icosphere->GetTopology(), Dusk::PrimitiveTopology::TriangleList);
    EXPECT_EQ(icosphere->GetVertexList().size(), 240);
    EXPECT_TRUE(icosphere->GetIndexList().empty());

    EXPECT_EQ(cylinder->GetTopology(), Dusk::PrimitiveTopology::TriangleList);
    EXPECT_EQ(cylinder->GetVertexList().size(), 372);
    EXPECT_TRUE(cylinder->GetIndexList().empty());

    // TODO: Spot check vertices
    // TODO: Material
}

TEST(LoadFromFile, NoFilename) {
    auto data = _MeshImporter.LoadFromFile("");
    EXPECT_TRUE(data.empty());
}

TEST(LoadFromFile, FileNotFound) {
    auto data = _MeshImporter.LoadFromFile("asdf.obj");
    EXPECT_TRUE(data.empty());
}

int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
