#include "namedElement.hpp"
#include "informationModelExceptions.hpp"

NamedElement::NamedElement(string refId, string name, string desc)
{
	if (refId.empty() || refId.length() < 3 || ContainsNonNumericCharacters(refId))
	{
		throw InvalidReferenceIdException();
	}
	this->Name = name;
	this->Desc = desc;
	this->RefId = refId;
}

bool NamedElement::ContainsNonNumericCharacters(string refId)
{	
	return refId.find_first_not_of("0123456789.") != std::string::npos;	
}

