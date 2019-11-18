#pragma once
#include "Module.h"
#include <GL/glew.h>

class ModuleTextures :
	public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool Start();

	bool loadTexture(const char*);

	GLuint texture;

private:
	const char* textureName;
};

