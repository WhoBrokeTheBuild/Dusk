#include <dusk/Dusk.hpp>

#include <memory>

int main(int argc, char** argv) {
    dusk::SetAssetPath(DUSK_ASSET_PATH);
    dusk::App app(argc, argv);
    
    auto scene = std::make_unique<dusk::Scene>();
    scene->LoadFromFile("models/TestScene.glb");
    //scene->LoadFromFile("models/DamagedHelmet.glb");
    //scene->LoadFromFile("models/SciFiHelmet/SciFiHelmet.gltf");
    //scene->LoadFromFile("models/BoomBox.glb");
    //scene->LoadFromFile("models/WaterBottle.glb");

    app.SetActiveScene(app.AddScene(move(scene)));

    app.Start();
    return 0;
}
