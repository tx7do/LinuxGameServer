/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		Debugger.cpp
* 摘    要:		
*     
* 作    者:		yanglinbo, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#include "Debugger.h"
#include <sstream>
#include <cstdlib>
#include <cstdio>


bool Debugger::isAvailable()
{
#if defined(_DEBUG)
	return ::IsDebuggerPresent() ? true : false;
#else
	return false;
#endif
}

void Debugger::message(const std::string& msg)
{
#if defined(_DEBUG)
	std::fputs("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n", stderr);
	std::fputs(msg.c_str(), stderr);
	std::fputs("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n", stderr);

	::MessageBoxA(NULL, msg.c_str(), ("Debugger"), MB_OK);

	if ( ::IsDebuggerPresent() )
	{
		::OutputDebugStringA(msg.c_str());
		::OutputDebugStringA("\n");
	}
#endif
}

void Debugger::message(const std::string& msg, const char* file, int line)
{
#if defined(_DEBUG)
	std::ostringstream str;
	str << msg << " [in file \"" << file << "\", line " << line << "]";
	message(str.str());
#endif
}

void Debugger::enter()
{
#if defined(_DEBUG)
	if ( ::IsDebuggerPresent() )
	{
		::DebugBreak();
	}
#endif
}

void Debugger::enter(const std::string& msg)
{
#if defined(_DEBUG)
	message(msg);
	enter();
#endif
}

void Debugger::enter(const std::string& msg, const char* file, int line)
{
#if defined(_DEBUG)
	message(msg, file, line);
	enter();
#endif
}

void Debugger::enter(const char* file, int line)
{
#if defined(_DEBUG)
	message("BREAK", file, line);
	enter();
#endif
}
