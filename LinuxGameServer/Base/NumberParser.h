/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		NumberParser.h
* 摘    要:		数字解析器，用于将字符串解析为数字
*     
* 作    者:		yanglinbo, 
* 修    改:		查看文件最下方.
* 
***********************************************************************/

#pragma once

#include "Types.h"
#include <string>

class NumberParser
{
public:
	/// 将字符串解析为整型数字
	/// @param[in] s 需要被解析的字符串
	/// @return 整型数字
	static int parse(const std::string& s);

	/// 将字符串解析为整型数字
	/// @param[in] s 需要被解析的字符串
	/// @param[out] value 解析出来的整型数字
	/// @return 解析成功与否
	static bool tryParse(const std::string& s, int& value);

public:
	static unsigned parseUnsigned(const std::string& s);

	static bool tryParseUnsigned(const std::string& s, unsigned& value);

	static unsigned parseHex(const std::string& s);

	static bool tryParseHex(const std::string& s, unsigned& value);

public:
	static long parseLong(const std::string& s);

	static bool tryParseLong(const std::string& s, long& value);

public:
	static unsigned long parseUlong(const std::string& s);

	static bool tryParseUlong(const std::string& s, unsigned long& value);

public:
	static	long long parseLongLong(const std::string& str);

	static bool tryParseLongLong(const std::string& str, long long& value);

public:

public:
	static Int64 parse64(const std::string& s);

	static bool tryParse64(const std::string& s, Int64& value);

public:
	static UInt64 parseUnsigned64(const std::string& s);

	static bool tryParseUnsigned64(const std::string& s, UInt64& value);

	static UInt64 parseHex64(const std::string& s);

	static bool tryParseHex64(const std::string& s, UInt64& value);

public:
	static float parseFloat(const std::string& s);

	static bool tryParseFloat(const std::string& s, float& value);

public:
	static	double parseDouble(const std::string& s);

	static bool tryParseDouble(const std::string& s, double& value);
};
