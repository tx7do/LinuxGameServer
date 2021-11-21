#pragma once

#include <algorithm>
#include <cstddef>
#include "BaseException.h"


template <class C>
class AutoPtr
{
public:
	AutoPtr() 
		: _ptr(nullptr)
	{
	}

	AutoPtr(C* ptr) 
		: _ptr(ptr)
	{
	}

	AutoPtr(C* ptr, bool shared)
		: _ptr(ptr)
	{
		if (shared && _ptr) _ptr->duplicate();
	}

	AutoPtr(const AutoPtr& ptr)
		: _ptr(ptr._ptr)
	{
		if (_ptr) _ptr->duplicate();
	}

	AutoPtr(AutoPtr&& ptr) noexcept
		: _ptr(std::move(ptr._ptr))
	{
		ptr._ptr = nullptr;
	}

	template <class Other>
	AutoPtr(const AutoPtr<Other>& ptr)
		: _ptr(const_cast<Other*>(ptr.get()))
	{
		if (_ptr) _ptr->duplicate();
	}

	~AutoPtr()
	{
		if (_ptr) _ptr->release();
	}

	AutoPtr& assign(C* ptr)
	{
		if (_ptr != ptr)
		{
			if (_ptr) _ptr->release();
			_ptr = ptr;
		}
		return *this;
	}

	AutoPtr& assign(C* ptr, bool shared)
	{
		if (_ptr != ptr)
		{
			if (_ptr) _ptr->release();
			_ptr = ptr;
			if (shared && _ptr) _ptr->duplicate();
		}
		return *this;
	}

	AutoPtr& assign(const AutoPtr& ptr)
	{
		if (&ptr != this)
		{
			if (_ptr) _ptr->release();
			_ptr = ptr._ptr;
			if (_ptr) _ptr->duplicate();
		}
		return *this;
	}

	template <class Other>
	AutoPtr& assign(const AutoPtr<Other>& ptr)
	{
		if (ptr.get() != _ptr)
		{
			if (_ptr) _ptr->release();
			_ptr = const_cast<Other*>(ptr.get());
			if (_ptr) _ptr->duplicate();
		}
		return *this;
	}

	void reset()
	{
		if (_ptr)
		{
			_ptr->release();
			_ptr = nullptr;
		}
	}

	void reset(C* ptr)
	{
		assign(ptr);
	}

	void reset(C* ptr, bool shared)
	{
		assign(ptr, shared);
	}

	void reset(const AutoPtr& ptr)
	{
		assign(ptr);
	}

	template <class Other>
	void reset(const AutoPtr<Other>& ptr)
	{
		assign<Other>(ptr);
	}

	AutoPtr& operator = (C* ptr)
	{
		return assign(ptr);
	}

	AutoPtr& operator = (const AutoPtr& ptr)
	{
		return assign(ptr);
	}

	AutoPtr& operator = (AutoPtr&& ptr) noexcept
	{
		if (_ptr) _ptr->release();
		_ptr = ptr._ptr;
		ptr._ptr = nullptr;
		return *this;
	}

	template <class Other>
	AutoPtr& operator = (const AutoPtr<Other>& ptr)
	{
		return assign<Other>(ptr);
	}

	void swap(AutoPtr& ptr)
	{
		std::swap(_ptr, ptr._ptr);
	}

	template <class Other>
	AutoPtr<Other> cast() const
	{
		Other* pOther = dynamic_cast<Other*>(_ptr);
		return AutoPtr<Other>(pOther, true);
	}

	template <class Other>
	AutoPtr<Other> unsafeCast() const
	{
		Other* pOther = static_cast<Other*>(_ptr);
		return AutoPtr<Other>(pOther, true);
	}

	C* operator -> ()
	{
		if (_ptr)
			return _ptr;
		else
			throw NullPointerException();
	}

	const C* operator -> () const
	{
		if (_ptr)
			return _ptr;
		else
			throw NullPointerException();
	}

	C& operator * ()
	{
		if (_ptr)
			return *_ptr;
		else
			throw NullPointerException();
	}

	const C& operator * () const
	{
		if (_ptr)
			return *_ptr;
		else
			throw NullPointerException();
	}

	C* get()
	{
		return _ptr;
	}

	const C* get() const
	{
		return _ptr;
	}

	operator C* ()
	{
		return _ptr;
	}

	operator const C* () const
	{
		return _ptr;
	}

	bool operator ! () const
	{
		return _ptr == nullptr;
	}

	bool isNull() const
	{
		return _ptr == nullptr;
	}

	C* duplicate()
	{
		if (_ptr) _ptr->duplicate();
		return _ptr;
	}

	bool operator == (const AutoPtr& ptr) const
	{
		return _ptr == ptr._ptr;
	}

	bool operator == (const C* ptr) const
	{
		return _ptr == ptr;
	}

	bool operator == (C* ptr) const
	{
		return _ptr == ptr;
	}

	bool operator == (std::nullptr_t ptr) const
	{
		return _ptr == ptr;
	}

	bool operator != (const AutoPtr& ptr) const
	{
		return _ptr != ptr._ptr;
	}

	bool operator != (const C* ptr) const
	{
		return _ptr != ptr;
	}

	bool operator != (C* ptr) const
	{
		return _ptr != ptr;
	}

	bool operator != (std::nullptr_t ptr) const
	{
		return _ptr != ptr;
	}

	bool operator < (const AutoPtr& ptr) const
	{
		return _ptr < ptr._ptr;
	}

	bool operator < (const C* ptr) const
	{
		return _ptr < ptr;
	}

	bool operator < (C* ptr) const
	{
		return _ptr < ptr;
	}

	bool operator <= (const AutoPtr& ptr) const
	{
		return _ptr <= ptr._ptr;
	}

	bool operator <= (const C* ptr) const
	{
		return _ptr <= ptr;
	}

	bool operator <= (C* ptr) const
	{
		return _ptr <= ptr;
	}

	bool operator > (const AutoPtr& ptr) const
	{
		return _ptr > ptr._ptr;
	}

	bool operator > (const C* ptr) const
	{
		return _ptr > ptr;
	}

	bool operator > (C* ptr) const
	{
		return _ptr > ptr;
	}

	bool operator >= (const AutoPtr& ptr) const
	{
		return _ptr >= ptr._ptr;
	}

	bool operator >= (const C* ptr) const
	{
		return _ptr >= ptr;
	}

	bool operator >= (C* ptr) const
	{
		return _ptr >= ptr;
	}

private:
	C* _ptr;
};


template <class C>
inline void swap(AutoPtr<C>& p1, AutoPtr<C>& p2)
{
	p1.swap(p2);
}


template<typename T, typename... Args>
AutoPtr<T> makeAuto(Args&&... args)
{
	return AutoPtr<T>(new T(std::forward<Args>(args)...));
}
