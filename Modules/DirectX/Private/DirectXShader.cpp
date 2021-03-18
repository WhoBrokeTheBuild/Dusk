#include <Dusk/DirectX/DirectXShader.hpp>

#include <Dusk/DirectX/DirectXGraphicsDriver.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>

#include <fstream>
#include <sstream>

namespace Dusk {

struct IncludeHandler : public IDxcIncludeHandler
{
public:

    IncludeHandler(IDxcLibrary * library)
        : _library(library)
    { }

    HRESULT QueryInterface(REFIID iid, void ** ppvObject) override {
        return S_OK;
    }

    ULONG AddRef() override {
        return 0;
    }

    ULONG Release() override {
        return 0;
    }

    HRESULT LoadSource(LPCWSTR wfilename, IDxcBlob ** source)
    {
        const auto& assetPathList = GetAssetPathList();

        string filename = ConvertWideStringToUTF8(wfilename);
        
        std::ifstream file;

        for (const auto& path : assetPathList) {
            Path fullPath = path / "Shaders" / filename;
            LogVerbose(DUSK_ANCHOR, "Checking shader include '{}'", fullPath);
            
            file.open(fullPath);
            if (file.is_open()) {
                break;
            }
        }

        if (!file.is_open()) {
            return E_INVALIDARG;
        }
        
        std::vector<uint8_t> data(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );

        return _library->CreateBlobWithEncodingOnHeapCopy(data.data(), data.size(), CP_UTF8, (IDxcBlobEncoding **)source);
    }

private:
    
