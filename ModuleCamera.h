#pragma once
#include "Module.h"
#include "mathlib/MathGeoLib.h"
#include "Globals.h"
#include <GL/glew.h>

#define CAM_SPEED 1.0f

class ModuleCamera :
	public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	bool Start();

	update_status PreUpdate(float) override;
	update_status Update(float) override;
	update_status PostUpdate(float) override;
	
	bool CleanUp();

	void SetFOV(float);
	void SetAspectRatio(float);
	void SetPlaneDistances(float, float);
	void SetPosition(float, float, float);
	void updatePosition(float);
	void Orientation();
	void LookAt(float3, float3, float3);
	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();

	float4x4 viewMatrix;
	float4x4 projectionMatrix;

	Frustum frustum;

private:

	void reloadMatrices();

	void moveUp(float);
	void moveDown(float);
	void moveLeft(float);
	void moveRight(float);
};

