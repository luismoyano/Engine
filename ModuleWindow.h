#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	// Called before quitting
	bool CleanUp();

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface screen_surface;
	SDL_Surface previous_surface;
};

#endif // __ModuleWindow_H__