#include "deviceMetric.h"
#include "informationModelExceptions.h"


WriteableMetric::WriteableMetric(string refId, string name, string desc, ValueType::ValueDataType valueType) : DeviceMetric(refId, name, desc, valueType) {
	this->elementType = ElementType::Writable;
}

void WriteableMetric::updateSensor(string updateValues)
{
	if (this->getElementType() != ElementType::Writable)
	{
		throw new ElementTypeMismatchException("This Element is not Writeable");
	}
}
