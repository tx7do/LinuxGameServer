
#include "Configuration.h"
#include "BaseException.h"
#include "NumberParser.h"
#include "NumberFormatter.h"
#include "TString.h"


Configuration::Configuration() :
	_depth(0),
	_eventsEnabled(true)
{
}


Configuration::~Configuration()
{
}


bool Configuration::hasProperty(const std::string& key) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	return getRaw(key, value);
}


bool Configuration::hasOption(const std::string& key) const
{
	return hasProperty(key);
}


bool Configuration::has(const std::string& key) const
{
	return hasProperty(key);
}


std::string Configuration::getString(const std::string& key) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return internalExpand(value);
	else
		throw NotFoundException(key);
}


std::string Configuration::getString(const std::string& key, const std::string& defaultValue) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return internalExpand(value);
	else
		return defaultValue;
}


std::string Configuration::getRawString(const std::string& key) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return value;
	else
		throw NotFoundException(key);
}


std::string Configuration::getRawString(const std::string& key, const std::string& defaultValue) const
{

	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return value;
	else
		return defaultValue;
}


Int32 Configuration::getInt(const std::string& key) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt(internalExpand(value));
	else
		throw NotFoundException(key);
}


Int32 Configuration::getInt(const std::string& key, Int32 defaultValue) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt(internalExpand(value));
	else
		return defaultValue;
}


UInt32 Configuration::getUInt(const std::string& key) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt(internalExpand(value));
	else
		throw NotFoundException(key);
}


UInt32 Configuration::getUInt(const std::string& key, UInt32 defaultValue) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt(internalExpand(value));
	else
		return defaultValue;
}


Int64 Configuration::getInt64(const std::string& key) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt64(internalExpand(value));
	else
		throw NotFoundException(key);
}


Int64 Configuration::getInt64(const std::string& key, Int64 defaultValue) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt64(internalExpand(value));
	else
		return defaultValue;
}


UInt64 Configuration::getUInt64(const std::string& key) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt64(internalExpand(value));
	else
		throw NotFoundException(key);
}


UInt64 Configuration::getUInt64(const std::string& key, UInt64 defaultValue) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt64(internalExpand(value));
	else
		return defaultValue;
}


double Configuration::getDouble(const std::string& key) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return NumberParser::parseFloat(internalExpand(value));
	else
		throw NotFoundException(key);
}


double Configuration::getDouble(const std::string& key, double defaultValue) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return NumberParser::parseFloat(internalExpand(value));
	else
		return defaultValue;
}


bool Configuration::getBool(const std::string& key) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseBool(internalExpand(value));
	else
		throw NotFoundException(key);
}


bool Configuration::getBool(const std::string& key, bool defaultValue) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseBool(internalExpand(value));
	else
		return defaultValue;
}


void Configuration::setString(const std::string& key, const std::string& value)
{
	setRawWithEvent(key, value);
}


void Configuration::setInt(const std::string& key, Int32 value)
{
	setRawWithEvent(key, NumberFormatter::format(value));
}


void Configuration::setUInt(const std::string& key, UInt32 value)
{
	setRawWithEvent(key, NumberFormatter::format(value));
}

void Configuration::setInt64(const std::string& key, Int64 value)
{
	std::unique_lock<std::mutex> lock(_mutex);

	setRawWithEvent(key, NumberFormatter::format(value));
}


void Configuration::setUInt64(const std::string& key, UInt64 value)
{
	std::unique_lock<std::mutex> lock(_mutex);

	setRawWithEvent(key, NumberFormatter::format(value));
}

void Configuration::setDouble(const std::string& key, double value)
{
	setRawWithEvent(key, NumberFormatter::format(value));
}


void Configuration::setBool(const std::string& key, bool value)
{
	setRawWithEvent(key, value ? "true" : "false");
}


void Configuration::keys(Keys& range) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	std::string key;
	range.clear();
	enumerate(key, range);
}


void Configuration::keys(const std::string& key, Keys& range) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	range.clear();
	enumerate(key, range);
}



