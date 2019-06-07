#ifndef VALUEDATATYPE
#define VALUEDATATYPE
#include <string>
#include <vector>

using namespace std;

static vector<std::string> ValueTypes = {
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
		Integer,
		String,
		Boolean,
		Float
	};	



	static string toString(ValueDataType dataType)
	{
		return ValueTypes[dataType];
	}
};

#endif // !VALUEDATATYPE


