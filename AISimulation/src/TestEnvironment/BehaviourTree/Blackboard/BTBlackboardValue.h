#ifndef BT_BLACKBOARD_VALUE_H
#define BT_BLACKBOARD_VALUE_H

#include "BTBlackboardOperator.h"

// BTBlackboardValue template

template<class T>
class BTBlackboardValue_
{
	T value;

public:

	BTBlackboardValue_(const T& v) : value(v) {}

	const T& Get() const { return value; }
	void Set(const T& v) { value = v; }
};

// Specific blackboard values

class BTBlackboardValueInt : public BTBlackboardValue_<int>
{
public:
	BTBlackboardValueInt() :BTBlackboardValue_(-1) {}
	BTBlackboardValueInt(int v) : BTBlackboardValue_(v) {}
};

class BTBlackboardValueFloat : public BTBlackboardValue_<float>
{
public:
	BTBlackboardValueFloat() : BTBlackboardValue_(-1.0f) {}
	BTBlackboardValueFloat(float v) : BTBlackboardValue_(v) {}
};

class BTBlackboardValueBool : public BTBlackboardValue_<bool>
{
public:
	BTBlackboardValueBool() : BTBlackboardValue_(false) {}
	BTBlackboardValueBool(bool v) : BTBlackboardValue_(v) {}
};

class BTBlackboardValueString : public BTBlackboardValue_<std::string>
{
public:
	BTBlackboardValueString() : BTBlackboardValue_("") {}
	BTBlackboardValueString(std::string v) : BTBlackboardValue_(v) {}
};

// BlackboardValue inherits from all the specialised values
class BTBlackboardValue
	: private BTBlackboardValueInt
	, private BTBlackboardValueFloat
	, private BTBlackboardValueBool
	, private BTBlackboardValueString
{

public:

	enum class Type
	{
		UNDEFINED,

		INT,
		FLOAT,
		BOOL,
		STRING
	};

private:

	Type type{ Type::UNDEFINED };

public:

	BTBlackboardValue() = default;

	BTBlackboardValue::Type GetType() const { return type; }

	// Setters
	void Set(int v)
	{
		BTBlackboardValueInt::Set(v);
		type = Type::INT;
	}

	void Set(float v)
	{
		BTBlackboardValueFloat::Set(v);
		type = Type::FLOAT;
	}

	void Set(bool v)
	{
		BTBlackboardValueBool::Set(v);
		type = Type::BOOL;
	}

	void Set(std::string v)
	{
		BTBlackboardValueString::Set(v);
		type = Type::STRING;
	}

	// Is satisfied
	bool isSatisfied(BTBlackboardOperator btOperator, const BTBlackboardValue& other) const
	{
		assert(type == other.type);
		if (type == other.type)
		{
			switch (type)
			{
			case BTBlackboardValue::Type::INT: return isSatisfied(btOperator, GetInt(), other.GetInt());
			case BTBlackboardValue::Type::FLOAT: return isSatisfied(btOperator, GetFloat(), other.GetFloat());
			case BTBlackboardValue::Type::BOOL: return isSatisfied(btOperator, GetBool(), other.GetBool());
			case BTBlackboardValue::Type::STRING: return isSatisfied(btOperator, GetString(), other.GetString());
			}
		}

		return false;
	}

private:

	// Getters
	int GetInt() const { assert(type == Type::INT); return BTBlackboardValueInt::Get(); }
	float GetFloat() const { assert(type == Type::FLOAT); return BTBlackboardValueFloat::Get(); }
	bool GetBool() const { assert(type == Type::BOOL); return BTBlackboardValueBool::Get(); }
	std::string GetString() const { assert(type == Type::STRING); return BTBlackboardValueString::Get(); }

	// Is satisified
	template<typename T>
	bool isSatisfied(BTBlackboardOperator btOperator, const T& value1, const T& value2) const
	{
		switch (btOperator)
		{
		case BTBlackboardOperator::IS_EQUAL: return value1 == value2;
		case BTBlackboardOperator::IS_NOT_EQUAL: return value1 != value2;
		case BTBlackboardOperator::IS_GREATER: return value1 > value2;
		case BTBlackboardOperator::IS_GREATER_OR_EQUAL: return value1 >= value2;
		case BTBlackboardOperator::IS_SMALLER: return value1 < value2;
		case BTBlackboardOperator::IS_SMALLER_OR_EQUAL: return value1 <= value2;
		}

		return false;
	}
};

#endif // !BT_BLACKBOARD_VALUE_H

