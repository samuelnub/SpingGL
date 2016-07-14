#pragma once
#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <SDL2/SDL.h>
#include <vector>
#include <array>

class Game;

//input must be processed in main loop before other classes request "was this pressed?"
class Input
{
private:
	Game *_gamePtr;

	SDL_Event _event;

	std::vector<SDL_Event> _frameEvents;

	std::array<bool, 1024> _heldKeys;
	std::array<bool, 1024> _pressedKeys;
	std::array<bool, 1024> _releasedKeys;

	std::array<bool, 5> _pressedMouseButtons;
	std::array<bool, 5> _releasedMouseButtons;

	int _cursorX;
	int _cursorY;
	int _cursorMovedX;
	int _cursorMovedY;

	int _scrolledX;
	int _scrolledY;

	float _deltaTime;
	float _lastFrameTime;

public:
	Input();
	~Input();

	void setup(Game *game);

	void update(const float &curFrameTime); //refresh all key maps every frame and get delta time, if poll event returned true, process the event

	inline float getDelta()
	{
		return this->_deltaTime;
	}

	inline bool wasKeyPressed(int key)
	{
		return this->_pressedKeys[key];
	}

	inline bool wasKeyReleased(int key)
	{
		return this->_releasedKeys[key];
	}

	inline bool isKeyHeld(int key)
	{
		return this->_heldKeys[key];
	}

	inline bool wasMouseButtonPressed(int button)
	{
		return this->_pressedMouseButtons[button];
	}

	inline bool wasMouseButtonReleased(int button)
	{
		return this->_releasedMouseButtons[button];
	}

	inline int getCursorX()
	{
		return this->_cursorX;
	}

	inline int getCursorY()
	{
		return this->_cursorY;
	}

	inline int getCursorMovedX()
	{
		return this->_cursorMovedX;
	}

	inline int getCursorMovedY()
	{
		return this->_cursorMovedY;
	}

	inline int getScrollX()
	{
		return this->_scrolledX;
	}

	inline int getScrollY()
	{
		return this->_scrolledY;
	}


protected:
	void process();

	void keyDownEvent(size_t &index);
	void keyHeldEvent(size_t &index);
	void keyUpEvent(size_t &index);
	void mouseMotionEvent(size_t &index);
	void mouseScrollEvent(size_t &index);
	void mouseButtonDownEvent(size_t &index);
	void mouseButtonUpEvent(size_t &index);

};

#endif