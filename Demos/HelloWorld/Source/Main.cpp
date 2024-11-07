
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

            glm::quat rotation(glm::vec3(0.0f));

            // From https://vulkan-tutorial.com/
            loaded = model.LoadFromFile("viking_room.glb");

            // Configure with -DGLTF_SAMPLE_ASSETS_PATH=path/to/glTF-Sample-Assets/
            // loaded = model.LoadFromFile("Models/DamagedHelmet/glTF-Binary/DamagedHelmet.glb"); rotation = glm::quat(glm::vec3(dusk::ToRadians(90.0f), 0.0f, 0.0f));
            // loaded = model.LoadFromFile("Models/BoxVertexColors/glTF-Binary/BoxVertexColors.glb");
            // loaded = model.LoadFromFile("Models/BoxVertexColors/glTF-Embedded/BoxVertexColors.gltf");
            // loaded = model.LoadFromFile("Models/BoomBox/glTF/BoomBox.gltf");
            // loaded = model.LoadFromFile("Models/BoomBox/glTF-Binary/BoomBox.glb");

            auto bounds = model.GetBounds();
            
            auto boxModel = dusk::Model();
            dusk::Mesh::Pointer boxMesh(new dusk::Mesh());
            boxMesh->AddWireCube(
                bounds.GetCenter(),
                bounds.GetSize(),
                dusk::Color::Black
            );
            dusk::List<dusk::Mesh::Pointer> boxMeshList;
            boxMeshList.push_back(std::move(boxMesh));
            boxModel.Create(std::move(boxMeshList));

            if (!loaded) {
                return 1;
            }

            dusk::VulkanPipeline::Pointer pipeline(new dusk::VulkanPipeline());
            pipeline->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
            pipeline->SetCullMode(VK_CULL_MODE_BACK_BIT);
            pipeline->Create(shader);

            dusk::VulkanPipeline::Pointer debugPipeline(new dusk::VulkanPipeline());
            debugPipeline->SetCullMode(VK_CULL_MODE_NONE);
            debugPipeline->SetPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
            debugPipeline->Create(shader);

            glm::vec3 camera(bounds.Upper * 2.0f);
            glm::vec3 center(bounds.GetCenter());
            // glm::vec3 light = glm::vec3(1.0f);
            float angle = 0.0f;

            dusk::Graphics::Globals.CameraPosition = camera;
            dusk::Graphics::Globals.CameraDirection = glm::normalize(-camera);
            dusk::Graphics::Globals.LightPosition = camera;

            dusk::Graphics::SetRenderCallback(
                [&](VkCommandBuffer commandBuffer) {

                    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetVkPipeline());

                    auto windowSize = dusk::Graphics::GetWindowSize();
                    float aspect = float(windowSize.width) / float(windowSize.height);

                    glm::mat4 view = glm::lookAt(
                        camera,
                        center,
                        glm::vec3(0.0f, 1.0f, 0.0f)
                    );

                    glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspect, 0.001f, 1000.0f);
                    // projection[1][1] *= -1;

                    model.Transform.Position = { 0, 0, 0 };
                    model.Transform.Orientation = glm::quat(glm::vec3(0.0f, angle, 0.0f)) * rotation;
                    // model.Transform.Orientation = glm::quat(glm::vec3(angle)) * rotation;
                    model.Transform.Scale = { 1, 1, 1 };
                    model.Update(view, projection);

                    model.GenerateCommands(commandBuffer);

                    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, debugPipeline->GetVkPipeline());

                    boxModel.Transform = model.Transform;
                    boxModel.Update(view, projection);
                    boxModel.GenerateCommands(commandBuffer);

                    angle -= 0.002f;
                }
            );

            dusk::Run();
        }

        dusk::Term();

    } catch (std::runtime_error& e) {
        printf("Exception: %s\n", e.what());
    }

    return 0;
}