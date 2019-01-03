#ifndef DUSK_SCRIPT_HOST_HPP
#define DUSK_SCRIPT_HOST_HPP

#include <dusk/Config.hpp>

#include <string>
#include <vector>

namespace dusk {

class ScriptHost
{
public:

    DISALLOW_COPY_AND_ASSIGN(ScriptHost)

    ScriptHost() = default;
    virtual ~ScriptHost() = default;

    bool LoadFile(const std::string& filename);

    void Run();

private:

}; // class ScriptHost

} // namespace dusk

#endif // DUSK_SCRIPT_HOST_HPP
