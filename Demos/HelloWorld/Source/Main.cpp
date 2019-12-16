#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Graphics/Model.hpp>
#include <Dusk/Scene/Camera.hpp>

int main(int argc, char** argv) 
{
    Dusk::SetApplicationName("HelloWorld");
    Dusk::SetApplicationVersion(1, 0, 0);

    Dusk::Initialize(argc, argv);
    
    if (!Dusk::LoadModuleArray({
        "DuskSTBI",
        "DuskTinyOBJ",
        "DuskOpenGL",
        // "DuskVulkan",
    })) {
        return 1;
    }

    Dusk::Camera camera;
    camera.SetPosition({ 3, 3, 3 });
    camera.SetLookAt({ 0, 0, 0 });
    glm::mat4 mvp = camera.GetProjection() * camera.GetView();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%f, ", mvp[i][j]);
        }
    }

    {
        auto gfx = Dusk::GetGraphicsDriver();

        auto model = Dusk::Model();
        model.LoadFromFile("Models/crate/crate.obj");

        auto shader = gfx->CreateShader();
        shader->LoadFromFiles({
            "Shaders/flat.vert.spv",
            "Shaders/flat.frag.spv",
            // "Shaders/flat.vert.glsl",
            // "Shaders/flat.frag.glsl",
        });

        auto tex = gfx->CreateTexture();
        tex->LoadFromFile("Models/crate/crate.png");

        gfx->SetWindowSize({ 1024, 768 });

        Dusk::SetRunning(true);
        while (Dusk::IsRunning()) {
            gfx->ProcessEvents();

            shader->Bind();
            model.Render();

            gfx->SwapBuffers();
        }
    }

    Dusk::Terminate();

    return 0;
}
