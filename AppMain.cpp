#include "app.h"
#include "appwindow.h"
#include "EntityPool.h"
//#include "imgui/imgui_impl_dx11.h"
//#include "imgui/imgui_impl_win32.h"
#include <sstream>
#include <memory>

//std::unique_ptr<Window> mWnd;
//void DoFrame();
//void Update();
//void FrameStats();
//
///*The entry point for graphical windows apps. The name 'WinMain' is just a convention.
//  Uses STDCALL instead of CDecl calling convention, as defined by the CALLBACK macro.
//https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain */
//
//int CALLBACK WinMain(
//    HINSTANCE   hInstance,      //Pointer to the current instance of the application.
//    HINSTANCE   hPrevInstance,  //Pointer to the previous instance of the app. This parameter is almost always NULL. 
//    LPSTR       lpCmdLine,      //The command line for the app.
//    int         nCmdShow )      //Indicates to the program how it should be shown upon construction.
//{
//    //Create the application window
//    OutputDebugString(">> Creating App Window\n");
//    mWnd = std::make_unique<Window>(1280, 720, "DX11 Box Demo");
//
//    //Evaluate Messages
//    BOOL gResult;
//    MSG msg = { 0 }; //An instance of the Windows API Message struct. See https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-msg
//
//    //Reset Timer
//    mWnd->Timer().Reset();
//
//    while (msg.message != WM_QUIT) {
//        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
//            TranslateMessage(&msg);
//            DispatchMessageW(&msg);
//        }
//        else {
//            DoFrame();
//        }
//    }
//
//    return msg.wParam;
//}
//
//void DoFrame()
//{
//    //Tick Timer
//    mWnd->Timer().Tick();
//
//    //Process if timer is unpaused
//    if ((mWnd->Timer().IsPaused()) == false){
//        bool bShowGuiDemo = false;
//
//        ImGui_ImplDX11_NewFrame();
//        ImGui_ImplWin32_NewFrame();
//
//        mWnd->Gfx().Clear();
//
//        Update();
//        mWnd->Gfx().Update(mWnd->Timer().DeltaTime());
//
//        if (bShowGuiDemo) {
//            ImGui::ShowDemoWindow(&bShowGuiDemo);    //IMGUI test
//        }
//
//        mWnd->Gfx().Draw();
//        
//
//    }
//    else {
//        Sleep(100);
//    }
//}
//
//void Update()
//{
//    //Colour changing buffer over time
//    double r, g, b;
//    float a = mWnd->Timer().GameTime();
//    r = (cos( mWnd->Timer().GameTime()) + 1) / 2;
//    g = (sin( mWnd->Timer().GameTime()) + 1) / 2;
//    b = (sin( mWnd->Timer().GameTime()) + 1) / 2;
//
//    //mWnd->Gfx().Clear(r, g, b);
//    
//}



bool Init();
void DoFrame();		//Process the current frame

std::unique_ptr<Window> mWindow;
std::unique_ptr<Graphics> mGraphics;
std::unique_ptr<EntityPool> mPool;
//ResourceManager mResources;
//Timer timer;

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hLastInstance,
	LPSTR lpCmd,
	int showCode
)
{
	if (!Init()) {
		return 0xDEAD;	//End execution if component Initialization is unsuccessful
	}

	//Message Loop
	BOOL gResult;
	MSG msg = { 0 };

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else {
			DoFrame();
		}
	}

	return 0;
}

bool Init() {
	UINT winWidth = 1280;
	UINT winHeight = 720;

	//Init Window
	mWindow = std::make_unique<Window>();
	mWindow->Init(winWidth, winHeight, "DX11 Box Demo");
	
	

	//Init Graphics
	mGraphics = std::make_unique<Graphics>();

	//Initialize Window GUI
	//ImGui_ImplWin32_Init(mWindow->GetWndInstance());
	mGraphics->Init(mWindow->GetWndInstance(), winWidth, winHeight);

	//Init Entity pool
	mPool = std::make_unique<EntityPool>();

	Entity* a = new DebugEntity();
	//a->Init();

	for (int i = 0; i < 60; i++) {
		mPool->Allocate(a, 9);
	}

	Entity* b = new Entity();
	for (int i = 0; i < 20; i++) {
		mPool->Allocate(b, 7);
	}
	
	Entity* c = new Entity();
	for (int i = 0; i < 30; i++) {
		mPool->Allocate(b, 14);
	}

	//Timer.Reset();
	return true;
}

void DoFrame()
{
	//ImGui_ImplDX11_NewFrame();
	//ImGui_ImplWin32_NewFrame();

	//Clear

	//Update
	mPool->Update(0);

	//Draw
}
