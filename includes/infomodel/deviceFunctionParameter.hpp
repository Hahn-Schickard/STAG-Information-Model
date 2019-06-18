#ifndef FUNCTIONPARAMETER
#define FUNCTIONPARAMETER

#include "valueDataType.hpp"
#include "namedElement.hpp"



class FunctionParameter : public NamedElement
{
private:
	bool isOptionalFlag;
	ValueType::ValueDataType dataType;

public:
	FunctionParameter(string refId, string name, string desc, ValueType::ValueDataType valueType);

	FunctionParameter(string refId, string name, string desc, ValueType::ValueDataType valueType, bool isOptional);

	bool isOptional();

	ValueType::ValueDataType getDataType();
};
#endif // !FUNCTIONPARAMETER
