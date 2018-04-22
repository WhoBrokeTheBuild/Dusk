#include "PopupWindow.hpp"
#include "EditorWindow.hpp"

void PopupWindow::Close()
{
   GetParent()->ClosePopup(this);
}
