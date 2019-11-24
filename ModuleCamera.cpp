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
	updateOrbit(dt);

	reloadMatrices();

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
}

void ModuleCamera::updateRotation(float dt)
{
	if (!navigationMode == FREE) return;

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (mouseMotion.x > 2.0f) yaw(mouseMotion.x, dt);
	if (mouseMotion.y > 2.0f) pitch(mouseMotion.y, dt);
}

void ModuleCamera::updateOrbit(float dt)
{
	if (!navigationMode == ORBIT) return;

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (mouseMotion.x > 2.0f && App->input->GetKey(SDL_SCANCODE_LALT)) yaw(mouseMotion.x, dt);
	if (mouseMotion.y > 2.0f && App->input->GetKey(SDL_SCANCODE_LALT)) pitch(mouseMotion.y, dt);
}

void ModuleCamera::updateNavModes()
{
	isFastMode = App->input->GetKey(SDL_SCANCODE_LSHIFT);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)) navigationMode = FREE;
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) && App->input->GetKey(SDL_SCANCODE_LALT)) navigationMode = ORBIT;
	else navigationMode = NONE;
}

void ModuleCamera::LookAt(float3 target)
{
	float3x3 rotation = float3x3::LookAt(frustum.front, (target - frustum.pos).Normalized(), frustum.up, float3::unitY);
	frustum.front = frustum.front * rotation;
	frustum.up = frustum.up * rotation;
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
	float adjustment = (dt + 0.005f) * getCamSpeed() * (angle*-1);
	float newAngle = math::Abs(adjustment + asinf(frustum.front.y / frustum.front.Length()));
	if (newAngle >= math::pi / 2) return;
	
	float3x3 rotationMatrix = float3x3::identity;
	
	rotationMatrix.SetRotatePart(frustum.WorldRight(), adjustment);
	frustum.up = rotationMatrix * frustum.up;
	frustum.front = rotationMatrix * frustum.front;

	LookAt(float3::zero);
}

void ModuleCamera::yaw(float angle, float dt)
{
	const float newAngle = (dt + 0.005f) * getCamSpeed() * (angle*-1);
	float3x3 rotationMatrix = float3x3::RotateY(newAngle);
	frustum.up = rotationMatrix * frustum.up;
	frustum.front = rotationMatrix * frustum.front;

	LookAt(float3::zero);
}

void ModuleCamera::orbitX(float angle, float dt)
{
	const float newAngle = (dt + 0.005f) * getCamSpeed() * (angle*-1);
	float3x3 rotation_matrix = float3x3::RotateY(newAngle);
	frustum.pos = rotation_matrix * frustum.pos;

	LookAt(float3::zero);

}

void ModuleCamera::orbitY(float angle, float dt)
{
	float adjustment = (dt + 0.005f) * getCamSpeed() * (angle*-1);
	float newAngle = math::Abs(adjustment + asinf(frustum.front.y / frustum.front.Length()));
	if (newAngle >= math::pi / 2) return;

	float3x3 rotationMatrix = float3x3::identity;
	rotationMatrix.SetRotatePart(frustum.WorldRight(), adjustment);
	frustum.pos = rotationMatrix * frustum.pos;

	LookAt(float3::zero);
}