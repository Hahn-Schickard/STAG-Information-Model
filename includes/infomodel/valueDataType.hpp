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


