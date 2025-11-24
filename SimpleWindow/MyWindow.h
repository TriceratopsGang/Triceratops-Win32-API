#include "BaseWindow.h"

class MyWindow : public BaseWindow<MyWindow>
{
public:
    PCWSTR ClassName() const { return L"MyWindow"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};