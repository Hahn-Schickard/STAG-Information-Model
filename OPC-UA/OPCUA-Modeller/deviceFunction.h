#ifndef FUNCTION_H
#define FUNCTION_H
#include "deviceElement.h"
#include "deviceFunctionParameter.h"



class DeviceFunction : public DeviceElement
{
public:

private:
	vector<FunctionParameter> parameters;

public:
	DeviceFunction(string refId, string name, string desc);
	
	vector<FunctionParameter> getParameterList();
	void addParameter(const FunctionParameter &functionParameter);
};

#endif