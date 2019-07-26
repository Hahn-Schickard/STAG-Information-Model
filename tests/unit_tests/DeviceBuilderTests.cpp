#include "gtest/gtest.h"
#include "Device.hpp"
#include <string>
#include "DeviceBuilder.hpp"
#include "MakeUnique.hpp"


using namespace std;
using namespace Model_Factory;
using namespace Information_Model;



TEST(DeviceBuilder, Create_NameRefIdAndDescription_ReturnsCorrectElementName)
{
	//Arrange
   	
	//Act
	auto builder = make_unique<DeviceBuilder>("TestDevice","123","A device for testing only");
	std::unique_ptr<Information_Model::Device> device = builder->getDevice();
	// //Assert

     ASSERT_EQ("TestDevice",device->getElementName());

}

TEST(DeviceBuilder, Create_NameRefIdAndDescription_ReturnsCorrectElementRefId)
{
	//Arrange
   	
	//Act
	auto builder = make_unique<DeviceBuilder>("TestDevice","123","A device for testing only");
	std::unique_ptr<Information_Model::Device> device = builder->getDevice();
	// //Assert

     ASSERT_EQ("123",device->getElementRefId());

}

TEST(DeviceBuilder, Create_NameRefIdAndDescription_ReturnsCorrectElementDescription)
{
	//Arrange
   	
	//Act
	auto builder = make_unique<DeviceBuilder>("TestDevice","123","A device for testing only");
	std::unique_ptr<Information_Model::Device> device = builder->getDevice();
	// //Assert

     ASSERT_EQ("A device for testing only",device->getElementDescription());

}


TEST(DeviceBuilder, AddDeviceElementGroup_ReturnsCorrectGroupName)
{
	//Arrange
   	
	//Act
	auto builder = make_unique<DeviceBuilder>("TestDevice","123","A device for testing only");
	builder->addDeviceElementGroup("TestBaseGroup","A base group");
	std::unique_ptr<Information_Model::Device> device = builder->getDevice();
	auto group = device->getDeviceElementGroup();
	
	// //Assert
    ASSERT_EQ("TestBaseGroup",group->getElementName());

}

TEST(DeviceBuilder, AddDeviceElementGroup_ReturnsCorrectGroupDescription)
{
	//Arrange
   	
	//Act
	auto builder = make_unique<DeviceBuilder>("TestDevice","123","A device for testing only");
	builder->addDeviceElementGroup("TestBaseGroup","A base group");
	std::unique_ptr<Information_Model::Device> device = builder->getDevice();
	auto group = device->getDeviceElementGroup();
	
	// //Assert
    ASSERT_EQ("A base group",group->getElementDescription());

}

TEST(DeviceBuilder, AddDeviceElementGroup_ReturnsCorrectGroupRefId)
{
	//Arrange
   	
	//Act
	auto builder = make_unique<DeviceBuilder>("TestDevice","123","A device for testing only");
	builder->addDeviceElementGroup("TestBaseGroup","A base group");
	std::unique_ptr<Information_Model::Device> device = builder->getDevice();
	auto group = device->getDeviceElementGroup();
	
	
	// //Assert
    ASSERT_EQ("123:",group->getElementRefId());

}

TEST(DeviceBuilder, AddSubGroup_ReturnsCorrectGroupRefId)
{
	//Arrange
   	auto builder = make_unique<DeviceBuilder>("TestDevice","123","A device for testing only");
	builder->addDeviceElementGroup("TestBaseGroup","A base group");
	
	//Act
	builder->addDeviceElement("Subgroup1", "A Subgroup",ElementType::Group);
	
	//Assert
	std::unique_ptr<Information_Model::Device> device = builder->getDevice();
	auto group = device->getDeviceElementGroup();
	auto subElements = group->getSubelements();

	std::shared_ptr<DeviceElementGroup> subgroup;

	for (auto el : subElements)
	{
		if (el->getElementType() == ElementType::Group)
		{
			subgroup = static_pointer_cast<DeviceElementGroup>(el);
		}
	}

	ASSERT_EQ(1, subElements.size());
	ASSERT_NE(nullptr,subgroup);
	//Assert
    ASSERT_EQ("123:0",subgroup->getElementRefId());
}


TEST(DeviceBuilder, AddMetricToGroup_ReturnsCorrectMetricRefId)
{
	//Arrange
   	auto builder = make_unique<DeviceBuilder>("TestDevice","123","A device for testing only");
	builder->addDeviceElementGroup("TestBaseGroup","A base group");
	
	//Act
	builder->addDeviceElement("Subgroup1", "A Subgroup",ElementType::Group);
	
	
	//Assert
	std::unique_ptr<Information_Model::Device> device = builder->getDevice();
	auto group = device->getDeviceElementGroup();
	auto subElements = group->getSubelements();

	

	std::shared_ptr<DeviceElementGroup> subgroup;

	for (auto el : subElements)
	{
		if (el->getElementType() == ElementType::Group)
		{
			subgroup = static_pointer_cast<DeviceElementGroup>(el);
		}
	}

	ASSERT_EQ(1, subElements.size());
	ASSERT_NE(nullptr,subgroup);
	//Assert
    ASSERT_EQ("123:0",subgroup->getElementRefId());
}


TEST(DeviceBuilder, AddMetricToSubgroup_ReturnsCorrectMetricRefId)
{
	//Arrange
   	auto builder = make_unique<DeviceBuilder>("TestDevice","123","A device for testing only");
	builder->addDeviceElementGroup("TestBaseGroup","A base group");
	
	//Act
	auto refId = builder->addDeviceElement("Subgroup1", "A Subgroup",ElementType::Group);
	cerr << "RefId: " << refId << endl;

	auto metricRefId = builder->addDeviceElement(refId, "Metric2", "An observable Metric added to a subgroup", ElementType::Observable);
	cerr << "Metric RefId: " << metricRefId << endl;
	
	
	// //Assert
	// std::unique_ptr<Information_Model::Device> device = builder->getDevice();
	// auto group = device->getDeviceElementGroup();
	// auto subElements = group->getSubelements();
	
	// std::shared_ptr<DeviceElementGroup> subgroup;
	// std::shared_ptr<DeviceElement> observableElement;

	// for (auto el : subElements)
	// {
	// 	if (el->getElementType() == ElementType::Group)
	// 	{
	// 		subgroup = static_pointer_cast<DeviceElementGroup>(el);
	// 		for (auto element : subgroup->getSubelements())
	// 		{
	// 			if (element->getElementType() == ElementType::Observable)
	// 			{
	// 				observableElement = element;
	// 			}

	// 		}
			
	// 	}
	// }

	// ASSERT_EQ(1, subElements.size());
	// ASSERT_NE(nullptr,subgroup);
	// //Assert
    // ASSERT_EQ("123:0",subgroup->getElementRefId());
	// ASSERT_NE(nullptr,observableElement);

	// EXPECT_EQ("123:0:1",observableElement->getElementRefId());
}