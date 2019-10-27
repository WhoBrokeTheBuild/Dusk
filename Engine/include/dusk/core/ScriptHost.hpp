#ifndef DUSK_SCRIPT_HOST_HPP
#define DUSK_SCRIPT_HOST_HPP

#include <dusk/core/Macros.hpp>

#include <string>

namespace dusk {

class ScriptHost
{
public:

    DISALLOW_COPY_AND_ASSIGN(ScriptHost)

    ScriptHost();
    virtual ~ScriptHost();

    bool RunFile(const std::string&);

private:

}; // class ScriptHost

} // namespace dusk

#endif // DUSK_SCRIPT_HOST_HPP
