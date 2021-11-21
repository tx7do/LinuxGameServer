#pragma once

#include <vector>
#include "Option.h"


class OptionManager
{
public:
	using OptionVec = std::vector<Option>;
	using Iterator = OptionVec::const_iterator;
	using ArgVec = std::vector<std::string>;

public:
	OptionManager();
	OptionManager(const OptionManager& options);
	~OptionManager();

	OptionManager& operator = (const OptionManager& options);

public:
	void addOption(const Option& option);

	bool hasOption(const std::string& name, bool matchShort = false) const;

	const Option& getOption(const std::string& name, bool matchShort = false) const;

	Iterator begin() const;
	Iterator end() const;

public:
	bool processArgs(const ArgVec& args);

	void stopOptionsProcessing();

	void setUnixOptions(bool flag);

protected:
	void handleOption(const std::string& name, const std::string& value);

private:
	OptionVec _options;

private:
	ArgVec          _unprocessedArgs;
	bool            _stopOptionsProcessing;
	bool			_unixOptions;
};
