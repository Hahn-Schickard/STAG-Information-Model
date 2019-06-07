#ifndef FUNCTION_H
#define FUNCTION_H
#include "deviceElement.hpp"
#include "deviceFunctionParameter.hpp"



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