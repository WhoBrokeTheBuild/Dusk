#include <dusk/Dusk.hpp>

#include <memory>

using namespace std;
using namespace glm;

class TestScene : public dusk::Scene
{
public:

    TestScene() 
    {
        _model.reset(new dusk::Model());
		//_model->AddMesh(std::make_shared<dusk::Mesh>("models/SciFiHelmet/SciFiHelmet.gltf"));
		//_model->AddMesh(std::make_shared<dusk::Mesh>("models/DamagedHelmet.glb"));
		_model->AddMesh(std::make_shared<dusk::Mesh>("models/WaterBottle.glb"));

        auto app = dusk::App::Inst();

        auto bounds = _model->GetBounds();
        float max = std::max({ bounds.Max.x, bounds.Max.y, bounds.Max.z });
        _camera.reset(new dusk::Camera());
        _camera->SetAspect(app->GetWindowSize());
        _camera->SetPosition(glm::vec3(max) * 2.0f);
        _camera->SetLookAt(bounds.GetSize() * 0.5f);
        app->GetRenderContext().CurrentCamera = _camera.get();
    }

    virtual void Update(dusk::UpdateContext& ctx) override
    {
        _model->SetRotation(glm::vec3(0.0f, (float)ctx.TotalTime * 0.001f, 0.0f));
        
        dusk::Scene::Update(ctx);
    }

    virtual void Render(dusk::RenderContext& ctx) override 
    {
        auto camera = ctx.CurrentCamera;

        _model->Render(ctx);

        dusk::Scene::Render(ctx);
    }
    
private:

    unique_ptr<dusk::Camera> _camera;
    unique_ptr<dusk::Model> _model;

};

int main(int argc, char** argv) {
    dusk::SetAssetPath(DUSK_ASSET_PATH);
    dusk::App app(argc, argv);

    auto shader = std::make_unique<dusk::Shader>(vector<string>({ "shaders/default/textured.vs.glsl", "shaders/default/textured.fs.glsl" }));
    auto pShader = shader.get();
    app.AddShader(move(shader));
    app.GetRenderContext().CurrentShader = pShader;

    auto scene = std::unique_ptr<dusk::Scene>(new TestScene());
    auto pScene = scene.get();
    app.AddScene(move(scene));
    app.SetActiveScene(pScene);

    app.Start();
    return 0;
}
