#include "deviceMetric.hpp"

using namespace std;

DeviceMetric::DeviceMetric(string refId, string name, string desc, ValueType::ValueDataType valueType) : DeviceElement(refId, name, desc)
{
	this->valueType = valueType;
	this->elementType = ElementType::Readonly;
}

ValueType::ValueDataType DeviceMetric::getMetricValueType() {
	return valueType;

}