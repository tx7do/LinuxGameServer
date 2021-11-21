/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		Debugger.h
* 摘    要:		调试器
*     
* 作    者:		yanglinbo, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#pragma once


#include <string>


//////////////////////////////////////////////////////////////////////////
/// 调试器
//////////////////////////////////////////////////////////////////////////
class Debugger
{
public:
	/// 检测调试器是否可用
	static bool isAvailable();

	/// 向调试器输出消息
	static void message(const std::string& msg);
	static void message(const std::string& msg, const char* file, int line);

	/// 暂停程序执行，打开调试器，进入调试模式。
	static void enter();
	static void enter(const std::string& msg);
	static void enter(const std::string& msg, const char* file, int line);
	static void enter(const char* file, int line);
};
