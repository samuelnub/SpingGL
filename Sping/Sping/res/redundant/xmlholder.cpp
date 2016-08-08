#include <tools/xmlholder.h>
#include <iostream>

XMLHolder::XMLHolder()
{
}

XMLHolder::~XMLHolder()
{
}

void XMLHolder::setup()
{

}

/*
int XMLParser::load(const char *name, const char *filePath)
{
	if (this->xmlDocs.count(name) == 0)
	{
		tinyxml2::XMLDocument *tempDoc = new tinyxml2::XMLDocument;
		this->_xmlErr = tempDoc->LoadFile(filePath);

		if (this->_xmlErr != tinyxml2::XML_SUCCESS)
		{
			throw "Could not load XML file!\n";
		}

		this->xmlDocs[name] = tempDoc;
		delete tempDoc;
	}
	else
	{
		std::cout << "XML doc with the name " << name << " has already been loaded and is in this map!\n";
		return -1;
	}
	return 0;
}

std::string XMLParser::getElement(const char * name, const std::vector<const char*>& parents)
{
	if (this->xmlDocs.count(name) != 0)
	{
		tinyxml2::XMLNode *tempRoot = this->xmlDocs[name].FirstChild(); //set to root element
		if (tempRoot == nullptr)
		{
			std::cout << "Could not get the root element of XML document named " << name << "!\n";
		}

		tinyxml2::XMLElement *tempElement = tempRoot->FirstChildElement(parents.at(0)); //get first element from your nested vector of parents requested
		if (tempElement == nullptr)
		{
			std::cout << "Could not find the element parent requested: " << parents.at(0) << "!\n";
			return;
		}

		if (parents.size() > 1)
		{
			//start at element index 1 since we've already covered 0
			for (unsigned int i = 1; i < parents.size(); i++)
			{
				tempElement = tempElement->FirstChildElement(parents.at(i));
				if (tempElement == nullptr)
				{
					std::cout << "Could not find the element parent requested: " << parents.at(0) << "!\n";
					return;
				}
			}
		}
		std::string text;
		tinyxml2::XMLText *textNode = tempElement->FirstChild()->ToText();
		text = textNode->Value();

		return text;
	}
	else
	{
		std::cout << "The XML document name you requested: " << name << "; doesn't exist!\n";
	}
}

void XMLParser::create(const char * name)
{
	if (this->xmlDocs.count(name) == 0)
	{
		tinyxml2::XMLDocument *tempXML = new tinyxml2::XMLDocument;
		//totally empty right now

		//create default root element called <root>
		tinyxml2::XMLNode *tempNode = tempXML->NewElement("root");

		tempXML->InsertFirstChild(tempNode);

		this->xmlDocs[name] = tempXML->Value;
		delete tempXML;
	}
	else
	{
		std::cout << "The XML document " << name << " you want to create already exists!\n";
	}
}

int XMLParser::addElement(const char * name, const std::vector<const char*>& parents, const char * value)
{
	if (this->xmlDocs.count(name) != 0)
	{
		tinyxml2::XMLDocument *tempDoc = new tinyxml2::XMLDocument;
		tempDoc = this->xmlDocs.at(name).Value;

		tinyxml2::XMLElement *tempElement = tempDoc->FirstChildElement("root"); //at root now
		if (tempElement == nullptr)
		{
			std::cout << "Couldn't find root element of XML document " << name << "!\n";
			return -1;
		}

		for (unsigned int i = 0; i < parents.size(); i++)
		{
			tinyxml2::XMLElement *temptempElement = tempElement->FirstChildElement(parents[i]); //check if the upcoming element exists
			if (temptempElement == nullptr)
			{
				std::cout << "XML element parent " << parents[i] << " doesn't exist! Adding one...\n";
				tinyxml2::XMLNode *rootNode = tempDoc->NewElement(parents[i]);
				tinyxml2::XMLNode *tempNode = tempElement->InsertEndChild(rootNode);
			}
			tempElement = tempElement->FirstChildElement(parents[i]);
		}

		tempElement->SetText(value);
		tempDoc->InsertEndChild(tempElement);

		this->xmlDocs[name] = tempDoc->Value;
		return 0;
	}
	else
	{
		std::cout << "Couldn't add element " << value << " to the XML document " << name << "! Don't think the doc you request exists!\n";
		return -1;
	}
}

int XMLParser::writeToFile(const char * name, const char * filePath)
{
	if (this->xmlDocs.count(name) != 0)
	{
		this->_xmlErr = this->xmlDocs[name].SaveFile(filePath);
		if (this->_xmlErr != tinyxml2::XML_SUCCESS)
		{
			throw "Could not write XML file!\n";
		}
	}
	else
	{
		std::cout << "XML document " << name << " can't be written to file because it doesn't exist!\n";
		return -1;
	}
}
*/
//a failed attempt, go implement your own functions to read your own xml files