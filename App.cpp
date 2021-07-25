#include "App.h"

App::App() :
    wnd(1600, 900, "KnightApp Win")
{}

int App::Go()
{
    //Evaluate Messages
    BOOL gResult;
    MSG msg; //An instance of the Windows API Message struct. See https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-msg

    while ((gResult = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) > 0) {

        if (msg.message == WM_QUIT) {
            return msg.wParam; 
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);

        DoFrame();
    }

    if (gResult == -1) {
        return -1;
    }
    return msg.wParam;
}

void App::DoFrame()
{
    //Top level game logic
    const float t = timer.Peek();
}
