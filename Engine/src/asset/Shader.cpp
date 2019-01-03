#include "dusk/asset/Shader.hpp"

#include <dusk/core/Benchmark.hpp>
#include <dusk/core/Log.hpp>

#include <fstream>
#include <sstream>

namespace dusk {

Shader::Shader(const std::initializer_list<std::string>& filenames)
{
    LoadFromFiles(filenames);
}

Shader::Shader(const std::vector<std::string>& filenames)
{
    LoadFromFiles(filenames);
}

Shader::~Shader()
{
    if (_glID > 0) glDeleteProgram(_glID);
}

void Shader::InitializeVersionString()
{
    _GLSLVersionString = std::string(reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

    auto it = _GLSLVersionString.find(' ');
    if (it != std::string::npos) {
        _GLSLVersionString.erase(_GLSLVersionString.find(' '));
    }

    _GLSLVersionString.erase(_GLSLVersionString.find('.'), 1);
    _GLSLVersionString = "#version " + _GLSLVersionString + " core";

    DuskLogVerbose("Using GLSL version string '%s'", _GLSLVersionString.c_str());
}

bool Shader::LoadFromFiles(const std::initializer_list<std::string>& filenames)
{
    return LoadFromFiles(std::vector<std::string>(filenames));
}

bool Shader::LoadFromFiles(const std::vector<std::string>& filenames)
{
    _filenames = filenames;

    GLint linked = GL_FALSE;
    std::vector<GLuint> shaders;

    if (_loaded)
    {
        DuskLogWarn("Attempt to load an already loaded shader.");
        return false;
    }

    _glID = glCreateProgram();
    if (0 == _glID)
    {
        DuskLogError("Failed to create shader program.");
        return false;
    }

#if defined(DUSK_ENABLE_BINARY_SHADERS) && defined(GL_VERSION_4_1)

    DuskLogVerbose("Attempting Binary Shader Stuff");

    glProgramParameteri(_glID, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);

    bool loadFromBinary = false;
    GLenum binaryFormat = GL_INVALID_ENUM;
    std::string binFilename = "shaders/" + GetBinaryName(filenames);

    std::ifstream binFile;
    
    const auto& paths = GetAssetPaths();
    for (auto& p : paths) {
        std::string fullPath = p + binFilename;

        DuskLogVerbose("Checking %s", fullPath.c_str());
        binFile.open(fullPath, std::ios::binary);

        if (binFile.is_open()) break;
    }

    if (binFile.is_open())
    {
        DuskLogLoad("Loading cached binary shader from '%s'.", binFilename.c_str());

        binFile.read((char *)&binaryFormat, sizeof(GLenum));

        std::vector<GLubyte> data((std::istreambuf_iterator<char>(binFile)),
                                  std::istreambuf_iterator<char>());
        binFile.close();

        glProgramBinary(_glID, binaryFormat, data.data(), data.size());
        glGetProgramiv(_glID, GL_LINK_STATUS, &linked);

        if (!linked)
        {
            DuskLogWarn("Failed to load cached binary shader '%s'.", binFilename.c_str());
        }
        else
        {
            _loaded = true;
            loadFromBinary = true;
        }
    }

#endif // defined(DUSK_ENABLE_BINARY_SHADERS) && defined(GL_VERSION_4_1)

    if (!_loaded)
    {
        for (const std::string& filename : filenames)
        {
            GLuint id = LoadShader(filename);
            if (0 == id)
            {
                continue;
            }

            glAttachShader(_glID, id);
            shaders.push_back(id);
        }

        if (shaders.size() != filenames.size())
        {
            DuskLogError("One or mode shaders failed to compile.");
        }
        else
        {
            glLinkProgram(_glID);
            glGetProgramiv(_glID, GL_LINK_STATUS, &linked);
            if (linked == GL_FALSE)
            {
                DuskLogError("Failed to link shader program");
                PrintProgramLog();
            }
            else
            {
                _loaded = true;
            }
        }
    }

#if defined(DUSK_ENABLE_BINARY_SHADERS) && defined(GL_VERSION_4_1)

    if (_loaded && !loadFromBinary)
    {
        GLint length = 0;
        glGetProgramiv(_glID, GL_PROGRAM_BINARY_LENGTH, &length);

        if (length > 0)
        {
            std::vector<GLubyte> data(length);

            glGetProgramBinary(_glID, data.size(), nullptr, &binaryFormat, data.data());

            std::string outBinFilename;
            std::ofstream outBinFile;
            for (auto &p : paths) {
                outBinFilename = p + binFilename;

                DuskLogVerbose("Checking %s", outBinFilename.c_str());
                outBinFile.open(outBinFilename, std::ios::binary);

                if (outBinFile.is_open()) break;
            }

            if (!outBinFile.is_open())
            {
                DuskLogWarn("Failed to create binary shader file '%s'.", binFilename.c_str());
            }
            else
            {
                DuskLogVerbose("Saving cached binary shader file '%s'.", outBinFilename.c_str());
                outBinFile.write((const char *)&binaryFormat, sizeof(GLenum));
                outBinFile.write((const char *)data.data(), data.size());
                outBinFile.close();
            }
        }
    }

#endif // defined(DUSK_ENABLE_BINARY_SHADERS) && defined(GL_VERSION_4_1)

    if (_loaded)
    {
        CacheUniforms();
        CacheAttributes();
    }

    for (GLuint shader : shaders)
    {
        glDetachShader(_glID, shader);
        glDeleteShader(shader);
    }

    return _loaded;
}

void Shader::Bind()
{
    glUseProgram(_glID);
}

GLint Shader::GetAttributeLocation(const std::string& name) const
{
    if (_attributes.find(name) == _attributes.end()) return -1;
    return _attributes.at(name);
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
    if (_uniforms.find(name) == _uniforms.end()) return -1;
    return _uniforms.at(name);
}

std::string Shader::GetBinaryName(const std::vector<std::string> filenames)
{
    // Magic number to start the hash
    unsigned long tmp = 5381;
    std::stringstream ss;

    for (std::string s : filenames)
    {
        for (char c : s)
        {
            tmp = ((tmp << 5) + tmp) + c;
        }
    }

    ss << std::hex << tmp;
    ss << ".glsl.bin";
    return ss.str();
}

GLuint Shader::LoadShader(const std::string& filename)
{
    GLuint type = GL_INVALID_ENUM;
    if (filename.find(".vs.glsl") != std::string::npos)
    {
        type = GL_VERTEX_SHADER;
    }
    else if (filename.find(".fs.glsl") != std::string::npos)
    {
        type = GL_FRAGMENT_SHADER;
    }
    else if (filename.find(".gs.glsl") != std::string::npos)
    {
        type = GL_GEOMETRY_SHADER;
    }
#ifdef GL_VERSION_4_0
    // Requires OpenGL 4.0+
    else if (filename.find(".tcs.glsl") != std::string::npos)
    {
        type = GL_TESS_CONTROL_SHADER;
    }
    else if (filename.find(".tes.glsl") != std::string::npos)
    {
        type = GL_TESS_EVALUATION_SHADER;
    }
#endif // GL_VERSION_4_0
#ifdef GL_VERSION_4_3
    // Requires OpenGL 4.3+
    else if (filename.find(".cs.glsl") != std::string::npos)
    {
        type = GL_COMPUTE_SHADER;
    }
#endif // GL_VERSION_4_3
    else
    {
        DuskLogError("Unable to infer shader type for '%s'", filename.c_str());
        return false;
    }

    const auto& paths = GetAssetPaths();

    std::string fullPath;
    std::ifstream file;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath.c_str());
        file.open(fullPath);

        if (file.is_open()) break;
    }

