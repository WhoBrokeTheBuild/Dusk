#include "dusk/asset/Shader.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>

#include <fstream>
#include <sstream>

namespace dusk {

std::string Shader::_GLSLVersionString;

std::queue<GLuint> Shader::_AvailableUniformBufferBindings;
std::unordered_map<std::string, Shader::UniformBufferRecord> Shader::_UniformBuffers;

Shader::Shader(const std::vector<std::string>& filenames)
{
    LoadFromFiles(filenames);
}

Shader::~Shader()
{
    if (_glId > 0) glDeleteProgram(_glId);
}

std::unique_ptr<Shader> Shader::Create()
{
    return std::make_unique<Shader>();
}

std::unique_ptr<Shader> Shader::Create(const std::vector<std::string>& filenames)
{
    return std::make_unique<Shader>(filenames);
}

void Shader::InitializeVersionString()
{
    _GLSLVersionString = std::string(reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    _GLSLVersionString.erase(_GLSLVersionString.find('.'), 1);
    _GLSLVersionString.erase(_GLSLVersionString.find(' '));
    _GLSLVersionString = "#version " + _GLSLVersionString + " core";

    DuskLogVerbose("Using GLSL version string '%s'", _GLSLVersionString.c_str());
}

void Shader::InitializeUniformBuffers()
{
    while (!_AvailableUniformBufferBindings.empty())
    {
        _AvailableUniformBufferBindings.pop();
    }

    int maxBindings;
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBindings);
    DuskLogVerbose("Max UBO Bindings %d", maxBindings);

    for (int i = 0; i < maxBindings; ++i)
    {
        _AvailableUniformBufferBindings.push(static_cast<GLuint>(i));
    }

    int tmp;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &tmp);
    DuskLogVerbose("Max UBO Size %d", tmp);

    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &tmp);
    DuskLogVerbose("Max Vertex UBOs %d", tmp);

    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &tmp);
    DuskLogVerbose("Max Fragment UBOs %d", tmp);

    glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &tmp);
    DuskLogVerbose("Max Geometry UBOs %d", tmp);
}

void Shader::SetUniformBufferData(const std::string& name, GLvoid * data)
{
    if (_UniformBuffers.find(name) == _UniformBuffers.end())
    {
        //DuskLogWarn("Attempt to set data to uknown UBO");
        return;
    }

    UniformBufferRecord& record = _UniformBuffers[name];

    glBindBuffer(GL_UNIFORM_BUFFER, record.GLID);

    GLvoid* ptr = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, data, record.Size);

    glUnmapBuffer(GL_UNIFORM_BUFFER);
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

    _glId = glCreateProgram();
    if (0 == _glId)
    {
        DuskLogError("Failed to create shader program.");
        return false;
    }

#if defined(DUSK_ENABLE_BINARY_SHADERS) && defined(GL_VERSION_4_1)

    DuskLogVerbose("Attempting Binary Shader Stuff");

    glProgramParameteri(_glId, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);

    bool loadFromBinary = false;
    GLenum binaryFormat = GL_INVALID_ENUM;
    std::string binFilename = "assets/shaders/" + GetBinaryName(filenames);

    std::ifstream binFile(binFilename, std::ios::binary);
    if (binFile.is_open())
    {
        DuskLogLoad("Loading cached binary shader from '%s'.", binFilename.c_str());

        binFile.read((char *)&binaryFormat, sizeof(GLenum));

        std::vector<GLubyte> data((std::istreambuf_iterator<char>(binFile)),
                                  std::istreambuf_iterator<char>());
        binFile.close();

        glProgramBinary(_glId, binaryFormat, data.data(), data.size());
        glGetProgramiv(_glId, GL_LINK_STATUS, &linked);

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

            glAttachShader(_glId, id);
            shaders.push_back(id);
        }

        if (shaders.size() != filenames.size())
        {
            DuskLogError("One or mode shaders failed to compile.");
        }
        else
        {
            glLinkProgram(_glId);
            glGetProgramiv(_glId, GL_LINK_STATUS, &linked);
            if (!linked)
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
        glGetProgramiv(_glId, GL_PROGRAM_BINARY_LENGTH, &length);

        if (length > 0)
        {
            std::vector<GLubyte> data(length);

            glGetProgramBinary(_glId, data.size(), nullptr, &binaryFormat, data.data());

            std::ofstream outBinFile(binFilename, std::ios::binary);
            if (!outBinFile)
            {
                DuskLogWarn("Failed to create binary shader file '%s'.", binFilename.c_str());
            }
            else
            {
                DuskLogVerbose("Saving cached binary shader file '%s'.", binFilename.c_str());
                outBinFile.write((const char *)&binaryFormat, sizeof(GLenum));
                outBinFile.write((const char *)data.data(), data.size());
                outBinFile.close();
            }
        }
    }

#endif // defined(DUSK_ENABLE_BINARY_SHADERS) && defined(GL_VERSION_4_1)

    for (GLuint shader : shaders)
    {
        glDetachShader(_glId, shader);
        glDeleteShader(shader);
    }

    if (_loaded)
    {
        CacheUniforms();
        CacheAttributes();
    }

    return _loaded;
}

