/**********************************************************************
* Copyright (C) 2008 - ʢ������ - All Rights Reserved
*
* �ļ�����:		NumberFormatter.h
* ժ    Ҫ:		���ֵ��ַ�����ʽ������
*
* ��    ��:		yanglinbo,
* ��    ��:		�鿴�ļ����·�.
*
***********************************************************************/

#pragma once


#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <limits>
#include <cmath>
#include <cctype>
#include <locale>

#include <string>
#include <vector>

#include "Types.h"
#include "NumericString.h"


class NumberFormatter
{
public:
	enum BoolFormat
	{
		FMT_TRUE_FALSE,
		FMT_YES_NO,
		FMT_ON_OFF
	};

	static const unsigned NF_MAX_INT_STRING_LEN = 32;
	static const unsigned NF_MAX_FLT_STRING_LEN = 780;

public:
	static std::string format(Int32 value);

	static std::string format(Int32 value, int width);

	static std::string format0(Int32 value, int width);

	static std::string formatHex(Int32 value, bool prefix = false);

	static std::string formatHex(Int32 value, int width, bool prefix = false);

	static std::string format(UInt32 value);

	static std::string format(UInt32 value, int width);

	static std::string format0(UInt32 value, int width);

	static std::string formatHex(UInt32 value, bool prefix = false);

	static std::string formatHex(UInt32 value, int width, bool prefix = false);

	static std::string format(Int64 value);

	static std::string format(Int64 value, int width);

	static std::string format0(Int64 value, int width);

	static std::string formatHex(Int64 value, bool prefix = false);

	static std::string formatHex(Int64 value, int width, bool prefix = false);

	static std::string format(UInt64 value);

	static std::string format(UInt64 value, int width);

	static std::string format0(UInt64 value, int width);

	static std::string formatHex(UInt64 value, bool prefix = false);

	static std::string formatHex(UInt64 value, int width, bool prefix = false);

	static std::string format(float value);

	static std::string format(float value, int precision);

	static std::string format(float value, int width, int precision);

	static std::string format(double value);

	static std::string format(double value, int precision);

	static std::string format(double value, int width, int precision);

	static std::string format(const void* ptr);

	static std::string format(bool value, BoolFormat format = FMT_TRUE_FALSE);

public:
	static void append(std::string& str, Int32 value);

	static void append(std::string& str, Int32 value, int width);

	static void append0(std::string& str, Int32 value, int width);

	static void appendHex(std::string& str, Int32 value);

	static void appendHex(std::string& str, Int32 value, int width);

	static void append(std::string& str, UInt32 value);

	static void append(std::string& str, UInt32 value, int width);

	static void append0(std::string& str, UInt32 value, int width);

	static void appendHex(std::string& str, UInt32 value);

	static void appendHex(std::string& str, UInt32 value, int width);

	static void append(std::string& str, Int64 value);

	static void append(std::string& str, Int64 value, int width);

	static void append0(std::string& str, Int64 value, int width);

	static void appendHex(std::string& str, Int64 value);

	static void appendHex(std::string& str, Int64 value, int width);

	static void append(std::string& str, UInt64 value);

	static void append(std::string& str, UInt64 value, int width);

	static void append0(std::string& str, UInt64 value, int width);

	static void appendHex(std::string& str, UInt64 value);

	static void appendHex(std::string& str, UInt64 value, int width);

	static void append(std::string& str, float value);

	static void append(std::string& str, float value, int precision);

	static void append(std::string& str, float value, int width, int precision);

	static void append(std::string& str, double value);

	static void append(std::string& str, double value, int precision);

	static void append(std::string& str, double value, int width, int precision);

	static void append(std::string& str, const void* ptr);

public:
	/// ����ֵת��Ϊ�ַ���,�������ַ����в����λ��
	template <class T>
	static std::string format_separator(const T& nValue, int nSplitNum = 4);
};


//////////////////////////////////////////////////////////////////////////


