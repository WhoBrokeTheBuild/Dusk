#include "dusk/core/ScriptHost.hpp"

#include <dusk/core/App.hpp>

namespace dusk {

ScriptHost::ScriptHost()
{
    _lua.open_libraries(sol::lib::base, sol::lib::math);
    _lua.new_usertype<ScriptHost>("ScriptHost");

    _lua["this_script_host"] = this;

    App::LuaSetup(_lua);
}

ScriptHost::~ScriptHost()
{
    // Cleanup callbacks before sol::state
    ClearCallbacks();
}

} // namespace dusk
