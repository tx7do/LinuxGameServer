/**********************************************************************
* Copyright (C) 2008 - ʢ������ - All Rights Reserved
*
* �ļ�����:		Timestamp.cpp
* ժ    Ҫ:		ʱ���
*     
* ��    ��:		yanglinbo, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#include "Timestamp.h"
#include "BaseException.h"

#include <algorithm>
#undef min
#undef max
#include <limits>
#include <ctime>
#include <sys/time.h>
#include <sys/times.h>

const Timestamp::TimeVal Timestamp::TIMEVAL_MIN = std::numeric_limits<Timestamp::TimeVal>::min();
const Timestamp::TimeVal Timestamp::TIMEVAL_MAX = std::numeric_limits<Timestamp::TimeVal>::max();

Timestamp::Timestamp()
{
	update();
}


Timestamp::Timestamp(TimeVal tv)
{
	m_ts = tv;
}


Timestamp::Timestamp(const Timestamp& other)
{
	m_ts = other.m_ts;
}


Timestamp::~Timestamp()
{
}


Timestamp& Timestamp::operator = (const Timestamp& other)
{
	m_ts = other.m_ts;
	return *this;
}


Timestamp& Timestamp::operator = (TimeVal tv)
{
	m_ts = tv;
	return *this;
}


void Timestamp::swap(Timestamp& timestamp)
{
	std::swap(m_ts, timestamp.m_ts);
}


Timestamp Timestamp::fromEpochTime(std::time_t t)
{
	return Timestamp(TimeVal(t)*resolution());
}


Timestamp Timestamp::fromUtcTime(UtcTimeVal val)
{
	val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
	val /= 10;
	return Timestamp(val);
}


void Timestamp::update()
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		throw SystemException("cannot get time of day");
	m_ts = TimeVal(tv.tv_sec) * resolution() + tv.tv_usec;
}
