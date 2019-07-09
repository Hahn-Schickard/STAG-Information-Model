#ifndef VALUEDATATYPE
#define VALUEDATATYPE
#include <string>
#include <vector>

static std::vector<std::string> ValueTypes = {
			"Integer",
			"String",
			"Boolean",
			"Float"
};

class ValueWrapper
{
	ValueType type;
	union value {
		int intValue;
		float floatValue;
		std::string stringValue;
	};
};

class ValueType
{
public:
	enum ValueDataType
	{
		Unknown,
		Integer,
		String,
		Boolean,
		Float
	};	



	static std::string toString(ValueDataType dataType)
	{
		return ValueTypes[dataType];
	}
};

#endif // !VALUEDATATYPE


