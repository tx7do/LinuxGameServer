#pragma once

#include "DateTime.h"
#include "LocalDateTime.h"

#include <string>

class Timestamp;
class Timespan;

class DateTimeFormatter
{
public:
	enum
	{
		UTC = 0xFFFF
	};

	static std::string format(const Timestamp& timestamp, const std::string& fmt, int timeZoneDifferential = UTC);

	static std::string format(const DateTime& dateTime, const std::string& fmt, int timeZoneDifferential = UTC);

	static std::string format(const LocalDateTime& dateTime, const std::string& fmt);

	static std::string format(const Timespan& timespan, const std::string& fmt = "%dd %H:%M:%S.%i");

	static void append(std::string& str, const Timestamp& timestamp, const std::string& fmt, int timeZoneDifferential = UTC);

	static void append(std::string& str, const DateTime& dateTime, const std::string& fmt, int timeZoneDifferential = UTC);

	static void append(std::string& str, const LocalDateTime& dateTime, const std::string& fmt);

	static void append(std::string& str, const Timespan& timespan, const std::string& fmt = "%dd %H:%M:%S.%i");

	static std::string tzdISO(int timeZoneDifferential);

	static std::string tzdRFC(int timeZoneDifferential);

	static void tzdISO(std::string& str, int timeZoneDifferential);

	static void tzdRFC(std::string& str, int timeZoneDifferential);
};


//
// inlines
//
inline std::string DateTimeFormatter::format(const Timestamp& timestamp, const std::string& fmt, int timeZoneDifferential)
{
	DateTime dateTime(timestamp);
	return format(dateTime, fmt, timeZoneDifferential);
}


inline std::string DateTimeFormatter::format(const DateTime& dateTime, const std::string& fmt, int timeZoneDifferential)
{
	std::string result;
	result.reserve(64);
	append(result, dateTime, fmt, timeZoneDifferential);
	return result;
}


inline std::string DateTimeFormatter::format(const LocalDateTime& dateTime, const std::string& fmt)
{
	return format(dateTime._dateTime, fmt, dateTime._tzd);
}


inline std::string DateTimeFormatter::format(const Timespan& timespan, const std::string& fmt)
{
	std::string result;
	result.reserve(32);
	append(result, timespan, fmt);
	return result;
}


inline void DateTimeFormatter::append(std::string& str, const Timestamp& timestamp, const std::string& fmt, int timeZoneDifferential)
{
	DateTime dateTime(timestamp);
	append(str, dateTime, fmt, timeZoneDifferential);
}


inline std::string DateTimeFormatter::tzdISO(int timeZoneDifferential)
{
	std::string result;
	result.reserve(8);
	tzdISO(result, timeZoneDifferential);
	return result;
}


inline std::string DateTimeFormatter::tzdRFC(int timeZoneDifferential)
{
	std::string result;
	result.reserve(8);
	tzdRFC(result, timeZoneDifferential);
	return result;
}
