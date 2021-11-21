#include "OptionManager.h"
#include "Bugcheck.h"
#include "OptionProcessor.h"
#include "BaseException.h"

OptionManager::OptionManager() : 
	_stopOptionsProcessing(false),
	_unixOptions(false)
{
}


OptionManager::OptionManager(const OptionManager& options) :
	_options(options._options),
	_stopOptionsProcessing(false),
	_unixOptions(false)
{
}


OptionManager::~OptionManager()
{
}


OptionManager& OptionManager::operator = (const OptionManager& options)
{
	if (&options != this)
		_options = options._options;
	return *this;
}


void OptionManager::addOption(const Option& option)
{
	ASSERT(!option.fullName().empty());
	OptionVec::const_iterator it = _options.begin();
	OptionVec::const_iterator itEnd = _options.end();
	for (; it != itEnd; ++it)
	{
		if (it->fullName() == option.fullName())
		{
			throw DuplicateOptionException(it->fullName());
		}
	}

	_options.push_back(option);
}


bool OptionManager::hasOption(const std::string& name, bool matchShort) const
{
	bool found = false;
	for (const auto& opt : _options)
	{
		if ((matchShort && opt.matchesShort(name)) || (!matchShort && opt.matchesFull(name)))
		{
			if (!found)
				found = true;
			else
				return false;
		}
	}
	return found;
}


const Option& OptionManager::getOption(const std::string& name, bool matchShort) const
{
	const Option* pOption = 0;
	for (const auto& opt : _options)
	{
		if ((matchShort && opt.matchesShort(name)) || (!matchShort && opt.matchesPartial(name)))
		{
			if (!pOption)
			{
				pOption = &opt;
				if (!matchShort && opt.matchesFull(name))
					break;
			}
			else if (!matchShort && opt.matchesFull(name))
			{
				pOption = &opt;
				break;
			}
			else throw AmbiguousOptionException(name);
		}
	}
	if (pOption)
		return *pOption;
	else
		throw UnknownOptionException(name);
}


OptionManager::Iterator OptionManager::begin() const
{
	return _options.begin();
}


OptionManager::Iterator OptionManager::end() const
{
	return _options.end();
}

bool OptionManager::processArgs(const ArgVec& args)
{
	OptionProcessor processor(*this);
	processor.setUnixStyle(_unixOptions);

	_unprocessedArgs = args;
	_unprocessedArgs.erase(_unprocessedArgs.begin());

	ArgVec::iterator it = _unprocessedArgs.begin();
	while (it != _unprocessedArgs.end() && !_stopOptionsProcessing)
	{
		std::string name;
		std::string value;
		if (processor.process(*it, name, value))
		{
			if (!name.empty()) // "--" option to end options processing or deferred argument
			{
				handleOption(name, value);
			}
			it = _unprocessedArgs.erase(it);
		}
		else ++it;
	}
	if (!_stopOptionsProcessing)
		processor.checkRequired();

	return true;
}

void OptionManager::stopOptionsProcessing()
{
	_stopOptionsProcessing = true;
}

void OptionManager::setUnixOptions(bool flag)
{
	_unixOptions = flag;
}

void OptionManager::handleOption(const std::string& name, const std::string& value)
{
	const Option& option = getOption(name);
	if (option.callback())
	{
		(*option.callback())(name, value);
	}
}
