#ifndef METRIC_H
#define METRIC_H
#include "deviceElement.h"
#include "valueDataType.h"

class DeviceMetric : public DeviceElement
{
public:
	
private:
	ValueType::ValueDataType valueType;

public:
	DeviceMetric(string refId, string name, string desc, ValueType::ValueDataType valueType);
	ValueType::ValueDataType getMetricValueType();
};


class WriteableMetric : public DeviceMetric
{
public:
	WriteableMetric(string refId, string name, string desc, ValueType::ValueDataType valueType);
	void registerMetric();
};

class ObservableMetric : public DeviceMetric
{
private:
	string registration_id;
	static int incrementId;
public:
	
	ObservableMetric(string refId, string name, string desc, ValueType::ValueDataType valueType);

	void registerMetric();

	string getRegistrationId();


};

#endif