
#include "Timezone.h"
#include "BaseException.h"

#include <ctime>
#include <mutex>

int Timezone::tzd()
{
	return utcOffset() + dst();
}

class TZInfo
{
public:
	TZInfo()
	{
		tzset();
	}

	int timeZone()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		tzset();
		return -timezone;
	}

	const char* name(bool dst)
	{
		std::unique_lock<std::mutex> lock(_mutex);

		tzset();
		return tzname[dst ? 1 : 0];
	}

private:
	std::mutex _mutex;
};


static TZInfo tzInfo;


int Timezone::utcOffset()
{
	return tzInfo.timeZone();
}


int Timezone::dst()
{
	std::time_t now = std::time(NULL);
	struct std::tm t;
	if (!localtime_r(&now, &t))
		throw SystemException("cannot get local time DST offset");
	return t.tm_isdst == 1 ? 3600 : 0;
}


bool Timezone::isDst(const Timestamp& timestamp)
{
	std::time_t time = timestamp.epochTime();
	struct std::tm* tms = std::localtime(&time);
	if (!tms) throw SystemException("cannot get local time DST flag");
	return tms->tm_isdst > 0;
}


std::string Timezone::name()
{
	return std::string(tzInfo.name(dst() != 0));
}


std::string Timezone::standardName()
{
	return std::string(tzInfo.name(false));
}


std::string Timezone::dstName()
{
	return std::string(tzInfo.name(true));
}
