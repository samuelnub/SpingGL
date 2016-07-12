#include <game/input.h>
#include <game/game.h>
#include <iostream>

Input::Input()
{
}

Input::~Input()
{
}

void Input::setup(Game *game)
{
	this->_gamePtr = game;

	std::fill(this->_pressedKeys.begin(), this->_pressedKeys.end(), false);
	std::fill(this->_heldKeys.begin(), this->_heldKeys.end(), false);
	std::fill(this->_releasedKeys.begin(), this->_releasedKeys.end(), false);

	std::fill(this->_pressedMouseButtons.begin(), this->_pressedMouseButtons.end(), false);
	std::fill(this->_releasedMouseButtons.begin(), this->_releasedMouseButtons.end(), false);

	this->_deltaTime = 0.0f;
	this->_lastFrameTime = 0.0f;

	std::cout << "Set up input!\n";
}

void Input::update(const float &curFrameTime)
{
	this->_deltaTime = curFrameTime - this->_lastFrameTime;
	this->_lastFrameTime = curFrameTime;

	//resets key events every frame, held keys are kept though, until further notice
	std::fill(this->_pressedKeys.begin(), this->_pressedKeys.end(), false);
	std::fill(this->_releasedKeys.begin(), this->_releasedKeys.end(), false);

	std::fill(this->_pressedMouseButtons.begin(), this->_pressedMouseButtons.end(), false);
	std::fill(this->_releasedMouseButtons.begin(), this->_releasedMouseButtons.end(), false);

	this->_cursorMovedX = 0;
	this->_cursorMovedY = 0;

	this->_scrolledX = 0;
	this->_scrolledY = 0;

	//handle multiple simultaneous events in one frame
	this->_frameEvents.clear();
	while (SDL_PollEvent(&this->_event) != 0)
	{
		this->_frameEvents.push_back(this->_event);
	}

	if(!this->_frameEvents.empty())
		this->process();
}

float Input::getDelta()
{
	return this->_deltaTime;
}

bool Input::wasKeyPressed(SDL_Scancode & key)
{
	return this->_pressedKeys[key];
}

bool Input::wasKeyReleased(SDL_Scancode & key)
{
	return this->_releasedKeys[key];
}

bool Input::isKeyHeld(SDL_Scancode & key)
{
	return this->_heldKeys[key];
}

bool Input::wasMouseButtonPressed(int button)
{
	return this->_pressedMouseButtons[button];
}

bool Input::wasMouseButtonReleased(int button)
{
	return this->_releasedMouseButtons[button];
}

int Input::getCursorX()
{
	return this->_cursorX;
}

int Input::getCursorY()
{
	return this->_cursorY;
}

int Input::getCursorMovedX()
{
	return this->_cursorMovedX;
}

int Input::getCursorMovedY()
{
	return this->_cursorMovedY;
}

int Input::getScrollX()
{
	return this->_scrolledX;
}

int Input::getScrollY()
{
	return this->_scrolledY;
}

void Input::process()
{
	for (size_t i = 0; i < this->_frameEvents.size(); i++)
	{
		switch (this->_frameEvents.at(i).type)
		{
		case SDL_QUIT:
			SDL_Quit();
			exit(EXIT_SUCCESS);
			break;
		case SDL_KEYDOWN:
			if (this->_frameEvents.at(i).key.repeat == true)
				this->keyHeldEvent(i);
			else if (this->_frameEvents.at(i).key.repeat == false)
				this->keyDownEvent(i);
			break;
		case SDL_KEYUP:
			this->keyUpEvent(i);
			break;
		case SDL_MOUSEMOTION:
			this->mouseMotionEvent(i);
			break;
		case SDL_MOUSEWHEEL:
			this->mouseScrollEvent(i);
			break;
		case SDL_MOUSEBUTTONDOWN:
			this->mouseButtonDownEvent(i);
			break;
		case SDL_MOUSEBUTTONUP:
			this->mouseButtonUpEvent(i);
			break;
		}
	}
}

void Input::keyDownEvent(size_t &index)
{
	std::cout << "Pressed key " << this->_frameEvents[index].key.keysym.scancode << "!\n";
	this->_pressedKeys[this->_frameEvents[index].key.keysym.scancode] = true;
	this->_heldKeys[this->_frameEvents[index].key.keysym.scancode] = true;

	//===========impulse commands when this event occurs==========
	this->_gamePtr->commands.impulseKeyDown();
}

void Input::keyHeldEvent(size_t & index)
{
	//===========impulse commands when this event occurs==========
	this->_gamePtr->commands.impulseKeyHeld();
}

void Input::keyUpEvent(size_t &index)
{
	this->_releasedKeys[this->_frameEvents[index].key.keysym.scancode] = true;
	this->_heldKeys[this->_frameEvents[index].key.keysym.scancode] = false;

	//===========impulse commands when this event occurs==========
	this->_gamePtr->commands.impulseKeyUp();
}

void Input::mouseMotionEvent(size_t &index)
{
	this->_cursorX = this->_frameEvents[index].motion.x;
	this->_cursorY = this->_frameEvents[index].motion.y;
	this->_cursorMovedX = this->_frameEvents[index].motion.xrel;
	this->_cursorMovedY = this->_frameEvents[index].motion.yrel;

	//===========impulse commands when this event occurs==========
	this->_gamePtr->commands.impulseMouseMotion();
}

void Input::mouseScrollEvent(size_t & index)
{
	this->_scrolledX = this->_frameEvents[index].wheel.x;
	this->_scrolledY = this->_frameEvents[index].wheel.y;

	//===========impulse commands when this event occurs==========
	this->_gamePtr->commands.impulseMouseScroll();
}

void Input::mouseButtonDownEvent(size_t & index)
{
	std::cout << "Pressed mouse button " << this->_frameEvents[index].button.button << " at " << this->getCursorX() << ", " << this->getCursorY() << "!\n";
	this->_pressedMouseButtons[this->_frameEvents[index].button.button] = true;

	//===========impulse commands when this event occurs==========
	this->_gamePtr->commands.impulseMouseButtonDown();
}

void Input::mouseButtonUpEvent(size_t & index)
{
	this->_releasedMouseButtons[this->_frameEvents[index].button.button] = true;

	//===========impulse commands when this event occurs==========
	this->_gamePtr->commands.impulseMouseButtonUp();
}
