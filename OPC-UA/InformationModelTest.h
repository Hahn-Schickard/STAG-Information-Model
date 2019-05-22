#ifndef INFORMATIONMODEL_H
#define INFORMATIONMODEL_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include "OPCUA-Modeller\opcua_modeller.hpp"

using namespace std;

class InformationModelTest
{
private:
	Device * device;
public:
	InformationModelTest();
	void CreateInformationModel(BluePrint recipee);
	void RunTests();
	void WriteDescriptors(NamedElement * element);

	//Unit Tests
	bool CreateDevice_ReferenceIdIsEmpty_ThrowsInvalidReferenceIdException();
	void CreateElementGroup_ReferenceIdLessThenThreeCharacters_ThrowsInvalidReferenceIdException();
	void CreateElementGroup_ReferenceIdIsEmpty_ThrowsInvalidReferenceIdException();
	void CreateElementGroup_ReferenceIdIsValid_NoExceptionThrown();

	void RunBooleanTest(string msg, bool test());
	void RunNoExceptionRaisedTest(string msg, bool test());
	void RunExceptionRaisedTest(string msg, bool test());

};

#endif  // !INFORMATIONMODEL_H