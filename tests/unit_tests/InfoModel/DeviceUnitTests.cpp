#include <gtest/gtest.h>
#include "device.hpp"
#include "deviceElementGroup.hpp"
#include "deviceMetric.hpp"
#include "deviceFunction.hpp"
#include "informationModelExceptions.hpp"


namespace {
	class DeviceTest : public ::testing::Test {
	public:
		Device * device;
		DeviceElementGroup * deviceElementGroup = new DeviceElementGroup("123","","");

		DeviceTest()
		{

		}

		virtual void SetUp()
		{
			device = new Device("1234", "Temperatur", "Sensor vom Typ Temperature");
			deviceElementGroup = new  DeviceElementGroup(device->getReferenceId() + ".1", device->getElementName() + " group", "Group Element of " + device->getElementName());
		}

		virtual void TearDown() {
			delete device;
		}
	};

	TEST(Device, CreatingDevice_ValidParameters_ConstructsNewDevice) {
		//Act
		auto device = new Device("1234", "Temperatur", "Sensor vom Typ Temperature");

		//Assert
		EXPECT_EQ("1234", device->getReferenceId());
		EXPECT_EQ("Temperatur", device->getElementName());
		EXPECT_EQ("Sensor vom Typ Temperature", device->getElementDescription());

		delete device;
	}

	TEST(Device, CreatingDevice_InvalidDeviceParameter_ThrowsInvalidDeviceParameterException) {
		EXPECT_THROW({ Device * device = new Device("1234a", "Temperatur", "Sensor vom Typ Temperatur"); }, InvalidReferenceIdException);
	}

	TEST_F(DeviceTest, AddingDeviceElementGroup_ValidParameters_AddsElementGroup) {
	
		//Act
		device->addDeviceElementGroup(deviceElementGroup);

		//Assert
		auto devElGr = device->getDeviceElementGroup();
		EXPECT_EQ("1234.1", devElGr->getReferenceId());
		EXPECT_EQ("Temperatur group", devElGr->getElementName());
		EXPECT_EQ("Group Element of Temperatur", devElGr->getElementDescription());
	}

	TEST_F(DeviceTest, AddingSubElementToGroup_ValidParameters_DeviceMetricIsAddedToGroup) {

		//Arrange
		DeviceMetric* temperature = new DeviceMetric(device->getReferenceId() + ".1.1", "Temperature", "Temperature Metric Element with ValueType as Float", ValueType::ValueDataType::Float);

		//Act
		
		deviceElementGroup->addElement(*temperature);
		device->addDeviceElementGroup(deviceElementGroup);

		//Assert
		auto g1 = device->getDeviceElementGroup();
		int numOfSubElements = g1->getSubElements().size();
		ASSERT_EQ(1, numOfSubElements);

		DeviceMetric * expectedDeviceElement = (DeviceMetric*)g1->getSubElements()[0];
		EXPECT_EQ("1234.1.1", expectedDeviceElement->getReferenceId());
		EXPECT_EQ("Temperature", expectedDeviceElement->getElementName());
		EXPECT_EQ("Temperature Metric Element with ValueType as Float", expectedDeviceElement->getElementDescription());
	}

	TEST_F(DeviceTest, AddingSubElementObservable_ValidParameters_ObservableMetricIsAddedToGroup) {

		//Act
		ObservableMetric * temperature = new ObservableMetric(device->getReferenceId() + ".1.1", "Temperature", "Temperature Metric Element with ValueType as Float", ValueType::ValueDataType::Float);
		deviceElementGroup->addElement(*temperature);
		device->addDeviceElementGroup(deviceElementGroup);

		//Assert
		auto g1 = device->getDeviceElementGroup();
		int numOfSubElements = g1->getSubElements().size();
		ASSERT_EQ(1, numOfSubElements);
		ASSERT_EQ(ElementType::Observable, g1->getSubElements()[0]->getElementType());

		ObservableMetric * actualDeviceElement = (ObservableMetric*)g1->getSubElements()[0];
		EXPECT_EQ("1234.1.1", actualDeviceElement->getReferenceId());
		EXPECT_EQ("Temperature", actualDeviceElement->getElementName());
		EXPECT_EQ("Temperature Metric Element with ValueType as Float", actualDeviceElement->getElementDescription());
	}

