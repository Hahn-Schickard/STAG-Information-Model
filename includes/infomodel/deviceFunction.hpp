#ifndef FUNCTION_H
#define FUNCTION_H
#include "deviceElement.hpp"
#include "deviceFunctionParameter.hpp"



class DeviceFunction : public DeviceElement
{
private:
	std::vector<FunctionParameter> parameters;

public:
	DeviceFunction(std::string refId, std::string name, std::string desc);
	
	std::vector<FunctionParameter> getParameterList();
	void addParameter(const FunctionParameter &functionParameter);
	
	



};

#endif