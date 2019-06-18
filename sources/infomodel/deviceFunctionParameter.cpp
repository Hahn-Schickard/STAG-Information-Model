#include "deviceFunctionParameter.hpp"

FunctionParameter::FunctionParameter(string refId, string name, string desc, ValueType::ValueDataType valueType) : NamedElement(refId, name, desc) {
	this->isOptionalFlag = false;
	this->dataType = valueType;
}

FunctionParameter::FunctionParameter(string refId, string name, string desc, ValueType::ValueDataType valueType, bool isOptional) : NamedElement(refId, name, desc) {
	this->isOptionalFlag = isOptional;
	this->dataType = valueType;
}

bool FunctionParameter::isOptional()
{
	return this->isOptionalFlag;
}

ValueType::ValueDataType FunctionParameter::getDataType()
{
	return this->dataType;
}