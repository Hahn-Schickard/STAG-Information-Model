#ifndef DEVICEELEMENTGROUPTEST_HPP
#define DEVICEELEMENTGROUPTEST_HPP

#include <iostream>

using namespace Model_Factory;
using namespace Information_Model;

class DeviceElementGroupTest : public ::testing::Test
{
public:
    Device *device;
    DeviceElementGroup *group;
    std::vector<std::shared_ptr<DeviceElement>> elements;

    DeviceElementGroupTest() {}

    void SetUp()
    {
        device = new DeviceImpl("1234", "TestDevice", "A hardcoded deviceelement");
        DeviceImpl *tmp_device = dynamic_cast<DeviceImpl *>(device);
        tmp_device->addDeviceElementGroup(
            "TestGroup", "This is a synthetic test for device element group.");
        group = device->getDeviceElementGroup();
        DeviceElementGroupImpl *tmp_group = dynamic_cast<DeviceElementGroupImpl *>(group);
        tmp_group->addDeviceEelment("SubTestDevice", "This is the first Subelement", ElementType::Readonly);
        elements = group->getSubelements();
    }

    void TearDown() { delete device; }
};

TEST_F(DeviceElementGroupTest, DeviceElementGroupSubelementIDIsCorrect)
{
    // Act
    for (auto element : elements)
    {
        //std::cerr << element->getElementRefId() << std::endl;
        std::string testedElement = element->getElementRefId();
        // Assert
        std::string expectedResult = "1234:0";
        EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
            << "expected: " << expectedResult << std::endl
            << "provided: " << testedElement << std::endl;
    }
}

TEST_F(DeviceElementGroupTest, DeviceElementGroupSubelementIDIsNotCorrect)
{
    // Act
    for (auto element : elements)
    {
        std::string testedElement = element->getElementRefId();
        // Assert
        std::string expectedResult = "1234:1";
        EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
            << "expected: " << expectedResult << std::endl
            << "provided: " << testedElement << std::endl;
    }
}

TEST_F(DeviceElementGroupTest, DeviceElementGroupSubelementNameIsCorrect)
{
    // Act
    for (auto element : elements)
    {
        std::string testedElement = element->getElementName();
        // Assert
        std::string expectedResult = "SubTestDevice";
        EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
            << "expected: " << expectedResult << std::endl
            << "provided: " << testedElement << std::endl;
    }
}

TEST_F(DeviceElementGroupTest, DeviceElementGroupSubelementNameIsNotCorrect)
{
    // Act
    for (auto element : elements)
    {
        std::string testedElement = element->getElementName();
        // Assert
        std::string expectedResult = "SubTestDevice2";
        EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
            << "expected: " << expectedResult << std::endl
            << "provided: " << testedElement << std::endl;
    }
}

TEST_F(DeviceElementGroupTest, DeviceElementGroupSubelementDescriptionIsCorrect)
{
    // Act
    for (auto element : elements)
    {
        std::string testedElement = element->getElementDescription();
        // Assert
        std::string expectedResult = "This is the first Subelement";
        EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
            << "expected: " << expectedResult << std::endl
            << "provided: " << testedElement << std::endl;
    }
}

TEST_F(DeviceElementGroupTest, DeviceElementGroupSubelementDescriptionIsNotCorrect)
{
    // Act
    for (auto element : elements)
    {
        std::string testedElement = element->getElementDescription();
        // Assert
        std::string expectedResult = "This is not the first Subelement";
        EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
            << "expected: " << expectedResult << std::endl
            << "provided: " << testedElement << std::endl;
    }
}

#endif