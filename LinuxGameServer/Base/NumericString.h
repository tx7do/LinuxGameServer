#pragma once

#include "Buffer.h"
#include "BaseException.h"
#include "Bugcheck.h"
#include "FPEnvironment.h"

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


//#define INTMAX_MAX std::numeric_limits<intmax_t>::max()

#define MAX_INT_STRING_LEN 65
#define MAX_FLT_STRING_LEN 780

#define FLT_INF "inf"
#define FLT_NAN "nan"
#define FLT_EXP 'e'


namespace Impl
{

	template<bool SIGNED, typename T>
	class IsNegativeImpl;

	template<typename T>
	class IsNegativeImpl<true, T>
	{
	public:
		bool operator()(T x) { return x < 0; }
	};

	template<typename T>
	class IsNegativeImpl<false, T>
	{
	public:
		bool operator()(T) { return false; }
	};

}


template<typename T>
inline bool isNegative(T x)
{
	using namespace Impl;
	return IsNegativeImpl<std::numeric_limits<T>::is_signed, T>()(x);
}


template<typename To, typename From>
inline bool isIntOverflow(From val)
{
	ASSERT_DBG(std::numeric_limits<From>::is_integer);
	ASSERT_DBG(std::numeric_limits<To>::is_integer);
	bool ret;
	if (std::numeric_limits<To>::is_signed)
	{
		ret = (!std::numeric_limits<From>::is_signed &&
			(uintmax_t)val > (uintmax_t)INTMAX_MAX) ||
			(intmax_t)val  < (intmax_t)std::numeric_limits<To>::min() ||
			(intmax_t)val  >(intmax_t)std::numeric_limits<To>::max();
	}
	else
	{
		ret = isNegative(val) ||
			(uintmax_t)val > (uintmax_t)std::numeric_limits<To>::max();
	}
	return ret;
}


template <typename F, typename T>
inline T& isSafeIntCast(F from)
{
	if (!isIntOverflow<T, F>(from)) return true;
	return false;
}


template <typename F, typename T>
inline T& safeIntCast(F from, T& to)
{
	if (!isIntOverflow<T, F>(from))
	{
		to = static_cast<T>(from);
		return to;
	}
	throw BadCastException("safeIntCast: Integer overflow");
}

inline char decimalSeparator()
{
	return std::use_facet<std::numpunct<char>>(std::locale()).decimal_point();
}


inline char thousandSeparator()
{
	return std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep();
}


template <typename I>
bool strToInt(const char* pStr, I& outResult, short base, char thSep = ',')
{
	ASSERT_DBG(base == 2 || base == 8 || base == 10 || base == 16);

	if (!pStr) return false;
	while (std::isspace(*pStr)) ++pStr;
	if (*pStr == '\0') return false;
	bool negative = false;
	if ((base == 10) && (*pStr == '-'))
	{
		if (!std::numeric_limits<I>::is_signed) return false;
		negative = true;
		++pStr;
	}
	else if (*pStr == '+') ++pStr;

	uintmax_t limitCheck = std::numeric_limits<I>::max();
	if (negative)
	{
		ASSERT_DBG(std::numeric_limits<I>::is_signed);
		if (sizeof(I) == sizeof(intmax_t))
			limitCheck = static_cast<uintmax_t>(FPEnvironment::copySign(static_cast<double>(std::numeric_limits<I>::min()), 1));
		else
		{
			intmax_t i = std::numeric_limits<I>::min();
			limitCheck = -i;
		}
	}

	uintmax_t result = 0;
	for (; *pStr != '\0'; ++pStr)
	{
		if (result > (limitCheck / base)) return false;
		switch (*pStr)
		{
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		{
			char add = (*pStr - '0');
			if ((limitCheck - result) < add) return false;
			result = result * base + add;
		}
		break;

		case '8': case '9':
			if ((base == 10) || (base == 0x10))
			{
				char  add = (*pStr - '0');
				if ((limitCheck - result) < add) return false;
				result = result * base + add;
			}
			else return false;

			break;

		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		{
			if (base != 0x10) return false;
			char  add = (*pStr - 'a');
			if ((limitCheck - result) < add) return false;
			result = result * base + (10 + add);
		}
		break;

		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		{
			if (base != 0x10) return false;
			char add = (*pStr - 'A');
			if ((limitCheck - result) < add) return false;
			result = result * base + (10 + add);
		}
		break;

		case '.':
			if ((base == 10) && (thSep == '.')) break;
			else return false;

		case ',':
			if ((base == 10) && (thSep == ',')) break;
			else return false;

		case ' ':
			if ((base == 10) && (thSep == ' ')) break;

		default:
			return false;
		}
	}

	if (negative && (base == 10))
	{
		ASSERT_DBG(std::numeric_limits<I>::is_signed);
		intmax_t i;
		if (sizeof(I) == sizeof(intmax_t))
			i = static_cast<intmax_t>(FPEnvironment::copySign(static_cast<double>(result), -1));
		else
			i = static_cast<intmax_t>(-result);
		if (isIntOverflow<I>(i)) return false;
		outResult = static_cast<I>(i);
	}
	else
	{
		if (isIntOverflow<I>(result)) return false;
		outResult = static_cast<I>(result);
	}

	return true;
}


