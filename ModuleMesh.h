#pragma once
#include "Module.h"
#include "mathlib/MathGeoLib.h"
#include "Globals.h"
#include <GL/glew.h>

class ModuleMesh : public Module
{
public:
	ModuleMesh();
	~ModuleMesh();

	bool Init();
	bool Start();
	update_status Update();

private:
	float4x4 modelMatrix;
	GLuint vertexBufferObject;
};

