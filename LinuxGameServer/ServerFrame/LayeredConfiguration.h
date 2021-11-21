#pragma once

#include "Configuration.h"
#include "AutoPtr.h"
#include <list>
#include <memory>

class LayeredConfiguration : public IConfiguration
{
public:
	using Ptr = AutoPtr<LayeredConfiguration>;
	friend Ptr;

	LayeredConfiguration();

	void add(IConfiguration::Ptr pConfig);

	void add(IConfiguration::Ptr pConfig, const std::string& label);

	void add(IConfiguration::Ptr pConfig, int priority);

	void add(IConfiguration::Ptr pConfig, const std::string& label, int priority);

	void add(IConfiguration::Ptr pConfig, int priority, bool writeable);

	void add(IConfiguration::Ptr pConfig, const std::string& label, int priority, bool writeable);

	void addWriteable(IConfiguration::Ptr pConfig, int priority);

	IConfiguration::Ptr find(const std::string& label) const;

	void removeConfiguration(IConfiguration::Ptr pConfig);

protected:
	struct ConfigItem
	{
		typedef IConfiguration::Ptr ACPtr;
		ACPtr       pConfig;
		int         priority;
		bool        writeable;
		std::string label;
	};

	bool getRaw(const std::string& key, std::string& value) const;
	void setRaw(const std::string& key, const std::string& value);
	void enumerate(const std::string& key, Keys& range) const;
	void removeRaw(const std::string& key);

	int lowest() const;
	int highest() const;
	void insert(const ConfigItem& item);

	~LayeredConfiguration();

private:
	LayeredConfiguration(const LayeredConfiguration&);
	LayeredConfiguration& operator = (const LayeredConfiguration&);

	typedef std::list<ConfigItem> ConfigList;

	ConfigList _configs;
};

