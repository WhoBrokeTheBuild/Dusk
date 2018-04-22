#include "EditorWindow.hpp"
#include "PopupWindow.hpp"

void EditorWindow::Render()
{
    if (_shown) {
        DoRender();
    }

    for (auto& w : _popupWindows) { w->Render(); }

    for (auto& w : _toRemove) {
        auto it = std::find_if(_popupWindows.begin(), _popupWindows.end(),
            [w](std::unique_ptr<PopupWindow>& check){
                return (check.get() == w);
            }
        );
        if (it != _popupWindows.end()) {
            _popupWindows.erase(it);
        }
    }
    _toRemove.clear();
}

void EditorWindow::AddPopup(std::unique_ptr<PopupWindow>&& popup)
{
    _popupWindows.push_back(std::move(popup));
}

void EditorWindow::ClosePopup(PopupWindow * window)
{
    _toRemove.push_back(window);
}
