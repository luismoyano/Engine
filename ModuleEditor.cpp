#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#include <GL/glew.h>    // Initialize with glewInit()
#include "Globals.h"



ModuleEditor::ModuleEditor()
{
}


ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	bool bOk = true;

	bOk = SDL_InitSubSystem(SDL_INIT_TIMER) == 0;
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	io.WantCaptureKeyboard = false;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	
	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);

	fpsRotate = 0;

	isInit = bOk;
	return bOk;
}

update_status ModuleEditor::PreUpdate()
{
	//Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	feedFPS();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	drawLogWindow();
	drawMenu();
	drawFPS();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	isInit = false;

	return true;
}

void ModuleEditor::drawLogWindow()
{
	ImGui::Begin("Log");
	ImGui::TextUnformatted(buffer.begin());
	ImGui::SetScrollHere(1.0f);
	ImGui::End();
}

void ModuleEditor::drawMenu()
{
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Gui Demo"))
		{
			//Trololol
		}

		if (ImGui::MenuItem("Documentation"))
		{
			App->requestBrowser("www.github.com");
		}

		if (ImGui::MenuItem("Download latest"))
		{
			App->requestBrowser("www.github.com");
		}

		if (ImGui::MenuItem("Report a bug"))
		{
			App->requestBrowser("www.github.com");
		}

		if (ImGui::MenuItem("About"))
		{
			App->requestBrowser("www.github.com");
		}
		ImGui::EndMenu();
	}
}

void ModuleEditor::appendLinesToBuffer(const char * line)
{
	if(isInit)buffer.appendf(line);
}

void ModuleEditor::feedFPS()
{
	if (fpsRotate >= 25) fpsRotate = 0;
	
	dtData[fpsRotate] = io.DeltaTime;
	fpsData[fpsRotate] = io.DeltaTime * 60.0f * 60.0f;

	++fpsRotate;
}

void ModuleEditor::drawFPS()
{
	ImGui::Begin("FPS");
	ImGui::PlotHistogram("", fpsData, IM_ARRAYSIZE(fpsData), 0, "FPS", 0.0f, 70.0f, ImVec2(0, 80));
	ImGui::PlotHistogram("", dtData, IM_ARRAYSIZE(dtData), 0, "Delta Time", 0.0f, 0.03f, ImVec2(0, 80));
	ImGui::End();

}
