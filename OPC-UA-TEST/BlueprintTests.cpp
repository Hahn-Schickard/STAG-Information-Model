#include "BlueprintTests.h"
#include "pch.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ConsumerAdapterTests.h"
#include "includes/blueprint.hpp"
#include "includes/device.hpp"
#include "includes/informationModelExceptions.hpp"
#include <string>

using ::testing::AtLeast;
using namespace std;


TEST(RefIdGenerationTests, AddDeviceGroup_DeviceRefIdIs1234_ResultIsNotNullPtr)
{
	//Arrange
	BluePrint * bluePrint = new BluePrint("1234", "TestDevice", "A hardcoded device for testing only");

	//Act
	bluePrint->AddDeviceGroup("TestGroup", "A Test Group");

	//Assert
	auto device = bluePrint->GetDevice();
	auto group = device->getDeviceElementGroup("1234.1");

	EXPECT_NE(group, nullptr);

}

TEST(RefIdGenerationTests, AddDeviceGroup_DeviceRefIdIs1234_ResultIs1234_1)
{
	//Arrange
	BluePrint * bluePrint = new BluePrint("1234", "TestDevice", "A hardcoded device for testing only");

	//Act
	bluePrint->AddDeviceGroup("TestGroup", "A Test Group");

	//Assert
	auto device = bluePrint->GetDevice();
	auto group = device->getDeviceElementGroup("1234.1");

	ASSERT_NE(group, nullptr);
	EXPECT_EQ(group->getReferenceId(), "1234.1");

}

TEST(RefIdGenerationTests, AddTwoDeviceGroups_DeviceRefIdIs1234_SecondGroupDeviceIdIs1234_2)
{
	//Arrange
	BluePrint * bluePrint = new BluePrint("1234", "TestDevice", "A hardcoded device for testing only");

	//Act
	bluePrint->AddDeviceGroup("TestGroup", "A Test Group");
	bluePrint->AddDeviceGroup("TestGroup2", "A Second Test Group");

	//Assert
	auto device = bluePrint->GetDevice();
	auto group = device->getDeviceElementGroup("1234.2");

	ASSERT_NE(group, nullptr);
	EXPECT_EQ(group->getReferenceId(), "1234.2");

}

TEST(RefIdGenerationTests, AddDeviceSubGroup_DeviceRefIdIs1234_ResultIs1234_1_1)
{
	//Arrange
	BluePrint * bluePrint = new BluePrint("1234", "TestDevice", "A hardcoded device for testing only");

	//Act
	bluePrint->AddDeviceGroup("TestGroup", "A Test Group");
	bluePrint->AddSubElement("1234.1", "TestSubGroup", "A Test Subgroup",ElementType::Group);

	//Assert
	auto device = bluePrint->GetDevice();
	auto group = device->getDeviceElementGroup("1234.1");
	auto groupElement = device->findSubElementGroup(group, "1234.1.1");

	ASSERT_NE(groupElement, nullptr);
	EXPECT_EQ(groupElement->getReferenceId(), "1234.1.1");
	EXPECT_EQ(groupElement->getElementName(), "TestSubGroup");
	EXPECT_EQ(groupElement->getElementDescription(), "A Test Subgroup");

}

TEST(RefIdGenerationTests, AddSecondDeviceSubGroup_DeviceRefIdIs1234_ResultIs1234_1_2)
{
	//Arrange
	BluePrint * bluePrint = new BluePrint("1234", "TestDevice", "A hardcoded device for testing only");

	//Act
	bluePrint->AddDeviceGroup("TestGroup", "A Test Group");
	bluePrint->AddSubElement("1234.1", "TestSubGroup", "A Test Subgroup",ElementType::Group);
	bluePrint->AddSubElement("1234.1", "TestSubGroup2", "A second Test Subgroup", ElementType::Group);
	//Assert
	auto device = bluePrint->GetDevice();
	auto group = device->getDeviceElementGroup("1234.1");
	auto groupElement = device->findSubElementGroup(group, "1234.1.1");
	auto groupElement2 = device->findSubElementGroup(group, "1234.1.2");

	ASSERT_NE(groupElement2, nullptr);
	EXPECT_EQ(groupElement2->getReferenceId(), "1234.1.2");
	EXPECT_EQ(groupElement2->getElementName(), "TestSubGroup2");
	EXPECT_EQ(groupElement2->getElementDescription(), "A second Test Subgroup");

}

