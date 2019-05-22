#ifndef DEVICEELEMENT_H
#define DEVICEELEMENT_H

#include "namedElement.h"

static enum ElementType {
	Undefined,
	Group,
	Readonly,
	Observable,
	Writable,
	Function
};

class DeviceElement : public NamedElement
{

public:
	

public:
	ElementType elementType = Undefined;


public:	
	
	DeviceElement(string refId, string name, string desc) ;

	ElementType getElementType();
};


#endif // !DEVICEELEMENT_H
