#pragma once
#include "Module.h"
#include "Globals.h"
#include "Point.h"

#define NUM_MOUSE_BUTTONS 5
#define MAX_KEYS 300


typedef unsigned __int8 Uint8;

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module {
public:

	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate(float) override;
	update_status Update(float) override;
	update_status PostUpdate(float) override;
	bool CleanUp();
	
	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	float getWheelSpeed()
	{
		return mouseWheel;
	}

	// Get mouse / axis position
	const iPoint& GetMouseMotion() const;
	const iPoint& GetMousePosition() const;

private:
	KeyState keyboard[MAX_KEYS];
	KeyState mouse_buttons[NUM_MOUSE_BUTTONS];
	iPoint mouse_motion;
	iPoint mouse;
	float mouseWheel = 0.0f;

	void initKeys();
};