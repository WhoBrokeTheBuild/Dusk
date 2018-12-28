#ifndef DUSK_UTIL_HPP
#define DUSK_UTIL_HPP

#include <dusk/Config.hpp>

using glm::vec3;

#include <string>
using std::string;

#include <vector>
using std::vector;

namespace dusk {

struct Box
{
public:

    vec3 Min;
    vec3 Max;

    Box() = default;
    Box(vec3 min, vec3 max)
        : Min(min)
        , Max(max)
    { }

    vec3 GetSize() { return Max - Min; }

    Box operator+(const Box& rhs)
    {
        Box box;
        box.Min = min(Min, rhs.Min);
        box.Max = max(Max, rhs.Max);
        return box;
    }

    Box& operator+=(const Box& rhs)
    {
        Min = min(Min, rhs.Min);
        Max = max(Max, rhs.Max);
        return *this;
    }

};

void SetAssetPath(const string& path);
string GetAssetPath();

vector<string> GetAssetPaths();

size_t GetGLTypeSize(GLenum type);

/** Get a string representation of the shader type.
 * @param type The shader type, e.g. GL_FRAGMENT_SHADER.
 * @return The string representation of the shader type, e.g. "Fragment"
 */
string GetShaderTypeString(GLuint type);

void PrintCode(const string& code);

void CleanSlashes(string& path);

vector<string> StringSplit(const char& sep, const string& str);
string StringJoin(const string& sep, const vector<string>& strs);

inline string StringJoin(const char& sep, const vector<string>& strs)
{
    return StringJoin(string(1, sep), strs);
}

string GetDirname(string path);
string GetBasename(string path);
string GetExtension(string path);

string RunCommand(const string& cmd);

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
