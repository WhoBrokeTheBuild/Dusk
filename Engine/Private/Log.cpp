#include <Dusk/Log.hpp>

#include <print>

namespace dusk {

void LogMessage(StringView tag, StringView message)
{
    std::print("({}) {}\n", tag, message);
}

} // namespace dusk