#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Application.h"
#include "Point.h"
#include <SDL.h>


ModuleInput::ModuleInput()
{
}

// Destructor
ModuleInput::~ModuleInput()
{
}

// Called before render is available
bool ModuleInput::Init()
{
	//LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	initKeys();

	if (SDL_InitSubSystem(SDL_INIT_TIMER) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate(float dt)
{
	update_status toBeReturned = UPDATE_CONTINUE;

	mouse_motion = { 0, 0 };
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
			{
				keyboard[i] = KEY_UP;
			}
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

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

	case SDL_MOUSEBUTTONDOWN:
		mouse_buttons[event.button.button - 1] = KEY_DOWN;
		break;

	case SDL_MOUSEBUTTONUP:
		mouse_buttons[event.button.button - 1] = KEY_UP;
		break;

	case SDL_MOUSEMOTION:
		mouse_motion.x = event.motion.xrel;
		mouse_motion.y = event.motion.yrel;
		mouse.x = event.motion.x / SCREEN_SIZE;
		mouse.y = event.motion.y / SCREEN_SIZE;
		break;

	case SDL_MOUSEWHEEL:
		mouseWheel = event.wheel.y;
		break;
	}

	return toBeReturned;
}

// Called every draw update
update_status ModuleInput::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleInput::PostUpdate(float dt)
{
	mouseWheel = 0.0f;
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::initKeys()
{
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

const iPoint& ModuleInput::GetMousePosition() const
{
	return mouse;
}

const fPoint& ModuleInput::GetMouseMotion() const
{
	return mouse_motion;
}

