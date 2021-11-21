#pragma once

#include <algorithm>
#include <typeinfo>
#include <cstring>
#include <string>
#include "MetaProgramming.h"
#include "BaseException.h"

class Any;


namespace Dynamic {

	class Var;
	class VarHolder;
	template <class T> class VarHolderImpl;

}


template <typename PlaceholderT>
union Placeholder
{
public:
	Placeholder()
	{
	}

	PlaceholderT* content() const
	{
		return pHolder;
	}

private:

	PlaceholderT* pHolder;

	friend class Any;
	friend class Dynamic::Var;
	friend class Dynamic::VarHolder;
	template <class> friend class Dynamic::VarHolderImpl;
};


class Any
{
public:
	Any() : _pHolder(0)
	{
	}

	template <typename ValueType>
	Any(const ValueType& value) :
		_pHolder(new Holder<ValueType>(value))
	{
	}

	Any(const Any& other) :
		_pHolder(other._pHolder ? other._pHolder->clone() : 0)
	{
	}

	~Any()
	{
		delete _pHolder;
	}

	Any& swap(Any& rhs)
	{
		std::swap(_pHolder, rhs._pHolder);
		return *this;
	}

	template <typename ValueType>
	Any& operator = (const ValueType& rhs)
	{
		Any(rhs).swap(*this);
		return *this;
	}

	Any& operator = (const Any& rhs)
	{
		Any(rhs).swap(*this);
		return *this;
	}

	bool empty() const
	{
		return !_pHolder;
	}

	const std::type_info& type() const
	{
		return _pHolder ? _pHolder->type() : typeid(void);
	}

private:
	class ValueHolder
	{
	public:
		virtual ~ValueHolder() = default;

		virtual const std::type_info& type() const = 0;
		virtual ValueHolder* clone() const = 0;
	};

	template <typename ValueType>
	class Holder : public ValueHolder
	{
	public:
		Holder(const ValueType& value) :
			_held(value)
		{
		}

		virtual const std::type_info& type() const
		{
			return typeid(ValueType);
		}

		virtual ValueHolder* clone() const
		{
			return new Holder(_held);
		}

		ValueType _held;

	private:
		Holder& operator = (const Holder&);
	};

	ValueHolder* content() const
	{
		return _pHolder;
	}

private:
	ValueHolder* _pHolder;

	template <typename ValueType>
	friend ValueType* AnyCast(Any*);

	template <typename ValueType>
	friend ValueType* UnsafeAnyCast(Any*);

	template <typename ValueType>
	friend const ValueType& RefAnyCast(const Any&);

	template <typename ValueType>
	friend ValueType& RefAnyCast(Any&);

	template <typename ValueType>
	friend ValueType AnyCast(Any&);
};


template <typename ValueType>
ValueType* AnyCast(Any* operand)
{
	return operand && operand->type() == typeid(ValueType)
		? &static_cast<Any::Holder<ValueType>*>(operand->content())->_held
		: 0;
}


template <typename ValueType>
const ValueType* AnyCast(const Any* operand)
{
	return AnyCast<ValueType>(const_cast<Any*>(operand));
}


template <typename ValueType>
ValueType AnyCast(Any& operand)
{
	typedef typename TypeWrapper<ValueType>::TYPE NonRef;

	NonRef* result = AnyCast<NonRef>(&operand);
	if (!result)
	{
		std::string s = "RefAnyCast: Failed to convert between Any types ";
		if (operand._pHolder)
		{
			s.append(1, '(');
			s.append(operand._pHolder->type().name());
			s.append(" => ");
			s.append(typeid(ValueType).name());
			s.append(1, ')');
		}
		throw BadCastException(s);
	}
	return *result;
}


template <typename ValueType>
ValueType AnyCast(const Any& operand)
{
	typedef typename TypeWrapper<ValueType>::TYPE NonRef;

	return AnyCast<NonRef&>(const_cast<Any&>(operand));
}


template <typename ValueType>
const ValueType& RefAnyCast(const Any& operand)
{
	ValueType* result = AnyCast<ValueType>(const_cast<Any*>(&operand));
	if (!result)
	{
		std::string s = "RefAnyCast: Failed to convert between Any types ";
		if (operand._pHolder)
		{
			s.append(1, '(');
			s.append(operand._pHolder->type().name());
			s.append(" => ");
			s.append(typeid(ValueType).name());
			s.append(1, ')');
		}
		throw BadCastException(s);
	}
	return *result;
}


template <typename ValueType>
ValueType& RefAnyCast(Any& operand)
{
	ValueType* result = AnyCast<ValueType>(&operand);
	if (!result)
	{
		std::string s = "RefAnyCast: Failed to convert between Any types ";
		if (operand._pHolder)
		{
			s.append(1, '(');
			s.append(operand._pHolder->type().name());
			s.append(" => ");
			s.append(typeid(ValueType).name());
			s.append(1, ')');
		}
		throw BadCastException(s);
	}
	return *result;
}


template <typename ValueType>
ValueType* UnsafeAnyCast(Any* operand)
{
	return &static_cast<Any::Holder<ValueType>*>(operand->content())->_held;
}


template <typename ValueType>
const ValueType* UnsafeAnyCast(const Any* operand)
{
	return AnyCast<ValueType>(const_cast<Any*>(operand));
}
