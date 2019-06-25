#include "deviceMetric.hpp"
#include "informationModelExceptions.hpp"


WriteableMetric::WriteableMetric(string refId, string name, string desc, ValueType::ValueDataType valueType) : DeviceMetric(refId, name, desc, valueType) {
	this->elementType = ElementType::Writable;
}

void WriteableMetric::updateSensor(string updateValues)
{
	if (getElementType() != ElementType::Writable)
	{
		throw ElementTypeMismatchException("This Element is not Writeable");
	}
}
