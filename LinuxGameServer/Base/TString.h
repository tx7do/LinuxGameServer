/**********************************************************************
* Copyright (C) 2008 - 盛扬天下 - All Rights Reserved
*
* 文件名称:		StringTemplate.h
* 摘    要:		字符串有关的模板方法
*
* 作    者:		yanglinbo,
* 修    改:		查看文件最下方.
*
***********************************************************************/

#pragma once


#include "BaseException.h"
#include "Bugcheck.h"
#include "Ascii.h"
#include <cstring>
#include <cctype>
#include <sstream>
#include <list>
#include <vector>


#define MG_MAX_INT_STRING_LEN 65
#define MG_MAX_FLT_STRING_LEN 780


inline char tolowercase(char c)
{
	return static_cast<char>(tolower(c));
}


const size_t SIZE_UNKNOWN = static_cast<size_t>(-1);

template<class CTYPE>
struct Traits
{
	// STL string type
	//typedef XXX string;
	// Null-terminated string
	//inline static const CTYPE* empty_str();
};

/// 返回空字符串而非空指针
template<class CTYPE>
inline const CTYPE* nonnull(const CTYPE* str, const CTYPE* def_str = NULL)
{
	return str ? str : (def_str ? def_str : Traits<CTYPE>::empty_str());
}

/// 查找字符串s中首次出现字符c的位置
template<class CTYPE>
const CTYPE* strchr(const CTYPE* str, const CTYPE* chs)
{
	for (size_t i = 0; str[i]; ++i)
	{
		for (size_t j = 0; chs[j]; ++j)
		{
			if (str[i] == chs[j])
			{
				return str + i;
			}
		}
	}
	return 0;
}

/// 
template<class CTYPE>
const CTYPE* strchrn(const CTYPE* str, size_t slen, CTYPE ch)
{
	for (size_t i = 0; i < slen && str[i]; ++i)
	{
		if (str[i] == ch)
		{
			return str + i;
		}
	}
	return 0;
}

/// 
template<class CTYPE>
size_t strlenn(const CTYPE* buffer, size_t buflen)
{
	size_t bufpos = 0;
	while (buffer[bufpos] && (bufpos < buflen))
	{
		++bufpos;
	}
	return bufpos;
}

template<class CTYPE>
size_t strcpyn(CTYPE* buffer, size_t buflen,
	const CTYPE* source, size_t srclen = SIZE_UNKNOWN)
{
	if (buflen <= 0)
		return 0;

	if (srclen == SIZE_UNKNOWN)
	{
		srclen = strlenn(source, buflen - 1);
	}
	else if (srclen >= buflen)
	{
		srclen = buflen - 1;
	}
	memcpy(buffer, source, srclen * sizeof(CTYPE));
	buffer[srclen] = 0;
	return srclen;
}

template<class CTYPE>
size_t strcatn(CTYPE* buffer, size_t buflen,
	const CTYPE* source, size_t srclen = SIZE_UNKNOWN)
{
	if (buflen <= 0)
		return 0;

	size_t bufpos = strlenn(buffer, buflen - 1);
	return bufpos + strcpyn(buffer + bufpos, buflen - bufpos, source, srclen);
}

/// 修剪掉字符串左边的空白字符
template <class S>
S trimLeft(const S& str)
{
	typename S::const_iterator it = str.begin();
	typename S::const_iterator end = str.end();

	while (it != end && isspace(*it)) ++it;
	return S(it, end);
}


/// 修剪掉字符串左边的空白字符
template <class S>
S& trimLeftInPlace(S& str)
{
	typename S::iterator it = str.begin();
	typename S::iterator end = str.end();

	while (it != end && isspace(*it)) ++it;
	str.erase(str.begin(), it);
	return str;
}


/// 修剪掉字符串右边的空白字符
template <class S>
S trimRight(const S& str)
{
	int pos = int(str.size()) - 1;

	while (pos >= 0 && isspace(str[pos])) --pos;
	return S(str, 0, pos + 1);
}


/// 
template <class S>
S& trimRightInPlace(S& str)
{
	int pos = int(str.size()) - 1;

	while (pos >= 0 && isspace(str[pos])) --pos;
	str.resize(pos + 1);

	return str;
}


/// 
template <class S>
S& trimInPlace(S& str)
{
	int first = 0;
	int last = int(str.size()) - 1;

	while (first <= last && isspace(str[first])) ++first;
	while (last >= first && isspace(str[last])) --last;

	str.resize(last + 1);
	str.erase(0, first);

	return str;
}


