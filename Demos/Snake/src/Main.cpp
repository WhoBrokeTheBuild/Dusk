#include <dusk/Dusk.hpp>

#include <memory>

int main(int argc, char** argv) {
    dusk::SetAssetPath(DUSK_ASSET_PATH);
    dusk::App app(argc, argv);

    auto scene = std::make_unique<dusk::Scene>();
    scene->LoadFromFile("models/Bounds.glb");

    //auto camera = std::make_unique<dusk::DebugCamera>();
    //app.GetRenderContext().CurrentCamera = camera.get();
    //camera->SetMode(dusk::Camera::Mode::Orthographic);
    //camera->SetPosition({ 0.f, 320.f, 0.f });
    //camera->SetLookAt({ 0.f, 0.f, 0.f });
    //scene->AddActor(std::move(camera));

    scene->Print();

    app.SetActiveScene(app.AddScene(std::move(scene)));
    app.Start();   
    return 0;
}