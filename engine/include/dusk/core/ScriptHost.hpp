#ifndef DUSK_SCRIPT_HOST_HPP
#define DUSK_SCRIPT_HOST_HPP

#include <dusk/Config.hpp>

#include <string>
#include <vector>

namespace dusk {

class IScriptRef
{
public:

    int Script_AddRef() { return 1; }
    int Script_Release() { return 1; }

}; // class IScriptRef

class ScriptHost : public IScriptRef
{
public:

    DISALLOW_COPY_AND_ASSIGN(ScriptHost)

    ScriptHost();
    virtual ~ScriptHost();

    int Script_AddRef() { return 1; }
    int Script_Release() { return 1; }

    //void Serialize(nlohmann::json& data);
    //void Deserialize(nlohmann::json& data);

    bool LoadFile(const std::string& filename);

    void Run();

    asIScriptEngine * GetEngine() const { return _as; }

    asIScriptContext * GetContextFromPool();
    void GiveContextToPool(asIScriptContext * asCtx);

private:

    std::vector<asIScriptContext*> _asContexts;

    asIScriptEngine * _as;
    asIScriptFunction * _asMain;

}; // class ScriptHost

} // namespace dusk

#endif // DUSK_SCRIPT_HOST_HPP
