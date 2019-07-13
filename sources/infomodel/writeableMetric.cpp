#ifndef WRITEABLEMETRIC
#define WRITEABLEMETRIC

#include "deviceMetric.hpp"
#include "informationModelExceptions.hpp"
#include <string>

using namespace std;

WriteableMetric::WriteableMetric(string refId, string name, string desc,
                                 ValueType::ValueDataType valueType)
    : DeviceMetric(refId, name, desc, valueType) {
  this->elementType = ElementType::Writable;
}

#endif // !WRITEABLEMETRIC