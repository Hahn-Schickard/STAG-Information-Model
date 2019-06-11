#ifndef NAMEDELEMENT_H
#define NAMEDELEMENT_H


#include <string>
#include <vector>

using namespace std;

class NamedElement
{
protected:
	string Name = "";
	string Desc = "";
	string RefId = "";

public:
	string getElementDescription() { return this->Desc; }
	string getElementName() { return this->Name; }
	const string getReferenceId() { return this->RefId; }



	NamedElement(string refId, string name, string desc);

	bool ContainsNonNumericCharacters(string refId);
	
};

#endif
