#include <Dusk/DirectX/Shader.hpp>
#include <Dusk/DirectX/GraphicsDriver.hpp>
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
        const auto& assetPaths = GetAssetPaths();

        std::string filename = ConvertWideStringToUTF8(wfilename);
        
        std::ifstream file;

        for (const auto& path : assetPaths) {
            const std::string& fullPath = path + "Shaders/" + filename;
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
bool DirectXShader::LoadFromFiles(const std::vector<std::string>& filenames)
{
    DuskBenchmarkStart();

    DirectXGraphicsDriver * gfx = DUSK_DIRECTX_GRAPHICS_DRIVER(GetGraphicsDriver());

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
        
    DuskBenchmarkEnd("DirectXShader::LoadFromFiles");
    return true;
}

bool DirectXShader::LoadCSO(const std::string& filename)
{
    DuskLogVerbose("Looking for Compiled Shader Object '%s'", filename);

    const auto& assetPaths = GetAssetPaths();

    std::ifstream file;

    for (const auto& path : assetPaths) {
        const std::string& fullPath = path + "Shaders/" + filename;
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

bool DirectXShader::LoadHLSL(const std::string& filename)
{
    DuskLogVerbose("Looking for HLSL shader '%s'", filename);

    HRESULT result;

    std::ifstream file;

    const auto& assetPaths = GetAssetPaths();

    for (const auto& path : assetPaths) {
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

    Type type = GetType(filename);

    ComPtr<IDxcOperationResult> compileResult;
    result = compiler->Compile(
        blob.Get(),
        wfilename.c_str(),
        GetEntrypoint(type),
        GetTargetProfile(type),
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

    compileResult->GetResult(_vs.GetAddressOf());

    // TODO
    return true;
}

Shader::Type DirectXShader::GetType(const std::string& filename) const
{
    std::string ext = GetExtension(filename);
    if (ext == "cso" || ext == "hlsl") {
        size_t pivot = filename.find_last_of('.');
        if (pivot == std::string::npos) {
            return Type::Unknown;
        }
        ext = GetExtension(filename.substr(0, pivot));
    }

    if (ext == "vert" || ext == "vertex") {
        return Type::Vertex;
    }
    else if (ext == "pixel" || ext == "frag" || ext == "fragment") {
        return Type::Pixel;
    }
    else if (ext == "hull") {
        return Type::Hull;
    }
    else if (ext == "domain") {
        return Type::Domain;
    }
    else if (ext == "geom" || ext == "geometry") {
        return Type::Geometry;
    }
    else if (ext == "comp" || ext == "compute") {
        return Type::Compute;
    }

    return Type::Unknown;
}

const wchar_t * DirectXShader::GetEntrypoint(const Shader::Type& type) const
{
    switch (type) {
    case Type::Vertex:
        return L"VSMain";
    case Type::Pixel:
        return L"PSMain";
    case Type::Hull:
        return L"HSMain";
    case Type::Domain:
        return L"DSMain";
    case Type::Geometry:
        return L"GSMain";
    case Type::Compute:
        return L"CSMain";
    default: ;
    }
    
    return L"";
}

const wchar_t * DirectXShader::GetTargetProfile(const Shader::Type& type) const
{
    switch (type) {
    case Type::Vertex:
        return L"vs_6_0";
    case Type::Pixel:
        return L"ps_6_0";
    case Type::Hull:
        return L"hs_6_0";
    case Type::Domain:
        return L"ds_6_0";
    case Type::Geometry:
        return L"gs_6_0";
    case Type::Compute:
        return L"cs_6_0";
    default: ;
    }

    return L"";
}

ComPtr<IDxcBlob>& DirectXShader::GetBlob(const Shader::Type& type)
{
    switch (type) {
    case Type::Vertex:
        return _vs;
    case Type::Pixel:
        return _ps;
    case Type::Hull:
        return _hs;
    case Type::Domain:
        return _ds;
    case Type::Geometry:
        return _gs;
    case Type::Compute:
        return _cs;
    default: ;
    }

    // TODO: Better "Error" return
    return _vs;
}

} // namespace Dusk