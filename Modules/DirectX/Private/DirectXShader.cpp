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
bool DirectXShader::LoadFromFiles(const std::vector<string>& filenameList, bool useAssetPath /*= true*/)
{
    DuskBenchmarkStart();

    for (const auto& filename : filenameList) {
        Path ext = GetExtension(filename);

        if (ext == "cso") {
            if (LoadCSO(Path(filename), useAssetPath)) {
                break;
            }
            else {
                LogError(DUSK_ANCHOR, "Failed to load shader '{}'", filename);
                return false;
            }
        }
        else if (ext == "hlsl") {
            if (LoadHLSL(Path(filename), useAssetPath)) {
                break;
            }
            else {
                LogError(DUSK_ANCHOR, "Failed to load shader '{}'", filename);
                return false;
            }
        }
        else {
            if (LoadCSO(Path(filename) + ".cso", useAssetPath) || LoadHLSL(filename + ".hlsl", useAssetPath)) {
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
bool DirectXShader::LoadCSO(const Path& path, ShaderStage stage, const string& entrypoint, bool useAssetPath)
{
    LogVerbose(DUSK_ANCHOR, "Looking for Compiled Shader Object '{}'", path);

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

    _bytecodeByStage[stage.value()] = std::vector<uint8_t>();
    auto& v = _bytecodeByStage[stage.value()];

    v.reserve(size);
    v.assign(begin, end);

    return true;
}

DUSK_DIRECTX_API
bool DirectXShader::LoadHLSL(const Path& path, ShaderStage stage, const string& entrypoint, bool useAssetPath)
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

    if (!entrypoint) {
        LogError(DUSK_ANCHOR, "Failed to determine shader entrypoint");
        return false;
    }

    std::wstring targetProfile;

    switch (stage) {
    case ShaderStage::Vertex:
        targetProfile = L"vs_6_0";
        break;
    case ShaderStage::Fragment:
        targetProfile = L"ps_6_0";
        break;
    case ShaderStage::TessControl:
        targetProfile = L"hs_6_0";
        break;
    case ShaderStage::TessEvaluation:
        targetProfile = L"ds_6_0";
        break;
    case ShaderStage::Geometry:
        targetProfile = L"gs_6_0";
        break;
    case ShaderStage::Compute:
        targetProfile = L"cs_6_0";
        break;
    default:
        LogError(DUSK_ANCHOR, "Unable to determine shader target profile");
        return false;
    }

    ComPtr<IDxcOperationResult> compileResult;
    result = compiler->Compile(
        sourceBlob.Get(),
        wfilename.c_str(),
        entrypoint.value(),
        targetProfile.c_str(),
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

    _bytecodeByStage[stage.value()] = std::vector<uint8_t>();
    auto& v = _bytecodeByStage[stage.value()];

    v.reserve(blob->GetBufferSize());
    v.assign(begin, end);
    
    return true;
}

DUSK_DIRECTX_API
D3D12_SHADER_BYTECODE DirectXShader::GetShaderBytecode(ShaderStage stage)
{
    auto it = _bytecodeByStage.find(stage);
    if (it != _bytecodeByStage.end()) {
        const auto& v = *it;
        return { static_cast<void *>(v.data()), v.size() };
    }

    return { nullptr, 0 };
}

} // namespace Dusk