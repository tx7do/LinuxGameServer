/**********************************************************************
* Copyright (C) 2008 - ʢ������ - All Rights Reserved
*
* �ļ�����:		NumberParser.h
* ժ    Ҫ:		���ֽ����������ڽ��ַ�������Ϊ����
*     
* ��    ��:		yanglinbo, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#pragma once

#include "Types.h"
#include <string>

class NumberParser
{
public:
	/// ���ַ�������Ϊ��������
	/// @param[in] s ��Ҫ���������ַ���
	/// @return ��������
	static int parse(const std::string& s);

	/// ���ַ�������Ϊ��������
	/// @param[in] s ��Ҫ���������ַ���
	/// @param[out] value ������������������
	/// @return �����ɹ����
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
