#pragma once

#include <string>

class Error
{
public:
	static int last();

	static std::string getMessage(int errorCode);
};

