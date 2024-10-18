#ifndef DUSK_NON_COPYABLE_HPP
#define DUSK_NON_COPYABLE_HPP

#include <Dusk/Macros.hpp>

namespace dusk {

class DUSK_API NonCopyable
{
protected:

    NonCopyable() = default;

    NonCopyable(NonCopyable&&) = default;

    virtual ~NonCopyable() = default;

    NonCopyable(NonCopyable&) = delete;

    NonCopyable(const NonCopyable&) = delete;

    NonCopyable& operator=(NonCopyable&) = delete;

    NonCopyable& operator=(const NonCopyable&) = delete;

}; // class NonCopyable

} // namespace dusk

#endif // DUSK_NON_COPYABLE_HPP