
#include "Bugcheck.h"

//#define double_conversion _double_conversion
#define UNIMPLEMENTED BUGCHECK

#include "v8/diy-fp.cc"
#include "v8/cached-powers.cc"
#include "v8/bignum-dtoa.cc"
#include "v8/bignum.cc"
#include "v8/fast-dtoa.cc"
#include "v8/fixed-dtoa.cc"
#include "v8/strtod.cc"
#include "v8/double-conversion.cc"
// --- double conversion ---

#include "NumericString.h"
STATIC_ASSERT(MAX_FLT_STRING_LEN == double_conversion::kMaxSignificantDecimalDigits);
#include "TString.h"
#include <memory>
#include <cctype>

namespace {


	void pad(std::string& str, int precision, int width, char prefix = ' ', char decSep = '.')
	{
		ASSERT_DBG(precision > 0);
		ASSERT_DBG(str.length());

		std::string::size_type decSepPos = str.find(decSep);
		if (decSepPos == std::string::npos)
		{
			str.append(1, decSep);
			decSepPos = str.size() - 1;
		}

		std::string::size_type frac = str.length() - decSepPos - 1;

		std::string::size_type ePos = str.find_first_of("eE");
		std::unique_ptr<std::string> eStr;
		if (ePos != std::string::npos)
		{
			eStr.reset(new std::string(str.substr(ePos, std::string::npos)));
			frac -= eStr->length();
			str = str.substr(0, str.length() - eStr->length());
		}

		if (frac != precision)
		{
			if (frac < precision)
			{
				str.append(precision - frac, '0');
			}
			else if ((frac > precision) && (decSepPos != std::string::npos))
			{
				int pos = static_cast<int>(decSepPos) + 1 + precision;
				if (str[pos] >= '5')
				{
					char carry = 0;
					if (str[--pos] == '9')
					{
						str[pos] = '0';
						carry = 1;
					}
					else
					{
						++str[pos];
						carry = 0;
					}
					while (--pos >= 0)
					{
						if (str[pos] == decSep) continue;
						if (carry)
						{
							if ((str[pos] + carry) <= '9')
							{
								++str[pos];
								carry = 0;
							}
							else
							{
								str[pos] = '0';
								carry = 1;
							}
						}
					}
					if (carry) str.insert(str.begin(), 1, '1');
				}
				str = str.substr(0, decSepPos + 1 + precision);
			}
		}

		if (eStr.get()) str += *eStr;

		if (width && (str.length() < width)) str.insert(str.begin(), width - str.length(), prefix);
	}


	void insertThousandSep(std::string& str, char thSep, char decSep = '.')
	{
		ASSERT(decSep != thSep);
		if (str.size() == 0) return;

		std::string::size_type exPos = str.find('e');
		if (exPos == std::string::npos) exPos = str.find('E');
		std::string::size_type decPos = str.find(decSep);
		std::string::iterator it = str.end();
		if (exPos != std::string::npos) it -= str.size() - exPos;

		if (decPos != std::string::npos)
		{
			while (it != str.begin())
			{
				--it;
				if (*it == decSep) break;
			}
		}
		int thCount = 0;
		if (it == str.end()) --it;
		for (; it != str.begin();)
		{
			std::string::iterator pos = it;
			std::string::value_type chr = *it;
			std::string::value_type prevChr = *--it;

			if (!std::isdigit(chr)) continue;

			if (++thCount == 3 && std::isdigit(prevChr))
				it = str.insert(pos, thSep);

			if (thCount == 3) thCount = 0;
		}
	}


} // namespace



void floatToStr(char* buffer, int bufferSize, float value, int lowDec, int highDec)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO |
		DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, FLT_INF, FLT_NAN, FLT_EXP, lowDec, highDec, 0, 0);
	dc.ToShortestSingle(value, &builder);
	builder.Finalize();
}


void floatToFixedStr(char* buffer, int bufferSize, float value, int precision)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO |
		DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, FLT_INF, FLT_NAN, FLT_EXP, -std::numeric_limits<float>::digits10, std::numeric_limits<float>::digits10, 0, 0);
	dc.ToFixed(value, precision, &builder);
	builder.Finalize();
}


std::string& floatToStr(std::string& str, float value, int precision, int width, char thSep, char decSep)
{
	if (!decSep) decSep = '.';
	if (precision == 0) value = std::floor(value);

	char buffer[MAX_FLT_STRING_LEN];
	floatToStr(buffer, MAX_FLT_STRING_LEN, value);
	str = buffer;

	if (decSep && (decSep != '.') && (str.find('.') != std::string::npos))
		replaceInPlace(str, '.', decSep);

	if (thSep) insertThousandSep(str, thSep, decSep);
	if (precision > 0 || width) pad(str, precision, width, ' ', decSep ? decSep : '.');
	return str;
}


