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
	initProgram(basicProgram, "shaders/basic_vertex.shader", "shaders/basic_fragment.shader");
	//initProgram(primitiveProgram, "shaders/primitive_vertex.shader", "shaders/primitive_fragment.shader");
	return true;
}

update_status ModuleProgram::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::Update(float dt)
{
	return UPDATE_CONTINUE;

}

update_status ModuleProgram::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;

}

bool ModuleProgram::CleanUp()
{
	glDeleteProgram(basicProgram);
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

void ModuleProgram::initProgram(GLuint &program, const char* vertexShaderPath, const char* fragmentShaderPath)
{
	const char *vertexShader = loadShader(vertexShaderPath);
	const char *fragmentShader = loadShader(fragmentShaderPath);

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShader, nullptr);
	glCompileShader(vertex);

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShader, nullptr);
	glCompileShader(fragment);

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
