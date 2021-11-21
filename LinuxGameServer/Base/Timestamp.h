/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		Timestamp.h
* 摘    要:		时间戳
*     
* 作    者:		yanglinbo, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#pragma once


#include <ctime>


//////////////////////////////////////////////////////////////////////////
/// 时间戳
/// @note 时间戳保存了一个时间值,其精度为微秒.
///       时间戳是UTC时间,基于当地时区.
//////////////////////////////////////////////////////////////////////////
class Timestamp
{
public:
	typedef signed long long	TimeVal;		///< UTC时间值,微秒精度.
	typedef signed long long	UtcTimeVal;		///< UTC时间值,100毫微秒精度.
	typedef signed long long	TimeDiff;		///< 时间差值,微秒精度.

	static const TimeVal TIMEVAL_MIN;
	static const TimeVal TIMEVAL_MAX;


	Timestamp();
	Timestamp(TimeVal tv);
	Timestamp(const Timestamp& other);
	~Timestamp();

	Timestamp& operator = (const Timestamp& other);
	Timestamp& operator = (TimeVal tv);

	/// 交换
	void swap(Timestamp& timestamp);

	/// 更新
	void update();

	bool operator == (const Timestamp& ts) const;
	bool operator != (const Timestamp& ts) const;
	bool operator >  (const Timestamp& ts) const;
	bool operator >= (const Timestamp& ts) const;
	bool operator <  (const Timestamp& ts) const;
	bool operator <= (const Timestamp& ts) const;

	Timestamp  operator +  (TimeDiff d) const;
	Timestamp  operator -  (TimeDiff d) const;
	TimeDiff   operator -  (const Timestamp& ts) const;
	Timestamp& operator += (TimeDiff d);
	Timestamp& operator -= (TimeDiff d);

	/// 获取新纪元时间(秒)
	std::time_t epochTime() const;

	/// 获取新纪元时间(微秒)
	TimeVal epochMicroseconds() const;

	/// 获取世界标准时间
	UtcTimeVal utcTime() const;

	/// 获取流逝时间
	TimeDiff elapsed() const;

	/// 是否时间流逝
	bool isElapsed(TimeDiff interval) const;

public:
	/// 创建一个时间戳,使用新纪元时间
	static Timestamp fromEpochTime(std::time_t t);

	/// 创建一个时间戳,使用世界标准时间
	static Timestamp fromUtcTime(UtcTimeVal val);

	/// 时间解析度
	static TimeVal resolution();

private:
	TimeVal		m_ts;	///< 时间(微秒)
};


inline bool Timestamp::operator == (const Timestamp& ts) const
{
	return m_ts == ts.m_ts;
}


inline bool Timestamp::operator != (const Timestamp& ts) const
{
	return m_ts != ts.m_ts;
}


inline bool Timestamp::operator >  (const Timestamp& ts) const
{
	return m_ts > ts.m_ts;
}


inline bool Timestamp::operator >= (const Timestamp& ts) const
{
	return m_ts >= ts.m_ts;
}


inline bool Timestamp::operator <  (const Timestamp& ts) const
{
	return m_ts < ts.m_ts;
}


inline bool Timestamp::operator <= (const Timestamp& ts) const
{
	return m_ts <= ts.m_ts;
}


inline Timestamp Timestamp::operator + (Timestamp::TimeDiff d) const
{
	return Timestamp(m_ts + d);
}


inline Timestamp Timestamp::operator - (Timestamp::TimeDiff d) const
{
	return Timestamp(m_ts - d);
}


inline Timestamp::TimeDiff Timestamp::operator - (const Timestamp& ts) const
{
	return m_ts - ts.m_ts;
}


inline Timestamp& Timestamp::operator += (Timestamp::TimeDiff d)
{
	m_ts += d;
	return *this;
}


inline Timestamp& Timestamp::operator -= (Timestamp::TimeDiff d)
{
	m_ts -= d;
	return *this;
}


inline std::time_t Timestamp::epochTime() const
{
	return std::time_t(m_ts / resolution());
}


inline Timestamp::UtcTimeVal Timestamp::utcTime() const
{
	return (m_ts * 10) + (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
}


inline Timestamp::TimeVal Timestamp::epochMicroseconds() const
{
	return m_ts;
}


inline Timestamp::TimeDiff Timestamp::elapsed() const
{
	Timestamp now;
	return now - *this;
}


inline bool Timestamp::isElapsed(Timestamp::TimeDiff interval) const
{
	Timestamp now;
	Timestamp::TimeDiff diff = now - *this;
	return diff >= interval;
}


inline Timestamp::TimeVal Timestamp::resolution()
{
	return 1000000;
}


inline void swap(Timestamp& s1, Timestamp& s2)
{
	s1.swap(s2);
}