/// 将字符串转换为大写
template <class S>
S toUpper(const S& str)
{
	typename S::const_iterator it = str.begin();
	typename S::const_iterator end = str.end();

	S result;
	result.reserve(str.size());
	while (it != end) result += std::toupper(*it++);
	return result;
}


/// 
template <class S>
S& toUpperInPlace(S& str)
{
	typename S::iterator it = str.begin();
	typename S::iterator end = str.end();

	while (it != end) { *it = std::toupper(*it); ++it; }
	return str;
}


/// 将字符串转换为小写
template <class S>
S toLower(const S& str)
{
	typename S::const_iterator it = str.begin();
	typename S::const_iterator end = str.end();

	S result;
	result.reserve(str.size());
	while (it != end) result += std::tolower(*it++);
	return result;
}


/// 
template <class S>
S& toLowerInPlace(S& str)
{
	typename S::iterator it = str.begin();
	typename S::iterator end = str.end();

	while (it != end) { *it = std::tolower(*it); ++it; }
	return str;
}


template <class S, class It>
int icompare(
	const S& str,
	typename S::size_type pos,
	typename S::size_type n,
	It it2,
	It end2)
	/// Case-insensitive string comparison
{
	typename S::size_type sz = str.size();
	if (pos > sz) pos = sz;
	if (pos + n > sz) n = sz - pos;
	It it1 = str.begin() + pos;
	It end1 = str.begin() + pos + n;
	while (it1 != end1 && it2 != end2)
	{
		typename S::value_type c1(static_cast<typename S::value_type>(Ascii::toLower(*it1)));
		typename S::value_type c2(static_cast<typename S::value_type>(Ascii::toLower(*it2)));
		if (c1 < c2)
			return -1;
		else if (c1 > c2)
			return 1;
		++it1; ++it2;
	}

	if (it1 == end1)
		return it2 == end2 ? 0 : -1;
	else
		return 1;
}


template <class S>
int icompare(const S& str1, const S& str2)
// A special optimization for an often used case.
{
	typename S::const_iterator it1(str1.begin());
	typename S::const_iterator end1(str1.end());
	typename S::const_iterator it2(str2.begin());
	typename S::const_iterator end2(str2.end());
	while (it1 != end1 && it2 != end2)
	{
		typename S::value_type c1(static_cast<typename S::value_type>(Ascii::toLower(*it1)));
		typename S::value_type c2(static_cast<typename S::value_type>(Ascii::toLower(*it2)));
		if (c1 < c2)
			return -1;
		else if (c1 > c2)
			return 1;
		++it1; ++it2;
	}

	if (it1 == end1)
		return it2 == end2 ? 0 : -1;
	else
		return 1;
}


template <class S>
int icompare(const S& str1, typename S::size_type n1, const S& str2, typename S::size_type n2)
{
	if (n2 > str2.size()) n2 = str2.size();
	return icompare(str1, 0, n1, str2.begin(), str2.begin() + n2);
}


template <class S>
int icompare(const S& str1, typename S::size_type n, const S& str2)
{
	if (n > str2.size()) n = str2.size();
	return icompare(str1, 0, n, str2.begin(), str2.begin() + n);
}


template <class S>
int icompare(const S& str1, typename S::size_type pos, typename S::size_type n, const S& str2)
{
	return icompare(str1, pos, n, str2.begin(), str2.end());
}


template <class S>
int icompare(
	const S& str1,
	typename S::size_type pos1,
	typename S::size_type n1,
	const S& str2,
	typename S::size_type pos2,
	typename S::size_type n2)
{
	typename S::size_type sz2 = str2.size();
	if (pos2 > sz2) pos2 = sz2;
	if (pos2 + n2 > sz2) n2 = sz2 - pos2;
	return icompare(str1, pos1, n1, str2.begin() + pos2, str2.begin() + pos2 + n2);
}


template <class S>
int icompare(
	const S& str1,
	typename S::size_type pos1,
	typename S::size_type n,
	const S& str2,
	typename S::size_type pos2)
{
	typename S::size_type sz2 = str2.size();
	if (pos2 > sz2) pos2 = sz2;
	if (pos2 + n > sz2) n = sz2 - pos2;
	return icompare(str1, pos1, n, str2.begin() + pos2, str2.begin() + pos2 + n);
}


