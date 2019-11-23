#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleInput.h"
#include <SDL.h>



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

	viewMatrix = frustum.ViewMatrix();

	return true;
}

bool ModuleCamera::Start()
{
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
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

void ModuleCamera::updatePosition()
{
	
	if (App->input->GetKey(SDL_SCANCODE_Q))
	{
		//Move upwards
	}

	if (App->input->GetKey(SDL_SCANCODE_E))
	{
		//Move downwards
	}

	if (App->input->GetKey(SDL_SCANCODE_W))
	{
		//Move forward
	}

	if (App->input->GetKey(SDL_SCANCODE_S))
	{
		//Move backwards
	}

	if (App->input->GetKey(SDL_SCANCODE_A))
	{
		//Move Left
	}

	if (App->input->GetKey(SDL_SCANCODE_D))
	{
		//Move Right
	}
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

float4x4 ModuleCamera::GetProjectionMatrix()
{
	return projectionMatrix;
}

float4x4 ModuleCamera::GetViewMatrix()
{
	return viewMatrix;
}

void ModuleCamera::reloadMatrices()
{
	projectionMatrix = frustum.ProjectionMatrix();
	viewMatrix = frustum.ViewMatrix();
}
