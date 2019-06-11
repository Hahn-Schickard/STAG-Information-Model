#include "pch.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ConsumerAdapterTests.h"
#include "includes/blueprint.hpp"
#include "includes/device.hpp"
#include <string>

using ::testing::AtLeast;
using namespace std;




TEST(ConsumerAdapter, GetDevice_ValidRefId_ReturnsDevice) {
	//Arrange
	BluePrint * bluePrint = new BluePrint("1234","TestDevice", "A hardcoded device for testing only");

	//Act
	auto device = bluePrint->GetDevice("1234");
	string refId = device->getReferenceId();
	
	//Assert
	ASSERT_NE(device, nullptr);
	EXPECT_EQ("1234", refId);
	EXPECT_EQ("TestDevice", device->getElementName());
	EXPECT_EQ("A hardcoded device for testing only", device->getElementDescription());
}

TEST(ConsumerAdapter, GetDeviceElementGroup_NoParameter_ReturnsFirstGroup) {
	//Arrange
	BluePrint * bluePrint = new BluePrint();
	auto device = bluePrint->GetDevice("1234");

	//Act
	auto group = device->getDeviceElementGroup();

	//Assert
	ASSERT_NE(group, nullptr);
	EXPECT_EQ("1234.1", group->getReferenceId());
	EXPECT_EQ("TestElementGroup", group->getElementName());
	EXPECT_EQ("A hardcoded device element group", group->getElementDescription());
}

TEST(ConsumerAdapter, GetDeviceElementGroups_ReturnsOneGroup) {
	//Arrange
	BluePrint * bluePrint = new BluePrint();
	auto device = bluePrint->GetDevice("1234");

	//Act
	auto groups = device->getDeviceElementGroups();

	//Assert

	EXPECT_EQ(1, groups.size());

}
TEST(ConsumerAdapter, GetDeviceElementGroups_ReturnsDeviceBlueprint) {
	//Arrange
	BluePrint * bluePrint = new BluePrint();
	auto device = bluePrint->GetDevice("1234");
	auto groups = device->getDeviceElementGroups();

	//Assert
	ASSERT_EQ(1, groups.size());
	auto group = groups[0];
	EXPECT_EQ("1234.1", group->getReferenceId());
	EXPECT_EQ("TestElementGroup", group->getElementName());
	EXPECT_EQ("A hardcoded device element group", group->getElementDescription());
	auto elements = group->getSubElements();
	ObservableMetric * observableMetric = nullptr;
	WriteableMetric * writeableMetric = nullptr;
	DeviceMetric * deviceMetric = nullptr;
	DeviceElementGroup * groupMetric = nullptr;

	ASSERT_GE(4, elements.size());
	for (int i = 0; i < elements.size(); i++) {
		switch (elements[i]->getElementType())
		{
			case Observable: {
				observableMetric = (ObservableMetric*)elements[i];
			} break;
			case Writable: {
				writeableMetric = (WriteableMetric*)elements[i];
			}break;
			case Readonly: {
				deviceMetric = (DeviceMetric*)elements[i];
			}break;
			case Group: {
				groupMetric = (DeviceElementGroup*)elements[i];
			}break;
		}
		

		
	}
	ASSERT_NE(observableMetric, nullptr);

	EXPECT_EQ("1234.1.1", observableMetric->getReferenceId());
	EXPECT_EQ("TestObservableMetric", observableMetric->getElementName());
	EXPECT_EQ("A hardcoded observableMetric Element", observableMetric->getElementDescription());

	ASSERT_NE(writeableMetric, nullptr);

	EXPECT_EQ("1234.1.2", writeableMetric->getReferenceId());
	EXPECT_EQ("TestWriteableMetric", writeableMetric->getElementName());
	EXPECT_EQ("A hardcoded writeableMetric", writeableMetric->getElementDescription());

	ASSERT_NE(deviceMetric, nullptr);

	EXPECT_EQ("1234.1.3", deviceMetric->getReferenceId());
	EXPECT_EQ("TestDeviceMetric", deviceMetric->getElementName());
	EXPECT_EQ("A hardcoded deviceMetric", deviceMetric->getElementDescription());

	ASSERT_NE(groupMetric, nullptr);

	EXPECT_EQ("1234.1.4", groupMetric->getReferenceId());
	EXPECT_EQ("TestElementGroup2", groupMetric->getElementName());
	EXPECT_EQ("A hardcoded device element group", groupMetric->getElementDescription());
	auto elements2 = groupMetric->getSubElements();

	ASSERT_GE(1, elements2.size());

	auto element = elements2[0];
	EXPECT_EQ("1234.1.4.1", element->getReferenceId());
	EXPECT_EQ("TestDeviceMetric2", element->getElementName());
	EXPECT_EQ("A hardcoded deviceMetric", element->getElementDescription());
}






ConsumerAdapterTests::ConsumerAdapterTests()
{

}


ConsumerAdapterTests::~ConsumerAdapterTests()
{
}
