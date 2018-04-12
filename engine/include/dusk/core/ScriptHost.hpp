#ifndef DUSK_SCRIPT_HOST_HPP
#define DUSK_SCRIPT_HOST_HPP

#include <dusk/Config.hpp>

#include <dusk/core/Event.hpp>
#include <unordered_map>
#include <memory>

namespace dusk {

class ScriptHost : public ICallbackHost
{
public:

    DISALLOW_COPY_AND_ASSIGN(ScriptHost)

    ScriptHost();
    virtual ~ScriptHost();

    void RunFile(const std::string& filename) {
    }

    void RunCode(const std::string& code) {
    }

private:


}; // class ScriptHost

} // namespace dusk

#endif // DUSK_SCRIPT_HOST_HPP
