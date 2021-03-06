#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"


#define NUM_MOUSE_BUTTONS 5
#define TEXT_INPUT_MAX_LENGHT 100

struct SDL_Rect;

enum EVENT_WINDOW //used to control mouse events
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{

public:

	ModuleInput();

	// Destructor
	virtual ~ModuleInput();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(EVENT_WINDOW ev);

	// Check key states (includes mouse and joy buttons)
	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int& x, int& y);
	void GetMouseMotion(int& x, int& y);

	void ActivateTextInput(SDL_Rect& rect);
	void DesactivateTextInput();

	const char* GetInputText();

private:
	//	bool TextHasSpace();
	void HandleTextInput();

public:
	bool inputTexActivated;

private:
	bool		windowEvents[(int)EVENT_WINDOW::WE_COUNT];
	KEY_STATE* keyboard;
	KEY_STATE	mouseButtons[NUM_MOUSE_BUTTONS];
	int			mouseMotionX;
	int			mouseMotionY;
	int			mouseX;
	int			mouseY;

	SString		text;
	int			cursorPos;
};

#endif // __INPUT_H__