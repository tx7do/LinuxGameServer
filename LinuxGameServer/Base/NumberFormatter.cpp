/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		NumberFormatter.cpp
* 摘    要:		数字的字符串格式化工具
*
* 作    者:		yanglinbo,
* 修    改:		查看文件最下方.
*
***********************************************************************/

#include "NumberFormatter.h"
#include "Bugcheck.h"
#include <locale>
#include <iomanip>
#include <cstdio>
#include <cinttypes>


#if defined(_MSC_VER)
#	define I64_FMT "I64"
#elif defined(__APPLE__) 
#	define I64_FMT "q"
#else
#	define I64_FMT "ll"
#endif


std::string NumberFormatter::format(bool value, BoolFormat format)
{
	switch (format)
	{
	case FMT_YES_NO:
		return value ? "yes" : "no";

	case FMT_ON_OFF:
		return value ? "on" : "off";

	default: // including FMT_TRUE_FALSE:
		return value ? "true" : "false";
	}
}

void NumberFormatter::append(std::string& str, Int32 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, Int32 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width);
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, Int32 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, Int32 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<unsigned int>(value), 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, Int32 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<unsigned int>(value), 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, UInt32 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, UInt32 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width);
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, UInt32 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, UInt32 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, UInt32 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, Int64 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, Int64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, Int64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, Int64 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<UInt64>(value), 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, Int64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<UInt64>(value), 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, UInt64 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, UInt64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, UInt64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, UInt64 value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, UInt64 value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}

void NumberFormatter::append(std::string& str, float value)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	floatToStr(buffer, NF_MAX_FLT_STRING_LEN, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, float value, int precision)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	floatToFixedStr(buffer, NF_MAX_FLT_STRING_LEN, value, precision);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, float value, int width, int precision)
{
	std::string result;
	str.append(floatToFixedStr(result, value, precision, width));
}


void NumberFormatter::append(std::string& str, double value)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	doubleToStr(buffer, NF_MAX_FLT_STRING_LEN, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, double value, int precision)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	doubleToFixedStr(buffer, NF_MAX_FLT_STRING_LEN, value, precision);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, double value, int width, int precision)
{
	std::string result;
	str.append(doubleToFixedStr(result, value, precision, width));
}


void NumberFormatter::append(std::string& str, const void* ptr)
{
	char buffer[24];
	std::sprintf(buffer, "%08" PRIXPTR, (UIntPtr)ptr);
	str.append(buffer);
}
