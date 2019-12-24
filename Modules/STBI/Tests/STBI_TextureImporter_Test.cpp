#include <Dusk/STBI/TextureImporter.hpp>

#include <gtest/gtest.h>

#include "test.h"

using namespace Dusk::STBI;

static TextureImporter _TextureImporter;

TEST(LoadFromFile, PNG) {
    auto data = _TextureImporter.LoadFromFile("test.png");
    EXPECT_FALSE(data == nullptr);
}

TEST(LoadFromFile, JPEG) {
    auto data = _TextureImporter.LoadFromFile("test.jpg");
    EXPECT_FALSE(data == nullptr);
}

TEST(LoadFromFile, BMP) {
    auto data = _TextureImporter.LoadFromFile("test.bmp");
    EXPECT_FALSE(data == nullptr);
}

TEST(LoadFromFile, TGA) {
    auto data = _TextureImporter.LoadFromFile("test.tga");
    EXPECT_FALSE(data == nullptr);
}

TEST(LoadFromFile, NoFilename) {
    auto data = _TextureImporter.LoadFromFile("");
    EXPECT_TRUE(data == nullptr);
}

TEST(LoadFromFile, FileNotFound) {
    auto data = _TextureImporter.LoadFromFile("asdf.png");
    EXPECT_TRUE(data == nullptr);
}

TEST(LoadFromMemory, PNG) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_png, ___Modules_STBI_Tests_test_png_len);
    EXPECT_FALSE(data == nullptr);
}

TEST(LoadFromMemory, JPEG) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_jpg, ___Modules_STBI_Tests_test_jpg_len);
    EXPECT_FALSE(data == nullptr);
}

TEST(LoadFromMemory, BMP) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_bmp, ___Modules_STBI_Tests_test_bmp_len);
    EXPECT_FALSE(data == nullptr);
}

TEST(LoadFromMemory, TGA) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_tga, ___Modules_STBI_Tests_test_tga_len);
    EXPECT_FALSE(data == nullptr);
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