template <class S>
int icompare(
	const S& str,
	typename S::size_type pos,
	typename S::size_type n,
	const typename S::value_type* ptr)
{
	CHECK_PTR(ptr);
	typename S::size_type sz = str.size();
	if (pos > sz) pos = sz;
	if (pos + n > sz) n = sz - pos;
	typename S::const_iterator it = str.begin() + pos;
	typename S::const_iterator end = str.begin() + pos + n;
	while (it != end && *ptr)
	{
		typename S::value_type c1(static_cast<typename S::value_type>(Ascii::toLower(*it)));
		typename S::value_type c2(static_cast<typename S::value_type>(Ascii::toLower(*ptr)));
		if (c1 < c2)
			return -1;
		else if (c1 > c2)
			return 1;
		++it; ++ptr;
	}

	if (it == end)
		return *ptr == 0 ? 0 : -1;
	else
		return 1;
}


template <class S>
int icompare(
	const S& str,
	typename S::size_type pos,
	const typename S::value_type* ptr)
{
	return icompare(str, pos, str.size() - pos, ptr);
}


template <class S>
int icompare(
	const S& str,
	const typename S::value_type* ptr)
{
	return icompare(str, 0, str.size(), ptr);
}


template <class S>
S translate(const S& str, const S& from, const S& to)
{
	S result;
	result.reserve(str.size());
	typename S::const_iterator it = str.begin();
	typename S::const_iterator end = str.end();
	typename S::size_type toSize = to.size();
	while (it != end)
	{
		typename S::size_type pos = from.find(*it);
		if (pos == S::npos)
		{
			result += *it;
		}
		else
		{
			if (pos < toSize) result += to[pos];
		}
		++it;
	}
	return result;
}


/// 
template <class S>
S translate(const S& str, const typename S::value_type* from, const typename S::value_type* to)
{
	CHECK_PTR(from);
	CHECK_PTR(to);
	return translate(str, S(from), S(to));
}


/// 
template <class S>
S& translateInPlace(S& str, const S& from, const S& to)
{
	str = translate(str, from, to);
	return str;
}


/// 
template <class S>
S translateInPlace(S& str, const typename S::value_type* from, const typename S::value_type* to)
{
	CHECK_PTR(from);
	CHECK_PTR(to);
	str = translate(str, S(from), S(to));
	return str;
}


/// 字符串替换
template <class S>
S replace(const S& str, const S& from, const S& to, typename S::size_type start = 0)
{
	S result(str);
	replaceInPlace(result, from, to, start);
	return result;
}


/// 字符串替换
template <class S>
S replace(const S& str, const typename S::value_type* from, const typename S::value_type* to, typename S::size_type start = 0)
{
	S result(str);
	replaceInPlace(result, from, to, start);
	return result;
}


template <class S>
S& replaceInPlace(S& str, const S& from, const S& to, typename S::size_type start = 0)
{
	ASSERT(from.size() > 0);

	S result;
	typename S::size_type pos = 0;
	result.append(str, 0, start);
	do
	{
		pos = str.find(from, start);
		if (pos != S::npos)
		{
			result.append(str, start, pos - start);
			result.append(to);
			start = pos + from.length();
		}
		else result.append(str, start, str.size() - start);
	} while (pos != S::npos);
	str.swap(result);
	return str;
}


template <class S>
S& replaceInPlace(S& str, const typename S::value_type* from, const typename S::value_type* to, typename S::size_type start = 0)
{
	ASSERT(*from);

	S result;
	typename S::size_type pos = 0;
	typename S::size_type fromLen = std::strlen(from);
	result.append(str, 0, start);
	do
	{
		pos = str.find(from, start);
		if (pos != S::npos)
		{
			result.append(str, start, pos - start);
			result.append(to);
			start = pos + fromLen;
		}
		else result.append(str, start, str.size() - start);
	} while (pos != S::npos);
	str.swap(result);
	return str;
}


/// 字符串拼接
template <class S>
S cat(const S& s1, const S& s2)
{
	S result = s1;
	result.reserve(s1.size() + s2.size());
	result.append(s2);
	return result;
}


/// 字符串拼接
template <class S>
S cat(const S& s1, const S& s2, const S& s3)
{
	S result = s1;
	result.reserve(s1.size() + s2.size() + s3.size());
	result.append(s2);
	result.append(s3);
	return result;
}


