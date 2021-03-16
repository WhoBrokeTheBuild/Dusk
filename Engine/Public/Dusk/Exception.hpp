#ifndef DUSK_EXCEPTION_HPP
#define DUSK_EXCEPTION_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Path.hpp>
#include <Dusk/String.hpp>

#include <stdexcept>

namespace Dusk {

typedef std::exception Exception;
typedef std::logic_error LogicError;
typedef std::invalid_argument InvalidArgument;
typedef std::runtime_error RuntimeError;
typedef std::out_of_range OutOfRange;

class FileNotFound : public Exception
{
public:

    FileNotFound(const Path& path)
        : _path(path)
    {
        _message = "File not found: '" + _path + "'";
    }

    const char * what() const noexcept override {
        return _message.c_str();
    }

    Path GetPath() const {
        return _path;
    }

private:

    Path _path;

    string _message;

}; // class FileNotFound

} // namespace Dusk

#endif // DUSK_EXCEPTION_HPP