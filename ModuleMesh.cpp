#include "ModuleMesh.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "Application.h"

using namespace std;

ModuleMesh::ModuleMesh()
{
}


ModuleMesh::~ModuleMesh()
{
}

bool ModuleMesh::Init()
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 50.0f;
	frustum.verticalFov = 0.15f;
	frustum.horizontalFov = 0.45f;

	return true;
}

bool ModuleMesh::Start()
{
	float vertices[] = {
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f, 0.8f, 0.0f,

		0.0f, 0.0f, //v0 texcoord
		1.0f, 0.0f, //v1 texcoord
		0.5f, 1.0f //v2 texcoord
	};

	modelMatrix = math::float4x4::FromTRS(math::float3(0.0f, 0.0f, -4.0f), math::float3x3::RotateY(math::pi / 4.0f), math::float3(1.0f, 1.0f, 1.0f));
	viewMatrix = LookAt(math::float3(0.0f, 0.5f, 0.4f), math::float3(0.0f, 0.0f, 0.0f), math::float3(0.0f, 1.0f, 0.0f));
	projectionMatrix = frustum.ProjectionMatrix();	
	math::float4x4 transform = projectionMatrix * math::float4x4(modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->textures->texture);
	glUniform1i(glGetUniformLocation(App->program->program, "texture0"), 0);
	
	glUseProgram(App->program->program);
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "model"), 1, GL_TRUE, &modelMatrix[0][0]);    
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "view"), 1, GL_TRUE, &viewMatrix[0][0]);    
	glUniformMatrix4fv(glGetUniformLocation(App->program->program, "proj"), 1, GL_TRUE, &projectionMatrix[0][0]);

	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

update_status ModuleMesh::Update()
{
	glEnableVertexAttribArray(0); // attribute 0            
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glVertexAttribPointer(
		0,                  // attribute 0                    
		3,                  // number of componentes (3 floats)                    
		GL_FLOAT,           // data type                    
		GL_FALSE,           // should be normalized?                    
		0,                  // stride                    
		(void*)0            // array buffer offset                    
	);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 
		2, 
		GL_FLOAT, 
		GL_FALSE, 
		0,
		(void*)(sizeof(float) * 3 * 3) // buffer offset
	);

	
	glDrawArrays(GL_TRIANGLES, 0, 3); // start at 0 and 3 tris            
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return UPDATE_CONTINUE;
}

float4x4 ModuleMesh::LookAt(float3 target, float3 eye, float3 up)
{
	float4x4 matrix;

	math::float3 f(target - eye); 
	f.Normalize();
	
	math::float3 s(f.Cross(up)); 
	s.Normalize();
	
	math::float3 u(s.Cross(f));
	
	matrix[0][0] = s.x; 
	matrix[0][1] = s.y; 
	matrix[0][2] = s.z;
	
	matrix[1][0] = u.x; 
	matrix[1][1] = u.y; 
	matrix[1][2] = u.z;

	matrix[2][0] = -f.x; 
	matrix[2][1] = -f.y; 
	matrix[2][2] = -f.z;
	
	matrix[0][3] = -s.Dot(eye); 
	matrix[1][3] = -u.Dot(eye); 
	matrix[2][3] = f.Dot(eye);
	
	matrix[3][0] = 0.0f; 
	matrix[3][1] = 0.0f; 
	matrix[3][2] = 0.0f; 
	matrix[3][3] = 1.0f;

	return matrix;
}
