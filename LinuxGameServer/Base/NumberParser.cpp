/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		NumberParser.cpp
* 摘    要:		数字解析器，用于将字符串解析为数字
*
* 作    者:		yanglinbo,
* 修    改:		查看文件最下方.
*
***********************************************************************/

#include "NumberParser.h"
#include "BaseException.h"
#include <cstdio>
#include <cctype>


#if defined(_MSC_VER)
#	define I64_FMT "I64"
#elif defined(__APPLE__) 
#	define I64_FMT "q"
#else
#	define I64_FMT "ll"
#endif


int NumberParser::parse(const std::string& s)
{
	int result = 0;
	if (tryParse(s, result))
	{
		return result;
	}
	else
	{
		throw SyntaxException("Not a valid integer", s);
	}
}

bool NumberParser::tryParse(const std::string& s, int& value)
{
	//char temp = 0;
	return std::sscanf(s.c_str(), "%d", &value) == 1;
}

unsigned NumberParser::parseUnsigned(const std::string& s)
{
	unsigned result = 0;
	if (tryParseUnsigned(s, result))
	{
		return result;
	}
	else
	{
		throw SyntaxException("Not a valid unsigned integer", s);
	}
}

bool NumberParser::tryParseUnsigned(const std::string& s, unsigned& value)
{
	//char temp = 0;
	return std::sscanf(s.c_str(), "%u", &value) == 1;
}

unsigned NumberParser::parseHex(const std::string& s)
{
	unsigned result = 0;
	if (tryParseHex(s, result))
	{
		return result;
	}
	else
	{
		throw SyntaxException("Not a valid hexadecimal integer", s);
	}
}

bool NumberParser::tryParseHex(const std::string& s, unsigned& value)
{
	//char temp = 0;
	return std::sscanf(s.c_str(), "%x", &value) == 1;
}

long NumberParser::parseLong(const std::string& s)
{
	long result = 0;
	if (tryParseLong(s, result))
	{
		return result;
	}
	else
	{
		throw SyntaxException("Not a valid long integer", s);
	}
}

bool NumberParser::tryParseLong(const std::string& s, long& value)
{
	//char temp = 0;
	return std::sscanf(s.c_str(), "%ld", &value) == 1;
}

unsigned long NumberParser::parseUlong(const std::string& s)
{
	unsigned long result = 0;
	if (tryParseUlong(s, result))
	{
		return result;
	}
	else
	{
		throw SyntaxException("Not a valid unsigned long integer", s);
	}
}

bool NumberParser::tryParseUlong(const std::string& s, unsigned long& value)
{
	//char temp = 0;
	return std::sscanf(s.c_str(), "%uld", &value) == 1;
}

long long NumberParser::parseLongLong(const std::string& s)
{
	long long result = 0;
	if (tryParseLongLong(s, result))
	{
		return result;
	}
	else
	{
		throw SyntaxException("Not a valid long long integer", s);
	}
}

bool NumberParser::tryParseLongLong(const std::string& s, long long& value)
{
	//char temp = 0;
	return std::sscanf(s.c_str(), "%I64d", &value) == 1;
}

Int64 NumberParser::parse64(const std::string& s)
{
	Int64 result = 0;
	if (tryParse64(s, result))
	{
		return result;
	}
	else
	{
		throw SyntaxException("Not a valid integer", s);
	}
}

bool NumberParser::tryParse64(const std::string& s, Int64& value)
{
	//char temp = 0;
	return std::sscanf(s.c_str(), "%I64d", &value) == 1;
}

UInt64 NumberParser::parseUnsigned64(const std::string& s)
{
	UInt64 result = 0;
	if (tryParseUnsigned64(s, result))
	{
		return result;
	}
	else
	{
		throw SyntaxException("Not a valid unsigned integer", s);
	}
}

bool NumberParser::tryParseUnsigned64(const std::string& s, UInt64& value)
{
	//char temp = 0;
	return std::sscanf(s.c_str(), "%I64u", &value) == 1;
}

UInt64 NumberParser::parseHex64(const std::string& s)
{
	UInt64 result = 0;
	if (tryParseHex64(s, result))
	{
		return result;
	}
	else
	{
		throw SyntaxException("Not a valid hexadecimal integer", s);
	}
}

bool NumberParser::tryParseHex64(const std::string& s, UInt64& value)
{
	//char temp = 0;
	return std::sscanf(s.c_str(), "%I64x", &value) == 1;
}

float NumberParser::parseFloat(const std::string& s)
{
	float result = 0;
	if (tryParseFloat(s, result))
	{
		return result;
	}
	else
	{
		throw SyntaxException("Not a valid floating-point number", s);
	}
}

bool NumberParser::tryParseFloat(const std::string& s, float& value)
{
	return std::sscanf(s.c_str(), "%f", &value) == 1;
}

double NumberParser::parseDouble(const std::string& s)
{
	double result = 0.0f;
	if (tryParseDouble(s, result))
	{
		return result;
	}
	else
	{
		throw SyntaxException("Not a valid floating-point number", s);
	}
}

bool NumberParser::tryParseDouble(const std::string& s, double& value)
{
	return std::sscanf(s.c_str(), "%lf", &value) == 1;
}
