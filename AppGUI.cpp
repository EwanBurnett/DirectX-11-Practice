#include "AppGUI.h"

GUI::GUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGui::SetCurrentContext(c);
	ImGui::StyleColorsDark();
}

GUI::~GUI()
{
	ImGui::DestroyContext();
}