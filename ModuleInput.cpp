#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Application.h"
#include <SDL.h>

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	//LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_TIMER) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate()
{
	update_status toBeReturned = UPDATE_CONTINUE;
	SDL_Event event;
	while (SDL_PollEvent(&event)) ImGui_ImplSDL2_ProcessEvent(&event);

	// Esc button is pressed
	switch (event.type)
	{
	case SDL_QUIT:
		toBeReturned = UPDATE_STOP;
		break;

	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			//App->renderer->WindowResized(event.window.data1, event.window.data2);
		break;
	}
	//keyboard = SDL_GetKeyboardState(NULL);
	return toBeReturned;
}

// Called every draw update
update_status ModuleInput::Update()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
