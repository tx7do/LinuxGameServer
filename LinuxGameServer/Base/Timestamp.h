/**********************************************************************
* Copyright (C) 2008 - ʢ������ - All Rights Reserved
*
* �ļ�����:		Timestamp.h
* ժ    Ҫ:		ʱ���
*     
* ��    ��:		yanglinbo, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#pragma once


#include <ctime>


//////////////////////////////////////////////////////////////////////////
/// ʱ���
/// @note ʱ���������һ��ʱ��ֵ,�侫��Ϊ΢��.
///       ʱ�����UTCʱ��,���ڵ���ʱ��.
//////////////////////////////////////////////////////////////////////////
class Timestamp
{
public:
	typedef signed long long	TimeVal;		///< UTCʱ��ֵ,΢�뾫��.
	typedef signed long long	UtcTimeVal;		///< UTCʱ��ֵ,100��΢�뾫��.
	typedef signed long long	TimeDiff;		///< ʱ���ֵ,΢�뾫��.

	static const TimeVal TIMEVAL_MIN;
	static const TimeVal TIMEVAL_MAX;


	Timestamp();
	Timestamp(TimeVal tv);
	Timestamp(const Timestamp& other);
	~Timestamp();

	Timestamp& operator = (const Timestamp& other);
	Timestamp& operator = (TimeVal tv);

	/// ����
	void swap(Timestamp& timestamp);

	/// ����
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

	/// ��ȡ�¼�Ԫʱ��(��)
	std::time_t epochTime() const;

	/// ��ȡ�¼�Ԫʱ��(΢��)
	TimeVal epochMicroseconds() const;

	/// ��ȡ�����׼ʱ��
	UtcTimeVal utcTime() const;

	/// ��ȡ����ʱ��
	TimeDiff elapsed() const;

	/// �Ƿ�ʱ������
	bool isElapsed(TimeDiff interval) const;

public:
	/// ����һ��ʱ���,ʹ���¼�Ԫʱ��
	static Timestamp fromEpochTime(std::time_t t);

	/// ����һ��ʱ���,ʹ�������׼ʱ��
	static Timestamp fromUtcTime(UtcTimeVal val);

	/// ʱ�������
	static TimeVal resolution();

private:
	TimeVal		m_ts;	///< ʱ��(΢��)
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