    if (!file.is_open()) {
        DuskLogError("Failed to open shader '%s'", filename.c_str());
        return 0;
    }

    std::string code((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
    file.close();

    code = _GLSLVersionString + "\n" + code;

    DuskLogLoad("Loading %s shader from '%s'", GetShaderTypeString(type).c_str(), fullPath.c_str());
    code = PreProcess(type, code, GetDirname(fullPath));

    GLint compiled = GL_FALSE;
    GLuint id = glCreateShader(type);
    const char * bufferPtr = code.c_str();

    glShaderSource(id, 1, (const GLchar **)&bufferPtr, nullptr);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        DuskLogError("Failed to compile shader '%s'.", filename.c_str());
        PrintShaderLog(id);
        PrintCode(code);

        glDeleteShader(id);
        id = 0;

        return 0;
    }

    return id;
}

std::string Shader::PreProcess(GLuint type, const std::string& code, const std::string& basedir /* = "" */)
{
    std::istringstream iss(code);
    std::string processedCode;
    std::string line;

    while (std::getline(iss, line))
    {
        if (line[0] == '#')
        {
            if (0 == line.compare(0, strlen("#include"), "#include"))
            {
                auto paths = GetAssetPaths();
                if (!basedir.empty()) {
                    paths.push_back((basedir.back() == '/' ? basedir : basedir + '/'));
                }

                std::string filename = line.substr(strlen("#include") + 1);

                std::string incFilename;
                std::ifstream incFile;
                for (auto& p : paths) {
                    incFilename = p + filename;

                    DuskLogVerbose("Checking %s", incFilename.c_str());
                    incFile.open(incFilename);

                    if (incFile.is_open()) break;
                }

                if (!incFile.is_open()) {
                    DuskLogError("Failed to shader include '%s'", filename.c_str());
                    continue;
                }

                std::string incCode((std::istreambuf_iterator<char>(incFile)),
                                     std::istreambuf_iterator<char>());
                incFile.close();

                processedCode += PreProcess(type, incCode, GetDirname(incFilename));

                continue;
            }
        }

        // TODO: Support more preprocessing

        processedCode += line;
        processedCode += "\n";
    }

    return processedCode;
}

void Shader::PrintShaderLog(GLuint id)
{
    std::vector<char> shaderLog;
    GLint logSize, retSize;

    if (!glIsShader(id))
    {
        DuskLogError("Cannot print shader log, %d is not a shader", _glID);
        return;
    }

    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logSize);

