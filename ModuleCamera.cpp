#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleInput.h"
#include "GL/glew.h"
#include <SDL.h>
#include "Point.h"



ModuleCamera::ModuleCamera()
{
}


ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::unitX;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	SetPlaneDistances(0.1f, 2000.0f);
	SetFOV(math::pi / 4.0f);

	reloadMatrices();
	return true;
}

bool ModuleCamera::Start()
{
	return true;
}

update_status ModuleCamera::PreUpdate(float dt)
{
	updateNavModes();
	updatePosition(dt);
	updateRotation(dt);

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::SetFOV(float fov)
{
	frustum.verticalFov = fov;
	frustum.horizontalFov = 2.f * atanf(fov * 0.5f) * (SCREEN_WIDTH / SCREEN_HEIGHT);
}

void ModuleCamera::SetAspectRatio(float h)
{
	frustum.horizontalFov = h;
}

void ModuleCamera::SetPlaneDistances(float nearPlane, float farPlane)
{
	frustum.nearPlaneDistance = nearPlane;
	frustum.farPlaneDistance = farPlane;
}

void ModuleCamera::SetPosition(float x, float y, float z)
{
	frustum.pos = float3(x, y, z);
}

void ModuleCamera::updatePosition(float dt)
{
	if (!navigationMode == FREE) return;
	
	if (App->input->GetKey(SDL_SCANCODE_W)) moveUp(dt);

	if (App->input->GetKey(SDL_SCANCODE_S)) moveDown(dt);

	if (App->input->GetKey(SDL_SCANCODE_A)) moveLeft(dt);

	if (App->input->GetKey(SDL_SCANCODE_D)) moveRight(dt);

	if (App->input->GetKey(SDL_SCANCODE_E) || App->input->getWheelSpeed() > 0.0f) moveForward(dt, App->input->getWheelSpeed());

	if (App->input->GetKey(SDL_SCANCODE_Q) || App->input->getWheelSpeed() < 0.0f) moveBackwards(dt, App->input->getWheelSpeed());
	
	reloadMatrices();
}

void ModuleCamera::updateRotation(float dt)
{
	if (!navigationMode == FREE) return;

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (mouseMotion.x > 2.0f) yaw(mouseMotion.x, dt);
	if (mouseMotion.y > 2.0f) pitch(mouseMotion.x, dt);
}

void ModuleCamera::updateNavModes()
{
	isFastMode = App->input->GetKey(SDL_SCANCODE_LSHIFT);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)) navigationMode = FREE;
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) && App->input->GetKey(SDL_SCANCODE_LALT)) navigationMode = ORBIT;
	else navigationMode = NONE;
}

void ModuleCamera::LookAt(float3 target, float3 eye, float3 up)
{
	math::float3 f(target - eye); f.Normalize();
	math::float3 s(f.Cross(up)); s.Normalize();
	math::float3 u(s.Cross(f));
	
	viewMatrix[0][0] = s.x; 
	viewMatrix[0][1] = s.y; 
	viewMatrix[0][2] = s.z;
	
	viewMatrix[1][0] = u.x; 
	viewMatrix[1][1] = u.y; 
	viewMatrix[1][2] = u.z;

	viewMatrix[2][0] = -f.x; 
	viewMatrix[2][1] = -f.y; 
	viewMatrix[2][2] = -f.z;

	viewMatrix[0][3] = -s.Dot(eye); 
	viewMatrix[1][3] = -u.Dot(eye); 
	viewMatrix[2][3] = f.Dot(eye);
	
	viewMatrix[3][0] = 0.0f; 
	viewMatrix[3][1] = 0.0f; 
	viewMatrix[3][2] = 0.0f; 
	viewMatrix[3][3] = 1.0f;
}

void ModuleCamera::reloadMatrices()
{
	projectionMatrix = frustum.ProjectionMatrix();
	viewMatrix = frustum.ViewMatrix();
}

float ModuleCamera::getCamSpeed()
{
	return isFastMode? CAM_SPEED * 2.0f : CAM_SPEED;
}

void ModuleCamera::moveUp(float dt)
{
	float3 newPosition = frustum.pos;
	newPosition.y = newPosition.y + ((0.05f + dt) * getCamSpeed());
	frustum.pos = newPosition;
}

void ModuleCamera::moveDown(float dt)
{
	float3 newPosition = frustum.pos;
	newPosition.y = newPosition.y - ((0.05f + dt) * getCamSpeed());
	frustum.pos = newPosition;
}

void ModuleCamera::moveLeft(float dt)
{
	frustum.pos -= frustum.WorldRight().ScaledToLength((0.05f + dt) * getCamSpeed());
}

void ModuleCamera::moveRight(float dt)
{
	frustum.pos += frustum.WorldRight().ScaledToLength((0.05f + dt) * getCamSpeed());
}

void ModuleCamera::moveForward(float dt, float extraSpeed)
{
	frustum.pos += frustum.front.ScaledToLength((0.05f + dt) * getCamSpeed() * ((extraSpeed > 0) ? math::Abs(extraSpeed) : 1.0f));
}

void ModuleCamera::moveBackwards(float dt, float extraSpeed)
{
	frustum.pos -= frustum.front.ScaledToLength((0.05f + dt) * getCamSpeed() * ((extraSpeed < 0)? math::Abs(extraSpeed) : 1.0f));
}

void ModuleCamera::pitch(float angle, float dt)
{
	float newAngle = abs((dt + 0.005f) * getCamSpeed() * (angle*-1) + asinf(frustum.front.y / frustum.front.Length()));
	if (newAngle >= math::pi / 2) return;
	
	float3x3 rotationMatrix = float3x3::identity;
	
	rotationMatrix.SetRotatePart(frustum.WorldRight(), newAngle);
	frustum.up = rotationMatrix * frustum.up;
	frustum.front = rotationMatrix * frustum.front;

}

void ModuleCamera::yaw(float angle, float dt)
{
	const float newAngle = (dt + 0.005f) * getCamSpeed() * (angle*-1);
	float3x3 rotationMatrix = float3x3::RotateY(newAngle);
	frustum.up = rotationMatrix * frustum.up;
	frustum.front = rotationMatrix * frustum.front;

}
