#pragma once
#include "Module.h"
#include "mathlib/MathGeoLib.h"
#include "Globals.h"
#include <GL/glew.h>

#define CAM_TRANSLATION_SPEED 2.5f
#define CAM_ROTATION_SPEED 0.1745f //10 deg


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
	void updateRotation(float);
	void updateOrbit(float);
	void updateFocus(float);
	void updateNavModes();
	void LookAt(float3);

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

	void pitch(bool, float);
	void yaw(bool, float);
	void orbitX(float, float);
	void orbitY(float, float);
};

