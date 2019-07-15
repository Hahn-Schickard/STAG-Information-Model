#include "NamedElement.hpp"
#include "InformationModelExceptions.hpp"

using namespace std;

NamedElement::NamedElement(string refId, string name, string desc)
{
	if (refId.empty() || refId.length() < 3)
	{
		throw InvalidReferenceIdException();
	}
	this->name = name;
	this->desc = desc;
	this->refId = refId;
}

const string
NamedElement::getElementDescription()
{
	return desc;
}

const string
NamedElement::getElementName()
{
	return name;
}

const string
NamedElement::getElementRefId()
{
	return refId;
}