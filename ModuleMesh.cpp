#include "ModuleMesh.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "ModuleCamera.h"
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

	modelMatrix = math::float4x4::FromTRS(math::float3(0.0f, 0.0f, -10.0f), math::float3x3::RotateY(0.0f), math::float3(1.0f, 1.0f, 1.0f));


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->textures->texture);
	glUniform1i(glGetUniformLocation(App->program->basicProgram, "texture0"), 0);
	
	glUseProgram(App->program->basicProgram);
	glUniformMatrix4fv(glGetUniformLocation(App->program->basicProgram, "model"), 1, GL_TRUE, &modelMatrix[0][0]);    
	glUniformMatrix4fv(glGetUniformLocation(App->program->basicProgram, "view"), 1, GL_TRUE, &App->camera->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->basicProgram, "proj"), 1, GL_TRUE, &App->camera->projectionMatrix[0][0]);

	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

update_status ModuleMesh::Update(float dt)
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