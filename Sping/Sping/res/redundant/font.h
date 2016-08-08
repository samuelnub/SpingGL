#pragma once
#ifndef TOOLS_FONT_H
#define TOOLS_FONT_H

#include <thirdparty/ftgl.h>
#include <map>
#include <string>

//include directories needs to reference the nested FTGL include folder, sorry for ruining the ocd
//holds one font instance
class Font
{
private:
	std::map<std::string, FTFont *> _fontList;
	std::map<std::string, FTFont *>::const_iterator _fontIter;

public:
	Font();
	~Font();

	Font& Instance();

protected:
	

};

#endif