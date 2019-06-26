#include "deviceMetric.hpp"

using namespace std;


ObservableMetric::ObservableMetric(string refId, string name, string desc, ValueType::ValueDataType valueType) : DeviceMetric(refId, name, desc, valueType) {

	elementType = ElementType::Observable;
}


int ObservableMetric::incrementId = 0;

void ObservableMetric::registerMetric()
{
	ObservableMetric::incrementId++;
	//char buffer;
	//sprintf_s(&buffer, 200, "%s_%s.%d", RefId.c_str(), this->getElementName().c_str(), incrementId);
	string buffer;
    buffer = RefId + this->getElementName() + to_string(incrementId);

	//registration_id = (string)&buffer;
	registration_id = buffer;
}

string ObservableMetric::getRegistrationId()
{
	return this->registration_id;
}