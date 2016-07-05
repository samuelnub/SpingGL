#pragma once
#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <SDL2/SDL.h>
#include <vector>
#include <array>

//input must be processed in main loop before other classes request "was this pressed?"
class Input
{
private:
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

	void setup();

	void update(const float &curFrameTime); //refresh all key maps every frame and get delta time, if poll event returned true, process the event

	float getDelta();

	bool wasKeyPressed(SDL_Scancode &key);
	bool wasKeyReleased(SDL_Scancode &key);
	bool isKeyHeld(SDL_Scancode &key);

	bool wasMouseButtonPressed(int &button);
	bool wasMouseButtonReleased(int &button);

	int getCursorX();
	int getCursorY();
	int getCursorMovedX();
	int getCursorMovedY();
	int getScrollX();
	int getScrollY();

protected:
	void process();

	void keyDownEvent(size_t &index);
	void keyUpEvent(size_t &index);
	void mouseMotionEvent(size_t &index);
	void mouseScrollEvent(size_t &index);
	void mouseButtonDownEvent(size_t &index);
	void mouseButtonUpEvent(size_t &index);

};

#endif