void Shader::Bind()
{
    glUseProgram(_glId);
}

bool Shader::HasAttribute(const std::string& name) const
{
    return (_attributes.find(name) != _attributes.end());
}

GLint Shader::GetAttributeLocation(const std::string& name) const
{
    if (_attributes.find(name) == _attributes.end()) return std::numeric_limits<GLint>::max();
    return _attributes.at(name);
}

bool Shader::HasUniform(const std::string& name) const
{
    return (_uniforms.find(name) != _uniforms.end());
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
    if (_uniforms.find(name) == _uniforms.end()) return 0;
    return _uniforms.at(name).Location;
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

    std::ifstream file(filename);
    std::string code((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
    file.close();

    code = _GLSLVersionString + "\n" + code;

    DuskLogLoad("Loading %s shader from '%s'", GetShaderTypeString(type).c_str(), filename.c_str());
    code = PreProcess(type, code, GetDirname(filename));

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

        return false;
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
                std::string incFilename = (basedir.empty() ? "" : basedir + "/") + line.substr(strlen("#include") + 1);

                std::ifstream incFile(incFilename);
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
        DuskLogError("Cannot print shader log, %d is not a shader", _glId);
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

    if (!glIsProgram(_glId))
    {
        DuskLogError("Cannot print shader program log, %d is not a shader program", _glId);
        return;
    }

    glGetProgramiv(_glId, GL_INFO_LOG_LENGTH, &logSize);

    programLog.resize(logSize);
    glGetProgramInfoLog(_glId, logSize, &retSize, programLog.data());

    DuskLogInfo("Log for shader program %d:\n%s", _glId, programLog.data());
}

void Shader::CacheUniforms()
{
    GLint tmpSize; // size of the variable
    GLenum tmpType; // type of the variable (float, vec3 or mat4, etc)

    GLchar buffer[256]; // variable name in GLSL
    GLsizei length; // name length

    GLint count;
    glGetProgramiv(_glId, GL_ACTIVE_UNIFORMS, &count);
    for (GLint i = 0; i < count; ++i)
    {
        glGetActiveUniform(_glId, (GLuint)i, sizeof(buffer), &length, &tmpSize, &tmpType, buffer);
        _uniforms.emplace(buffer, UniformRecord{
            glGetUniformLocation(_glId, buffer),
            tmpSize,
            tmpType
        });
    }

    const int STRIDE = 16;

    std::string name;
    UniformRecord uniform;
    std::unordered_map<std::string, size_t> uboSizes;
    for (auto pair : _uniforms)
    {
        std::tie(name, uniform) = pair;

        if (name.find('.') == std::string::npos) continue;

        std::string uboName = name.substr(0, name.find('.'));
        if (uboSizes.find(uboName) == uboSizes.end())
        {
            uboSizes.emplace(uboName, 0);
        }

        size_t uniformSize = uniform.Size * GetGLTypeSize(uniform.Type);
        size_t allowedSize = STRIDE - (uboSizes[uboName] % STRIDE);

        if (uniformSize > allowedSize && allowedSize < STRIDE)
        {
            uboSizes[uboName] += allowedSize; // Pad to stride
        }

        uboSizes[uboName] += uniformSize;
    }

    size_t size;
    for (auto pair : uboSizes)
    {
        std::tie(name, size) = pair;

        if (size % STRIDE > 0)
        {
            size += STRIDE - (size % STRIDE);
        }

        if (_UniformBuffers.find(name) == _UniformBuffers.end())
        {
            if (_AvailableUniformBufferBindings.empty())
            {
                DuskLogError("Reached UBO binding limit");
                continue;
            }

            std::vector<uint8_t> dummy(size, 0);

            GLuint uboId = 0;
            glGenBuffers(1, &uboId);
            if (0 == uboId)
            {
                DuskLogError("Failed to create UBO");
                continue;
            }

            glBindBuffer(GL_UNIFORM_BUFFER, uboId);
            glBufferData(GL_UNIFORM_BUFFER, size, dummy.data(), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            glBindBufferBase(GL_UNIFORM_BUFFER, _AvailableUniformBufferBindings.front(), uboId);

            DuskLogVerbose("Binding UBO %s to %u", name.c_str(), _AvailableUniformBufferBindings.front());
            _UniformBuffers.emplace(name, UniformBufferRecord{
                uboId,
                _AvailableUniformBufferBindings.front(),
                size,
            });
            _AvailableUniformBufferBindings.pop();
        }

        GLuint blockIndex = glGetUniformBlockIndex(_glId, name.c_str());
        glUniformBlockBinding(_glId, blockIndex, _UniformBuffers[name].Binding);
    }
}

void Shader::CacheAttributes()
{
    GLint size;
    GLenum type;

    GLchar name[256];
    GLsizei length;

    GLint count;
    glGetProgramiv(_glId, GL_ACTIVE_ATTRIBUTES, &count);
    for (GLint i = 0; i < count; ++i)
    {
        glGetActiveAttrib(_glId, (GLuint)i, sizeof(name), &length, &size, &type, name);
        _attributes.emplace(name, glGetAttribLocation(_glId, name));
    }
}

} // namespace dusk
