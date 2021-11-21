/**********************************************************************
* Copyright (C) 2008 - ʢ������ - All Rights Reserved
*
* �ļ�����:		Timespan.h
* ժ    Ҫ:		ʱ����
*     
* ��    ��:		yanglinbo, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#pragma once


#include "Timestamp.h"


//////////////////////////////////////////////////////////////////////////
/// ʱ����
/// @note ������������ʱ����,ʹ�õ���΢��ľ���.
//////////////////////////////////////////////////////////////////////////
class Timespan
{
public:
	typedef Timestamp::TimeDiff	TimeDiff;	///< ʱ���ֵ����

	Timespan();

	Timespan(TimeDiff microseconds);

	Timespan(long seconds, long microseconds);

	Timespan(int days, int hours, int minutes, int seconds, int microseconds);

	Timespan(const Timespan& timespan);

	~Timespan();

	Timespan& operator = (const Timespan& timespan);

	Timespan& operator = (TimeDiff microseconds);

	/// ��ֵ
	/// @param[in] day �� ֵ��[1,31]
	/// @param[in] hour Сʱ ֵ��[0,23]
	/// @param[in] minute ���� ֵ��[0,59]
	/// @param[in] second �� ֵ��[0,59]
	/// @param[in] microsecond ΢�� ֵ��[0,999]
	Timespan& assign(int days, int hours, int minutes, int seconds, int microseconds);

	/// ��ֵ
	/// @param[in] second �� ֵ��[0,59]
	/// @param[in] microsecond ΢�� ֵ��[0,999]
	Timespan& assign(long seconds, long microseconds);

	/// ����
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
	/// ��ȡ����
	int days() const;

	/// ��ȡСʱ(0 - 23)
	int hours() const;

	/// ��ȡ������(0 - 59)
	int minutes() const;

	/// ��ȡ��(0 - 59)
	int seconds() const;

	/// ��ȡ������(0 - 999)
	int milliseconds() const;

public:
	/// ��ȡ�ܹ���Сʱ��
	int totalHours() const;

	/// ��ȡ�ܹ��ķ�����
	int totalMinutes() const;

	/// ��ȡ�ܹ�������
	int totalSeconds() const;

	/// ��ȡ�ܹ��ĺ�����
	TimeDiff totalMilliseconds() const;

	/// ��ȡ�ܹ���΢����
	TimeDiff totalMicroseconds() const;

public:
	/// ��ȡ΢������ ��λ:΢�� ֵ��[0, 999]
	int microseconds() const;

	/// ��ȡ������ ��λ:΢�� ֵ��[0, 999999]
	int useconds() const;

public:
	static const TimeDiff MILLISECONDS;	///< һ�����΢����
	static const TimeDiff SECONDS;		///< һ���΢����
	static const TimeDiff MINUTES;		///< һ���ӵ�΢����
	static const TimeDiff HOURS;		///< һСʱ��΢����
	static const TimeDiff DAYS;			///< һ���΢����

private:
	TimeDiff				m_span;		///< ʱ����ֵ(��λ:΢��)
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
