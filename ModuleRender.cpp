#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include <GL/glew.h>

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); 
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	context = SDL_GL_CreateContext(App->window->window);
	//SDL_GL_MakeCurrent(App->window->window, context);

	GLenum err = glewInit();
	if (err != GLEW_OK) return false;
	
	// ... check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));// Should be 2.0
	LOG("Vendor: %s", glGetString(GL_VENDOR)); 
	LOG("Renderer: %s", glGetString(GL_RENDERER)); 
	LOG("OpenGL version supported %s", glGetString(GL_VERSION)); 
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	glClearDepth(1.0f); 
	glClearColor(0.f, 0.f, 0.f, 1.f); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_DEPTH_TEST); 
	glFrontFace(GL_CCW); 
	glEnable(GL_CULL_FACE); 
	glEnable(GL_TEXTURE_2D); 
	glViewport(0, 0, App->window->screen_surface.w, App->window->screen_surface.h);

	return true;
}

update_status ModuleRender::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update(float dt)
{
	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");
	SDL_GL_DeleteContext(context);

	//Destroy window

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);
}

