#include <dusk/Dusk.hpp>

#include <memory>
using std::unique_ptr;
using std::make_unique;

class TestScene : public dusk::Scene
{
public:

    TestScene() 
    {
        _mesh.reset(new dusk::Mesh("models/teapot.fbx"));

        auto app = dusk::App::Inst();

        auto camera = make_unique<dusk::Camera>();
        auto pCamera = camera.get();
        camera->SetAspect(app->GetWindowSize());
        camera->SetPosition({ 3.0f, 3.0f, 3.0f });
        camera->SetLookAt({ 0.0f, 0.0f, 0.0f });
        app->GetRenderContext().CurrentCamera = pCamera;
    }

    virtual void Update(dusk::UpdateContext& ctx) override
    {
        _transform.Model = mat4(1.0f);
        _transform.Model = glm::scale(_transform.Model, vec3(0.01f));
        _transform.Model = glm::rotate(_transform.Model, (float)ctx.TotalTime * 0.001f, vec3(0.0f, 1.0f, 0.0f));
        
        dusk::Scene::Update(ctx);
    }

    virtual void Render(dusk::RenderContext& ctx) override 
    {
        auto camera = ctx.CurrentCamera;

        _transform.View = camera->GetView();
        _transform.Proj = camera->GetProjection();
        _transform.MVP = _transform.Proj * _transform.View * _transform.Model;
        dusk::Shader::SetUniformBufferData("DuskTransformData", &_transform);

        _mesh->Render(ctx);

        dusk::Scene::Render(ctx);
    }
    
private:

    dusk::TransformData _transform;

    unique_ptr<dusk::Mesh> _mesh;

};

int main(int argc, char** argv) {
    dusk::SetAssetPath(DUSK_ASSET_PATH);
    dusk::App app(argc, argv);

    auto shader = make_unique<dusk::Shader>(vector<string>({ "shaders/default/textured.vs.glsl", "shaders/default/textured.fs.glsl" }));
    auto pShader = shader.get();
    app.AddShader(move(shader));
    app.GetRenderContext().CurrentShader = pShader;

    auto scene = unique_ptr<dusk::Scene>(new TestScene());
    auto pScene = scene.get();
    app.AddScene(move(scene));
    app.SetActiveScene(pScene);

    app.Start();
    return 0;
}
