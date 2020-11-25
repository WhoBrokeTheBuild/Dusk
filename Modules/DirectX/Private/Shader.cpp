#include <Dusk/DirectX/Shader.hpp>
#include <Dusk/DirectX/GraphicsDriver.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>

#include <fstream>
#include <sstream>

namespace Dusk::DirectX {

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
        const auto& shaderPaths = GetShaderPaths();

        std::string filename = ConvertWideStringToUTF8(wfilename);
        
        std::ifstream file;

        for (const auto& path : shaderPaths) {
            const std::string& fullPath = path + filename;
            DuskLogVerbose("Checking shader include '%s'", fullPath);
            
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
bool Shader::LoadFromFiles(const std::vector<std::string>& filenames)
{
    DuskBenchmarkStart();

    GraphicsDriver * gfx = (GraphicsDriver *)GetGraphicsDriver();

    for (const auto& filename : filenames) {
        const std::string& ext = GetExtension(filename);

        if (ext == "cso") {
            if (LoadCSO(filename)) {
                break;
            }
            else {
                DuskLogError("Failed to load shader '%s'", filename);
                return false;
            }
        }
        else if (ext == "hlsl") {
            if (LoadHLSL(filename)) {
                break;
            }
            else {
                DuskLogError("Failed to load shader '%s'", filename);
                return false;
            }
        }
        else {
            if (LoadCSO(filename + ".cso") || LoadHLSL(filename + ".hlsl")) {
                break;
            }
            else {
                DuskLogError("Failed to load shader '%s', tried extensions '.cso' and '.hlsl'", filename);
                return false;
            }
        }
    }
        
    DuskBenchmarkEnd("DirectX::Shader::LoadFromFiles");
    return true;
}

DUSK_DIRECTX_API
void Shader::Bind()
{

}

bool Shader::LoadCSO(const std::string& filename)
{
    DuskLogVerbose("Looking for Compiled Shader Object '%s'", filename);

    const auto& shaderPaths = GetShaderPaths();

    std::ifstream file;

    for (const auto& path : shaderPaths) {
        const std::string& fullPath = path + filename;
        DuskLogVerbose("Checking '%s'", fullPath);
        
        file.open(fullPath, std::ios::binary);
        if (file.is_open()) {
            break;
        }
    }

    if (!file.is_open()) {
        return false;
    }

    file.unsetf(std::ios::skipws);

    DuskLogLoad("Loading Compiled Shader Object '%s'", filename);

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    size_t pivot = filename.find_last_of('.');
    if (pivot == std::string::npos) {
        DuskLogWarn("Failed to find shader type extension for '%s'", filename.c_str());
        return false;
    }

    // TODO
    return true;
}

bool Shader::LoadHLSL(const std::string& filename)
{
    DuskLogVerbose("Looking for HLSL shader '%s'", filename);

    HRESULT result;

    std::ifstream file;

    const auto& shaderPaths = GetShaderPaths();

    for (const auto& path : shaderPaths) {
        const std::string& fullPath = path + filename;
        DuskLogVerbose("Checking '%s'", fullPath);
        
        file.open(fullPath, std::ios::binary);
        if (file.is_open()) {
            break;
        }
    }

    if (!file.is_open()) {
        return false;
    }

    file.unsetf(std::ios::skipws);

    DuskLogLoad("Loading HLSL shader '%s'", filename);

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    size_t pivot = filename.find_last_of('.');
    if (pivot == std::string::npos) {
        DuskLogWarn("Failed to find shader type extension for '%s'", filename.c_str());
        return false;
    }

    ComPtr<IDxcLibrary> library;
    result = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(library.GetAddressOf()));
    if (FAILED(result)) {
        DuskLogError("Failed to create IDxcLibrary");
        return false;
    }

    ComPtr<IDxcCompiler> compiler;
    result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(compiler.GetAddressOf()));
    if (FAILED(result)) {
        DuskLogError("Failed to create IDxcCompiler");
        return false;
    }

    ComPtr<IDxcBlobEncoding> blob;
    result = library->CreateBlobWithEncodingFromPinned(data.data(), (uint32_t)data.size(), CP_UTF8, blob.GetAddressOf());
    if (FAILED(result)) {
        DuskLogError("CreateBlobWithEncodingFromPinned failed");
        return false;
    }

    const wchar_t * args[] = {
        L"-Zpr", // Use row-major matricies
    };

    std::wstring wfilename = ConvertUTF8ToWideString(filename);
    if (wfilename.empty()) {
        DuskLogError("Failed to convert filename '%s' to wstring", filename);
        return false;
    }

    IncludeHandler inc(library.Get());

    ComPtr<IDxcOperationResult> compileResult;
    result = compiler->Compile(
        blob.Get(),
        wfilename.c_str(),
        GetEntrypoint(filename),
        GetTargetProfile(filename),
        &args[0],
        sizeof(args) / sizeof(args[0]),
        nullptr, 0,
        &inc,
        compileResult.GetAddressOf()
    );
    
    if (FAILED(result)) {
        DuskLogError("Failed to compile '%s'", filename);
        return false;
    }

    HRESULT isCompiled;
    result = compileResult->GetStatus(&isCompiled);
    if (FAILED(result) || FAILED(isCompiled)) {
        ComPtr<IDxcBlobEncoding> errorBlob;
        result = compileResult->GetErrorBuffer(errorBlob.GetAddressOf());
        if (SUCCEEDED(result)) {
            DuskLogError("Failed to compile shader '%s'\n%s", filename, (char *)errorBlob->GetBufferPointer());
            return false;
        }
    }

    // TODO
    return true;
}

const wchar_t * Shader::GetEntrypoint(const std::string& filename)
{
    std::string ext = GetExtension(filename);
    if (ext == "cso" || ext == "hlsl") {
        size_t pivot = filename.find_last_of('.');
        if (pivot == std::string::npos) {
            return L"";
        }
        ext = GetExtension(filename.substr(0, pivot));
    }

    if (ext == "vert" || ext == "vertex") {
        return L"VSMain";
    }
    else if (ext == "frag" || ext == "fragment" || ext == "pixel") {
        return L"PSMain";
    }
    else if (ext == "hull") {
        return L"HSMain";
    }
    else if (ext == "domain") {
        return L"DSMain";
    }
    else if (ext == "geom" || ext == "geometry") {
        return L"GSMain";
    }
    else if (ext == "comp" || ext == "compute") {
        return L"CSMain";
    }

    return L"";
}

const wchar_t * Shader::GetTargetProfile(const std::string& filename)
{
    std::string ext = GetExtension(filename);
    if (ext == "cso" || ext == "hlsl") {
        size_t pivot = filename.find_last_of('.');
        if (pivot == std::string::npos) {
            return L"";
        }
        ext = GetExtension(filename.substr(0, pivot));
    }

    if (ext == "vert" || ext == "vertex") {
        return L"vs_6_0";
    }
    else if (ext == "frag" || ext == "fragment" || ext == "pixel") {
        return L"ps_6_0";
    }
    else if (ext == "hull") {
        return L"hs_6_0";
    }
    else if (ext == "domain") {
        return L"ds_6_0";
    }
    else if (ext == "geom" || ext == "geometry") {
        return L"gs_6_0";
    }
    else if (ext == "comp" || ext == "compute") {
        return L"cs_6_0";
    }

    return L"";
}

} // namespace Dusk::DirectX