#include "deviceFunction.hpp"

using namespace std;

DeviceFunction::DeviceFunction(string refId, string name, string desc) : DeviceElement(refId, name, desc) {
	this->elementType = ElementType::Function;
}

vector<FunctionParameter> DeviceFunction::getParameterList() {
	return parameters;
}

void DeviceFunction::addParameter(const FunctionParameter &functionParameter) {
	parameters.push_back(functionParameter);
}