	TEST_F(DeviceTest, AddingSubElementReadonly_ValidParameters_WriteableMetricIsAddedToGroup) {


		//Act
		DeviceMetric temperature = DeviceMetric(device->getReferenceId() + ".1.1", "Temperature", "Temperature Metric Element with ValueType as Float", ValueType::ValueDataType::Float);
		deviceElementGroup->addElement(temperature);
		device->addDeviceElementGroup(deviceElementGroup);

		//Assert
		auto g1 = device->getDeviceElementGroup();
		int numOfSubElements = g1->getSubElements().size();
		ASSERT_EQ(1, numOfSubElements);
		ASSERT_EQ(ElementType::Readonly, g1->getSubElements()[0]->getElementType());

		WriteableMetric * actualDeviceElement = (WriteableMetric*)g1->getSubElements()[0];
		EXPECT_EQ("1234.1.1", actualDeviceElement->getReferenceId());
		EXPECT_EQ("Temperature", actualDeviceElement->getElementName());
		EXPECT_EQ("Temperature Metric Element with ValueType as Float", actualDeviceElement->getElementDescription());
	}

	TEST_F(DeviceTest, AddingSubElementWriteable_ValidParameters_WriteableMetricIsAddedToGroup) {
		//Arrange

		WriteableMetric * temperature = new WriteableMetric(device->getReferenceId() + ".1.1", "Temperature", "Temperature Metric Element with ValueType as Float", ValueType::ValueDataType::Float);
		deviceElementGroup->addElement(*temperature);

		//Act	
		device->addDeviceElementGroup(deviceElementGroup);

		//Assert
		auto g1 = device->getDeviceElementGroup();
		int numOfSubElements = g1->getSubElements().size();
		ASSERT_EQ(1, numOfSubElements);
		ASSERT_EQ(ElementType::Writable, g1->getSubElements()[0]->getElementType());

		WriteableMetric * actualDeviceElement = (WriteableMetric*)g1->getSubElements()[0];
		EXPECT_EQ("1234.1.1", actualDeviceElement->getReferenceId());
		EXPECT_EQ("Temperature", actualDeviceElement->getElementName());
		EXPECT_EQ("Temperature Metric Element with ValueType as Float", actualDeviceElement->getElementDescription());		
	}

	TEST_F(DeviceTest, AddingFunctionToGroup_ValidParameters_DeviceMetricAdded) {
		//Arrange

		DeviceFunction* deviceFunction = new DeviceFunction(device->getReferenceId() + ".1.3", "GetTemperature", "Returns the Temperature Value");
		deviceElementGroup->addElement(*deviceFunction);

		//Act	
		device->addDeviceElementGroup(deviceElementGroup);

		//Assert
		auto g1 = device->getDeviceElementGroup();
		int numOfSubElements = g1->getSubElements().size();
		ASSERT_EQ(1, numOfSubElements);
		ASSERT_EQ(ElementType::Function, g1->getSubElements()[0]->getElementType());

		DeviceMetric * actualDeviceElement = (DeviceMetric*)g1->getSubElements()[0];
		EXPECT_EQ("1234.1.3", actualDeviceElement->getReferenceId());
		EXPECT_EQ("GetTemperature", actualDeviceElement->getElementName());
		EXPECT_EQ("Returns the Temperature Value", actualDeviceElement->getElementDescription());
	}

	TEST_F(DeviceTest, AddingParameterToFunction_StringType_ParameterOfTypeStringAdded) {
		//Arrange
		DeviceFunction* deviceFunction = new DeviceFunction(device->getReferenceId() + ".1.3", "GetTemperature", "Returns the Temperature Value");
		FunctionParameter* functionParameter = new FunctionParameter(device->getReferenceId() + ".1.3.1", "SensorID", "This is a sensor", ValueType::String);
		
		//Act			
		deviceFunction->addParameter(*functionParameter);
		
		//Assert
		deviceElementGroup->addElement(*deviceFunction);
		device->addDeviceElementGroup(deviceElementGroup);

		auto g1 = device->getDeviceElementGroup();
		DeviceFunction* subElement = (DeviceFunction*)g1->getSubElements()[0];

		EXPECT_EQ(1, subElement->getParameterList().size());
		FunctionParameter fp = subElement->getParameterList()[0];
		ValueType::ValueDataType vt = fp.getDataType();
		EXPECT_EQ(ValueType::ValueDataType::String, vt);
	}




}