/// 字符串拼接
template <class S>
S cat(const S& s1, const S& s2, const S& s3, const S& s4)
{
	S result = s1;
	result.reserve(s1.size() + s2.size() + s3.size() + s4.size());
	result.append(s2);
	result.append(s3);
	result.append(s4);
	return result;
}


/// 字符串拼接
template <class S>
S cat(const S& s1, const S& s2, const S& s3, const S& s4, const S& s5)
{
	S result = s1;
	result.reserve(s1.size() + s2.size() + s3.size() + s4.size() + s5.size());
	result.append(s2);
	result.append(s3);
	result.append(s4);
	result.append(s5);
	return result;
}


/// 字符串拼接
template <class S>
S cat(const S& s1, const S& s2, const S& s3, const S& s4, const S& s5, const S& s6)
{
	S result = s1;
	result.reserve(s1.size() + s2.size() + s3.size() + s4.size() + s5.size() + s6.size());
	result.append(s2);
	result.append(s3);
	result.append(s4);
	result.append(s5);
	result.append(s6);
	return result;
}


/// 字符串拼接
template <class S, class It>
S cat(const S& delim, const It& begin, const It& end)
{
	S result;
	for (It it = begin; it != end; ++it)
	{
		if (!result.empty()) result.append(delim);
		result += *it;
	}
	return result;
}


/// 分割字符串
/// @param[out] ret 分割后滴字符串列表
/// @param[in] rString 需要被分割的字符串
/// @param[in] delims 分隔符
/// @param[in] maxSplits 最大分割数
template <class S>
void split(std::list<S>& ret,
	const S& rString,
	const S& delims = "\t\n",
	unsigned int maxSplits = 0)
{
	unsigned int numSplits = 0;

	typename S::size_type start, pos;
	start = 0;
	do
	{
		pos = rString.find_first_of(delims, start);
		if (pos == start)
		{
			start = pos + 1;
		}
		else if (pos == rString.npos || (maxSplits && numSplits == maxSplits))
		{
			ret.push_back(rString.substr(start));
		}
		else
		{
			ret.push_back(rString.substr(start, pos - start));
			start = pos + 1;
		}

		start = rString.find_first_not_of(delims, start);
		++numSplits;

	} while (pos != rString.npos);
}

/// 分割字符串
/// @param[in] rString 需要被分割的字符串
/// @param[in] delims 分隔符
/// @param[in] maxSplits 最大分割数
/// @return 分割后滴字符串列表
template <class S>
std::list<S> split(const S& rString,
	const S& delims = "\t\n",
	unsigned int maxSplits = 0)
{
	static unsigned dl;
	std::list<S> ret;
	unsigned int numSplits = 0;

	typename S::size_type start, pos;
	start = 0;
	do
	{
		pos = rString.find_first_of(delims, start);
		if (pos == start)
		{
			start = pos + 1;
		}
		else if (pos == rString.npos || (maxSplits && numSplits == maxSplits))
		{
			ret.push_back(rString.substr(start));
		}
		else
		{
			ret.push_back(rString.substr(start, pos - start));
			start = pos + 1;
		}
		start = rString.find_first_not_of(delims, start);
		++numSplits;

	} while (pos != rString.npos);

	return ret;
}


/// 清除字符串中的空白字符
/// @note 只要是字符串中出现的空白字符都将被清除掉.
/// @note 方法不对传入参数进行任何修改
/// @param[in] str 要被处理的字符串
/// @return 处理后的字符串
template <class S>
S clearSpace(S& str)
{
	typename S::iterator iter = str.begin();
	typename S::iterator& iEnd = str.end();
	S strTmp;

	for (; iter != iEnd; ++iter)
	{
		if (!isspace(*iter))
		{
			strTmp.push_back(*iter);
		}
	}

	return strTmp;
}




template <class S>
S& replaceInPlace(S& str, const typename S::value_type from, const typename S::value_type to = 0, typename S::size_type start = 0)
{
	if (from == to) return str;

	typename S::size_type pos = 0;
	do
	{
		pos = str.find(from, start);
		if (pos != S::npos)
		{
			if (to) str[pos] = to;
			else str.erase(pos, 1);
		}
	} while (pos != S::npos);

	return str;
}


template <class S>
S& removeInPlace(S& str, const typename S::value_type ch, typename S::size_type start = 0)
{
	return replaceInPlace(str, ch, 0, start);
}



template <class S>
S remove(const S& str, const typename S::value_type ch, typename S::size_type start = 0)
{
	S result(str);
	replaceInPlace(result, ch, 0, start);
	return result;
}
