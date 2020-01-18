#ifndef DUSK_VERSION_HPP
#define DUSK_VERSION_HPP

#include <Dusk/Config.hpp>

#include <string>

namespace Dusk {

struct DUSK_CORE_API Version 
{
    int Major;
    int Minor;
    int Patch;

    Version(int major = 0, int minor = 0, int patch = 0)
        : Major(major)
        , Minor(minor)
        , Patch(patch)
    { }

    Version(const Version& rhs)
        : Major(rhs.Major)
        , Minor(rhs.Minor)
        , Patch(rhs.Patch)
    { }

    inline Version& operator=(const Version& rhs)
    {
        Major = rhs.Major;
        Minor = rhs.Minor;
        Patch = rhs.Patch;
        return *this;
    }

    inline bool operator==(const Version& rhs)
    {
        return (Compare(*this, rhs) == 0);
    }

    inline bool operator!=(const Version& rhs)
    {
        return !(*this == rhs);
    }

    inline friend bool operator>(const Version& lhs, const Version& rhs)
    {
        return (Version::Compare(lhs, rhs) == 1);
    }

    inline friend bool operator>=(const Version& lhs, const Version& rhs)
    {
        int res = Version::Compare(lhs, rhs);
        return (res == 0 || res == 1);
    }

    inline friend bool operator<(const Version& lhs, const Version& rhs)
    {
        return (Version::Compare(lhs, rhs) == -1);
    }

    inline friend bool operator<=(const Version& lhs, const Version& rhs)
    {
        int res = Version::Compare(lhs, rhs);
        return (res == 0 || res == -1);
    }

    inline std::string GetString() const
    {
        return std::to_string(Major) + 
            "." + 
            std::to_string(Minor) + 
            "." + 
            std::to_string(Patch);
    }

    inline static int Compare(const Version& a, const Version& b)
    {
        int result = CompareSingle(a.Major, b.Major);
        if (result == 0) {
            result = CompareSingle(a.Minor, b.Minor);
            if (result == 0) {
                result = CompareSingle(a.Patch, b.Patch);
            }
        }
        return result;
    }

    inline static int CompareSingle(int a, int b)
    {
        if (a == b) {
            return 0;
        }
        if (a > b) {
            return 1;
        }
        return -1;
    }

}; // struct Version

} // namespace Dusk

#endif // DUSK_VERSION_HPP