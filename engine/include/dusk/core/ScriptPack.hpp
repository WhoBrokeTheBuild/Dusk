#ifndef DUSK_SCRIPT_PACK_HPP
#define DUSK_SCRIPT_PACK_HPP

#include <dusk/Config.hpp>

#include <dusk/core/Context.hpp>
#include <dusk/core/ScriptHost.hpp>

namespace dusk {

inline void ScriptPack_None(asIScriptContext * asCtx)
{
}

inline void ScriptPack_UpdateContext(asIScriptContext * asCtx, UpdateContext& ctx)
{
    asCtx->SetArgObject(0, &ctx);
}

inline void ScriptPack_RenderContext(asIScriptContext * asCtx, RenderContext& ctx)
{
    asCtx->SetArgObject(0, &ctx);
}

inline void ScriptPack_Key_Flags(asIScriptContext * asCtx, Key key, Flags flags)
{
    asCtx->SetArgDWord(0, key);
    asCtx->SetArgDWord(1, flags);
}

inline void ScriptPack_Button_vec2_Flags(asIScriptContext * asCtx, Button button, glm::vec2 vec, Flags flags)
{
    asCtx->SetArgDWord(0, button);
    asCtx->SetArgObject(1, &vec);
    asCtx->SetArgDWord(2, flags);
}

inline void ScriptPack_vec2_vec2_Flags(asIScriptContext * asCtx, glm::vec2 vecA, glm::vec2 vecB, Flags flags)
{
    asCtx->SetArgObject(0, &vecA);
    asCtx->SetArgObject(1, &vecB);
    asCtx->SetArgDWord(2, flags);
}

inline void ScriptPack_vec2(asIScriptContext * asCtx, glm::vec2 vec)
{
    asCtx->SetArgObject(0, &vec);
}

inline void ScriptPack_ivec2(asIScriptContext * asCtx, glm::ivec2 vec)
{
    asCtx->SetArgObject(0, &vec);
}

inline void ScriptPack_string(asIScriptContext * asCtx, std::string str)
{
    asCtx->SetArgObject(0, &str);
}

} // namespace dusk

#endif // DUSK_SCRIPT_PACK_HPP
