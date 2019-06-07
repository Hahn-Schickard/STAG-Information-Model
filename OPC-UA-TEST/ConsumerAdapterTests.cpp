#include "pch.h"
#include "ConsumerAdapterTests.h"
#include "includes/blueprint.hpp"

TEST(ConsumerAdapter, GetDevice_ValidRefId_ReturnsDevice) {
	//Arrange
	BluePrint * bluePrint = new BluePrint();

	//Act
	auto device = bluePrint->GetDevice("1234");
	
	//Assert
	ASSERT_NE(device, nullptr);
	EXPECT_EQ("1234", device->getReferenceId());
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

TEST(ConsumerAdapter, GetDeviceElementGroups_ReturnsObservableMetric) {
	//Arrange
	BluePrint * bluePrint = new BluePrint();
	auto device = bluePrint->GetDevice("1234");

	//Act
	auto groups = device->getDeviceElementGroups();

	//Assert
	ASSERT_EQ(1, groups.size());
	auto group = groups[0];
	auto elements = group->getSubElements();
	ASSERT_GE(1, elements.size());
	auto element = elements[0];
	EXPECT_EQ("1234.1.1", element->getReferenceId());
	EXPECT_EQ("TestObservableMetric", element->getElementName());
	EXPECT_EQ("A hardcoded observableMetric Element", element->getElementDescription());


}





ConsumerAdapterTests::ConsumerAdapterTests()
{

}


ConsumerAdapterTests::~ConsumerAdapterTests()
{
}
