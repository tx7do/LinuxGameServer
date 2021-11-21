#pragma once

#include "BaseException.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <atomic>


class ReferenceCounter
{
public:
	ReferenceCounter() : _cnt(1)
	{
	}

	void duplicate()
	{
		++_cnt;
	}

	int release()
	{
		return --_cnt;
	}

	int referenceCount() const
	{
		return _cnt.load();
	}

private:
	std::atomic_int _cnt;
};


template <class C>
class ReleasePolicy
{
public:
	static void release(C* pObj) noexcept
	{
		delete pObj;
	}
};


template <class C>
class ReleaseArrayPolicy
{
public:
	static void release(C* pObj) noexcept
	{
		delete[] pObj;
	}
};


template <class C, class RC = ReferenceCounter, class RP = ReleasePolicy<C>>
class SharedPtr
{
public:
	typedef C Type;

	SharedPtr() :
		_pCounter(nullptr),
		_ptr(nullptr)
	{
	}

	SharedPtr(C* ptr)
		try :
		_pCounter(ptr ? new RC : nullptr),
		_ptr(ptr)
	{
	}
	catch (...)
	{
		RP::release(ptr);
	}

	template <class Other, class OtherRP>
	SharedPtr(const SharedPtr<Other, RC, OtherRP>& ptr) :
		_pCounter(ptr._pCounter),
		_ptr(const_cast<Other*>(ptr.get()))
	{
		if (_pCounter) _pCounter->duplicate();
	}

	SharedPtr(const SharedPtr& ptr) :
		_pCounter(ptr._pCounter),
		_ptr(ptr._ptr)
	{
		if (_pCounter) _pCounter->duplicate();
	}

	SharedPtr(SharedPtr&& ptr) noexcept :
		_pCounter(ptr._pCounter),
		_ptr(ptr._ptr)
	{
		ptr._pCounter = nullptr;
		ptr._ptr = nullptr;
	}

	~SharedPtr()
	{
		release();
	}

	SharedPtr& assign(C* ptr)
	{
		if (get() != ptr)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	SharedPtr& assign(const SharedPtr& ptr)
	{
		if (&ptr != this)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	template <class Other, class OtherRP>
	SharedPtr& assign(const SharedPtr<Other, RC, OtherRP>& ptr)
	{
		if (ptr.get() != _ptr)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	void reset()
	{
		assign(nullptr);
	}

	void reset(C* ptr)
	{
		assign(ptr);
	}

	void reset(const SharedPtr& ptr)
	{
		assign(ptr);
	}

	template <class Other, class OtherRP>
	void reset(const SharedPtr<Other, RC, OtherRP>& ptr)
	{
		assign<Other, OtherRP>(ptr);
	}

	SharedPtr& operator = (C* ptr)
	{
		return assign(ptr);
	}

	SharedPtr& operator = (const SharedPtr& ptr)
	{
		return assign(ptr);
	}

	SharedPtr& operator = (SharedPtr&& ptr) noexcept
	{
		release();
		_ptr = ptr._ptr;
		ptr._ptr = nullptr;
		_pCounter = ptr._pCounter;
		ptr._pCounter = nullptr;
		return *this;
	}

	template <class Other, class OtherRP>
	SharedPtr& operator = (const SharedPtr<Other, RC, OtherRP>& ptr)
	{
		return assign<Other>(ptr);
	}

	void swap(SharedPtr& ptr)
	{
		std::swap(_ptr, ptr._ptr);
		std::swap(_pCounter, ptr._pCounter);
	}

	template <class Other>
	SharedPtr<Other, RC, RP> cast() const
	{
		Other* pOther = dynamic_cast<Other*>(_ptr);
		if (pOther)
			return SharedPtr<Other, RC, RP>(_pCounter, pOther);
		return SharedPtr<Other, RC, RP>();
	}

	template <class Other>
	SharedPtr<Other, RC, RP> unsafeCast() const
	{
		Other* pOther = static_cast<Other*>(_ptr);
		return SharedPtr<Other, RC, RP>(_pCounter, pOther);
	}

	C* operator -> ()
	{
		return deref();
	}

	const C* operator -> () const
	{
		return deref();
	}

	C& operator * ()
	{
		return *deref();
	}

	const C& operator * () const
	{
		return *deref();
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

	bool operator == (const SharedPtr& ptr) const
	{
		return get() == ptr.get();
	}

	bool operator == (const C* ptr) const
	{
		return get() == ptr;
	}

	bool operator == (C* ptr) const
	{
		return get() == ptr;
	}

	bool operator == (std::nullptr_t ptr) const
	{
		return get() == ptr;
	}

	bool operator != (const SharedPtr& ptr) const
	{
		return get() != ptr.get();
	}

	bool operator != (const C* ptr) const
	{
		return get() != ptr;
	}

	bool operator != (C* ptr) const
	{
		return get() != ptr;
	}

	bool operator != (std::nullptr_t ptr) const
	{
		return get() != ptr;
	}

	bool operator < (const SharedPtr& ptr) const
	{
		return get() < ptr.get();
	}

	bool operator < (const C* ptr) const
	{
		return get() < ptr;
	}

	bool operator < (C* ptr) const
	{
		return get() < ptr;
	}

	bool operator <= (const SharedPtr& ptr) const
	{
		return get() <= ptr.get();
	}

	bool operator <= (const C* ptr) const
	{
		return get() <= ptr;
	}

	bool operator <= (C* ptr) const
	{
		return get() <= ptr;
	}

	bool operator > (const SharedPtr& ptr) const
	{
		return get() > ptr.get();
	}

	bool operator > (const C* ptr) const
	{
		return get() > ptr;
	}

	bool operator > (C* ptr) const
	{
		return get() > ptr;
	}

	bool operator >= (const SharedPtr& ptr) const
	{
		return get() >= ptr.get();
	}

	bool operator >= (const C* ptr) const
	{
		return get() >= ptr;
	}

	bool operator >= (C* ptr) const
	{
		return get() >= ptr;
	}

	int referenceCount() const
	{
		return _pCounter ? _pCounter->referenceCount() : 0;
	}

private:
	C* deref() const
	{
		if (!_ptr)
			throw NullPointerException();

		return _ptr;
	}

	void release() noexcept
	{
		if (_pCounter && _pCounter->release() == 0)
		{
			RP::release(_ptr);
			_ptr = nullptr;

			delete _pCounter;
			_pCounter = nullptr;
		}
	}

	SharedPtr(RC* pCounter, C* ptr) : _pCounter(pCounter), _ptr(ptr)
	{
		ASSERT_DBG(_pCounter);
		_pCounter->duplicate();
	}

private:
	RC* _pCounter;
	C* _ptr;

	template <class OtherC, class OtherRC, class OtherRP> friend class SharedPtr;
};


template <class C, class RC, class RP>
inline void swap(SharedPtr<C, RC, RP>& p1, SharedPtr<C, RC, RP>& p2)
{
	p1.swap(p2);
}


template <typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args)
{
	return SharedPtr<T>(new T(std::forward<Args>(args)...));
}


template <typename T>
SharedPtr<T, ReferenceCounter, ReleaseArrayPolicy<T>> makeSharedArray(std::size_t size)
{
	return SharedPtr<T, ReferenceCounter, ReleaseArrayPolicy<T>>(new T[size]);
}