template <class T>
std::string NumberFormatter::format_separator(const T& nValue, int nSplitNum)
{
	// ת������
	std::string ss;
	ss = NumberFormatter::format(nValue);
	if (ss.empty()) return ss;

	// ����С�ڷֶ���
	if ((ss.size() <= nSplitNum) && (nSplitNum != 0)) return ss;

	// ���ֵĶ���
	size_t nStrLen = ss.size();

	// ���ֵĶ���
	size_t nCount = nStrLen / nSplitNum;
	nCount += (nStrLen % nSplitNum) ? 1 : 0;

	// ����ַ���
	size_t nLeftChar = 0;
	int nIndex = (int)(nStrLen - nSplitNum);
	std::vector<std::string> listStr;
	for (int i = 0; i < nCount; ++i)
	{
		if (nIndex < 0)
		{
			nSplitNum += nIndex;
			nIndex = 0;
		}
		listStr.push_back(ss.substr(nIndex, nSplitNum));

		nLeftChar = nIndex - nSplitNum;
		nIndex -= nSplitNum;
	}

	// ƴ���ַ���
	std::string strOut;
	for (int i = (int)listStr.size() - 1; i >= 0; --i)
	{
		strOut += listStr[i];
		if (i == 1 && listStr[i].size() == 1 && listStr[i] == "-") continue;
		if (i != 0) strOut += ",";
	}

	return strOut;
};

inline std::string NumberFormatter::format(Int32 value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(Int32 value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(Int32 value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(Int32 value, bool prefix)
{
	std::string result;
	uIntToStr(static_cast<unsigned int>(value), 0x10, result, prefix);
	return result;
}


inline std::string NumberFormatter::formatHex(Int32 value, int width, bool prefix)
{
	std::string result;
	uIntToStr(static_cast<unsigned int>(value), 0x10, result, prefix, width, '0');
	return result;
}


inline std::string NumberFormatter::format(UInt32 value)
{
	std::string result;
	uIntToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(UInt32 value, int width)
{
	std::string result;
	uIntToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(UInt32 value, int width)
{
	std::string result;
	uIntToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(UInt32 value, bool prefix)
{
	std::string result;
	uIntToStr(value, 0x10, result, prefix);
	return result;
}


inline std::string NumberFormatter::formatHex(UInt32 value, int width, bool prefix)
{
	std::string result;
	uIntToStr(value, 0x10, result, prefix, width, '0');
	return result;
}


inline std::string NumberFormatter::format(Int64 value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(Int64 value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(Int64 value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(Int64 value, bool prefix)
{
	std::string result;
	uIntToStr(static_cast<UInt64>(value), 0x10, result, prefix);
	return result;
}


inline std::string NumberFormatter::formatHex(Int64 value, int width, bool prefix)
{
	std::string result;
	uIntToStr(static_cast<UInt64>(value), 0x10, result, prefix, width, '0');
	return result;
}


inline std::string NumberFormatter::format(UInt64 value)
{
	std::string result;
	uIntToStr(value, 10, result);
	return result;
}


inline std::string NumberFormatter::format(UInt64 value, int width)
{
	std::string result;
	uIntToStr(value, 10, result, false, width, ' ');
	return result;
}


inline std::string NumberFormatter::format0(UInt64 value, int width)
{
	std::string result;
	uIntToStr(value, 10, result, false, width, '0');
	return result;
}


inline std::string NumberFormatter::formatHex(UInt64 value, bool prefix)
{
	std::string result;
	uIntToStr(value, 0x10, result, prefix);
	return result;
}


inline std::string NumberFormatter::formatHex(UInt64 value, int width, bool prefix)
{
	std::string result;
	uIntToStr(value, 0x10, result, prefix, width, '0');
	return result;
}

inline std::string NumberFormatter::format(float value)
{
	char buffer[MAX_FLT_STRING_LEN];
	floatToStr(buffer, MAX_FLT_STRING_LEN, value);
	return std::string(buffer);
}


inline std::string NumberFormatter::format(float value, int precision)
{
	char buffer[MAX_FLT_STRING_LEN];
	floatToFixedStr(buffer, MAX_FLT_STRING_LEN, value, precision);
	return std::string(buffer);
}


inline std::string NumberFormatter::format(float value, int width, int precision)
{
	std::string result;
	floatToFixedStr(result, value, precision, width);
	return result;
}


inline std::string NumberFormatter::format(double value)
{
	char buffer[MAX_FLT_STRING_LEN];
	doubleToStr(buffer, MAX_FLT_STRING_LEN, value);
	return std::string(buffer);
}


inline std::string NumberFormatter::format(double value, int precision)
{
	char buffer[MAX_FLT_STRING_LEN];
	doubleToFixedStr(buffer, MAX_FLT_STRING_LEN, value, precision);
	return std::string(buffer);
}


inline std::string NumberFormatter::format(double value, int width, int precision)
{
	std::string result;
	doubleToFixedStr(result, value, precision, width);
	return result;
}


inline std::string NumberFormatter::format(const void* ptr)
{
	std::string result;
	append(result, ptr);
	return result;
}