    IDxcLibrary * _library;

}; // struct IncludeHandler

DUSK_DIRECTX_API
bool DirectXShader::LoadFromFiles(const std::vector<string>& filenames)
{
    DuskBenchmarkStart();

    for (const auto& filename : filenames) {
        string_view ext = GetExtension(filename);

        if (ext == "cso") {
            if (LoadCSO(filename)) {
                break;
            }
            else {
                LogError(DUSK_ANCHOR, "Failed to load shader '{}'", filename);
                return false;
            }
        }
        else if (ext == "hlsl") {
            if (LoadHLSL(filename)) {
                break;
            }
            else {
                LogError(DUSK_ANCHOR, "Failed to load shader '{}'", filename);
                return false;
            }
        }
        else {
            if (LoadCSO(filename + ".cso") || LoadHLSL(filename + ".hlsl")) {
                break;
            }
            else {
                LogError(DUSK_ANCHOR, "Failed to load shader '{}', tried extensions '.cso' and '.hlsl'", filename);
                return false;
            }
        }
    }
        
    DuskBenchmarkEnd();
    return true;
}

DUSK_DIRECTX_API
bool DirectXShader::LoadCSO(const Path& path, bool useAssetPath)
{
    LogVerbose(DUSK_ANCHOR, "Looking for Compiled Shader Object '{}'", filename);

    auto stage = GetShaderStageFromFilename(path);
    if (!stage) {
        return false;
    }

    std::ifstream file;

    if (useAssetPath) {
        for (const auto& assetPath : GetAssetPathList()) {
            Path fullPath = assetPath / "Shaders" / path;
            LogVerbose(DUSK_ANCHOR, "Checking '{}'", fullPath);
            
            file.open(fullPath, std::ios::binary | std::ios::ate);
            if (file.is_open()) {
                break;
            }
        }
    else {
        file.open(path, std::ios::binary | std::ios::ate);
    }

    if (!file.is_open()) {
        return false;
    }

    file.unsetf(std::ios::skipws);

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    LogVerbose(DUSK_ANCHOR, "Loading Compiled Shader Object '{}'", filename);

    auto begin = std::istreambuf_iterator<char>(file);
    auto end = std::istreambuf_iterator<char>();

    switch (stage.value()) {
    case ShaderStage::Vertex:
        _vertex.reserve(size);
        _vertex.assign(begin, end);
        break;
    case ShaderStage::Fragment:
        _pixel.reserve(size);
        _pixel.assign(begin, end);
        break;
    case ShaderStage::TessControl:
        _domain.reserve(size);
        _domain.assign(begin, end);
        break;
    case ShaderStage::TessEvaluation:
        _hull.reserve(size);
        _hull.assign(begin, end);
        break;
    case ShaderStage::Geometry:
        _geometry.reserve(size);
        _geometry.assign(begin, end);
        break;
    case ShaderStage::Compute:
        _compute.reserve(size);
        _compute.assign(begin, end);
        break;
    }

    return true;
}

DUSK_DIRECTX_API
bool DirectXShader::LoadHLSL(string_view path, bool useAssetPath)
{
    LogVerbose(DUSK_ANCHOR, "Looking for HLSL shader '{}'", path);

    HRESULT result;

    std::ifstream file;

    if (useAssetPath) {
        for (const auto& assetPath : GetAssetPathList()) {
            Path fullPath = assetPath / "Shaders" / path;
            LogVerbose(DUSK_ANCHOR, "Checking '{}'", fullPath);
            
            file.open(fullPath);
            if (file.is_open()) {
                break;
            }
        }
    else {
        file.open(path);
    }

    if (!file.is_open()) {
        return false;
    }

    file.unsetf(std::ios::skipws);

    LogVerbose(DUSK_ANCHOR, "Loading HLSL shader '{}'", path);

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    ComPtr<IDxcLibrary> library;
    result = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(library.GetAddressOf()));
    if (FAILED(result)) {
        LogError(DUSK_ANCHOR, "Failed to create IDxcLibrary");
        return false;
    }

    ComPtr<IDxcCompiler> compiler;
    result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(compiler.GetAddressOf()));
    if (FAILED(result)) {
        LogError(DUSK_ANCHOR, "Failed to create IDxcCompiler");
        return false;
    }

    ComPtr<IDxcBlobEncoding> sourceBlob;
    result = library->CreateBlobWithEncodingFromPinned(data.data(), (uint32_t)data.size(), CP_UTF8, sourceBlob.GetAddressOf());
    if (FAILED(result)) {
        LogError(DUSK_ANCHOR, "CreateBlobWithEncodingFromPinned failed");
        return false;
    }

    const wchar_t * args[] = {
        L"-Zpr", // Use row-major matricies
    };

    std::wstring wfilename = ConvertUTF8ToWideString(filename);
    if (wfilename.empty()) {
        LogError(DUSK_ANCHOR, "Failed to convert filename '{}' to wstring", filename);
        return false;
    }

    IncludeHandler inc(library.Get());

    auto stage = GetShaderStageFromFilename(filename);

    if (!stage) {
        LogError(DUSK_ANCHOR, "Failed to determine shader stage");
        return false;
    }

    auto entrypoint = GetShaderEntrypoint(stage.value());
    auto targetProfile = GetShaderTargetProfile(stage.value());

    if (!entrypoint) {
        LogError(DUSK_ANCHOR, "Failed to determine shader entrypoint");
        return false;
    }

    if (!targetProfile) {
        LogError(DUSK_ANCHOR, "Failed to determine shader target profile");
        return false;
    }

    ComPtr<IDxcOperationResult> compileResult;
    result = compiler->Compile(
        sourceBlob.Get(),
        wfilename.c_str(),
        entrypoint.value(),
        targetProfile.value(),
        &args[0],
        sizeof(args) / sizeof(args[0]),
        nullptr, 0,
        &inc,
        compileResult.GetAddressOf()
    );
    
    if (FAILED(result)) {
        LogError(DUSK_ANCHOR, "Failed to compile '{}'", filename);
        return false;
    }

    HRESULT isCompiled;
    result = compileResult->GetStatus(&isCompiled);
    if (FAILED(result) || FAILED(isCompiled)) {
        ComPtr<IDxcBlobEncoding> errorBlob;
        result = compileResult->GetErrorBuffer(errorBlob.GetAddressOf());
        if (SUCCEEDED(result)) {
            LogError(DUSK_ANCHOR, "Failed to compile shader '{}'\n{}", filename, (char *)errorBlob->GetBufferPointer());
            return false;
        }
    }

    ComPtr<IDxcBlob> blob;
    compileResult->GetResult(blob.GetAddressOf());

    uint8_t * begin = static_cast<uint8_t *>(blob->GetBufferPointer());
    uint8_t * end = begin + blob->GetBufferSize();

    switch (stage.value()) {
    case ShaderStage::Vertex:
        _vertex.reserve(blob->GetBufferSize());
        _vertex.assign(begin, end);
        break;
    case ShaderStage::Fragment:
        _pixel.reserve(blob->GetBufferSize());
        _pixel.assign(begin, end);
        break;
    case ShaderStage::TessControl:
        _domain.reserve(blob->GetBufferSize());
        _domain.assign(begin, end);
        break;
    case ShaderStage::TessEvaluation:
        _hull.reserve(blob->GetBufferSize());
        _hull.assign(begin, end);
        break;
    case ShaderStage::Geometry:
        _geometry.reserve(blob->GetBufferSize());
        _geometry.assign(begin, end);
        break;
    case ShaderStage::Compute:
        _compute.reserve(blob->GetBufferSize());
        _compute.assign(begin, end);
        break;
    }
    
    return true;
}

DUSK_DIRECTX_API
D3D12_SHADER_BYTECODE DirectXShader::GetShaderBytecode(const ShaderStage& stage)
{
    switch (stage) {
    case ShaderStage::Vertex:
        return { static_cast<void *>(_vertex.data()), _vertex.size() };
    case ShaderStage::Fragment:
        return { static_cast<void *>(_pixel.data()), _pixel.size() };
    case ShaderStage::TessControl:
        return { static_cast<void *>(_domain.data()), _domain.size() };
    case ShaderStage::TessEvaluation:
        return { static_cast<void *>(_hull.data()), _hull.size() };
    case ShaderStage::Geometry:
        return { static_cast<void *>(_geometry.data()), _geometry.size() };
    case ShaderStage::Compute:
        return { static_cast<void *>(_compute.data()), _compute.size() };
    }

    return { nullptr, 0 };
}

DUSK_DIRECTX_API
std::optional<ShaderStage> GetShaderStageFromFilename(string_view filename)
{
    string ext = GetExtension(filename);
    if (ext == "cso" || ext == "hlsl") {
        size_t pivot = filename.find_last_of('.');
        if (pivot == string::npos) {
            return {};
        }
        ext = GetExtension(filename.substr(0, pivot));
    }

    if (ext == "vert" || ext == "vertex") {
        return ShaderStage::Vertex;
    }
    else if (ext == "frag" || ext == "fragment" || ext == "pixel") {
        return ShaderStage::Fragment;
    }
    else if (ext == "tesc" || ext == "tesscontrol" || ext == "hull") {
        return ShaderStage::TessControl;
    }
    else if (ext == "tese" || ext == "tesseval" || ext == "domain") {
        return ShaderStage::TessEvaluation;
    }
    else if (ext == "geom" || ext == "geometry") {
        return ShaderStage::Geometry;
    }
    else if (ext == "comp" || ext == "compute") {
        return ShaderStage::Compute;
    }

    return {};
}

DUSK_DIRECTX_API
std::optional<const wchar_t *> GetShaderEntrypoint(const ShaderStage& stage)
{
    switch (stage) {
    case ShaderStage::Vertex:
        return L"VSMain";
    case ShaderStage::Fragment:
        return L"PSMain";
    case ShaderStage::TessControl:
        return L"HSMain";
    case ShaderStage::TessEvaluation:
        return L"DSMain";
    case ShaderStage::Geometry:
        return L"GSMain";
    case ShaderStage::Compute:
        return L"CSMain";
    }
    
    return {};
}

DUSK_DIRECTX_API
std::optional<const wchar_t *> GetShaderTargetProfile(const ShaderStage& stage)
{
    switch (stage) {
    case ShaderStage::Vertex:
        return L"vs_6_0";
    case ShaderStage::Fragment:
        return L"ps_6_0";
    case ShaderStage::TessControl:
        return L"hs_6_0";
    case ShaderStage::TessEvaluation:
        return L"ds_6_0";
    case ShaderStage::Geometry:
        return L"gs_6_0";
    case ShaderStage::Compute:
        return L"cs_6_0";
    }

    return {};
}

} // namespace Dusk