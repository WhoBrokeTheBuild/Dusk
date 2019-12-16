#include <Dusk/OpenGL/Shader.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>

#include <fstream>
#include <vector>

namespace Dusk::OpenGL {

bool Shader::LoadFromFiles(const std::vector<std::string>& filenames)
{
    DuskBenchmarkStart();

    std::vector<GLuint> shaders;

    for (const auto& filename : filenames) {
        GLuint shader = 0;

        const std::string& ext = GetExtension(filename);

        if (ext == "spv") {
            shader = LoadSPV(filename);
        }
        else if (ext == "glsl") {
            shader = LoadGLSL(filename);
        }
        else {
            shader = LoadSPV(filename + ".spv");
            if (shader == 0) {
                shader = LoadGLSL(filename + ".glsl");
            }
        }

        if (shader == 0) {
            for (GLuint shader : shaders) {
                glDeleteShader(shader);
            }

            DuskLogError("Failed to load '%s'");
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

            DuskLogError("Failed to compile shader '%s'\n%s", filename, log.data());
            return false;
        }

        shaders.push_back(shader);
    }

    _glID = glCreateProgram();
    if (!_glID) {
        DuskLogError("Failed to create shader program");
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

        DuskLogError("Failed to link shader\n%s", log.data());
        return false;
    }

    for (GLuint shader : shaders) {
        glDetachShader(_glID, shader);
    }

    DuskBenchmarkEnd("OpenGL::Shader::LoadFromFiles");
    return true;
}

void Shader::Bind()
{
    glUseProgram(_glID);
}

GLuint Shader::LoadSPV(const std::string& filename)
{
    DuskLogVerbose("Looking for SPIR-V shader '%s'", filename);

    std::ifstream file(filename, std::ios::binary);
    file.unsetf(std::ios::skipws);

    if (!file) {
        return 0;
    }

    DuskLogLoad("Loading SPIR-V shader '%s'", filename);

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    GLenum type = GetGLShaderType(filename);
    if (type == GL_INVALID_ENUM) {
        DuskLogError("Unable to determine shader type of '%s'", filename);
    }

    GLuint shader = glCreateShader(type);

    glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, data.data(), data.size());

    glSpecializeShader(shader, "main", 0, nullptr, nullptr);

    return shader;
}

GLuint Shader::LoadGLSL(const std::string& filename)
{
    DuskLogVerbose("Looking for GLSL shader '%s'", filename);

    std::ifstream file(filename);

    if (!file) {
        return 0;
    }
    
    DuskLogLoad("Loading GLSL shader '%s'", filename);

    std::string data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    GLenum type = GetGLShaderType(filename);
    if (type == GL_INVALID_ENUM) {
        DuskLogError("Unable to determine shader type of '%s'", filename);
    }

    GLuint shader = glCreateShader(type);
    
    const char * ptr = data.c_str();
    glShaderSource(shader, 1, (const GLchar **)&ptr, nullptr);

    glCompileShader(shader);

    return shader;
}

GLenum Shader::GetGLShaderType(const std::string& filename)
{
    // Strip .glsl or .spv
    size_t pivot = filename.find_last_of('.');
    if (pivot == std::string::npos) {
        return GL_INVALID_ENUM;
    }

    const auto& ext = GetExtension(filename.substr(0, pivot));
    if (ext == "vert") {
        return GL_VERTEX_SHADER;
    }
    else if (ext == "frag") {
        return GL_FRAGMENT_SHADER;
    }
    // TODO

    return GL_INVALID_ENUM;
}

} // namespace Dusk::OpenGL
