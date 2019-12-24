#include <Dusk/STBI/TextureImporter.hpp>

#include <gtest/gtest.h>

#include "test.h"

using namespace Dusk;
using namespace Dusk::STBI;

static TextureImporter _TextureImporter;

TEST(LoadFromFile, PNG_RGB) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.png");
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 3);
}

TEST(LoadFromFile, PNG_RGBA) {
    auto data = _TextureImporter.LoadFromFile("test_rgba.png");
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 4);
}

TEST(LoadFromFile, PNG_Grey) {
    auto data = _TextureImporter.LoadFromFile("test_grey.png");
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 1);
}

TEST(LoadFromFile, JPEG_RGB) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.jpg");
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 3);
}

TEST(LoadFromFile, JPEG_Grey) {
    auto data = _TextureImporter.LoadFromFile("test_grey.jpg");
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 1);
}

TEST(LoadFromFile, BMP_RGB) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.bmp");
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 3);
}

TEST(LoadFromFile, BMP_Grey) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.bmp");
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 3);
}

TEST(LoadFromFile, TGA) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.tga");
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 3);
}
TEST(LoadFromFile, GIF_RGB) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.gif");
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 4);
}

TEST(LoadFromFile, GIF_RGBA) {
    auto data = _TextureImporter.LoadFromFile("test_rgba.gif");
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 4);
}

TEST(LoadFromFile, GIF_Grey) {
    auto data = _TextureImporter.LoadFromFile("test_grey.gif");
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 4);
}

TEST(LoadFromFile, NoFilename) {
    auto data = _TextureImporter.LoadFromFile("");
    EXPECT_TRUE(data == nullptr);
}

TEST(LoadFromFile, FileNotFound) {
    auto data = _TextureImporter.LoadFromFile("asdf.png");
    EXPECT_TRUE(data == nullptr);
}

TEST(LoadFromMemory, PNG_RGB) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_png, ___Modules_STBI_Tests_test_png_len);
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 3);
}

TEST(LoadFromMemory, JPEG_RGB) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_jpg, ___Modules_STBI_Tests_test_jpg_len);
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 3);
}

TEST(LoadFromMemory, BMP_RGB) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_bmp, ___Modules_STBI_Tests_test_bmp_len);
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 3);
}

TEST(LoadFromMemory, TGA_RGB) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_tga, ___Modules_STBI_Tests_test_tga_len);
    EXPECT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), uvec2(32, 32));
    EXPECT_EQ(data->GetComponents(), 3);
}

TEST(LoadFromMemory, NullInput) {
    auto data = _TextureImporter.LoadFromMemory(nullptr, 0);
    EXPECT_TRUE(data == nullptr);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}