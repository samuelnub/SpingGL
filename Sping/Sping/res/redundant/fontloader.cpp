#include <tools/fontloader.h>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define FONTSTASH_IMPLEMENTATION
#include <thirdparty/fontstash.h>

#define GLFONTSTASH_IMPLEMENTATION
#include <thirdparty/glfontstash.h>

Font::Font()
{
}

Font::~Font()
{
	glfonsDelete(this->_fStash);
}

int Font::setup(const char *name, const char * filePath)
{
	this->_font = FONS_INVALID;

	this->_fStash = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT); //512x512 texture to store ttf font to
	if (this->_fStash == NULL)
	{
		std::cout << "Couldn't create stash for your requested font " << name << "!\n";
		return -1;
	}

	this->_font = fonsAddFont(this->_fStash, name, filePath);
	if (this->_font == FONS_INVALID)
	{
		std::cout << "Couldn't add the font at " << filePath << "!\n";
		return -2;
	}
	
	this->_posX = 32;
	this->_posY = 32;

	this->_r = 255;
	this->_g = 255;
	this->_b = 255;
	this->_a = 255;

	this->_size = 32.0f;

	this->_alignX = FONS_ALIGN_LEFT;
	this->_alignY = FONS_ALIGN_BASELINE;

	this->_text = "sample text";
	
	return 0;
}

void Font::setPos(const int & x, const int & y)
{
	this->_posX = x;
	this->_posY = y;
}

void Font::setColor(const unsigned char & r, const unsigned char & g, const unsigned char & b, const unsigned char & a)
{
	fonsSetColor(this->_fStash, glfonsRGBA(r, g, b, a));
	this->_r = r;
	this->_g = g;
	this->_b = b;
	this->_a = a;
}

void Font::setSize(const float & size)
{
	fonsSetSize(this->_fStash, size);
	this->_size = size;
}

void Font::setAlign(const int & alignX, const int & alignY)
{
	fonsSetAlign(this->_fStash, alignX | alignY);
	this->_alignX = alignX;
	this->_alignY = alignY;
}

void Font::setText(const std::string & text)
{
	this->_text = text;
}

void Font::draw()
{
	fonsDrawText(this->_fStash, this->_posX, this->_posY, this->_text.c_str(), NULL);
}

void Font::draw(const int & x, const int & y, const char *text)
{
	float tempo = 0;

	fonsClearState(this->_fStash);

	fonsVertMetrics(this->_fStash, NULL, NULL, &tempo);

	fonsSetFont(this->_fStash, this->_font);

	fonsSetSize(this->_fStash, this->_size);

	fonsSetColor(this->_fStash, glfonsRGBA(this->_r, this->_g, this->_b, this->_a));

	fonsDrawText(this->_fStash, x, y, text, NULL);

	fonsDrawDebug(this->_fStash, 300.0, 50.0);
}
