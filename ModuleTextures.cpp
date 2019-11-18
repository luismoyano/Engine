#include "ModuleTextures.h"
#include "SDL.h"
#include <GL/glew.h>
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

ModuleTextures::ModuleTextures()
{
}


ModuleTextures::~ModuleTextures()
{
}

bool ModuleTextures::Init()
{
	ilInit();
	iluInit();
	ilutInit();

	return true;
}

bool ModuleTextures::Start()
{
	textureName = "Assets/lenna.png";
	loadTexture(textureName);

	return true;
}

bool ModuleTextures::loadTexture(const char * path)
{
	bool result = false;
	
	ILuint tex;
	ilGenImages(1, &tex);
	ilBindImage(tex);
	
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// load and generate the texture
	result = ilLoadImage(path);
	
	if (result)
	{
		int w = ilGetInteger(IL_IMAGE_WIDTH);
		ILubyte* b = ilGetData();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOG("Failed to load texture");
	}

	ilutRenderer(ILUT_OPENGL);
	texture = ilutGLBindTexImage();

	ilDeleteImage(tex);
	
	return result;
}
