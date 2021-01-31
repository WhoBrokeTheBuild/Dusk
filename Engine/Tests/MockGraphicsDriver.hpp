#ifndef DUSK_MOCK_GRAPHICS_DRIVER_HPP
#define DUSK_MOCK_GRAPHICS_DRIVER_HPP

#include <Dusk/GraphicsDriver.hpp>

#include <gmock/gmock.h>

using namespace Dusk;

class MockTexture : public Texture
{
public:

    MOCK_METHOD2(Load, bool(const std::unique_ptr<TextureData>&, Options));
};

class MockShader : public Shader
{
public:

    MOCK_METHOD1(LoadFromFiles, bool(const std::vector<std::string>&));

};

class MockPipeline : public Pipeline
{
public:

    MOCK_METHOD0(Initialize, bool());
    MOCK_METHOD0(Terminate, void());

};

class MockMesh : public Mesh
{
public:

    MOCK_METHOD1(Render, void(RenderContext*));

};

class MockPrimitive : public Primitive
{
public:

    MOCK_METHOD1(Load, bool(const std::unique_ptr<PrimitiveData>&));

};

class MockGraphicsDriver : public GraphicsDriver
{
public:

    MOCK_METHOD0(Initialize, bool());
    MOCK_METHOD0(Terminate, void());

    MOCK_METHOD1(SetWindowTitle, void(const std::string&));
    MOCK_METHOD0(GetWindowTitle, std::string());

    MOCK_METHOD1(SetWindowSize, void(const ivec2&));
    MOCK_METHOD0(GetWindowSize, ivec2());

    MOCK_METHOD0(ProcessEvents, void());
    MOCK_METHOD0(Render, void());

    std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader) override {
        auto pipeline = std::make_shared<MockPipeline>();
        pipeline->SetShader(shader);
        return pipeline;
    }

    std::shared_ptr<Texture> CreateTexture() override {
        return std::make_shared<MockTexture>();
    }

    std::shared_ptr<Shader> CreateShader() override {
        return std::make_shared<MockShader>();
    }

    std::shared_ptr<Mesh> CreateMesh() override {
        return std::make_shared<MockMesh>();
    }

    std::unique_ptr<Primitive> CreatePrimitive() override {
        return std::make_unique<MockPrimitive>();
    }

    MOCK_METHOD0(GetUpdateContext, UpdateContext*());
    MOCK_METHOD0(GetRenderContext, RenderContext*());
};

#endif // DUSK_MOCK_GRAPHICS_DRIVER_HPP