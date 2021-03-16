#include <Dusk/OpenGL/OpenGLShader.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/Benchmark.hpp>

#include <fstream>
#include <vector>
#include <sstream>
#include <functional>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
bool OpenGLShader::LoadFromFiles(const std::vector<string>& filenameList, bool useAssetPath /*= true*/)
{
    DuskBenchmarkStart();

    std::vector<GLuint> shaders;

    for (const auto& filename : filenameList) {
        GLuint shader = 0;

        Path path = filename;
        string ext = path.GetExtension();

        if (ext == "spv") {
            shader = LoadSPV(path, useAssetPath);
        }
        else if (ext == "glsl") {
            shader = LoadGLSL(path, useAssetPath);
        }
        else {
            shader = LoadSPV(path + ".spv", useAssetPath);
            if (shader == 0) {
                shader = LoadGLSL(path + ".glsl", useAssetPath);
            }
        }

        if (shader == 0) {
            for (GLuint shader : shaders) {
                glDeleteShader(shader);
            }

            LogError(DUSK_ANCHOR, "Failed to load '{}'", path);
            return false;
        }

        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<GLchar> log(logLength);
            glGetShaderInfoLog(shader, logLength, &logLength, log.data());

            for (GLuint shader : shaders) {
                glDeleteShader(shader);
            }

            LogError(DUSK_ANCHOR, "Failed to compile shader '{}'\n{}", path, log.data());
            return false;
        }

        shaders.push_back(shader);
    }

    _glID = glCreateProgram();
    if (!_glID) {
        LogError(DUSK_ANCHOR, "Failed to create shader program");
        return false;
    }

    for (GLuint shader : shaders) {
        glAttachShader(_glID, shader);
    }

    glLinkProgram(_glID);
    
    GLint linked = GL_FALSE;
    glGetProgramiv(_glID, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint logLength = 0;
        glGetProgramiv(_glID, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> log(logLength);
        glGetProgramInfoLog(_glID, logLength, &logLength, log.data());

        glDeleteProgram(_glID);
        _glID = 0;

        for (GLuint shader : shaders) {
            glDeleteShader(shader);
        }

        LogError(DUSK_ANCHOR, "Failed to link shader\n{}", log.data());
        return false;
    }

    for (GLuint shader : shaders) {
        glDetachShader(_glID, shader);
    }

    DuskBenchmarkEnd();
    return true;
}

DUSK_OPENGL_API
void OpenGLShader::Bind()
{
    glUseProgram(_glID);
}

DUSK_OPENGL_API
GLuint OpenGLShader::GetID()
{
    return _glID;
}

DUSK_OPENGL_API
GLuint OpenGLShader::LoadSPV(const Path& path, bool useAssetPath)
{
    LogVerbose(DUSK_ANCHOR, "Looking for SPIR-V shader '{}'", path);

    std::ifstream file;

    if (useAssetPath) {
        for (const auto& assetPath : GetAssetPathList()) {
            Path fullPath = assetPath / "Shaders" / path;
            LogVerbose(DUSK_ANCHOR, "Checking '{}'", fullPath);
            
            file.open(fullPath, std::ios::binary);
            if (file.is_open()) {
                break;
            }
        }
    }
    else {
        file.open(path, std::ios::binary);
    }

    if (!file.is_open()) {
        return 0;
    }

    file.unsetf(std::ios::skipws);

    LogVerbose(DUSK_ANCHOR, "Loading SPIR-V shader '{}'", path);

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    GLenum type = GetGLShaderType(path);
    if (type == GL_INVALID_ENUM) {
        LogError(DUSK_ANCHOR, "Failed to determine shader type of '{}'", path);
        return 0;
    }

    GLuint shader = glCreateShader(type);

    glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, data.data(), data.size());

    glSpecializeShader(shader, "main", 0, nullptr, nullptr);

    return shader;
}

