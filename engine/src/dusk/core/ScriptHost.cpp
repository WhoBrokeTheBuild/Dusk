#include "dusk/core/ScriptHost.hpp"

#include <fstream>
#include <add_on/datetime/datetime.h>
#include <add_on/scriptarray/scriptarray.h>
#include <add_on/scriptstdstring/scriptstdstring.h>

#include <dusk/core/Log.hpp>
#include <dusk/core/App.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/scene/Scene.hpp>

namespace dusk {

void as_print(std::string& str) {
    printf("%s", str.c_str());
}

void MessageCallback(const asSMessageInfo *msg, void *param)
{
    if (msg->type == asMSGTYPE_INFORMATION)
	{
        DuskLogInfo("%s (%d:%d) %s", msg->section, msg->row, msg->col, msg->message);
    }
	else if (msg->type == asMSGTYPE_WARNING)
    {
        DuskLogWarn("%s (%d:%d) %s", msg->section, msg->row, msg->col, msg->message);
    }
    else if (msg->type == asMSGTYPE_ERROR)
    {
        DuskLogError("%s (%d:%d) %s", msg->section, msg->row, msg->col, msg->message);
    }
}

ScriptHost::ScriptHost()
{
    _as = asCreateScriptEngine();
    if (!_as) {
        fprintf(stderr, "Failed to create AngelScript Engine\n");
        return;
    }

    _as->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

    RegisterScriptDateTime(_as);
    RegisterScriptArray(_as, true);
    RegisterStdString(_as);

    _as->RegisterGlobalFunction("void print(string &in)", asFUNCTION(as_print), asCALL_CDECL);

    _as->RegisterObjectType("ScriptHost", 0, asOBJ_REF);

    _as->RegisterObjectBehaviour("ScriptHost", asBEHAVE_ADDREF, "void f()", asMETHOD(ScriptHost, Script_AddRef), asCALL_THISCALL);
    _as->RegisterObjectBehaviour("ScriptHost", asBEHAVE_RELEASE, "void f()", asMETHOD(ScriptHost, Script_Release), asCALL_THISCALL);

    _as->RegisterGlobalProperty("ScriptHost _ScriptHost", this);

    _as->RegisterObjectType("vec2", sizeof(glm::vec2), asOBJ_VALUE | asOBJ_POD);
    _as->RegisterObjectType("ivec2", sizeof(glm::ivec2), asOBJ_VALUE | asOBJ_POD);
    _as->RegisterObjectType("uvec2", sizeof(glm::uvec2), asOBJ_VALUE | asOBJ_POD);
    _as->RegisterObjectType("mat4", sizeof(glm::mat4), asOBJ_VALUE | asOBJ_POD);

    App::Script_Init(_as);
    UpdateContext::Script_Init(_as);
    RenderContext::Script_Init(_as);
    //Scene::Script_Init(_as);
    //Actor::Script_Init(_as);
}

ScriptHost::~ScriptHost()
{
    for (auto asCtx : _asContexts) {
        asCtx->Release();
    }

    if (_as) {
        _as->ShutDownAndRelease();
    }
}

bool ScriptHost::LoadFile(const std::string& filename)
{
    const auto& paths = GetAssetPaths();

    std::string fullPath;
    std::ifstream file;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath.c_str());
        file.open(fullPath);

        if (file.is_open()) break;
    }

    if (!file.is_open()) {
        fprintf(stderr, "Could not open file '%s'\n", filename.c_str());
        return false;
    }
    std::string script((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();

    asIScriptModule * mod = _as->GetModule(0, asGM_ALWAYS_CREATE);
    mod->AddScriptSection("script", script.data(), script.size());
    mod->Build();

    _asMain = _as->GetModule(0)->GetFunctionByDecl("void main()");

    DuskLogInfo("Finished loading script '%s'", fullPath.c_str());

    return true;
}

void ScriptHost::Run() {
    asIScriptContext  * asCtx = GetContextFromPool();

    asCtx->Prepare(_asMain);
    asCtx->Execute();

    GiveContextToPool(asCtx);
}

asIScriptContext * ScriptHost::GetContextFromPool()
{
    if (_asContexts.empty()) {
        return _as->CreateContext();
    }

    asIScriptContext * asCtx = _asContexts.back();
    _asContexts.pop_back();
    return asCtx;
}

void ScriptHost::GiveContextToPool(asIScriptContext * asCtx)
{
    _asContexts.push_back(asCtx);
}

} // namespace dusk
