/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		Timespan.cpp
* 摘    要:		时间间隔
*     
* 作    者:		yanglinbo, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#include "Timespan.h"
#include <algorithm>


const Timespan::TimeDiff Timespan::MILLISECONDS	= 1000;
const Timespan::TimeDiff Timespan::SECONDS		= 1000 * Timespan::MILLISECONDS;
const Timespan::TimeDiff Timespan::MINUTES		= 60 * Timespan::SECONDS;
const Timespan::TimeDiff Timespan::HOURS		= 60 * Timespan::MINUTES;
const Timespan::TimeDiff Timespan::DAYS			= 24 * Timespan::HOURS;


Timespan::Timespan() : m_span(0)
{
}

Timespan::Timespan(TimeDiff microseconds) : m_span(microseconds)
{
}


Timespan::Timespan(long seconds, long microseconds)
: m_span(TimeDiff(seconds)*SECONDS + microseconds)
{
}


Timespan::Timespan(int days, int hours, int minutes, int seconds, int microseconds)
: m_span(TimeDiff(microseconds) + TimeDiff(seconds)*SECONDS + TimeDiff(minutes)*MINUTES + TimeDiff(hours)*HOURS + TimeDiff(days)*DAYS)
{
}


Timespan::Timespan(const Timespan& timespan) : m_span(timespan.m_span)
{
}


Timespan::~Timespan()
{
}


Timespan& Timespan::operator = (const Timespan& timespan)
{
	m_span = timespan.m_span;
	return *this;
}


Timespan& Timespan::operator = (TimeDiff microseconds)
{
	m_span = microseconds;
	return *this;
}


Timespan& Timespan::assign(int days, int hours, int minutes, int seconds, int microseconds)
{
	m_span = TimeDiff(microseconds)
		+ TimeDiff(seconds) * SECONDS 
		+ TimeDiff(minutes) * MINUTES 
		+ TimeDiff(hours) * HOURS 
		+ TimeDiff(days) * DAYS;
	return *this;
}


Timespan& Timespan::assign(long seconds, long microseconds)
{
	m_span = TimeDiff(seconds)*SECONDS + TimeDiff(microseconds);
	return *this;
}


void Timespan::swap(Timespan& timespan)
{
	std::swap(m_span, timespan.m_span);
}


Timespan Timespan::operator + (const Timespan& d) const
{
	return Timespan(m_span + d.m_span);
}


Timespan Timespan::operator - (const Timespan& d) const
{
	return Timespan(m_span - d.m_span);
}


Timespan& Timespan::operator += (const Timespan& d)
{
	m_span += d.m_span;
	return *this;
}


Timespan& Timespan::operator -= (const Timespan& d)
{
	m_span -= d.m_span;
	return *this;
}


Timespan Timespan::operator + (TimeDiff microseconds) const
{
	return Timespan(m_span + microseconds);
}


Timespan Timespan::operator - (TimeDiff microseconds) const
{
	return Timespan(m_span - microseconds);
}


Timespan& Timespan::operator += (TimeDiff microseconds)
{
	m_span += microseconds;
	return *this;
}


Timespan& Timespan::operator -= (TimeDiff microseconds)
{
	m_span -= microseconds;
	return *this;
}
