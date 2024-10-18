
#include <Dusk/Dusk.hpp>

int main(int argc, char * argv[])
{
    dusk::ParseAssetPathList(DUSK_ASSET_PATH);
    
    try {

        dusk::Init();

        {
            bool loaded = true;

            dusk::VulkanShader::Pointer shader(new dusk::VulkanShader());
            loaded = shader->LoadFromFiles({
                "test.vert.spv",
                "test.frag.spv",
            });

            if (!loaded) {
                return 1;
            }

            auto model = dusk::Model();

            // From https://vulkan-tutorial.com/
            loaded = model.LoadFromFile("viking_room.glb");

            // Configure with -DGLTF_SAMPLE_ASSETS_PATH=path/to/glTF-Sample-Assets/
            // loaded = model.LoadFromFile("Models/DamagedHelmet/glTF-Binary/DamagedHelmet.glb");
            // loaded = model.LoadFromFile("Models/BoxVertexColors/glTF-Binary/BoxVertexColors.glb");
            // loaded = model.LoadFromFile("Models/BoxVertexColors/glTF-Embedded/BoxVertexColors.gltf");
            // loaded = model.LoadFromFile("Models/BoomBox/glTF/BoomBox.gltf");
            // loaded = model.LoadFromFile("Models/BoomBox/glTF-Binary/BoomBox.glb");

            if (!loaded) {
                return 1;
            }

            dusk::VulkanPipeline::Pointer pipeline(new dusk::VulkanPipeline());
            pipeline->SetFrontFace(vk::FrontFace::eCounterClockwise);
            pipeline->SetCullMode(vk::CullModeFlagBits::eBack);
            pipeline->Create(shader);

            // glm::vec3 light = glm::vec3(1.0f);
            float angle = 0.0f;

            dusk::Graphics::SetRenderCallback(
                [&](vk::CommandBuffer commandBuffer) {

                    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->GetVkPipeline());

                    auto windowSize = dusk::Graphics::GetWindowSize();
                    float aspect = float(windowSize.width) / float(windowSize.height);

                    glm::mat4 view = glm::lookAt(
                        glm::vec3(2.0f),
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f)
                    );

                    glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspect, 0.01f, 1000.0f);
                    projection[1][1] *= -1;

                    model.Transform.Position = { 0, 0, 0 };
                    model.Transform.Orientation = glm::quat(glm::vec3(0.0f, angle, 0.0f));
                    model.Transform.Scale = { 1, 1, 1 };
                    model.Update(view, projection);

                    model.GenerateCommands(commandBuffer);

                    angle += 0.005f;
                }
            );

            dusk::Run();
        }

        dusk::Term();

    } catch (std::runtime_error * e) {
        printf("Exception: %s\n", e->what());
    }

    return 0;
}