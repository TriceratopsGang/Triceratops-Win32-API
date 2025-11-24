// BaseWindow.h
#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <windows.h>

// This is a template class. DERIVED_TYPE will be replaced with whatever
// class inherits from BaseWindow (like MainWindow)
template <class DERIVED_TYPE>
class BaseWindow
{
public:
    // This static function is the actual window procedure that Windows calls.
    // It must be static because Windows needs a C-style function pointer,
    // not a pointer to a member function.
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE* pThis = NULL;

        // On the first message (WM_NCCREATE), extract the pointer to the
        // derived class instance that we passed to CreateWindowEx
        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;

            // Store the pointer in the window's user data so we can retrieve it later
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            // Store the window handle in the object so member functions can use it
            pThis->m_hwnd = hwnd;
        }
        else
        {
            // For all other messages, retrieve the pointer we stored earlier
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        // If we have a valid pointer, call the derived class's HandleMessage method
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            // If something went wrong, use the default window procedure
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BaseWindow() : m_hwnd(NULL) {}

    // Create creates and registers the window
    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        // Set up the window class
        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;  // Point to our static function
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();              // Get the class name from derived class

        // Register the window class
        RegisterClass(&wc);

        // Create the window. Pass "this" pointer as the last parameter.
        // This will be retrieved in WM_NCCREATE.
        m_hwnd = CreateWindowEx(
            dwExStyle,
            ClassName(),
            lpWindowName,
            dwStyle,
            x, y, nWidth, nHeight,
            hWndParent,
            hMenu,
            GetModuleHandle(NULL),
            this  // This pointer is passed to WM_NCCREATE
        );

        return (m_hwnd ? TRUE : FALSE);
    }

    // Simple getter for the window handle
    HWND Window() const { return m_hwnd; }

protected:
    // These are pure virtual functions that derived classes MUST implement

    // Return the window class name for this window type
    virtual PCWSTR ClassName() const = 0;

    // Handle window messages. Derived classes override this to respond to messages.
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    // The window handle, accessible to all derived classes
    HWND m_hwnd;
};

#endif