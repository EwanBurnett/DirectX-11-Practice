#pragma once
#include "KnightWin.h"

class Window {

    //Manages registration and cleanup of the window class

    class WindowClass {
    public:
        static const char* GetName();
        static const HINSTANCE GetInstance();
        
    private:
        WindowClass();
        ~WindowClass();

        static constexpr const char* wndClassName = "Knight DX11 Engine Window";
        static WindowClass wndClass;
        HINSTANCE hInst;
    };

public:
    Window(int width, int height, const char* name);
    ~Window();

    
private:
    static LRESULT CALLBACK MsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK MsgProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    int width;
    int height;
    HWND hWnd;
};