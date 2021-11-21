#pragma once

#include <map>

class TLSAbstractSlot
{
public:
	TLSAbstractSlot();
	virtual ~TLSAbstractSlot();
};


template <class C>
class TLSSlot : public TLSAbstractSlot
{
public:
	TLSSlot() :
		_value()
	{
	}

	~TLSSlot()
	{
	}

	C& value()
	{
		return _value;
	}

private:
	TLSSlot(const TLSSlot&);
	TLSSlot& operator = (const TLSSlot&);

	C _value;
};


class ThreadLocalStorage
{
public:
	ThreadLocalStorage();

	~ThreadLocalStorage();

	TLSAbstractSlot*& get(const void* key);

	static ThreadLocalStorage& current();

	static void clear();

private:
	typedef std::map<const void*, TLSAbstractSlot*> TLSMap;

	TLSMap _map;

	friend class Thread;
};


template <class C>
class ThreadLocal
{
	typedef TLSSlot<C> Slot;

public:
	ThreadLocal()
	{
	}

	~ThreadLocal()
	{
	}

	C* operator -> ()
	{
		return &get();
	}

	C& operator * ()
	{
		return get();
	}

	C& get()
	{
		TLSAbstractSlot*& p = ThreadLocalStorage::current().get(this);
		if (!p) p = new Slot;
		return static_cast<Slot*>(p)->value();
	}

private:
	ThreadLocal(const ThreadLocal&);
	ThreadLocal& operator = (const ThreadLocal&);
};
