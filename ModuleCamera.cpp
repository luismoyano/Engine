#include "ModuleCamera.h"



ModuleCamera::ModuleCamera()
{
}


ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	SetPlaneDistances(1.0f, 2000.0f);
	SetFOV(0.15f, 0.45f);

	viewMatrix = frustum.ViewMatrix();
	LookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.1f, 0.0f));

	projectionMatrix = frustum.ProjectionMatrix();

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

void ModuleCamera::SetFOV(float v, float h)
{
	frustum.verticalFov = v;
	frustum.horizontalFov = h;
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
