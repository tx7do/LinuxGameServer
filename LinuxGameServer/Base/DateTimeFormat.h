#pragma once

#include <string>

class DateTimeFormat
{
public:
	static const std::string ISO8601_FORMAT;

	static const std::string ISO8601_FRAC_FORMAT;

	static const std::string RFC822_FORMAT;

	static const std::string RFC1123_FORMAT;

	static const std::string HTTP_FORMAT;

	static const std::string RFC850_FORMAT;

	static const std::string RFC1036_FORMAT;

	static const std::string ASCTIME_FORMAT;

	static const std::string SORTABLE_FORMAT;

	static const std::string WEEKDAY_NAMES[7];

	static const std::string MONTH_NAMES[12];
};
