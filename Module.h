#pragma once

#include "Globals.h"

class Application;

class Module
{
public:

	Module()
	{
	}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

protected:

	bool isInit = false;
};
