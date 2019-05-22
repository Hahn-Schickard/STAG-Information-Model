#include "InformationModelTest.h"
#include <stdlib.h>
#include <vector>


InformationModelTest::InformationModelTest() {};

void InformationModelTest::CreateInformationModel(BluePrint recipee) {
		device = new Device(recipee.RefId, recipee.Name, recipee.Desc);
		DeviceElementGroup g = DeviceElementGroup(recipee.RefId + ".1", recipee.Name + " group", "Group Element of " + recipee.Name);

		DeviceMetric* temperature = new DeviceMetric(recipee.RefId + ".1.1", "Temperature", "Temperature Metric Element with ValueType as Float", ValueType::ValueDataType::Float);
		g.addElement(*temperature);

		ObservableMetric* airflow = new ObservableMetric(recipee.RefId + ".1.2", "Air Flow", "Airflow Metric Element with ValueType as Int", ValueType::Integer);
		g.addElement(*airflow);

		DeviceFunction* deviceFunction = new DeviceFunction(recipee.RefId + ".1.3", "GetTemperature", "Returns the Temperature Value");
		FunctionParameter* functionParameter = new FunctionParameter(recipee.RefId + ".1.3.1", "SensorID", "This is a sensor", ValueType::String);

		deviceFunction->addParameter(*functionParameter);
		g.addElement(*deviceFunction);

		device->addDeviceElementGroup(g);

		
	}

void InformationModelTest::CreateElementGroup_ReferenceIdIsEmpty_ThrowsInvalidReferenceIdException()
{
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << "Empty RefrenceId should throw an InvalidReferenceIdException" << std::endl;
	try {
		DeviceElementGroup g = DeviceElementGroup("", "", "");
		std::cout << "FAIL" << std::endl;
	}
	catch (const InvalidReferenceIdException& e)
	{
		std::cout << "PASS" << std::endl;
	}
	std::cout << "-------------------------------------------------------------------" << std::endl;
}

bool RaisesException()
{
	DeviceElementGroup g = DeviceElementGroup("22", "", "");
	return false;
}

void InformationModelTest::CreateElementGroup_ReferenceIdLessThenThreeCharacters_ThrowsInvalidReferenceIdException()
{
	/*std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << "Null RefrenceId should throw a ReferenceId Exception when length is less than three characters: " << std::endl;
	try {
		DeviceElementGroup g = DeviceElementGroup("22", "", "");
		std::cout << "FAIL" << std::endl;
	}
	catch (const InvalidReferenceIdException& e)
	{
		std::cout << "PASS" << std::endl;
	}
	std::cout << "-------------------------------------------------------------------" << std::endl;*/

	this->RunExceptionRaisedTest("Null RefrenceId should throw a ReferenceId Exception when length is less than three characters: ", RaisesException);
}



bool TestDeviceElementGroup()
{
	try {
		DeviceElementGroup g = DeviceElementGroup("222", "", "");
		return true;
	}
	catch(exception)
	{
		return false;
	}
	
}

void InformationModelTest::CreateElementGroup_ReferenceIdIsValid_NoExceptionThrown()
{
	RunBooleanTest("Valid RefrenceId should throw no exception", &TestDeviceElementGroup);
}


void InformationModelTest::RunNoExceptionRaisedTest(string msg, bool test())
{
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << msg << std::endl;

	try {
		test();
		std::cout << "PASS" << std::endl;
	}
	catch (exception)
	{
		std::cout << "FAIL" << std::endl;
	}
	
	std::cout << "-------------------------------------------------------------------" << std::endl;
}

void InformationModelTest::RunExceptionRaisedTest(string msg, bool test())
{
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << msg << std::endl;

	try {
		test();
		std::cout << "FAIL" << std::endl;
	}
	catch (exception)
	{
		std::cout << "PASS" << std::endl;
	}

	std::cout << "-------------------------------------------------------------------" << std::endl;
}

