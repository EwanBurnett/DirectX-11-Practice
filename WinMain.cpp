#include "Window.h"
#include <sstream>


/*The entry point for graphical windows apps. The name 'WinMain' is just a convention.
  Uses STDCALL instead of CDecl calling convention, as defined by the CALLBACK macro.
https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain */

int CALLBACK WinMain(
    HINSTANCE   hInstance,      //Pointer to the current instance of the application.
    HINSTANCE   hPrevInstance,  //Pointer to the previous instance of the app. This parameter is almost always NULL. 
    LPSTR       lpCmdLine,      //The command line for the app.
    int         nCmdShow )      //Indicates to the program how it should be shown upon construction.
{
    Window wnd(1600, 900, "Knight DX11 Framework");
    
    //Evaluate Messages
    BOOL gResult;
    MSG msg; //An instance of the Windows API Message struct. See https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-msg
    while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) { 
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if (gResult == -1) {
        return -1;
    }
    else {
        return msg.wParam;
    }
    return 0;
}