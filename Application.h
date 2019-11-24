#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include "imgui/imgui.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleEditor;
class ModuleMesh;
class ModuleProgram;
class ModuleCamera;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleMesh* mesh = nullptr;
	ModuleProgram* program = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleCamera* camera = nullptr;

	void requestBrowser(const char* url);

private:

	std::list<Module*> modules;
	float frameStartTimeStamp;

	float getDeltaTimeInSecs();
};

extern Application* App;
