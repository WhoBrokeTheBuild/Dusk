#include "dusk/core/Util.hpp"

#include <dusk/core/Log.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace dusk {

std::string _assetPath;

std::vector<std::string> _assetPaths;

void SetAssetPath(const std::string& path)
{
    DuskLogInfo("Setting Asset Path: %s", path.c_str());
    _assetPath = path;
    _assetPaths.clear();
}

std::string GetAssetPath()
{
    return _assetPath;
}

std::vector<std::string> GetAssetPaths()
{
    if (_assetPaths.empty()) {
        std::stringstream ss(GetAssetPath());
        std::string path;
        while (std::getline(ss, path, ':')) {
            if (path.empty()) continue;
            if (path.back() != '/') path.push_back('/');

            _assetPaths.push_back(path);
        }
        _assetPaths.push_back("");
        reverse(_assetPaths.begin(), _assetPaths.end());
    }
    return _assetPaths;
}

size_t GetGLTypeSize(GLenum type)
{
    switch (type)
    {
    case GL_FLOAT:              return sizeof(GLfloat);
    case GL_FLOAT_VEC2:         return sizeof(glm::vec2);
    case GL_FLOAT_VEC3:         return sizeof(glm::vec3);
    case GL_FLOAT_VEC4:         return sizeof(glm::vec4);
    case GL_DOUBLE:             return sizeof(GLdouble);
#ifdef GL_VERSION_4_1
    case GL_DOUBLE_VEC2:        return sizeof(glm::dvec2);
    case GL_DOUBLE_VEC3:        return sizeof(glm::dvec3);
    case GL_DOUBLE_VEC4:        return sizeof(glm::dvec4);
#endif // GL_VERSION_4_1
    case GL_INT:                return sizeof(GLint);
    case GL_INT_VEC2:           return sizeof(glm::ivec2);
    case GL_INT_VEC3:           return sizeof(glm::ivec3);
    case GL_INT_VEC4:           return sizeof(glm::ivec4);
    case GL_UNSIGNED_INT:       return sizeof(GLuint);
    case GL_UNSIGNED_INT_VEC2:  return sizeof(glm::uvec2);
    case GL_UNSIGNED_INT_VEC3:  return sizeof(glm::uvec3);
    case GL_UNSIGNED_INT_VEC4:  return sizeof(glm::uvec4);
    case GL_BOOL:               return sizeof(GLboolean);
    case GL_BOOL_VEC2:          return sizeof(glm::bvec2);
    case GL_BOOL_VEC3:          return sizeof(glm::bvec3);
    case GL_BOOL_VEC4:          return sizeof(glm::bvec4);
    case GL_FLOAT_MAT2:         return sizeof(glm::mat2);
    case GL_FLOAT_MAT3:         return sizeof(glm::mat3);
    case GL_FLOAT_MAT4:         return sizeof(glm::mat4);
    case GL_FLOAT_MAT2x3:       return sizeof(glm::mat2x3);
    case GL_FLOAT_MAT2x4:       return sizeof(glm::mat2x4);
    case GL_FLOAT_MAT3x2:       return sizeof(glm::mat3x2);
    case GL_FLOAT_MAT3x4:       return sizeof(glm::mat3x4);
    case GL_FLOAT_MAT4x2:       return sizeof(glm::mat4x2);
    case GL_FLOAT_MAT4x3:       return sizeof(glm::mat4x3);
#ifdef GL_VERSION_4_1
    case GL_DOUBLE_MAT2:        return sizeof(glm::dmat2);
    case GL_DOUBLE_MAT3:        return sizeof(glm::dmat3);
    case GL_DOUBLE_MAT4:        return sizeof(glm::dmat4);
    case GL_DOUBLE_MAT2x3:      return sizeof(glm::dmat2x3);
    case GL_DOUBLE_MAT2x4:      return sizeof(glm::dmat2x4);
    case GL_DOUBLE_MAT3x2:      return sizeof(glm::dmat3x2);
    case GL_DOUBLE_MAT3x4:      return sizeof(glm::dmat3x4);
    case GL_DOUBLE_MAT4x2:      return sizeof(glm::dmat4x2);
    case GL_DOUBLE_MAT4x3:      return sizeof(glm::dmat4x3);
#endif // GL_VERSION_4_1

    case GL_RED:                return 1;
    case GL_RG:                 return 2;
    case GL_RGB:                return 3;
    case GL_BGR:                return 3;
    case GL_RGBA:               return 4;
    case GL_BGRA:               return 4;
    case GL_DEPTH_COMPONENT:    return 1;
    case GL_DEPTH_STENCIL:      return 2;

    // TODO: Finish
    }

    return 0;
}

std::string GetShaderTypeString(GLuint type)
{
    switch (type)
    {
    case GL_VERTEX_SHADER:
        return "Vertex";

    case GL_FRAGMENT_SHADER:
        return "Fragment";

    case GL_GEOMETRY_SHADER:
        return "Geometry";

#ifdef GL_VERSION_4_0
    case GL_TESS_CONTROL_SHADER:
        return "Tessellation Control";

    case GL_TESS_EVALUATION_SHADER:
        return "Tessellation Evaluation";
#endif // GL_VERSION_4_0
#ifdef GL_VERSION_4_3
    case GL_COMPUTE_SHADER:
        return "Compute";
#endif // GL_VERSION_4_3

    default:
        break;
    }

    return "Unknown";
}

void PrintCode(const std::string& code)
{
    std::istringstream iss(code);

    unsigned int lineNum = 1;
    std::string line;
    while (std::getline(iss, line))
    {
        if (lineNum < 10) printf(" ");

        printf("%d: %s\n", lineNum++, line.c_str());
    }
}

void CleanSlashes(std::string& path)
{
    for (unsigned int i = 0; i < path.size(); ++i)
    {
        if (path[i] == '\\')
        {
            path[i] = '/';
        }
    }
}

std::vector<std::string> StringSplit(const char& sep, const std::string& str)
{
    size_t it, left;
    std::vector<std::string> parts;

    if (str.empty()) return parts;

    left = 0;
    it = str.find(sep);
    while (it != std::string::npos) {
        if (left != it) {
            parts.push_back(str.substr(left, it - left));
        }
        left = ++it;
        it = str.find(sep, left);
    }

    if (it == std::string::npos && left < str.size()) {
        parts.push_back(str.substr(left));
    }

    return parts;
}

std::string StringJoin(const std::string& sep, const std::vector<std::string>& strs)
{
    std::string str;

    for (const std::string& s : strs) {
        str += s;
        str += sep;
    }

    if (!str.empty()) {
        str.pop_back();
    }

    return str;
}

std::string GetDirname(std::string path)
{
    CleanSlashes(path);
    size_t pivot = path.find_last_of('/');
    return (pivot == std::string::npos
        ? "./"
        : path.substr(0, pivot));
}

std::string GetBasename(std::string path)
{
    CleanSlashes(path);
    size_t pivot = path.find_last_of('/');
    return (pivot == std::string::npos
        ? std::string()
        : path.substr(pivot + 1));
}

std::string GetExtension(std::string path)
{
    size_t pivot = path.find_last_of('.');
    return (pivot == std::string::npos
        ? std::string()
        : path.substr(pivot + 1));
}

std::string RunCommand(const std::string& cmd)
{
    std::string result;

#ifdef __linux__

    std::array<char, 128> buffer;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
        {
            result += buffer.data();
        }
    }

#endif // __linux__

    // TODO: Windows

    return result;
}

} // namespace dusk
