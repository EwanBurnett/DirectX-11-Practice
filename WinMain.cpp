#include "Window.h"
#include <sstream>
#include <memory>

std::unique_ptr<Window> mWnd;
void DoFrame();
void Update();

/*The entry point for graphical windows apps. The name 'WinMain' is just a convention.
  Uses STDCALL instead of CDecl calling convention, as defined by the CALLBACK macro.
https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain */

int CALLBACK WinMain(
    HINSTANCE   hInstance,      //Pointer to the current instance of the application.
    HINSTANCE   hPrevInstance,  //Pointer to the previous instance of the app. This parameter is almost always NULL. 
    LPSTR       lpCmdLine,      //The command line for the app.
    int         nCmdShow )      //Indicates to the program how it should be shown upon construction.
{
    //Create the application window
    OutputDebugString(">> Creating App Window\n");
    mWnd = std::make_unique<Window>(1280, 720, "DX11 Application");

    //Evaluate Messages
    BOOL gResult;
    MSG msg = { 0 }; //An instance of the Windows API Message struct. See https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-msg

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);

            DoFrame();
        }
    }

    return msg.wParam;
}

void DoFrame()
{
    mWnd->Gfx().ClearBuffer(0.5, 0.2, 0.5);
    Update();
    mWnd->Gfx().EndFrame();
}

void Update()
{
    double r = 0.3, g = 0.9, b = 0.4;

    mWnd->Gfx().ClearBuffer(r, g, b);

}