std::string& floatToFixedStr(std::string& str, float value, int precision, int width, char thSep, char decSep)
{
	if (!decSep) decSep = '.';
	if (precision == 0) value = std::floor(value);

	char buffer[MAX_FLT_STRING_LEN];
	floatToFixedStr(buffer, MAX_FLT_STRING_LEN, value, precision);
	str = buffer;

	if (decSep && (decSep != '.') && (str.find('.') != std::string::npos))
		replaceInPlace(str, '.', decSep);

	if (thSep) insertThousandSep(str, thSep, decSep);
	if (precision > 0 || width) pad(str, precision, width, ' ', decSep ? decSep : '.');
	return str;
}


void doubleToStr(char* buffer, int bufferSize, double value, int lowDec, int highDec)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO |
		DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, FLT_INF, FLT_NAN, FLT_EXP, lowDec, highDec, 0, 0);
	dc.ToShortest(value, &builder);
	builder.Finalize();
}


void doubleToFixedStr(char* buffer, int bufferSize, double value, int precision)
{
	using namespace double_conversion;

	StringBuilder builder(buffer, bufferSize);
	int flags = DoubleToStringConverter::UNIQUE_ZERO |
		DoubleToStringConverter::EMIT_POSITIVE_EXPONENT_SIGN;
	DoubleToStringConverter dc(flags, FLT_INF, FLT_NAN, FLT_EXP,
		-std::numeric_limits<double>::digits10, std::numeric_limits<double>::digits10, 0, 0);
	dc.ToFixed(value, precision, &builder);
	builder.Finalize();
}


std::string& doubleToStr(std::string& str, double value, int precision, int width, char thSep, char decSep)
{
	if (!decSep) decSep = '.';
	if (precision == 0) value = std::floor(value);

	char buffer[MAX_FLT_STRING_LEN];
	doubleToStr(buffer, MAX_FLT_STRING_LEN, value);

	str = buffer;

	if (decSep && (decSep != '.') && (str.find('.') != std::string::npos))
		replaceInPlace(str, '.', decSep);

	if (thSep) insertThousandSep(str, thSep, decSep);
	if (precision > 0 || width) pad(str, precision, width, ' ', decSep ? decSep : '.');
	return str;
}


std::string& doubleToFixedStr(std::string& str, double value, int precision, int width, char thSep, char decSep)
{
	if (!decSep) decSep = '.';
	if (precision == 0) value = std::floor(value);

	char buffer[MAX_FLT_STRING_LEN];
	doubleToFixedStr(buffer, MAX_FLT_STRING_LEN, value, precision);

	str = buffer;

	if (decSep && (decSep != '.') && (str.find('.') != std::string::npos))
		replaceInPlace(str, '.', decSep);

	if (thSep) insertThousandSep(str, thSep, decSep);
	if (precision > 0 || width) pad(str, precision, width, ' ', decSep ? decSep : '.');
	return str;
}


float strToFloat(const char* str, const char* inf, const char* nan)
{
	using namespace double_conversion;

	int processed;
	int flags = StringToDoubleConverter::ALLOW_LEADING_SPACES |
		StringToDoubleConverter::ALLOW_TRAILING_SPACES;
	StringToDoubleConverter converter(flags, 0.0, Single::NaN(), inf, nan);
	float result = converter.StringToFloat(str, static_cast<int>(strlen(str)), &processed);
	return result;
}


double strToDouble(const char* str, const char* inf, const char* nan)
{
	using namespace double_conversion;
	int processed;
	int flags = StringToDoubleConverter::ALLOW_LEADING_SPACES |
		StringToDoubleConverter::ALLOW_TRAILING_SPACES;
	StringToDoubleConverter converter(flags, 0.0, Double::NaN(), inf, nan);
	double result = converter.StringToDouble(str, static_cast<int>(strlen(str)), &processed);
	return result;
}


bool strToFloat(const std::string& str, float& result, char decSep, char thSep, const char* inf, const char* nan)
{
	using namespace double_conversion;

	std::string tmp(str);
	trimInPlace(tmp);
	removeInPlace(tmp, thSep);
	removeInPlace(tmp, 'f');
	replaceInPlace(tmp, decSep, '.');
	result = strToFloat(tmp.c_str(), inf, nan);
	return !FPEnvironment::isInfinite(result) &&
		!FPEnvironment::isNaN(result);
}


bool strToDouble(const std::string& str, double& result, char decSep, char thSep, const char* inf, const char* nan)
{
	if (str.empty()) return false;

	using namespace double_conversion;

	std::string tmp(str);
	trimInPlace(tmp);
	removeInPlace(tmp, thSep);
	replaceInPlace(tmp, decSep, '.');
	removeInPlace(tmp, 'f');
	result = strToDouble(tmp.c_str(), inf, nan);
	return !FPEnvironment::isInfinite(result) &&
		!FPEnvironment::isNaN(result);
}
