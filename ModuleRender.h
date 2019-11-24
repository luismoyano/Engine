#pragma once
#include "Module.h"
#include "Globals.h"
#include <GL/glew.h>
#include <SDL.h>


struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	bool Start();
	update_status PreUpdate(float) override;
	update_status Update(float) override;
	update_status PostUpdate(float) override;
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

	SDL_GLContext context;

private:
	GLuint vertexBufferObject;

	void renderGrid();
};
