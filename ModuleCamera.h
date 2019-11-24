#pragma once
#include "Module.h"
#include "mathlib/MathGeoLib.h"
#include "Globals.h"
#include <GL/glew.h>

#define CAM_SPEED 2.5f

enum MovementMode
{
	MOVEMENT_ERROR = -1,
	NONE,
	FREE,
	ORBIT,

};

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
	void updateRotation();
	void updateOrientation();
	void updateNavModes();
	void LookAt(float3, float3, float3);

	float4x4 viewMatrix;
	float4x4 projectionMatrix;

	Frustum frustum;

private:

	MovementMode navigationMode = NONE;
	bool isFastMode = false;

	void reloadMatrices();

	float getCamSpeed();

	void moveUp(float);
	void moveDown(float);
	void moveLeft(float);
	void moveRight(float);
	void moveForward(float, float speed = 0.0f);
	void moveBackwards(float, float speed = 0.0f);

	void pitch(float);
	void yaw(float);
	void orbitX(float);
	void orbitY(float);
};

