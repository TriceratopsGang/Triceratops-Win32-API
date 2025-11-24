#include <windows.h>
#include "MyWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Create an instance of MainWindow
    MyWindow window;

    // Attempt to create the window
    if (!window.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW))
    {
        MessageBox(NULL, L"Failed to create window", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Show the window
    ShowWindow(window.Window(), nCmdShow);

    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}