#pragma once
#ifndef GLOBALS_XMLDEFAULTS_H
#define GLOBALS_XMLDEFAULTS_H

namespace XMLDefaults
{
	const char *defaultSettings =
	{
		"<?xml version=\"1.0\"?>"
		"<root>"
		"	<WindowSettings>"
		"		<glMajor>4</glMajor>"
		"		<glMinor>3</glMinor>"
		"		"
		"		<depthBits>24</depthBits>"
		"		<stencilBits>8</stencilBits>"
		"		<doubleBuffer>1</doubleBuffer>"
		"		<multisamples>8</multisamples>"
		"		<title>Sping!</title>"
		"		"
		"		<width>1280</width>"
		"		<height>720</height>"
		"		<fullscreen>0</fullscreen>"
		"	</WindowSettings>"
		"</root>"
	};

}

#endif