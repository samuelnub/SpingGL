#pragma once
#ifndef TOOLS_XMLPARSER_H
#define TOOLS_XMLPARSER_H

#include <tinyxml2/tinyxml2.h>
#include <vector>
#include <map>
#include <string>

//all xml files made by the idiot (yours truly) will and must have a root node called <root>, generic amirite
//in the functions below, an element requested's parents includes the element its attached to
//referenced from https://shilohjames.wordpress.com/2014/04/27/tinyxml2-tutorial/, great resource for this library
//test return values to do the appropriate conditional statements on your end
class XMLHolder
{
private:

public:
	XMLHolder();
	~XMLHolder();

	void setup();

	/*
	int load(const char *name, const char *filePath); //load from file onto map
	std::string getElement(const char *name, const std::vector<const char *> &parents);

	void create(const char *name);
	int addElement(const char *name, const std::vector<const char *> &parents, const char *value);
	int writeToFile(const char *name, const char *filePath);
	*/

	//forgive my public variables
	std::map<const char *, tinyxml2::XMLDocument> xmlDocs;
	tinyxml2::XMLError xmlErr;

protected:


};

#endif