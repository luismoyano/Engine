#pragma once
#include "Module.h"
#include "imgui/imgui.h"
#include <list>

using namespace std;

class ModuleEditor :
	public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	// Called before quitting
	bool CleanUp();

	void appendLinesToBuffer(const char* line);


private:
	bool showDemoWindow = true;
	ImGuiIO io;
	ImGuiTextBuffer buffer;
	float dtData[25];
	float fpsData[25];
	int fpsRotate;

	void drawLogWindow();

	void drawMenu();

	void drawFPS();
	void feedFPS();
};

