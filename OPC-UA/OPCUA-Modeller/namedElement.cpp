#include "namedElement.h"
#include "informationModelExceptions.h"

NamedElement::NamedElement(string refId, string name, string desc)
{
	if (refId.empty() || refId.length() < 3)
	{
		throw InvalidReferenceIdException();
	}
	this->Name = name;
	this->Desc = desc;
	this->RefId = refId;
}

NamedElement::~NamedElement() {};