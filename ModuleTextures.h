#pragma once
#include "Module.h"
#include "IL/il.h"

class ModuleTextures :
	public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool Start();

	bool loadTexture(const char*);

	ILuint generateTexture();

private:

	const char* textureName;


};

