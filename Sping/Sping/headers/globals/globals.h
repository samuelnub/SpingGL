#pragma once
#ifndef GLOBALS_GLOBALS_H
#define GLOBALS_GLOBALS_H

enum Status
{
	SUCC = 0,
	GENERIC_ERROR = -1,
	FATAL_ERROR = -2,
	NO_HOPE = -3,
	GOD_HAVE_MERCY = -4,
	OH_NO = -5
};


namespace Windowsettings
{
	const int glMajor = 4;
	const int glMinor = 5;

	const int depthBits = 24;
	const int stencilBits = 8;
	const bool doublebuffer = 1;
	const int multisamples = 8;

	const char *name = "Sping!";

	const int width = 1280;
	const int height = 720;
}

#endif