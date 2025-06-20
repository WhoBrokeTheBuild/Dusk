
#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>

int main(int argc, char * argv[])
{
    dusk::ParseAssetPathList(DUSK_ASSET_PATH);
    
    try {

        dusk::Init();

        {
            bool loaded = true;

            // dusk::VulkanTexture::Pointer texture(new dusk::VulkanTexture());
            // texture->LoadFromFile("viking_room.png");
            
            // dusk::JSON::Object asset;
            // texture->Serialize(asset);
            // dusk::Log(DUSK_ANCHOR, "{}", asset.dump(4));

            // texture->Destroy();
            // texture->Deserialize(asset);


            dusk::VulkanShader::Pointer shader(new dusk::VulkanShader());
            loaded = shader->LoadFromFiles({
                "test.vert.spv",
                "test.frag.spv",
                // "Dusk/Debug.vert.spv",
                // "Dusk/Debug.frag.spv",
            });

            if (!loaded) {
                return 1;
            }

            auto model = dusk::Model();

            bool rotating = true;
            glm::quat initialOrientation(glm::vec3(0.0f));

            // From https://vulkan-tutorial.com/
            loaded = model.LoadFromFile("viking_room.glb");

            // Configure with -DGLTF_SAMPLE_ASSETS_PATH=path/to/glTF-Sample-Assets/
            // loaded = model.LoadFromFile("Models/DamagedHelmet/glTF-Binary/DamagedHelmet.glb"); rotation = glm::quat(glm::vec3(dusk::ToRadians(90.0f), 0.0f, 0.0f));
            // loaded = model.LoadFromFile("Models/BoxVertexColors/glTF-Binary/BoxVertexColors.glb");
            // loaded = model.LoadFromFile("Models/BoxVertexColors/glTF-Embedded/BoxVertexColors.gltf");
            // loaded = model.LoadFromFile("Models/BoomBox/glTF/BoomBox.gltf");
            // loaded = model.LoadFromFile("Models/BoomBox/glTF-Binary/BoomBox.glb");

            auto bounds = model.GetBounds();
            
            auto axisModel = dusk::Model();
            dusk::Mesh::Pointer axisMesh(new dusk::Mesh());
            axisMesh->AddWireAxis(
                { 0.0f, 0.0f, 0.0f },
                std::abs(std::max({ bounds.GetSize().x, bounds.GetSize().y, bounds.GetSize().z })) * 0.1f
            );
            dusk::List<dusk::Mesh::Pointer> axisMeshList;
            axisMeshList.push_back(std::move(axisMesh));
            axisModel.Create(std::move(axisMeshList));

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
            glm::quat orientation = initialOrientation;

            dusk::Graphics::Globals.CameraPosition = camera;
            dusk::Graphics::Globals.CameraDirection = glm::normalize(-camera);
            dusk::Graphics::Globals.LightPosition = camera;

            bool mouseHeld = false;
            dusk::Input::SetMouseCallback(
                [&](SDL_Event * event) {
                    if (mouseHeld) {
                        if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT) {
                            mouseHeld = false;
                            SDL_SetWindowRelativeMouseMode(dusk::Graphics::Window, false);
                        }
                        else if (event->type == SDL_EVENT_MOUSE_MOTION) {
                            orientation *= glm::quat(glm::vec3(
                                event->motion.yrel * -0.01f,
                                event->motion.xrel * 0.01f,
                                0.0f
                            ));
                        }
                    }
                    else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT) {
                        mouseHeld = true;
                        rotating = false;
                        SDL_SetWindowRelativeMouseMode(dusk::Graphics::Window, true);
                    }
                }
            );

            dusk::Graphics::SetRenderCallback(
                [&](VkCommandBuffer commandBuffer) {

                    pipeline->GenerateCommands(commandBuffer);

                    auto windowSize = dusk::Graphics::GetWindowSize();
                    float aspect = float(windowSize.width) / float(windowSize.height);

                    glm::mat4 view = glm::lookAt(
                        camera,
                        center,
                        glm::vec3(0.0f, 1.0f, 0.0f)
                    );

                    glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspect, 0.001f, 1000.0f);
                    // projection[1][1] *= -1;

                    if (rotating) {
                        orientation *= glm::quat(glm::vec3(0.0f, 0.002f, 0.0f));
                    }

                    model.Transform.Position = { 0, 0, 0 };
                    model.Transform.Orientation = orientation;
                    model.Transform.Scale = { 1, 1, 1 };
                    model.Update(view, projection);

                    model.GenerateCommands(commandBuffer);

                    dusk::Graphics::DebugPipeline->GenerateCommands(commandBuffer);
                    
                    axisModel.Transform.Position = glm::vec4(bounds.Upper, 1.0f);
                    axisModel.Transform.Orientation = model.Transform.Orientation;
                    axisModel.Update(view, projection);
                    axisModel.GenerateCommands(commandBuffer);
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