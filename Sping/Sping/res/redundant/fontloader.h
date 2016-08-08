#pragma once
#ifndef TOOLS_FONTLOADER_H
#define TOOLS_FONTLOADER_H

#include <stdio.h>
#include <string.h>






#include <map>
#include <string>

class FONScontext;

//holds one font instance
class Font
{
private:
	FONScontext *_fStash;
	int _font;

	int _posX;
	int _posY;

	unsigned char _r;
	unsigned char _g;
	unsigned char _b;
	unsigned char _a;

	float _size;

	int _alignX;
	int _alignY;

	std::string _text;

public:
	Font();
	~Font();

	int setup(const char *name, const char *filePath);

	void setPos(const int &x, const int &y);
	void setColor(const unsigned char &r, const unsigned char &g, const unsigned char &b, const unsigned char &a);
	void setSize(const float &size);
	void setAlign(const int &alignX, const int &alignY);
	void setText(const std::string &text);
	//font stash also has options for blurring and drop shadows, may use them, dunno

	void draw();
	void draw(const int &x, const int &y, const char *text); //overloaded version for temp rendering purposes

protected:


};

#endif