#pragma once

#include "Timestamp.h"
#include "Timespan.h"

struct tm;

class DateTime
{
public:
	enum Months
	{
		JANUARY = 1,
		FEBRUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER
	};

	enum DaysOfWeek
	{
		SUNDAY = 0,
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY
	};

	DateTime();

	DateTime(const tm& tmStruct);

	DateTime(const Timestamp& timestamp);

	DateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);

	DateTime(double julianDay);

	DateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff);

	DateTime(const DateTime& dateTime);

	~DateTime();

	DateTime& operator = (const DateTime& dateTime);

	DateTime& operator = (const Timestamp& timestamp);

	DateTime& operator = (double julianDay);

	DateTime& assign(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microseconds = 0);

	void swap(DateTime& dateTime);

	int year() const;

	int month() const;

	int week(int firstDayOfWeek = MONDAY) const;

	int day() const;

	int dayOfWeek() const;

	int dayOfYear() const;

	int hour() const;

	int hourAMPM() const;

	bool isAM() const;

	bool isPM() const;

	int minute() const;

	int second() const;

	int millisecond() const;

	int microsecond() const;

	double julianDay() const;

	Timestamp timestamp() const;

	Timestamp::UtcTimeVal utcTime() const;

	bool operator == (const DateTime& dateTime) const;
	bool operator != (const DateTime& dateTime) const;
	bool operator <  (const DateTime& dateTime) const;
	bool operator <= (const DateTime& dateTime) const;
	bool operator >  (const DateTime& dateTime) const;
	bool operator >= (const DateTime& dateTime) const;

	DateTime  operator +  (const Timespan& span) const;
	DateTime  operator -  (const Timespan& span) const;
	Timespan  operator -  (const DateTime& dateTime) const;
	DateTime& operator += (const Timespan& span);
	DateTime& operator -= (const Timespan& span);

	tm makeTM() const;

	void makeUTC(int tzd);

	void makeLocal(int tzd);

	static bool isLeapYear(int year);

	static int daysOfMonth(int year, int month);

	static bool isValid(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);

protected:
	static double toJulianDay(Timestamp::UtcTimeVal utcTime);

	static double toJulianDay(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);

	static Timestamp::UtcTimeVal toUtcTime(double julianDay);

	void computeGregorian(double julianDay);

	void computeDaytime();

private:
	void checkLimit(short& lower, short& higher, short limit);
	void normalize();

	Timestamp::UtcTimeVal _utcTime;
	short _year;
	short _month;
	short _day;
	short _hour;
	short _minute;
	short _second;
	short _millisecond;
	short _microsecond;
};


//
// inlines
//


inline double DateTime::toJulianDay(Timestamp::UtcTimeVal utcTime)
{
	double utcDays = double(utcTime) / 864000000000.0;
	return utcDays + 2299160.5;
}


inline Timestamp::UtcTimeVal DateTime::toUtcTime(double julianDay)
{
	return Timestamp::UtcTimeVal((julianDay - 2299160.5) * 864000000000.0);
}


inline Timestamp DateTime::timestamp() const
{
	return Timestamp::fromUtcTime(_utcTime);
}


inline Timestamp::UtcTimeVal DateTime::utcTime() const
{
	return _utcTime;
}


inline int DateTime::year() const
{
	return _year;
}


inline int DateTime::month() const
{
	return _month;
}


inline int DateTime::day() const
{
	return _day;
}


inline int DateTime::hour() const
{
	return _hour;
}


inline int DateTime::hourAMPM() const
{
	if (_hour < 1)
		return 12;
	else if (_hour > 12)
		return _hour - 12;
	else
		return _hour;
}


inline bool DateTime::isAM() const
{
	return _hour < 12;
}


inline bool DateTime::isPM() const
{
	return _hour >= 12;
}


inline int DateTime::minute() const
{
	return _minute;
}


inline int DateTime::second() const
{
	return _second;
}


inline int DateTime::millisecond() const
{
	return _millisecond;
}


inline int DateTime::microsecond() const
{
	return _microsecond;
}


inline bool DateTime::operator == (const DateTime& dateTime) const
{
	return _utcTime == dateTime._utcTime;
}


inline bool DateTime::operator != (const DateTime& dateTime) const
{
	return _utcTime != dateTime._utcTime;
}


inline bool DateTime::operator <  (const DateTime& dateTime) const
{
	return _utcTime < dateTime._utcTime;
}


inline bool DateTime::operator <= (const DateTime& dateTime) const
{
	return _utcTime <= dateTime._utcTime;
}


inline bool DateTime::operator >  (const DateTime& dateTime) const
{
	return _utcTime > dateTime._utcTime;
}


inline bool DateTime::operator >= (const DateTime& dateTime) const
{
	return _utcTime >= dateTime._utcTime;
}


inline bool DateTime::isLeapYear(int year)
{
	return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}


inline void swap(DateTime& d1, DateTime& d2)
{
	d1.swap(d2);
}
