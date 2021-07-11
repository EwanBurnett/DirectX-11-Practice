#include <windows.h>

/* A new Windows Procedure. This is how we'll be handling WinAPI messages. */
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {   

    switch (msg) {
    case WM_CLOSE:              //Windows messages are all macroed, so you don't need to remember each code.
        PostQuitMessage(11);    //Tells our application to terminate execution with a custom exit code.
        break;
    }
    return(DefWindowProc(hWnd, msg, wParam, lParam));
}

/*Classic program entry point. Not the one we want to be using, however.*/
/*
int main() {
    return 0;
}
*/


/*The entry point for graphical windows apps. The name 'WinMain' is just a convention.
  Uses STDCALL instead of CDecl calling convention, as defined by the CALLBACK macro.
https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain */

int CALLBACK WinMain(
    HINSTANCE   hInstance,      //Pointer to the current instance of the application.
    HINSTANCE   hPrevInstance,  //Pointer to the previous instance of the app. This parameter is almost always NULL. 
    LPSTR       lpCmdLine,      //The command line for the app.
    int         nCmdShow )      //Indicates to the program how it should be shown upon construction.
{
    
    //1. Register your window class

    const auto pClassName = L"SDx11";
    const auto pWindowName = L"DX11 Demo";

    /* https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa */
    WNDCLASSEX wc = { 0 };      //Create a Window Class to be registered. Good practice to 0 out configuration structures.
    wc.cbSize = sizeof(wc);     //The size of the struct in bytes.
    wc.style = CS_OWNDC;        //The style of the window class. see https://docs.microsoft.com/en-us/windows/win32/winmsg/window-class-styles for more. OR these to combine.
    wc.lpfnWndProc = WndProc;   //Pointer to the windows procedure. This is used in message handling. see https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms633573(v=vs.85) 
    wc.cbClsExtra = 0;          //How many extra bytes to allocate to the window class struct. 0 by default.
    wc.cbWndExtra = 0;          //Same as above, but for the Window instance. 
    wc.hIcon = nullptr;         //A handle to an icon resource. If null, uses the system default
    wc.hCursor = nullptr;       //A handle to a custom cursor resource. If null, will use the system default.
    wc.hbrBackground = nullptr; //Background Resource.
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = pClassName;  //The name of our window class. Must be constant across all calls. 
    wc.hIconSm = nullptr;           

    RegisterClassEx(&wc);      //Registers our new Window Class with the operating system. 


    //2. Create the window instance
    HWND hWnd = CreateWindowEx(
        0, pClassName, pWindowName,
        WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
        200, 200,
        1280, 720,
        nullptr, nullptr, hInstance, nullptr
    );

    //3. Show the window
    ShowWindow(hWnd, SW_SHOW);


    //4. Evaluate Messages
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
    //return 0;
}