DUSK_OPENGL_API
GLuint OpenGLShader::LoadGLSL(const Path& path, bool useAssetPath)
{
    LogVerbose(DUSK_ANCHOR, "Looking for GLSL shader '{}'", path);

    const auto& assetPathList = GetAssetPathList();

    std::ifstream file;

    if (useAssetPath) {
        for (const auto& assetPath : assetPathList) {
            Path fullPath = assetPath / "Shaders" / path;
            LogVerbose(DUSK_ANCHOR, "Checking '{}'", fullPath);
            
            file.open(fullPath);
            if (file.is_open()) {
                break;
            }
        }
    }
    else {
        file.open(path);
    }

    if (!file.is_open()) {
        return 0;
    }
    
    LogVerbose(DUSK_ANCHOR, "Loading GLSL shader '{}'", path);

    string code(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    // TODO: Support relative includes, without asset path

    std::function<string(string)> preprocessGLSL = 
        [&](string code) -> string {
            std::istringstream iss(code);
            string outCode;
            string line;

            while (std::getline(iss, line)) {
                if (line.compare(0, sizeof("#include")-1, "#include") == 0) {
                    size_t left = line.find('"');
                    size_t right = line.rfind('"');

                    if (left == string::npos || right == string::npos) {
                        left = line.find('<');
                        right = line.rfind('>');

                        if (left == string::npos || right == string::npos) {
                            LogError(DUSK_ANCHOR, "Unable to parse filename from shader include");
                            return "";
                        }
                    }

                    string incFilename = line.substr(left + 1, right - left - 1);
                    std::ifstream incFile;

                    for (const auto& path : assetPathList) {
                        Path fullPath = path / "Shaders" / incFilename;
                        LogVerbose(DUSK_ANCHOR, "Checking '{}'", fullPath);
                        
                        incFile.open(fullPath);
                        if (incFile.is_open()) {
                            break;
                        }
                    }

                    if (!incFile.is_open()) {
                        LogError(DUSK_ANCHOR, "Unable to find shader include '{}'", incFilename);
                        return "";
                    }

                    string incCode(
                        (std::istreambuf_iterator<char>(incFile)),
                        std::istreambuf_iterator<char>()
                    );

                    outCode += preprocessGLSL(incCode);
                }
                else {
                    outCode += line + "\n";
                }
            }

            return outCode;
        };

    code = preprocessGLSL(code);

    GLenum type = GetGLShaderType(path);
    if (type == GL_INVALID_ENUM) {
        LogError(DUSK_ANCHOR, "Unable to determine shader type of '{}'", path);
        return 0;
    }

    GLuint shader = glCreateShader(type);
    
    const char * ptr = code.c_str();
    glShaderSource(shader, 1, (const GLchar **)&ptr, nullptr);

    glCompileShader(shader);

    return shader;
}

DUSK_OPENGL_API
GLenum OpenGLShader::GetGLShaderType(const Path& path)
{
    string filename = path.GetFilename();
    string ext = path.GetExtension();

    if (ext == "spv" || ext == "glsl") {
        size_t pivot = filename.find_last_of('.');
        if (pivot == string::npos) {
            return GL_INVALID_ENUM;
        }
        ext = GetExtension(filename.substr(0, pivot));
    }

    if (ext == "vert" || ext == "vertex") {
        return GL_VERTEX_SHADER;
    }
    else if (ext == "frag" || ext == "fragment") {
        return GL_FRAGMENT_SHADER;
    }
    else if (ext == "tesc" || ext == "tesscontrol") {
        return GL_TESS_CONTROL_SHADER;
    }
    else if (ext == "tese" || ext == "tesseval") {
        return GL_TESS_EVALUATION_SHADER;
    }
    else if (ext == "comp" || ext == "compute") {
        return GL_COMPUTE_SHADER;
    }

    return GL_INVALID_ENUM;
}

} // namespace Dusk::OpenGL
