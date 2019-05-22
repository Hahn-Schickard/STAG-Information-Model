#include "pch.h"
#include "device.h"
#include "deviceElementGroup.h"
#include "deviceMetric.h"
#include "deviceFunction.h"
#include "informationModelExceptions.h"


TEST(Device, CreatingDevice) {
	//Arrange and Act
	Device * device = new Device("1234", "Temperatur", "Sensor vom Typ Temperatur");
	
	string refId = device->getReferenceId();
	string name = device->getElementName();
	string desc = device->getElementDescription();

	//Assert
	EXPECT_EQ("1234", refId);
	EXPECT_EQ("Temperatur", name);
	EXPECT_EQ("Sensor vom Typ Temperatur", desc);
}

TEST(Device, AddingDeviceElementGroup) {
	//Arrange
	Device * device = new Device("1234", "Temperatur", "Sensor vom Typ Temperatur");
	DeviceElementGroup g = DeviceElementGroup(device->getReferenceId() + ".1", device->getElementName() + " group", "Group Element of " + device->getElementName());
	
	//Act
	device->addDeviceElementGroup(g);

	//Assert
	DeviceElementGroup devElGr = device->GetDeviceElementGroup();
	EXPECT_EQ("1234.1", devElGr.getReferenceId());
	EXPECT_EQ("Temperatur group", devElGr.getElementName());
	EXPECT_EQ("Group Element of Temperatur", devElGr.getElementDescription());
}

TEST(Device, AddingSubElementToGroup) {
	//Arrange
	Device * device = new Device("1234", "Temperatur", "Sensor vom Typ Temperatur");
	DeviceElementGroup g = DeviceElementGroup(device->getReferenceId() + ".1", device->getElementName() + " group", "Group Element of " + device->getElementName());

    //Act
	DeviceMetric* temperature = new DeviceMetric(device->getReferenceId() + ".1.1", "Temperature", "Temperature Metric Element with ValueType as Float", ValueType::ValueDataType::Float);
	g.addElement(*temperature);
	device->addDeviceElementGroup(g);

	//Assert
	DeviceElementGroup g1 = device->GetDeviceElementGroup();
	int numOfSubElements = g1.getSubElements().size();
	EXPECT_EQ(1, numOfSubElements);

	DeviceMetric * expectedDeviceElement = (DeviceMetric*)g1.getSubElements()[0];
	EXPECT_EQ("1234.1.1", expectedDeviceElement->getReferenceId());
	EXPECT_EQ("Temperature", expectedDeviceElement->getElementName());
	EXPECT_EQ("Temperature Metric Element with ValueType as Float", expectedDeviceElement->getElementDescription());
}

TEST(Device, AddingSubElementObservable) {
	//Arrange
	Device * device = new Device("1234", "Temperatur", "Sensor vom Typ Temperatur");
	DeviceElementGroup g = DeviceElementGroup(device->getReferenceId() + ".1", device->getElementName() + " group", "Group Element of " + device->getElementName());

	//Act
	ObservableMetric * temperature = new ObservableMetric(device->getReferenceId() + ".1.1", "Temperature", "Temperature Metric Element with ValueType as Float", ValueType::ValueDataType::Float);
	g.addElement(*temperature);
	device->addDeviceElementGroup(g);
	
	//Assert
	DeviceElementGroup g1 = device->GetDeviceElementGroup();
	int numOfSubElements = g1.getSubElements().size();
	EXPECT_EQ(1, numOfSubElements);
	EXPECT_EQ(ElementType::Observable, g1.getSubElements()[0]->getElementType());

	ObservableMetric * actualDeviceElement = (ObservableMetric*)g1.getSubElements()[0];	
	EXPECT_EQ("1234.1.1", actualDeviceElement->getReferenceId());
	EXPECT_EQ("Temperature", actualDeviceElement->getElementName());
	EXPECT_EQ("Temperature Metric Element with ValueType as Float", actualDeviceElement->getElementDescription());
}

//TEST(Device, AddingSubElementReadonly) {
//	//Arrange
//	Device * device = new Device("1234", "Temperatur", "Sensor vom Typ Temperatur");
//	DeviceElementGroup g = DeviceElementGroup(device->getReferenceId() + ".1", device->getElementName() + " group", "Group Element of " + device->getElementName());
//
//	//Act
//	DeviceMetric temperature =  DeviceMetric(device->getReferenceId() + ".1.1", "Temperature", "Temperature Metric Element with ValueType as Float", ValueType::ValueDataType::Float);
//	g.addElement(temperature);
//	device->addDeviceElementGroup(g);
//
//	//Assert
//	DeviceElementGroup g1 = device->GetDeviceElementGroup();
//	int numOfSubElements = g1.getSubElements().size();
//	EXPECT_EQ(1, numOfSubElements);
//	EXPECT_EQ(ElementType::Readonly, g1.getSubElements()[0]->getElementType());
//
//	WriteableMetric * actualDeviceElement = (WriteableMetric*)g1.getSubElements()[0];
//	EXPECT_EQ("1234.1.1", actualDeviceElement->getReferenceId());
//	EXPECT_EQ("Temperature", actualDeviceElement->getElementName());
//	EXPECT_EQ("Temperature Metric Element with ValueType as Float", actualDeviceElement->getElementDescription());
//	EXPECT_THROW({ 
//		try {
//		actualDeviceElement->updateSensor("Test");
//		} 
//		catch (ElementTypeMismatchException ex)
//		{
//			throw;
//		}
//		catch (exception)
//		{
//			throw;
//		}
//		}, ElementTypeMismatchException);
//}

