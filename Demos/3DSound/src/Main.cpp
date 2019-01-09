#include <dusk/Dusk.hpp>

#include <memory>

int main(int argc, char** argv) {
    dusk::SetAssetPath(DUSK_ASSET_PATH);
    dusk::App app(argc, argv);
    
    auto scene = std::make_unique<dusk::Scene>();

    auto camera = std::make_unique<dusk::DebugCamera>();
    app.GetRenderContext().CurrentCamera = camera.get();
    camera->SetPosition({ 20.f, 20.f, 20.f });
    camera->SetLookAt({ 0.f, 0.f, 0.f });
    camera->AddComponent(std::make_unique<dusk::ListenerComponent>());
    scene->AddActor(std::move(camera));

    {
        auto actor = std::make_unique<dusk::Actor>();
        actor->SetPosition({ 10.f, 0.f, 0.f });
        actor->SetScale({ 0.3f, 0.3f, 0.3f });

        auto meshComp = std::make_unique<dusk::MeshComponent>();
        meshComp->AddMesh(std::make_shared<dusk::Mesh>("models/default/cube.glb"));
        actor->AddComponent(std::move(meshComp));

        auto soundComp = std::make_unique<dusk::SoundComponent>();
        soundComp->SetSound(std::make_shared<dusk::Sound>("music/commonGround.ogg"));
        actor->AddComponent(std::move(soundComp));

        scene->AddActor(std::move(actor));
    }

    {
        auto actor = std::make_unique<dusk::Actor>();
        actor->SetPosition({ 0.f, 0.f, 10.f });
        actor->SetScale({ 0.3f, 0.3f, 0.3f });

        auto meshComp = std::make_unique<dusk::MeshComponent>();
        meshComp->AddMesh(std::make_shared<dusk::Mesh>("models/default/uvsphere.glb"));
        actor->AddComponent(std::move(meshComp));

        auto soundComp = std::make_unique<dusk::SoundComponent>();
        soundComp->SetSound(std::make_shared<dusk::Sound>("music/nightWalk.ogg"));
        actor->AddComponent(std::move(soundComp));
        
        scene->AddActor(std::move(actor));
    }

    app.SetActiveScene(app.AddScene(move(scene)));

    app.Start();
    return 0;
}
