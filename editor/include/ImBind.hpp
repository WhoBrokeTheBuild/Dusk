#ifndef DUSK_EDITOR_IMBIND_HPP
#define DUSK_EDITOR_IMBIND_HPP

#include "Config.hpp"

class ImBind
{
public:

    static bool Init(SDL_Window * window, const char * glsl_version = nullptr);
    static void Term();

    static void NewFrame();

    static void RenderDrawData(ImDrawData * draw_data);

    static bool ProcessEvent(SDL_Event * event);

    static bool CreateDeviceObjects();
    static void InvalidateDeviceObjects();

    static void CreateFontsTexture();

    static const char * GetClipboardText(void *) {
        return SDL_GetClipboardText();
    }

    static void SetClipboardText(void *, const char * text) {
        SDL_SetClipboardText(text);
    }

private:

    static SDL_Window * _Window;

    // SDL data
    static Uint64       _Time;
    static bool         _MousePressed[3];
    static SDL_Cursor*  _MouseCursors[ImGuiMouseCursor_COUNT];

    // OpenGL data
    static char         _GlslVersion[32];
    static GLuint       _FontTexture;
    static int          _ShaderHandle, _VertHandle, _FragHandle;
    static int          _AttribLocationTex, _AttribLocationProjMtx;
    static int          _AttribLocationPosition, _AttribLocationUV, _AttribLocationColor;
    static unsigned int _VboHandle,_ElementsHandle;

}; // class ImBind

#endif // DUSK_EDITOR_IMBIND_HPP
