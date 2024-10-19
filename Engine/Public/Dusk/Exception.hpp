#ifndef DUSK_EXCEPTION_HPP
#define DUSK_EXCEPTION_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/String.hpp>

#include <format>

#include <exception>

namespace dusk {

class DUSK_API Exception : public std::runtime_error
{
public:

    template <typename ...Args>
    Exception(StringView format, Args... args)
        : std::runtime_error(std::vformat(format, std::make_format_args(args...)))
    { }

    using std::runtime_error::what;

}; // class Exception

} // namespace dusk

#endif // DUSK_EXCEPTION_HPP