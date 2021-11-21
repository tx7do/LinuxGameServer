#pragma once

#include <set>
#include <string>

class OptionManager;

class OptionProcessor
{
public:
	OptionProcessor(const OptionManager& options);

	~OptionProcessor();

	void setUnixStyle(bool flag);

	bool isUnixStyle() const;

	bool process(const std::string& argument, std::string& optionName, std::string& optionArg);

	void checkRequired() const;

private:
	bool processUnix(const std::string& argument, std::string& optionName, std::string& optionArg);
	bool processDefault(const std::string& argument, std::string& optionName, std::string& optionArg);
	bool processCommon(const std::string& option, bool isShort, std::string& optionName, std::string& optionArg);

	const OptionManager& _options;
	bool _unixStyle;
	bool _ignore;
	std::set<std::string> _groups;
	std::set<std::string> _specifiedOptions;
	std::string _deferredOption;
};


//
// inlines
//
inline bool OptionProcessor::isUnixStyle() const
{
	return _unixStyle;
}