TEST(RefIdGenerationTests, AddElementtoGroup_DeviceRefIdIs1234_ResultIs1234_1_2)
{
	//Arrange
	BluePrint * bluePrint = new BluePrint("1234", "TestDevice", "A hardcoded device for testing only");

	//Act
	bluePrint->AddDeviceGroup("TestGroup", "A Test Group");
	bluePrint->AddSubElement("1234.1", "TestSubGroup", "A Test Subgroup", ElementType::Group);
	bluePrint->AddSubElement("1234.1", "TestElement", "An element to a group", ElementType::Observable);
	//Assert
	auto device = bluePrint->GetDevice();
	auto group = device->getDeviceElementGroup("1234.1");
	auto subElement = device->findSubElement(group, "1234.1.2");

	ASSERT_NE(subElement, nullptr);
	EXPECT_EQ(subElement->getReferenceId(), "1234.1.2");
	EXPECT_EQ(subElement->getElementName(), "TestElement");
	EXPECT_EQ(subElement->getElementDescription(), "An element to a group");

}

TEST(RefIdGenerationTests, AddElementtoGroup_DeviceRefIdIs1234_ResultIs1234_1_3_1)
{
	//Arrange
	BluePrint * bluePrint = new BluePrint("1234", "TestDevice", "A hardcoded device for testing only");

	//Act
	bluePrint->AddDeviceGroup("TestGroup", "A Test Group");
	bluePrint->AddSubElement("1234.1", "TestSubGroup", "A Test Subgroup", ElementType::Group);
	bluePrint->AddSubElement("1234.1", "TestElement", "An element to a group", ElementType::Observable);
	bluePrint->AddSubElement("1234.1", "TestSubGroup2", "A second Test Subgroup", ElementType::Group);
	bluePrint->AddSubElement("1234.1.3", "WriteableElement", "A writeable Element to the second group.", ElementType::Group);
	//Assert
	auto device = bluePrint->GetDevice();
	auto group = device->getDeviceElementGroup("1234.1");
	auto subElement = device->findSubElement(group, "1234.1.3.1");

	ASSERT_NE(subElement, nullptr);
	EXPECT_EQ(subElement->getReferenceId(), "1234.1.3.1");
	EXPECT_EQ(subElement->getElementName(), "WriteableElement");
	EXPECT_EQ(subElement->getElementDescription(), "A writeable Element to the second group.");

}

TEST(RefIdGenerationTests, AddElementToAnotherElement_ThrowsException)
{
	//Arrange
	BluePrint * bluePrint = new BluePrint("1234", "TestDevice", "A hardcoded device for testing only");

	//Act
	bluePrint->AddDeviceGroup("TestGroup", "A Test Group");
	bluePrint->AddSubElement("1234.1", "TestSubGroup", "A Test Subgroup", ElementType::Group);
	bluePrint->AddSubElement("1234.1", "TestElement", "An element to a group", ElementType::Observable);
	bluePrint->AddSubElement("1234.1", "TestSubGroup2", "A second Test Subgroup", ElementType::Group);
	bluePrint->AddSubElement("1234.1.3", "WriteableElement", "A writeable Element to the second group.", ElementType::Writable);
	
	//Act and Assert
	EXPECT_THROW({ bluePrint->AddSubElement("1234.1.3.1", "ObservableElement", "A observable element", ElementType::Observable); }, ElementTypeMismatchException);	
}

BlueprintTests::BlueprintTests()
{
}


BlueprintTests::~BlueprintTests()
{
}
