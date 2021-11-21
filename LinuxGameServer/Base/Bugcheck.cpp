/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		Bugcheck.cpp
* 摘    要:		错误检测，提供一些运行时错误检测的宏。
*     
* 作    者:		yanglinbo, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#include "Bugcheck.h"
#include "Debugger.h"
#include <sstream>
#include "BaseException.h"


void Bugcheck::assertion(const char* cond, const char* file, int line)
{
	Debugger::enter(std::string("Assertion violation: ") + cond, file, line);
	throw AssertionViolationException(what(cond, file, line));
}


void Bugcheck::nullPointer(const char* ptr, const char* file, int line)
{
	Debugger::enter(std::string("NULL pointer: ") + ptr, file, line);
	throw NullPointerException(what(ptr, file, line));
}


void Bugcheck::bugcheck(const char* file, int line)
{
	Debugger::enter("Bugcheck", file, line);
	throw BugcheckException(what(0, file, line));
}


void Bugcheck::bugcheck(const char* msg, const char* file, int line)
{
	std::string m("Bugcheck");
	if (msg)
	{
		m.append(": ");
		m.append(msg);
	}
	Debugger::enter(m, file, line);
	throw BugcheckException(what(msg, file, line));
}


void Bugcheck::debugger(const char* file, int line)
{
	Debugger::enter(file, line);
}


void Bugcheck::debugger(const char* msg, const char* file, int line)
{
	Debugger::enter(msg, file, line);
}


std::string Bugcheck::what(const char* msg, const char* file, int line)
{
	std::ostringstream str;
	if (msg) str << msg << " ";
	str << "in file \"" << file << "\", line " << line;
	return str.str();
}

void Bugcheck::unexpected(const char* file, int line)
{
#ifdef _DEBUG
	try
	{
		std::string msg("Unexpected exception in noexcept function or destructor: ");
		try
		{
			throw;
		}
		catch (Exception& exc)
		{
			msg += exc.displayText();
		}
		catch (std::exception& exc)
		{
			msg += exc.what();
		}
		catch (...)
		{
			msg += "unknown exception";
		}
		Debugger::enter(msg, file, line);
	}
	catch (...)
	{
	}
#endif	
}
