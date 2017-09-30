#ifndef DUSK_EDITOR_HPP
#define DUSK_EDITOR_HPP

#include <dusk/Dusk.hpp>

// ImGUI
#define IMGUI_DISABLE_INCLUDE_IMCONFIG_H
#define IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>

class Editor : public dusk::App
{
public:

    Editor(int argc, char ** argv)
        : dusk::App(argc, argv)
    {
        DuskLogInfo("Running from the Editor");
    }
};

#endif // DUSK_EDITOR_HPP
