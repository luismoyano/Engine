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
	update_status PreUpdate(float) override;
	update_status Update(float) override;
	update_status PostUpdate(float) override;
	bool CleanUp();
	const char* loadShader(const char*);
	
	unsigned int program;
};

