#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/InputDriver.hpp>

namespace Dusk {

GraphicsDriver * GraphicsDriver::_Instance = nullptr;

DUSK_CORE_API
GraphicsDriver::GraphicsDriver()
{
    _Instance = this;
}

DUSK_CORE_API
GraphicsDriver::~GraphicsDriver()
{

    _Instance = nullptr;
}

DUSK_CORE_API
void GraphicsDriver::InitializeUpdateContext()
{
    if (!_updateContext) {
        _updateContext.reset(New UpdateContext());
    }

    _updateContext->SetTargetFPS(60.0f);
    _updateContext->ResetTime();
}

DUSK_CORE_API
void GraphicsDriver::InitializeRenderContext()
{
    if (!_renderContext) {
        _renderContext.reset(New RenderContext());
    }

    auto globals = _renderContext->GetShaderGlobals();
    globals->Resolution = GetWindowSize();
    globals->Mouse = { 0, 0 };
    globals->FrameCount = 0;
    globals->TotalTime = 0;
    globals->FrameSpeedRatio = 0.0f;

    auto transform = _renderContext->GetShaderTransform();
    transform->Model = mat4(1.0f);
    transform->View = mat4(1.0f);
    transform->Projection = mat4(1.0f);
    transform->MVP = mat4(1.0f);
}

DUSK_CORE_API
bool GraphicsDriver::InitializeConstantBuffers()
{
    bool result;

    _shaderGlobalsBuffer = CreateBuffer();
    result = _shaderGlobalsBuffer->Initialize(
        sizeof(ShaderGlobals),
        nullptr,
        BufferUsage::Constant,
        MemoryUsage::UploadOften
    );

    return result;
}

DUSK_CORE_API
bool GraphicsDriver::InitializeDefaults()
{
    uint8_t pixel[4] = { 255, 255, 255, 255 };
    std::unique_ptr<TextureData> textureData(New ConstantTextureData(
        pixel, uvec2(1, 1), 4, TextureDataType::UnsignedByte
    ));

    _defaultTexture = CreateTexture();
    if (!_defaultTexture->Load(textureData.get(), Texture::Options())) {
        LogError(DUSK_ANCHOR, "Failed to create default texture");
        return false;
    }

    _defaultMaterial = CreateMaterial();
    _defaultMaterial->Initialize();

    auto shader = CreateShader();
    if (!shader->LoadFromFiles({
        "Dusk/Default.vert",
        "Dusk/Default.frag",
    })) {
        LogError(DUSK_ANCHOR, "Failed to create default shader");
        return false;
    }

    _defaultPipeline = CreatePipeline(shader);


    std::vector<std::string> attributeShaders = {
        "AttributeColor",
        "AttributeNormal",
        "AttributeTangent",
        "AttributeTexCoord1",
        "AttributeTexCoord2",
    };

    for (const auto& name : attributeShaders) {
        auto shader = CreateShader();
        shader->LoadFromFiles({
            "Dusk/Debug/" + name + ".vert",
            "Dusk/Debug/Attribute.frag",
        });

        _debugShaderMap[name] = shader;
    }

    std::vector<std::string> materialShaders = {
        "MaterialBaseColorFactor",
        "MaterialBaseColorMap",
        "MaterialBaseColor",
        "MaterialNormalScale",
        "MaterialNormalMap",
        "MaterialNormal",
        "MaterialMetallicFactor",
        "MaterialMetallicMap",
        "MaterialMetallic",
        "MaterialRoughnessFactor",
        "MaterialRoughnessMap",
        "MaterialRoughness",
        "MaterialEmissiveFactor",
        "MaterialEmissiveMap",
        "MaterialEmissive",
        "MaterialOcclusionStrength",
        "MaterialOcclusionMap",
        "MaterialOcclusion",
    };

    for (const auto& name : materialShaders) {
        auto shader = CreateShader();
        shader->LoadFromFiles({
            "Dusk/Debug/Material.vert",
            "Dusk/Debug/" + name + ".frag",
        });

        _debugShaderMap[name] = shader;
    }

    std::vector<std::string> customShaders = {
        "Normal"
    };

    for (const auto& name : customShaders) {
        auto shader = CreateShader();
        shader->LoadFromFiles({
            "Dusk/Debug/" + name + ".vert",
            "Dusk/Debug/" + name + ".frag",
        });

        _debugShaderMap[name] = shader;
    }

    return true;
}

void GraphicsDriver::SetWindowTitle(const string& title)
{
    _windowTitle = title;
    UpdateWindowTitle(title);
}

void GraphicsDriver::SetWindowSize(const ivec2& size)
{
    _windowSize = size;
    UpdateWindowSize(size);
}

void GraphicsDriver::SetBackbufferCount(unsigned backbufferCount)
{
    _backbufferCount = backbufferCount;
}

void GraphicsDriver::SetClearColor(const vec4& color)
{
    _clearColor = color;
}

DUSK_CORE_API
void GraphicsDriver::Render()
{
    UpdateContext * updateCtx = GetUpdateContext();

    // Update FPS
    _fpsUpdateElapsedTime += updateCtx->GetPreviousFrameDuration();
    if (_fpsUpdateFrameCount >= updateCtx->GetTargetFPS()) {
        float currentFPS = (float)_fpsUpdateFrameCount / (float)_fpsUpdateElapsedTime.count();
        currentFPS *= 1000000.0f; // Microseconds -> Seconds

        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.2f FPS", currentFPS);
        UpdateWindowTitle(_windowTitle + " - " + buffer); // FIX

        _fpsUpdateElapsedTime = 0ms;
        _fpsUpdateFrameCount = 0;
    }

    ++_fpsUpdateFrameCount;
    
    auto input = GetInputDriver();

    auto globals = _renderContext->GetShaderGlobals();
    globals->Resolution = GetWindowSize();
    globals->Mouse = input->GetMouseCoordinates();
    ++globals->FrameCount;
    globals->TotalTime = (float)_updateContext->GetTotalDuration().count() / 1000.0f;
    globals->FrameSpeedRatio = _updateContext->GetFrameSpeedRatio();

    _shaderGlobalsBuffer->WriteTo(0, sizeof(ShaderGlobals), reinterpret_cast<uint8_t *>(globals));
}

DUSK_CORE_API
UpdateContext * GraphicsDriver::GetUpdateContext()
{
    return _updateContext.get();
}

DUSK_CORE_API
RenderContext * GraphicsDriver::GetRenderContext()
{
    return _renderContext.get();
}

DUSK_CORE_API
void GraphicsDriver::EnableDebugShader(const std::string& name)
{
    DisableDebugShader();

    auto it = _debugShaderMap.find(name);
    if (it != _debugShaderMap.end()) {
        _activeDebugShader = _debugShaderMap[name];
    }
}

DUSK_CORE_API
void GraphicsDriver::DisableDebugShader()
{
    _activeDebugShader = nullptr;
}

DUSK_CORE_API
Shader * GraphicsDriver::GetActiveDebugShader()
{
    return _activeDebugShader.get();
}

DUSK_CORE_API
PyObject * WindowResizedEventData::GetPyObject() const
{
    PyObject * dict = PyDict_New();

    PyObject * size = PyTuple_New(2);
    PyTuple_SET_ITEM(size, 0, PyLong_FromLong(Size.x));
    PyTuple_SET_ITEM(size, 1, PyLong_FromLong(Size.y));
    PyDict_SetItemString(dict, "Size", size);

    return dict;
}

} // namespace Dusk