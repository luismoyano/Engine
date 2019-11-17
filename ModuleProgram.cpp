#include "ModuleProgram.h"
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>




ModuleProgram::ModuleProgram()
{
}


ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Init()
{
	return true;

}

bool ModuleProgram::Start()
{
	const char *vertexShader = loadShader("shaders/vertex_basic.shader");
	const char *fragmentShader = loadShader("shaders/fragment_basic.shader");


	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShader, nullptr);
	glCompileShader(vertex);

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShader, nullptr);
	glCompileShader(fragment);

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

update_status ModuleProgram::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::Update()
{
	return UPDATE_CONTINUE;

}

update_status ModuleProgram::PostUpdate()
{
	return UPDATE_CONTINUE;

}

bool ModuleProgram::CleanUp()
{
	glDeleteProgram(program);
	return true;
}

const char* ModuleProgram::loadShader(const char* path) 
{

	FILE *file = nullptr;
	char* result = nullptr;

	fopen_s(&file, path, "rb");
	
	if (file)
	{

		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		result = (char*)malloc(size + 1);

		fread(result, 1, size, file);
		result[size] = 0;

		fclose(file);
	}

	return result;
}
