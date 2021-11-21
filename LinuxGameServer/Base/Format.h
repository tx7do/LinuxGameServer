#pragma once

#include "Any.h"
#include <vector>
#include <type_traits>
#include <any>
#include <string>

std::string format(const std::string& fmt, const Any& value);

void format(std::string& result, const char* fmt, const std::vector<Any>& values);

void format(std::string& result, const std::string& fmt, const std::vector<Any>& values);


template <
	typename T,
	typename... Args>
	void format(std::string& result, const std::string& fmt, T arg1, Args... args)
{
	std::vector<Any> values;
	values.reserve(sizeof...(Args) + 1);
	values.emplace_back(arg1);
	values.insert(values.end(), { args... });
	format(result, fmt, values);
}


template <
	typename T,
	typename... Args>
	void format(std::string& result, const char* fmt, T arg1, Args... args)
{
	std::vector<Any> values;
	values.reserve(sizeof...(Args) + 1);
	values.emplace_back(arg1);
	values.insert(values.end(), { args... });
	format(result, fmt, values);
}


template <
	typename T,
	typename... Args>
	std::string format(const std::string& fmt, T arg1, Args... args)
{
	std::vector<Any> values;
	values.reserve(sizeof...(Args) + 1);
	values.emplace_back(arg1);
	values.insert(values.end(), { args... });
	std::string result;
	format(result, fmt, values);
	return result;
}


template <
	typename T,
	typename... Args>
	std::string format(const char* fmt, T arg1, Args... args)
{
	std::vector<Any> values;
	values.reserve(sizeof...(Args) + 1);
	values.emplace_back(arg1);
	values.insert(values.end(), { args... });
	std::string result;
	format(result, fmt, values);
	return result;
}