    shaderLog.resize(logSize);
    glGetShaderInfoLog(id, logSize, &retSize, shaderLog.data());

    DuskLogInfo("Log for shader %d:\n%s", id, shaderLog.data());
}

void Shader::PrintProgramLog()
{
    std::vector<char> programLog;
    GLint logSize, retSize;

    if (!glIsProgram(_glID))
    {
        DuskLogError("Cannot print shader program log, %d is not a shader program", _glID);
        return;
    }

    glGetProgramiv(_glID, GL_INFO_LOG_LENGTH, &logSize);

    programLog.resize(logSize);
    glGetProgramInfoLog(_glID, logSize, &retSize, programLog.data());

    DuskLogInfo("Log for shader program %d:\n%s", _glID, programLog.data());
}

void Shader::CacheUniforms()
{
    GLint size;
    GLenum type;
    GLchar name[256]; // variable name in GLSL

    GLint count;
    glGetProgramiv(_glID, GL_ACTIVE_UNIFORMS, &count);
    for (GLint i = 0; i < count; ++i)
    {
        glGetActiveUniform(_glID, (GLuint)i, sizeof(name), nullptr, &size, &type, name);
        _uniforms.emplace(name, glGetUniformLocation(_glID, name));
    }
}

void Shader::CacheAttributes()
{
    GLint size;
    GLenum type;
    GLchar name[256];

    GLint count;
    glGetProgramiv(_glID, GL_ACTIVE_ATTRIBUTES, &count);
    for (GLint i = 0; i < count; ++i)
    {
        glGetActiveAttrib(_glID, (GLuint)i, sizeof(name), nullptr, &size, &type, name);
        _attributes.emplace(name, glGetAttribLocation(_glID, name));
    }
}

} // namespace dusk
