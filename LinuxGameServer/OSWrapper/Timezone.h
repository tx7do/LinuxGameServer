#pragma once

#include "Timestamp.h"
#include <string>

class Timezone
{
public:
	static int utcOffset();

	static int dst();

	static bool isDst(const Timestamp& timestamp);

	static int tzd();

	static std::string name();

	static std::string standardName();

	static std::string dstName();
};
