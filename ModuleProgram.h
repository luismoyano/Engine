#pragma once
#include "Module.h"
class ModuleProgram :
	public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	const char* loadShader(const char*);
	
	unsigned int program;
};

