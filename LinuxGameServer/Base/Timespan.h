/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		Timespan.h
* 摘    要:		时间间隔
*     
* 作    者:		yanglinbo, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#pragma once


#include "Timestamp.h"


//////////////////////////////////////////////////////////////////////////
/// 时间间隔
/// @note 该类用于描述时间间隔,使用的是微秒的精度.
//////////////////////////////////////////////////////////////////////////
class Timespan
{
public:
	typedef Timestamp::TimeDiff	TimeDiff;	///< 时间差值定义

	Timespan();

	Timespan(TimeDiff microseconds);

	Timespan(long seconds, long microseconds);

	Timespan(int days, int hours, int minutes, int seconds, int microseconds);

	Timespan(const Timespan& timespan);

	~Timespan();

	Timespan& operator = (const Timespan& timespan);

	Timespan& operator = (TimeDiff microseconds);

	/// 赋值
	/// @param[in] day 日 值域[1,31]
	/// @param[in] hour 小时 值域[0,23]
	/// @param[in] minute 分钟 值域[0,59]
	/// @param[in] second 秒 值域[0,59]
	/// @param[in] microsecond 微秒 值域[0,999]
	Timespan& assign(int days, int hours, int minutes, int seconds, int microseconds);

	/// 赋值
	/// @param[in] second 秒 值域[0,59]
	/// @param[in] microsecond 微秒 值域[0,999]
	Timespan& assign(long seconds, long microseconds);

	/// 交换
	void swap(Timespan& timespan);

	bool operator == (const Timespan& ts) const;
	bool operator != (const Timespan& ts) const;
	bool operator >  (const Timespan& ts) const;
	bool operator >= (const Timespan& ts) const;
	bool operator <  (const Timespan& ts) const;
	bool operator <= (const Timespan& ts) const;

	bool operator == (TimeDiff microseconds) const;
	bool operator != (TimeDiff microseconds) const;
	bool operator >  (TimeDiff microseconds) const;
	bool operator >= (TimeDiff microseconds) const;
	bool operator <  (TimeDiff microseconds) const;
	bool operator <= (TimeDiff microseconds) const;

	Timespan operator + (const Timespan& d) const;
	Timespan operator - (const Timespan& d) const;
	Timespan& operator += (const Timespan& d);
	Timespan& operator -= (const Timespan& d);

	Timespan operator + (TimeDiff microseconds) const;
	Timespan operator - (TimeDiff microseconds) const;
	Timespan& operator += (TimeDiff microseconds);
	Timespan& operator -= (TimeDiff microseconds);

public:
	/// 获取天数
	int days() const;

	/// 获取小时(0 - 23)
	int hours() const;

	/// 获取分钟数(0 - 59)
	int minutes() const;

	/// 获取秒(0 - 59)
	int seconds() const;

	/// 获取毫秒数(0 - 999)
	int milliseconds() const;

public:
	/// 获取总共的小时数
	int totalHours() const;

	/// 获取总共的分钟数
	int totalMinutes() const;

	/// 获取总共的描述
	int totalSeconds() const;

	/// 获取总共的毫秒数
	TimeDiff totalMilliseconds() const;

	/// 获取总共的微秒数
	TimeDiff totalMicroseconds() const;

public:
	/// 获取微秒数段 单位:微秒 值域[0, 999]
	int microseconds() const;

	/// 获取秒数段 单位:微秒 值域[0, 999999]
	int useconds() const;

public:
	static const TimeDiff MILLISECONDS;	///< 一毫秒的微秒数
	static const TimeDiff SECONDS;		///< 一秒的微秒数
	static const TimeDiff MINUTES;		///< 一分钟的微秒数
	static const TimeDiff HOURS;		///< 一小时的微秒数
	static const TimeDiff DAYS;			///< 一天的微秒数

private:
	TimeDiff				m_span;		///< 时间间隔值(单位:微秒)
};


//
// inlines
//
inline int Timespan::days() const
{
	return int(m_span / DAYS);
}


inline int Timespan::hours() const
{
	return int((m_span / HOURS) % 24);
}


inline int Timespan::totalHours() const
{
	return int(m_span / HOURS);
}


inline int Timespan::minutes() const
{
	return int((m_span / MINUTES) % 60);
}


inline int Timespan::totalMinutes() const
{
	return int(m_span / MINUTES);
}


inline int Timespan::seconds() const
{
	return int((m_span / SECONDS) % 60);
}


inline int Timespan::totalSeconds() const
{
	return int(m_span / SECONDS);
}


inline int Timespan::milliseconds() const
{
	return int((m_span / MILLISECONDS) % 1000);
}


inline Timespan::TimeDiff Timespan::totalMilliseconds() const
{
	return m_span / MILLISECONDS;
}


inline int Timespan::microseconds() const
{
	return int(m_span % 1000);
}


inline int Timespan::useconds() const
{
	return int(m_span % 1000000);
}


inline Timespan::TimeDiff Timespan::totalMicroseconds() const
{
	return m_span;
}


inline bool Timespan::operator == (const Timespan& ts) const
{
	return m_span == ts.m_span;
}


inline bool Timespan::operator != (const Timespan& ts) const
{
	return m_span != ts.m_span;
}


inline bool Timespan::operator >  (const Timespan& ts) const
{
	return m_span > ts.m_span;
}


inline bool Timespan::operator >= (const Timespan& ts) const
{
	return m_span >= ts.m_span;
}


inline bool Timespan::operator <  (const Timespan& ts) const
{
	return m_span < ts.m_span;
}


inline bool Timespan::operator <= (const Timespan& ts) const
{
	return m_span <= ts.m_span;
}


inline bool Timespan::operator == (TimeDiff microseconds) const
{
	return m_span == microseconds;
}


inline bool Timespan::operator != (TimeDiff microseconds) const
{
	return m_span != microseconds;
}


inline bool Timespan::operator >  (TimeDiff microseconds) const
{
	return m_span > microseconds;
}


inline bool Timespan::operator >= (TimeDiff microseconds) const
{
	return m_span >= microseconds;
}


inline bool Timespan::operator <  (TimeDiff microseconds) const
{
	return m_span < microseconds;
}


inline bool Timespan::operator <= (TimeDiff microseconds) const
{
	return m_span <= microseconds;
}


inline void swap(Timespan& s1, Timespan& s2)
{
	s1.swap(s2);
}
