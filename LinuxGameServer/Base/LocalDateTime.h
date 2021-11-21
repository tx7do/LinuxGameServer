#pragma once

#include "DateTime.h"


class LocalDateTime
{
public:
	LocalDateTime();

	LocalDateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);

	LocalDateTime(const DateTime& dateTime);

	LocalDateTime(double julianDay);

	LocalDateTime(const LocalDateTime& dateTime);

	~LocalDateTime();

	LocalDateTime& operator = (const LocalDateTime& dateTime);

	LocalDateTime& operator = (const Timestamp& timestamp);

	LocalDateTime& operator = (double julianDay);

	LocalDateTime& assign(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microseconds = 0);

	void swap(LocalDateTime& dateTime);

	int year() const;

	int month() const;

	int week(int firstDayOfWeek = DateTime::MONDAY) const;

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

	int tzd() const;

	DateTime utc() const;

	Timestamp timestamp() const;

	Timestamp::UtcTimeVal utcTime() const;

	bool operator == (const LocalDateTime& dateTime) const;
	bool operator != (const LocalDateTime& dateTime) const;
	bool operator <  (const LocalDateTime& dateTime) const;
	bool operator <= (const LocalDateTime& dateTime) const;
	bool operator >  (const LocalDateTime& dateTime) const;
	bool operator >= (const LocalDateTime& dateTime) const;

	LocalDateTime  operator +  (const Timespan& span) const;
	LocalDateTime  operator -  (const Timespan& span) const;
	Timespan       operator -  (const LocalDateTime& dateTime) const;
	LocalDateTime& operator += (const Timespan& span);
	LocalDateTime& operator -= (const Timespan& span);

protected:
	LocalDateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff, int tzd);

	void determineTzd(bool adjust = false);

	void adjustForTzd();

	std::time_t dstOffset(int& dstOffset) const;

private:
	DateTime _dateTime;
	int      _tzd;

	friend class DateTimeFormatter;
	friend class DateTimeParser;
};


//
// inlines
//
inline int LocalDateTime::year() const
{
	return _dateTime.year();
}


inline int LocalDateTime::month() const
{
	return _dateTime.month();
}


inline int LocalDateTime::week(int firstDayOfWeek) const
{
	return _dateTime.week(firstDayOfWeek);
}


inline int LocalDateTime::day() const
{
	return _dateTime.day();
}


inline int LocalDateTime::dayOfWeek() const
{
	return _dateTime.dayOfWeek();
}


inline int LocalDateTime::dayOfYear() const
{
	return _dateTime.dayOfYear();
}


inline int LocalDateTime::hour() const
{
	return _dateTime.hour();
}


inline int LocalDateTime::hourAMPM() const
{
	return _dateTime.hourAMPM();
}


inline bool LocalDateTime::isAM() const
{
	return _dateTime.isAM();
}


inline bool LocalDateTime::isPM() const
{
	return _dateTime.isPM();
}


inline int LocalDateTime::minute() const
{
	return _dateTime.minute();
}


inline int LocalDateTime::second() const
{
	return _dateTime.second();
}


inline int LocalDateTime::millisecond() const
{
	return _dateTime.millisecond();
}


inline int LocalDateTime::microsecond() const
{
	return _dateTime.microsecond();
}


inline double LocalDateTime::julianDay() const
{
	return _dateTime.julianDay();
}


inline int LocalDateTime::tzd() const
{
	return _tzd;
}


inline Timestamp LocalDateTime::timestamp() const
{
	return Timestamp::fromUtcTime(_dateTime.utcTime());
}


inline Timestamp::UtcTimeVal LocalDateTime::utcTime() const
{
	return _dateTime.utcTime() - ((Timestamp::TimeDiff)_tzd) * 10000000;
}


inline void LocalDateTime::adjustForTzd()
{
	_dateTime += Timespan(((Timestamp::TimeDiff)_tzd) * Timespan::SECONDS);
}


inline void swap(LocalDateTime& d1, LocalDateTime& d2)
{
	d1.swap(d2);
}
