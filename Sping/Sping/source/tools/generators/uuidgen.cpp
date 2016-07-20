#include <tools/generators/uuidgen.h>
#include <iostream>

UUIDGenerator::UUIDGenerator()
{
}

UUIDGenerator::~UUIDGenerator()
{
}

int UUIDGenerator::setup()
{
	std::cout << "Successfully set up the UUID generator!\n";
	return 0;
}
