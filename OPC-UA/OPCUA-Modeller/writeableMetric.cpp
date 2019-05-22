#include "deviceMetric.h"


WriteableMetric::WriteableMetric(string refId, string name, string desc, ValueType::ValueDataType valueType) : DeviceMetric(refId, name, desc, valueType) {
	this->elementType = ElementType::Writable;
}

void WriteableMetric::registerMetric()
{

}
