#include <dusk/Dusk.hpp>

#include <memory>

int main(int argc, char** argv) {
    dusk::SetAssetPath(DUSK_ASSET_PATH);
    dusk::App app(argc, argv);
    
    auto scene = std::make_unique<dusk::Scene>();
    //scene->LoadFromFile("models/TestScene.glb");
    scene->LoadFromFile("models/DamagedHelmet.glb");
    //scene->LoadFromFile("models/SciFiHelmet/SciFiHelmet.gltf");
    //scene->LoadFromFile("models/BoomBox.glb");
    //scene->LoadFromFile("models/WaterBottle.glb");

    auto camera = std::make_unique<dusk::DebugCamera>();
    app.GetRenderContext().CurrentCamera = camera.get();
    camera->SetPosition({ 5.f, 5.f, 5.f });
    camera->SetLookAt({ 0.f, 0.f, 0.f });
    scene->AddActor(std::move(camera));

    app.SetActiveScene(app.AddScene(move(scene)));

    app.Start();
    return 0;
}