template <typename I>
bool strToInt(const std::string& str, I& result, short base, char thSep = ',')
{
	return strToInt(str.c_str(), result, base, thSep);
}


namespace Impl {

	class Ptr
	{
	public:
		Ptr(char* ptr, std::size_t offset) : _beg(ptr), _cur(ptr), _end(ptr + offset)
		{
		}

		char*& operator ++ () // prefix
		{
			checkBounds(_cur + 1);
			return ++_cur;
		}

		char* operator ++ (int) // postfix
		{
			checkBounds(_cur + 1);
			char* tmp = _cur++;
			return tmp;
		}

		char*& operator -- () // prefix
		{
			checkBounds(_cur - 1);
			return --_cur;
		}

		char* operator -- (int) // postfix
		{
			checkBounds(_cur - 1);
			char* tmp = _cur--;
			return tmp;
		}

		char*& operator += (int incr)
		{
			checkBounds(_cur + incr);
			return _cur += incr;
		}

		char*& operator -= (int decr)
		{
			checkBounds(_cur - decr);
			return _cur -= decr;
		}

		operator char* () const
		{
			return _cur;
		}

		std::size_t span() const
		{
			return _end - _beg;
		}

	private:
		void checkBounds(char* ptr)
		{
			if (ptr > _end) throw RangeException();
		}

		const char* _beg;
		char* _cur;
		const char* _end;
	};

} // namespace Impl


template <typename T>
bool intToStr(T value,
	unsigned short base,
	char* result,
	std::size_t& size,
	bool prefix = false,
	int width = -1,
	char fill = ' ',
	char thSep = 0)
{
	if (base < 2 || base > 0x10)
	{
		*result = '\0';
		return false;
	}

	Impl::Ptr ptr(result, size);
	int thCount = 0;
	T tmpVal;
	do
	{
		tmpVal = value;
		value /= base;
		*ptr++ = "FEDCBA9876543210123456789ABCDEF"[15 + (tmpVal - value * base)];
		if (thSep && (base == 10) && (++thCount == 3))
		{
			*ptr++ = thSep;
			thCount = 0;
		}
	} while (value);

	if ('0' == fill)
	{
		if (tmpVal < 0) --width;
		if (prefix && base == 010) --width;
		if (prefix && base == 0x10) width -= 2;
		while ((ptr - result) < width) *ptr++ = fill;
	}

	if (prefix && base == 010) *ptr++ = '0';
	else if (prefix && base == 0x10)
	{
		*ptr++ = 'x';
		*ptr++ = '0';
	}

	if (tmpVal < 0) *ptr++ = '-';

	if ('0' != fill)
	{
		while ((ptr - result) < width) *ptr++ = fill;
	}

	size = ptr - result;
	ASSERT_DBG(size <= ptr.span());
	ASSERT_DBG((-1 == width) || (size >= size_t(width)));
	*ptr-- = '\0';

	char* ptrr = result;
	char tmp;
	while (ptrr < ptr)
	{
		tmp = *ptr;
		*ptr-- = *ptrr;
		*ptrr++ = tmp;
	}

	return true;
}


