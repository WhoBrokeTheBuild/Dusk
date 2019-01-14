#ifndef DUSK_UTIL_HPP
#define DUSK_UTIL_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Math.hpp>
#include <dusk/core/OpenGL.hpp>

#include <string>
#include <vector>

#ifndef DUSK_ASSET_PATH
#define DUSK_ASSET_PATH "."
#endif // DUSK_ASSET_PATH

namespace dusk {

static glm::vec3 GetWorldUp() {
	return glm::vec3(0.f, 1.f, 0.f);
}

static glm::vec3 GetWorldForward() {
	return glm::vec3(0.f, 0.f, -1.f);
}

struct Box
{
public:

    glm::vec3 Min;
    glm::vec3 Max;

    Box()
        : Min(std::numeric_limits<float>::max())
        , Max(std::numeric_limits<float>::min())
    { }

    Box(glm::vec3 min, glm::vec3 max)
        : Min(min)
        , Max(max)
    { }

    glm::vec3 GetSize() { return Max - Min; }

    Box operator+(const Box& rhs)
    {
        Box box;
        box.Min = glm::min(Min, rhs.Min);
        box.Max = glm::max(Max, rhs.Max);
        return box;
    }

    Box& operator+=(const Box& rhs)
    {
        Min = glm::min(Min, rhs.Min);
        Max = glm::max(Max, rhs.Max);
        return *this;
    }

};

void SetAssetPath(const std::string& path);
std::string GetAssetPath();

std::vector<std::string> GetAssetPaths();

size_t GetGLTypeSize(GLenum type);

/** Get a string representation of the shader type.
 * @param type The shader type, e.g. GL_FRAGMENT_SHADER.
 * @return The string representation of the shader type, e.g. "Fragment"
 */
std::string GetShaderTypeString(GLuint type);

void PrintCode(const std::string& code);

void CleanSlashes(std::string& path);

std::vector<std::string> StringSplit(const char& sep, const std::string& str);
std::string StringJoin(const std::string& sep, const std::vector<std::string>& strs);

inline std::string StringJoin(const char& sep, const std::vector<std::string>& strs)
{
    return StringJoin(std::string(1, sep), strs);
}

std::string GetDirname(std::string path);
std::string GetBasename(std::string path);
std::string GetExtension(std::string path);

std::string RunCommand(const std::string& cmd);

static inline bool IsBase64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string Base64Decode(std::string const& encoded);

} // namespace dusk

#endif // DUSK_UTIL_HPP
