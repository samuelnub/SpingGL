#pragma once
#ifndef TOOLS_GENERATORS_UUID_GENERATOR_H
#define TOOLS_GENERATORS_UUID_GENERATOR_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

#include <vector>

class UUIDGenerator
{
private:
	boost::uuids::random_generator _generator;

public:
	UUIDGenerator();
	~UUIDGenerator();

	int setup();

	inline void gen(boost::uuids::uuid &uuid)
	{
		uuid = this->_generator();
	}

protected:


};

#endif