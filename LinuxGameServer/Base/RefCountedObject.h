#pragma once

#include "Bugcheck.h"
#include <atomic>

class RefCountedObject
{
public:
	RefCountedObject();

	void duplicate() const;

	void release() const noexcept;

	int referenceCount() const;

protected:
	virtual ~RefCountedObject();

private:
	RefCountedObject(const RefCountedObject&);
	RefCountedObject& operator = (const RefCountedObject&);

	mutable std::atomic_int _counter;
};


inline int RefCountedObject::referenceCount() const
{
	return _counter.load();
}


inline void RefCountedObject::duplicate() const
{
	++_counter;
}


inline void RefCountedObject::release() const noexcept
{
	try
	{
		if (--_counter == 0) delete this;
	}
	catch (...)
	{
		UNEXPECTED();
	}
}