template <typename T>
bool uIntToStr(T value,
	unsigned short base,
	char* result,
	std::size_t& size,
	bool prefix = false,
	int width = -1,
	char fill = ' ',
	char thSep = 0)
{
	if (base < 2 || base > 0x10)
	{
		*result = '\0';
		return false;
	}

	Impl::Ptr ptr(result, size);
	int thCount = 0;
	T tmpVal;
	do
	{
		tmpVal = value;
		value /= base;
		*ptr++ = "FEDCBA9876543210123456789ABCDEF"[15 + (tmpVal - value * base)];
		if (thSep && (base == 10) && (++thCount == 3))
		{
			*ptr++ = thSep;
			thCount = 0;
		}
	} while (value);

	if ('0' == fill)
	{
		if (prefix && base == 010) --width;
		if (prefix && base == 0x10) width -= 2;
		while ((ptr - result) < width) *ptr++ = fill;
	}

	if (prefix && base == 010) *ptr++ = '0';
	else if (prefix && base == 0x10)
	{
		*ptr++ = 'x';
		*ptr++ = '0';
	}

	if ('0' != fill)
	{
		while ((ptr - result) < width) *ptr++ = fill;
	}

	size = ptr - result;
	ASSERT_DBG(size <= ptr.span());
	ASSERT_DBG((-1 == width) || (size >= size_t(width)));
	*ptr-- = '\0';

	char* ptrr = result;
	char tmp;
	while (ptrr < ptr)
	{
		tmp = *ptr;
		*ptr-- = *ptrr;
		*ptrr++ = tmp;
	}

	return true;
}


template <typename T>
bool intToStr(T number, unsigned short base, std::string& result, bool prefix = false, int width = -1, char fill = ' ', char thSep = 0)
{
	char res[MAX_INT_STRING_LEN] = { 0 };
	std::size_t size = MAX_INT_STRING_LEN;
	bool ret = intToStr(number, base, res, size, prefix, width, fill, thSep);
	result.assign(res, size);
	return ret;
}


template <typename T>
bool uIntToStr(T number, unsigned short base, std::string& result, bool prefix = false, int width = -1, char fill = ' ', char thSep = 0)
{
	char res[MAX_INT_STRING_LEN] = { 0 };
	std::size_t size = MAX_INT_STRING_LEN;
	bool ret = uIntToStr(number, base, res, size, prefix, width, fill, thSep);
	result.assign(res, size);
	return ret;
}


void floatToStr(char* buffer,
	int bufferSize,
	float value,
	int lowDec = -std::numeric_limits<float>::digits10,
	int highDec = std::numeric_limits<float>::digits10);

void floatToFixedStr(char* buffer,
	int bufferSize,
	float value,
	int precision);


std::string& floatToStr(std::string& str,
	float value,
	int precision = -1,
	int width = 0,
	char thSep = 0,
	char decSep = 0);


std::string& floatToFixedStr(std::string& str,
	float value,
	int precision,
	int width = 0,
	char thSep = 0,
	char decSep = 0);


void doubleToStr(char* buffer,
	int bufferSize,
	double value,
	int lowDec = -std::numeric_limits<double>::digits10,
	int highDec = std::numeric_limits<double>::digits10);


void doubleToFixedStr(char* buffer,
	int bufferSize,
	double value,
	int precision);


std::string& doubleToStr(std::string& str,
	double value,
	int precision = -1,
	int width = 0,
	char thSep = 0,
	char decSep = 0);


std::string& doubleToFixedStr(std::string& str,
	double value,
	int precision = -1,
	int width = 0,
	char thSep = 0,
	char decSep = 0);


float strToFloat(const char* str,
	const char* inf = FLT_INF, const char* nan = FLT_NAN);


bool strToFloat(const std::string&, float& result,
	char decSep = '.', char thSep = ',',
	const char* inf = FLT_INF, const char* nan = FLT_NAN);


double strToDouble(const char* str,
	const char* inf = FLT_INF, const char* nan = FLT_NAN);


bool strToDouble(const std::string& str, double& result,
	char decSep = '.', char thSep = ',',
	const char* inf = FLT_INF, const char* nan = FLT_NAN);
