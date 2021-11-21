#pragma once

#include <atomic>

class AtomicCounter
{
public:
	typedef int ValueType;

	AtomicCounter();
	explicit AtomicCounter(ValueType initialValue);
	AtomicCounter(const AtomicCounter& counter);
	~AtomicCounter();

	AtomicCounter& operator = (const AtomicCounter& counter);
	AtomicCounter& operator = (ValueType value);
	operator ValueType () const;

	ValueType value() const;

	ValueType operator ++ ();

	ValueType operator ++ (int);

	ValueType operator -- ();
	ValueType operator -- (int);

	bool operator ! () const;

private:
	std::atomic_int _counter;
};

inline AtomicCounter::operator AtomicCounter::ValueType() const
{
	return _counter.load();
}


inline AtomicCounter::ValueType AtomicCounter::value() const
{
	return _counter.load();
}


inline AtomicCounter::ValueType AtomicCounter::operator ++ () // prefix
{
	return ++_counter;
}


inline AtomicCounter::ValueType AtomicCounter::operator ++ (int) // postfix
{
	return _counter++;
}


inline AtomicCounter::ValueType AtomicCounter::operator -- () // prefix
{
	return --_counter;
}


inline AtomicCounter::ValueType AtomicCounter::operator -- (int) // postfix
{
	return _counter--;
}


inline bool AtomicCounter::operator ! () const
{
	return _counter.load() == 0;
}
