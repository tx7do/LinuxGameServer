#pragma once

#include "RefCountedObject.h"

class Application;
class OptionManager;

class Subsystem : public RefCountedObject
{
public:
	Subsystem();

	virtual const char* name() const = 0;

protected:
	virtual void initialize(Application& app) = 0;

	virtual void uninitialize() = 0;

	virtual void reinitialize(Application& app);

	virtual void defineOptions(OptionManager& options);

	virtual ~Subsystem();

	friend class Application;

private:
	Subsystem(const Subsystem&);
	Subsystem& operator = (const Subsystem&);
};