namespace
{
	class AutoCounter
	{
	public:
		AutoCounter(int& count) : _count(count)
		{
			++_count;
		}

		~AutoCounter()
		{
			--_count;
		}

	private:
		int& _count;
	};
}


std::string Configuration::expand(const std::string& value) const
{
	std::unique_lock<std::mutex> lock(_mutex);

	return internalExpand(value);
}


void Configuration::remove(const std::string& key)
{
	if (_eventsEnabled)
	{
		//propertyRemoving(this, key);
	}
	{

		std::unique_lock<std::mutex> lock(_mutex);
		removeRaw(key);
	}
	if (_eventsEnabled)
	{
		//propertyRemoved(this, key);
	}
}


void Configuration::enableEvents(bool enable)
{
	_eventsEnabled = enable;
}


bool Configuration::eventsEnabled() const
{
	return _eventsEnabled;
}


void Configuration::removeRaw(const std::string& key)
{
	throw NotImplementedException("removeRaw()");
}


std::string Configuration::internalExpand(const std::string& value) const
{
	AutoCounter counter(_depth);
	if (_depth > 10) throw CircularReferenceException("Too many property references encountered");
	return uncheckedExpand(value);
}


std::string Configuration::uncheckedExpand(const std::string& value) const
{
	std::string result;
	std::string::const_iterator it = value.begin();
	std::string::const_iterator end = value.end();
	while (it != end)
	{
		if (*it == '$')
		{
			++it;
			if (it != end && *it == '{')
			{
				++it;
				std::string prop;
				while (it != end && *it != '}') prop += *it++;
				if (it != end) ++it;
				std::string value;
				if (getRaw(prop, value))
				{
					result.append(internalExpand(value));
				}
				else
				{
					result.append("${");
					result.append(prop);
					result.append("}");
				}
			}
			else result += '$';
		}
		else result += *it++;
	}
	return result;
}


Int32 Configuration::parseInt(const std::string& value)
{
	if ((value.compare(0, 2, "0x") == 0) || (value.compare(0, 2, "0X") == 0))
		return static_cast<int>(NumberParser::parseHex(value));
	else
		return NumberParser::parse(value);
}


UInt32 Configuration::parseUInt(const std::string& value)
{
	if ((value.compare(0, 2, "0x") == 0) || (value.compare(0, 2, "0X") == 0))
		return NumberParser::parseHex(value);
	else
		return NumberParser::parseUnsigned(value);
}


Int64 Configuration::parseInt64(const std::string& value)
{
	if ((value.compare(0, 2, "0x") == 0) || (value.compare(0, 2, "0X") == 0))
		return static_cast<Int64>(NumberParser::parseHex64(value));
	else
		return NumberParser::parse64(value);
}


UInt64 Configuration::parseUInt64(const std::string& value)
{
	if ((value.compare(0, 2, "0x") == 0) || (value.compare(0, 2, "0X") == 0))
		return NumberParser::parseHex64(value);
	else
		return NumberParser::parseUnsigned64(value);
}


bool Configuration::parseBool(const std::string& value)
{
	int n;
	if (NumberParser::tryParse(value, n))
		return n != 0;
	else if (icompare(value, std::string("true")) == 0)
		return true;
	else if (icompare(value, std::string("yes")) == 0)
		return true;
	else if (icompare(value, std::string("on")) == 0)
		return true;
	else if (icompare(value, std::string("false")) == 0)
		return false;
	else if (icompare(value, std::string("no")) == 0)
		return false;
	else if (icompare(value, std::string("off")) == 0)
		return false;
	else
		throw SyntaxException("Cannot convert to boolean", value);
}


void Configuration::setRawWithEvent(const std::string& key, std::string value)
{
	KeyValue kv(key, value);
	if (_eventsEnabled)
	{
		//propertyChanging(this, kv);
	}
	{
		std::unique_lock<std::mutex> lock(_mutex);
		setRaw(key, value);
	}
	if (_eventsEnabled)
	{
		//propertyChanged(this, kv);
	}
}
