#ifndef DUSK_CONTEXT_HPP
#define DUSK_CONTEXT_HPP

#include <dusk/Config.hpp>
#include <dusk/core/ScriptHost.hpp>

#include <chrono>

namespace dusk
{

class Shader;
class Camera;

struct UpdateContext : public IScriptRef
{
    float TargetFPS = 60.0f;
    float CurrentFPS;

    float DeltaTime;

    double ElapsedTime;
    double TotalTime;

    static void Script_Init(asIScriptEngine * as)
    {
        as->RegisterObjectType("UpdateContext", 0, asOBJ_REF);

        as->RegisterObjectBehaviour("UpdateContext", asBEHAVE_ADDREF, "void f()", asMETHOD(UpdateContext, Script_AddRef), asCALL_THISCALL);
    	as->RegisterObjectBehaviour("UpdateContext", asBEHAVE_RELEASE, "void f()", asMETHOD(UpdateContext, Script_Release), asCALL_THISCALL);

        as->RegisterObjectProperty("UpdateContext", "float TargetFPS", asOFFSET(UpdateContext, TargetFPS));
        as->RegisterObjectProperty("UpdateContext", "float CurrentFPS", asOFFSET(UpdateContext, CurrentFPS));
        as->RegisterObjectProperty("UpdateContext", "float DeltaTime", asOFFSET(UpdateContext, DeltaTime));
        as->RegisterObjectProperty("UpdateContext", "double ElapsedTime", asOFFSET(UpdateContext, ElapsedTime));
        as->RegisterObjectProperty("UpdateContext", "double TotalTime", asOFFSET(UpdateContext, TotalTime));
    }

};

struct RenderContext : public IScriptRef
{
    int CurrentPass;

    Shader * CurrentShader = nullptr;

    Camera * CurrentCamera = nullptr;

    SDL_GLContext SDLGLContext;

    static void Script_Init(asIScriptEngine * as)
    {
        as->RegisterObjectType("RenderContext", 0, asOBJ_REF);

        as->RegisterObjectBehaviour("RenderContext", asBEHAVE_ADDREF, "void f()", asMETHOD(RenderContext, Script_AddRef), asCALL_THISCALL);
    	as->RegisterObjectBehaviour("RenderContext", asBEHAVE_RELEASE, "void f()", asMETHOD(RenderContext, Script_Release), asCALL_THISCALL);

        as->RegisterObjectProperty("RenderContext", "int CurrentPass", asOFFSET(RenderContext, CurrentPass));
        as->RegisterObjectProperty("RenderContext", "int CurrentShader", asOFFSET(RenderContext, CurrentShader));
        as->RegisterObjectProperty("RenderContext", "int CurrentCamera", asOFFSET(RenderContext, CurrentCamera));
    }
};

} // namespace dusk

#endif // DUSK_CONTEXT_HPP
