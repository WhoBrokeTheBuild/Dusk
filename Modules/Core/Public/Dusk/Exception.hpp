#ifndef DUSK_EXCEPTION_HPP
#define DUSK_EXCEPTION_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Path.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Log.hpp>

#include <stdexcept>

namespace Dusk {

class DUSK_CORE_API Exception : public std::exception
{
public:

    Exception(string_view message) noexcept
        : _message(message)
    { }

    template <class... Args>
    Exception(string_view format, const Args&... args) noexcept
        : _message(fmt::format(format, args...))
    { }

    const char * what() const noexcept override {
        return _message.c_str();
    }

private:

    string _message;

}; // class Exception

class DUSK_CORE_API InvalidArgument : public Exception
{
public:

    template <class Name>
    InvalidArgument(const Name& name) noexcept
        : Exception("Invalid argument: {}", name)
    { }

    template <class Name>
    InvalidArgument(const Name& name, string_view message) noexcept
        : Exception("Invalid argument: {} {}", name, message)
    { }

    template <class Name, class... Args>
    InvalidArgument(const Name& name, string_view format, const Args&... args) noexcept
        : Exception("Invalid argument: {} {}", name, fmt::format(format, args...))
    { }

}; // class InvalidArgument

class DUSK_CORE_API OutOfRange : public Exception
{
public:

    OutOfRange(string_view argument) noexcept
        : Exception("Out of range: {}", argument)
    { }

    template <class Min, class Max>
    OutOfRange(string_view argument, Min min, Max max) noexcept
        : Exception("Out of range: {} <= {} <= {}", min, argument, max)
    { }

}; // class OutOfRange

class DUSK_CORE_API FileNotFound : public Exception
{
public:

    FileNotFound(const Path& path) noexcept
        : Exception("File not found: '{}'", path)
        , _path(path)
    { }

    Path GetPath() const noexcept {
        return _path;
    }

private:

    Path _path;

}; // class FileNotFound

} // namespace Dusk

#endif // DUSK_EXCEPTION_HPP