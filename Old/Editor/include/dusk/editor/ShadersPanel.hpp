#ifndef DUSK_SHADER_WINDOW_HPP
#define DUSK_SHADER_WINDOW_HPP

#include "EditorConfig.hpp"
#include "EditorPanel.hpp"

class ShadersPanel : public EditorPanel
{
public:

    ShadersPanel(Editor * editor);

protected:

    virtual void DoReset() override;
    virtual void DoApply() override;
    virtual void DoRender() override;

private:

};

#endif // DUSK_SHADER_WINDOW_HPP
