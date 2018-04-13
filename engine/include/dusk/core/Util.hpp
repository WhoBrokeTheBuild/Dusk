#ifndef DUSK_UTIL_HPP
#define DUSK_UTIL_HPP

#include <dusk/Config.hpp>

#include <string>

namespace dusk {

struct Box
{
public:

    glm::vec3 Min;
    glm::vec3 Max;

    Box() = default;
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

inline std::vector<std::string> GetAssetPaths()
{
    static std::vector<std::string> paths;

    if (paths.empty()) {
        std::stringstream ss(ASSET_PATH);
        std::string path;
        while (std::getline(ss, path, ':')) {
            paths.push_back(path);
        }
    }

    return paths;
}

size_t GetGLTypeSize(GLenum type);

/** Get a string representation of the shader type.
 * @param type The shader type, e.g. GL_FRAGMENT_SHADER.
 * @return The string representation of the shader type, e.g. "Fragment"
 */
std::string GetShaderTypeString(GLuint type);

void PrintCode(const std::string& code);

void CleanSlashes(std::string& path);

std::string GetDirname(std::string path);
std::string GetBasename(std::string path);
std::string GetExtension(std::string path);


template <typename F>
struct privDefer {
	F f;
	privDefer(F f) : f(f) {}
	~privDefer() { f(); }
};

template <typename F>
privDefer<F> defer_func(F f) {
	return privDefer<F>(f);
}

#define DEFER_1(x, y) x##y
#define DEFER_2(x, y) DEFER_1(x, y)
#define DEFER_3(x)    DEFER_2(x, __COUNTER__)
#define defer(code)   auto DEFER_3(_defer_) = defer_func([&](){code;})


} // namespace dusk

#endif // DUSK_UTIL_HPP
