#include <dusk/Dusk.hpp>

#include <dusk/core/glTF2.hpp>

#include <memory>

int main(int argc, char** argv) {
    dusk::SetAssetPath(DUSK_ASSET_PATH);
    dusk::App app(argc, argv);
    
    dusk::glTF2::LoadSceneFromFile("models/SciFiHelmet/SciFiHelmet.gltf");

    /*
    auto scene = std::make_unique<dusk::Scene>();
    scene->LoadFromFile("models/TestScene.glb");
    //scene->LoadFromFile("models/DamagedHelmet.glb");
    //scene->LoadFromFile("models/SciFiHelmet/SciFiHelmet.gltf");
    //scene->LoadFromFile("models/BoomBox.glb");
    //scene->LoadFromFile("models/WaterBottle.glb");

    auto camera = std::make_unique<dusk::DebugCamera>();
    app.GetRenderContext().CurrentCamera = camera.get();
    camera->SetPosition({ 20.f, 20.f, 20.f });
    camera->SetLookAt({ 0.f, 0.f, 0.f });
    scene->AddActor(std::move(camera));

    app.SetActiveScene(app.AddScene(std::move(scene)));

    app.Start();
    */
    return 0;
}
