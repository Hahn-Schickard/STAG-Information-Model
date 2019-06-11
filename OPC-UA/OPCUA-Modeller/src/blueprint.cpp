#include "../includes/blueprint.hpp"

Device * BluePrint::GetDevice(std::string refId) 
{
	return this->device;
}

Device * BluePrint::GetDevice()
{
	return this->device;
}

BluePrint::BluePrint(std::string refId, std::string name, std::string desc)
{
	this->device = new Device(refId, name, desc);
}
