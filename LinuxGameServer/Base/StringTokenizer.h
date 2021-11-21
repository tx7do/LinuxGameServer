#pragma once

#include "BaseException.h"
#include <vector>
#include <cstddef>

class StringTokenizer
{
public:
	enum Options
	{
		TOK_IGNORE_EMPTY = 1,
		TOK_TRIM = 2
	};

	typedef std::vector<std::string> TokenVec;
	typedef TokenVec::const_iterator Iterator;

	StringTokenizer(const std::string& str, const std::string& separators, int options = 0);

	~StringTokenizer();

	Iterator begin() const;
	Iterator end() const;

	const std::string& operator [] (std::size_t index) const;

	std::string& operator [] (std::size_t index);

	bool has(const std::string& token) const;

	std::string::size_type find(const std::string& token, std::string::size_type pos = 0) const;

	std::size_t replace(const std::string& oldToken, const std::string& newToken, std::string::size_type pos = 0);

	std::size_t count() const;

	std::size_t count(const std::string& token) const;

private:
	void trim(std::string& token);

	TokenVec _tokens;
};


//
// inlines
//


inline StringTokenizer::Iterator StringTokenizer::begin() const
{
	return _tokens.begin();
}


inline StringTokenizer::Iterator StringTokenizer::end() const
{
	return _tokens.end();
}


inline std::string& StringTokenizer::operator [] (std::size_t index)
{
	if (index >= _tokens.size()) throw RangeException();
	return _tokens[index];
}


inline const std::string& StringTokenizer::operator [] (std::size_t index) const
{
	if (index >= _tokens.size()) throw RangeException();
	return _tokens[index];
}


inline std::size_t StringTokenizer::count() const
{
	return _tokens.size();
}

