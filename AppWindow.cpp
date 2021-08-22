#include "AppWindow.h"
#include <assert.h>

//Window Class Singleton
Window::WindowClass Window::WindowClass::wndClass;

const char* Window::WindowClass::GetName()
{
    return wndClassName;
}

const HINSTANCE Window::WindowClass::GetInstance()
{
    return wndClass.hInst;
}

Window::WindowClass::WindowClass() :
    hInst( GetModuleHandle( nullptr ))
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = Window::MsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = nullptr;

    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName,  hInst);
}

Window::Window(int width, int height, const char* name):
    width(width), height(height)
{
    //Calculates window size based on desired client region size
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    //Create the window, and get hWnd

    hWnd = CreateWindow(
        WindowClass::GetName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, WindowClass::GetInstance(), this
    );
    
    //Show the window
    ShowWindow(hWnd, SW_SHOWDEFAULT);

    //Initialize our Graphics
    mGfx = std::make_unique<Graphics>();
    assert(mGfx->Init(hWnd, width, height));
}

Window::~Window()
{
    DestroyWindow(hWnd);
}

Graphics& Window::Gfx()
{
    return(*mGfx);
}

GameTimer& Window::Timer()
{
    return(mTimer);
}

LRESULT Window::MsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    OutputDebugString(">> Setting up message handler\n");

    //using create parameter passed through CreateWindow() to store a handle to the window class
    if (msg == WM_NCCREATE) {
        //Gets the pointer from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

        //Set WinAPI managed userdata to store window class handle
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

        //Set message procedure to Process, now that setup is complete
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::MsgProcess));

        //Forward the message to window class handler
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    //if a message is recieved before WM_NCCREATE, handle using default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::MsgProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //Retrieve handle to window class
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    //Forward the message to window class handler
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CLOSE:                  //Windows messages are all macroed, so you don't need to remember each code. see https://wiki.winehq.org/List_Of_Windows_Messages
        PostQuitMessage(0xD3D11);         //Tells our application to terminate execution with a custom exit code.
        break;

    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            mTimer.Stop();
            break;
        }
        else {
            Gfx().SetWireframeMode(true);
            break;
        }

    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            mTimer.Start();
            break;
        }
        else {
            Gfx().SetWireframeMode(false);
            break;
        }

    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
