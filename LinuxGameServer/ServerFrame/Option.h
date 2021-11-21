#pragma once


#include <string>
#include <functional>


typedef std::function<void(const std::string& name, const std::string& value)> OptionCallback;

class Option
{
public:
	Option();

	Option(const Option& option);

	Option(const std::string& fullName, const std::string& shortName);

	Option(const std::string& fullName, const std::string& shortName, const std::string& description, bool required = false);

	Option(const std::string& fullName, const std::string& shortName, const std::string& description, bool required, const std::string& argName, bool argRequired = false);

	~Option();

	Option& operator = (const Option& option);

	void swap(Option& option);

	Option& shortName(const std::string& name);

	Option& fullName(const std::string& name);

	Option& description(const std::string& text);

	Option& required(bool flag);

	Option& repeatable(bool flag);

	Option& argument(const std::string& name, bool required = true);

	Option& noArgument();

	Option& group(const std::string& group);

	Option& binding(const std::string& propertyName);

	Option& callback(OptionCallback cb);

	const std::string& shortName() const;

	const std::string& fullName() const;

	const std::string& description() const;

	bool required() const;

	bool repeatable() const;

	bool takesArgument() const;

	bool argumentRequired() const;

	const std::string& argumentName() const;

	const std::string& group() const;

	const std::string& binding() const;

	OptionCallback* callback() const;

	bool matchesShort(const std::string& option) const;

	bool matchesFull(const std::string& option) const;

	bool matchesPartial(const std::string& option) const;

	void process(const std::string& option, std::string& arg) const;

private:
	std::string _shortName;
	std::string _fullName;
	std::string _description;
	bool        _required;
	bool        _repeatable;
	std::string _argName;
	bool        _argRequired;
	std::string _group;
	std::string _binding;
	mutable OptionCallback _pCallback;
};


inline const std::string& Option::shortName() const
{
	return _shortName;
}


inline const std::string& Option::fullName() const
{
	return _fullName;
}


inline const std::string& Option::description() const
{
	return _description;
}


inline bool Option::required() const
{
	return _required;
}


inline bool Option::repeatable() const
{
	return _repeatable;
}


inline bool Option::takesArgument() const
{
	return !_argName.empty();
}


inline bool Option::argumentRequired() const
{
	return _argRequired;
}


inline const std::string& Option::argumentName() const
{
	return _argName;
}


inline const std::string& Option::group() const
{
	return _group;
}


inline const std::string& Option::binding() const
{
	return _binding;
}


inline OptionCallback* Option::callback() const
{
	return &_pCallback;
}
