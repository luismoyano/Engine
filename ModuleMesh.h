#pragma once
#include "Module.h"
#include "mathlib/MathGeoLib.h"
#include "Globals.h"
#include <GL/glew.h>
#include <initializer_list>
using namespace std;

class ModuleMesh : public Module
{
public:
	ModuleMesh();
	~ModuleMesh();

	bool Init();
	bool Start();
	update_status Update();
	float4x4 LookAt(float3, float3, float3);

private:
	GLuint vertexBufferObject;
	float4x4 modelMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;

	Frustum frustum;
};

