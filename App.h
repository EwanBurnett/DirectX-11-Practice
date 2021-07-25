#pragma once
#include "Window.h"
#include "KnightTimer.h"

class App {
public:
    App();
    int Go();

private:
    void DoFrame();
    Window wnd;
    KnightTimer timer;
};