void InformationModelTest::RunBooleanTest(string msg, bool test())
{
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << msg << std::endl;

	if (test())
	{
		std::cout << "PASS" << std::endl;
	}
	else
	{
		std::cout << "FAIL" << std::endl;
	}
	
	std::cout << "-------------------------------------------------------------------" << std::endl;
}

void InformationModelTest::CreateDevice_ReferenceIdIsEmpty_ThrowsInvalidReferenceIdException()
{
	device = new Device("", "", "");
}


void InformationModelTest::RunTests()
	{

		std::cout << "=========================" << std::endl;
		std::cout << ">> Device Properites" << std::endl;
		std::cout << device->getReferenceId() << std::endl;
		std::cout << device->getElementName() << std::endl;
		std::cout << device->getElementDescription() << std::endl;

		std::cout << ">> DeviceElementGroup Properties" << std::endl;
		DeviceElementGroup devElGr = device->GetDeviceElementGroup();
		std::cout << devElGr.getReferenceId() << std::endl;
		std::cout << devElGr.getElementName() << std::endl;
		std::cout << devElGr.getElementDescription() << std::endl;

		std::cout << ">> DeviceElementGroup SubElements" << std::endl;
		vector<DeviceElement*> subElements = devElGr.getSubElements();

		std::cout << "Number of Subelements: " << subElements.size() << std::endl;
		for (size_t i = 0; i < subElements.size(); i++)
		{
			WriteDescriptors(subElements[i]);

			string className = typeid(subElements[i]).name();
			const std::type_info& info = typeid(subElements[i]);
			std::cout << "Subelement is of type " << className << std::endl;
			if (subElements[i]->getElementType() == ElementType::Observable)
			{
				string registrationId = ((ObservableMetric *)subElements[i])->getReferenceId();
				std::cout << "ObservableMetric RegistrationId: " << registrationId << std::endl;
			}
			else if (subElements[i]->getElementType() == ElementType::Writable)
			{
				std::cout << "WriteableMetric" << std::endl;
			}
			else if (subElements[i]->getElementType() == ElementType::Readonly)
			{
				std::cout << "ReadonlyMetric" << std::endl;
			}
			else if (subElements[i]->getElementType() == ElementType::Function)
			{
				std::cout << "Function" << std::endl;
				vector<FunctionParameter> functionParameters = ((DeviceFunction*)subElements[i])->getParameterList();

				std::cout << "Funktion hat einen Parameter: ";
				if (functionParameters.size() <= 0)
					std::cout << "FAIL" << std::endl;
				else
					std::cout << "PASS" << std::endl;


				for (size_t i = 0; i < functionParameters.size(); i++)
				{
				std:cout << "Parameter Name is 'SensorID': ";

					if ("SensorID" == functionParameters[i].getElementName())
						std::cout << "PASS" << std::endl;
					else
						std::cout << "FAIL" << std::endl;

					std::cout << "Parameter Description is 'This is a sensor': ";

					if ("This is a sensor" == functionParameters[i].getElementDescription())
						std::cout << "PASS" << std::endl;
					else
						std::cout << "FAIL" << std::endl;
				}
			}
			std::cout << "-----------------------" << std::endl;
		}

		DeviceMetric * metric = (DeviceMetric *)subElements[0];

		ValueType::ValueDataType valueType = metric->getMetricValueType();

		//std::cout << "Temperature Value Type: " << ValueType::toString(valueType) << std::endl;


		std::cout << "=========================" << std::endl;

		CreateElementGroup_ReferenceIdIsEmpty_ThrowsInvalidReferenceIdException();
		CreateElementGroup_ReferenceIdLessThenThreeCharacters_ThrowsInvalidReferenceIdException();
		CreateElementGroup_ReferenceIdIsValid_NoExceptionThrown();
	}

	void InformationModelTest::WriteDescriptors(NamedElement * element)
	{
		std::cout << element->getReferenceId() << std::endl;
		std::cout << element->getElementName() << std::endl;
		std::cout << element->getElementDescription() << std::endl;
	}
	

