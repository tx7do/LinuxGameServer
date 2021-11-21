#pragma once


#include "RefCountedObject.h"
#include "Types.h"
#include "AutoPtr.h"

#include <vector>
#include <string>
#include <mutex>
#include <memory>


class Configuration : public RefCountedObject
{
public:
	using Keys = std::vector<std::string>;
	using Ptr = AutoPtr<Configuration>;

	friend Ptr;

	class KeyValue
	{
	public:
		KeyValue(const std::string& key, std::string& value) :
			_key(key),
			_value(value)
		{
		}

		const std::string& key() const
		{
			return _key;
		}

		const std::string& value() const
		{
			return _value;
		}

		std::string& value()
		{
			return _value;
		}

	private:
		const std::string& _key;
		std::string& _value;
	};

public:
	Configuration();

public:
	//BasicEvent<KeyValue> propertyChanging;
	//BasicEvent<const KeyValue> propertyChanged;
	//BasicEvent<const std::string> propertyRemoving;
	//BasicEvent<const std::string> propertyRemoved;

public:
	bool hasProperty(const std::string& key) const;
	bool hasOption(const std::string& key) const;
	bool has(const std::string& key) const;

public:
	std::string getString(const std::string& key) const;
	std::string getString(const std::string& key, const std::string& defaultValue) const;

	std::string getRawString(const std::string& key) const;
	std::string getRawString(const std::string& key, const std::string& defaultValue) const;

	Int32 getInt(const std::string& key) const;
	Int32 getInt(const std::string& key, Int32 defaultValue) const;

	UInt32 getUInt(const std::string& key) const;
	UInt32 getUInt(const std::string& key, UInt32 defaultValue) const;

	UInt64 getUInt64(const std::string& key) const;
	UInt64 getUInt64(const std::string& key, UInt64 defaultValue) const;

	Int64 getInt64(const std::string& key) const;
	Int64 getInt64(const std::string& key, Int64 defaultValue) const;

	double getDouble(const std::string& key) const;
	double getDouble(const std::string& key, double defaultValue) const;

	bool getBool(const std::string& key) const;
	bool getBool(const std::string& key, bool defaultValue) const;

public:
	virtual void setString(const std::string& key, const std::string& value);

	virtual void setInt(const std::string& key, Int32 value);

	virtual void setUInt(const std::string& key, UInt32 value);

	virtual void setInt64(const std::string& key, Int64 value);

	virtual void setUInt64(const std::string& key, UInt64 value);

	virtual void setDouble(const std::string& key, double value);

	virtual void setBool(const std::string& key, bool value);

public:
	void keys(Keys& range) const;

	void keys(const std::string& key, Keys& range) const;

	std::string expand(const std::string& value) const;

	void remove(const std::string& key);

	void enableEvents(bool enable = true);

	bool eventsEnabled() const;

protected:
	virtual bool getRaw(const std::string& key, std::string& value) const
	{
		return false;
	}
	virtual void setRaw(const std::string& key, const std::string& value)
	{
	}
	void setRawWithEvent(const std::string& key, std::string value);
	virtual void removeRaw(const std::string& key);

	virtual void enumerate(const std::string& key, Keys& range) const
	{
		;
	}

protected:
	static Int32 parseInt(const std::string& value);

	static UInt32 parseUInt(const std::string& value);

	static Int64 parseInt64(const std::string& value);

	static UInt64 parseUInt64(const std::string& value);

	static bool parseBool(const std::string& value);

protected:
	virtual ~Configuration();

private:
	std::string internalExpand(const std::string& value) const;
	std::string uncheckedExpand(const std::string& value) const;

	Configuration(const Configuration&);
	Configuration& operator = (const Configuration&);

private:
	mutable int _depth;
	bool        _eventsEnabled;
	mutable std::mutex _mutex;
};
