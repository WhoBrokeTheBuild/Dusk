#include "dusk/core/ScriptHost.hpp"

#include <dusk/core/App.hpp>

namespace dusk {

ScriptHost::ScriptHost()
{
}

ScriptHost::~ScriptHost()
{
    // Cleanup callbacks before sol::state
    ClearCallbacks();
}

} // namespace dusk
