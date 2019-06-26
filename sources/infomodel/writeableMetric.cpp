#ifndef WRITEABLEMETRIC
#define WRITEABLEMETRIC

#include <string>
#include "deviceMetric.hpp"
#include "informationModelExceptions.hpp"

using namespace std;



WriteableMetric::WriteableMetric(string refId, string name, string desc, ValueType::ValueDataType valueType) : DeviceMetric(refId, name, desc, valueType) {
	this->elementType = ElementType::Writable;
}

/* void WriteableMetric::updateSensor(std::string updateValues)
{
	if (getElementType() != ElementType::Writable)
	{
		throw ElementTypeMismatchException("This Element is not Writeable");
	}
}*/

void WriteableMetric::CheckIfWriteableMetric(std::string updateValues)
{
	if (getElementType() != ElementType::Writable)
	{
		throw ElementTypeMismatchException("This Element is not Writeable");
	}
}


#endif // !WRITEABLEMETRIC