TEST(Device, AddingSubElementWriteable) {
	//Arrange
	Device * device = new Device("1234", "Temperatur", "Sensor vom Typ Temperatur");
	DeviceElementGroup g = DeviceElementGroup(device->getReferenceId() + ".1", device->getElementName() + " group", "Group Element of " + device->getElementName());

	//Act
	WriteableMetric * temperature = new WriteableMetric(device->getReferenceId() + ".1.1", "Temperature", "Temperature Metric Element with ValueType as Float", ValueType::ValueDataType::Float);
	g.addElement(*temperature);
	device->addDeviceElementGroup(g);

	//Assert
	DeviceElementGroup g1 = device->GetDeviceElementGroup();
	int numOfSubElements = g1.getSubElements().size();
	EXPECT_EQ(1, numOfSubElements);
	EXPECT_EQ(ElementType::Writable, g1.getSubElements()[0]->getElementType());

	WriteableMetric * actualDeviceElement = (WriteableMetric*)g1.getSubElements()[0];
	EXPECT_EQ("1234.1.1", actualDeviceElement->getReferenceId());
	EXPECT_EQ("Temperature", actualDeviceElement->getElementName());
	EXPECT_EQ("Temperature Metric Element with ValueType as Float", actualDeviceElement->getElementDescription());

	EXPECT_NO_THROW({ actualDeviceElement->updateSensor("Test");});
}

TEST(Device, AddingFunctionToGroup) {
	//Arrange
	Device * device = new Device("1234", "Temperatur", "Sensor vom Typ Temperatur");
	DeviceElementGroup g = DeviceElementGroup(device->getReferenceId() + ".1", device->getElementName() + " group", "Group Element of " + device->getElementName());

	//Act
	DeviceFunction* deviceFunction = new DeviceFunction(device->getReferenceId() + ".1.3", "GetTemperature", "Returns the Temperature Value");
	g.addElement(*deviceFunction);
	device->addDeviceElementGroup(g);

	//Assert
	DeviceElementGroup g1 = device->GetDeviceElementGroup();
	int numOfSubElements = g1.getSubElements().size();
	EXPECT_EQ(1, numOfSubElements);
	EXPECT_EQ(ElementType::Function, g1.getSubElements()[0]->getElementType());

	DeviceMetric * actualDeviceElement = (DeviceMetric*)g1.getSubElements()[0];
	EXPECT_EQ("1234.1.3", actualDeviceElement->getReferenceId());
	EXPECT_EQ("GetTemperature", actualDeviceElement->getElementName());
	EXPECT_EQ("Returns the Temperature Value", actualDeviceElement->getElementDescription());
}

TEST(Device, AddingParameterToFunction) {
	//Arrange
	Device * device = new Device("1234", "Temperatur", "Sensor vom Typ Temperatur");
	DeviceElementGroup g = DeviceElementGroup(device->getReferenceId() + ".1", device->getElementName() + " group", "Group Element of " + device->getElementName());
	DeviceFunction* deviceFunction = new DeviceFunction(device->getReferenceId() + ".1.3", "GetTemperature", "Returns the Temperature Value");

	//Act	
	FunctionParameter* functionParameter = new FunctionParameter(device->getReferenceId() + ".1.3.1", "SensorID", "This is a sensor", ValueType::String);
	deviceFunction->addParameter(*functionParameter);
	g.addElement(*deviceFunction);
	device->addDeviceElementGroup(g);

	//Assert
	DeviceElementGroup g1 = device->GetDeviceElementGroup();
	DeviceFunction* subElement = (DeviceFunction*) g1.getSubElements()[0];

	EXPECT_EQ(1, subElement->getParameterList().size());
	FunctionParameter fp = subElement->getParameterList()[0];
	ValueType::ValueDataType vt = fp.getDataType();
	EXPECT_EQ(ValueType::ValueDataType::String, vt);
	DeviceMetric * actualDeviceElement = (DeviceMetric*)g1.getSubElements()[0];
	EXPECT_EQ("1234.1.3", actualDeviceElement->getReferenceId());
	EXPECT_EQ("GetTemperature", actualDeviceElement->getElementName());
	EXPECT_EQ("Returns the Temperature Value", actualDeviceElement->getElementDescription());
}

