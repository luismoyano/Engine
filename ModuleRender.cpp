#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "SDL.h"
#include <GL/glew.h>


static void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) 
{
	(void)source; 
	(void)type; 
	(void)id;
	(void)severity; 
	(void)length; 
	(void)userParam;

	char src[256];
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		sprintf_s(src, "API");
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		sprintf_s(src, "Window");
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		sprintf_s(src, "Shaders");
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		sprintf_s(src, "Third Party");
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		sprintf_s(src, "Application");
		break;
	case GL_DEBUG_SOURCE_OTHER:
		sprintf_s(src, "Other");
		break;
	}

	char error[256];
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		sprintf_s(error, "Error");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		sprintf_s(error, "Deprecation");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		sprintf_s(error, "Undefined Behaviour");
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		sprintf_s(error, "Portability");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		sprintf_s(error, "Performance");
		break;
	case GL_DEBUG_TYPE_MARKER:
		sprintf_s(error, "Marker");
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		sprintf_s(error, "Push Group");
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		sprintf_s(error, "Pop Group");
		break;
	case GL_DEBUG_TYPE_OTHER:
		sprintf_s(error, "Other");
		break;
	}

	char errorMsg[4096];
	sprintf_s(errorMsg, "%s %s %s", src, error, message);

	LOG("----------------OPEN GL ERROR----------------");
	LOG(errorMsg);
	LOG("----------------END OF OPEN GL ERROR----------------");

}

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

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);

	return true;
}

bool ModuleRender::Start()
{
	renderGrid();
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
	renderGrid();
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

void ModuleRender::renderGrid()
{
	glUseProgram(App->program->primitiveProgram);

	float4x4 model = float4x4::FromTRS(float3::zero, float3x3::identity, float3::one);

	glUniformMatrix4fv( glGetUniformLocation(App->program->primitiveProgram, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->primitiveProgram, "view"), 1, GL_TRUE, &App->camera->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->primitiveProgram, "proj"), 1, GL_TRUE, &App->camera->projectionMatrix[0][0]);

	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
}

