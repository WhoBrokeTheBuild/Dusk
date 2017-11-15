#ifndef DUSK_SCRIPT_HOST_HPP
#define DUSK_SCRIPT_HOST_HPP

#include <dusk/Config.hpp>

#include <dusk/core/Event.hpp>
#include <unordered_map>
#include <memory>

namespace dusk {

class ScriptHost
{
public:

    DISALLOW_COPY_AND_ASSIGN(ScriptHost);

    ScriptHost();
    virtual ~ScriptHost();

    void RunFile(const std::string& filename)
    {
        //_lua.script_file(filename);
        //luaL_loadfile(_lua, filename.c_str());
        //if (lua_pcall(_lua, 0, 0, 0)) {
        //    // Failed
        //}
    }

    void RunCode(const std::string& code)
    {
        //_lua.script(code);
    }

    //lua_State * GetLuaState()
    //{
    //    //return _lua.lua_state();
    //    return _lua;
    //}

    Event<> EvtCleanup;

private:

    //sol::state _lua;
    //lua_State * _lua;

}; // class ScriptHost

} // namespace dusk

#endif // DUSK_SCRIPT_HOST_HPP
