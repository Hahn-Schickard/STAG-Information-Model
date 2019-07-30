#ifndef REFIDTEST_HPP
#define REFIDTEST_HPP
#include "DeviceElementBuilder.hpp"
#include "DeviceElementGroupImpl.hpp"
#include "DeviceImpl.hpp"
#include "Device.hpp"
#include <string>
#include <gtest/gtest.h>

using namespace Model_Factory;
using namespace Information_Model;

TEST(RefIdTests, DeviceRefIdIsCorrect)
{
    // Arrange
    Device *element =
        new DeviceImpl("1234", "TestDevice", "A hardcoded deviceelement");

    // Act
    std::string testedElement = element->getElementRefId();

    // Assert
    std::string expectedResult = "1234";
    EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(RefIdTests, DeviceRefIdIsNotCorrect)
{
    // Arrange
    Device *element =
        new DeviceImpl("1234", "TestDevice", "A hardcoded deviceelement");

    // Act
    std::string testedElement = element->getElementRefId();
    // Assert
    std::string expectedResult = "1235";
    EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(RefIdTests, DeviceElementRefIdIsCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Group);

    // Act
    std::string testedElement = element->getElementRefId();
    // Assert
    std::string expectedResult = "1234";
    EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(RefIdTests, DeviceElementRefIdIsNotCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Group);

    // Act
    std::string testedElement = element->getElementRefId();
    // Assert
    std::string expectedResult = "1235";
    EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(RefIdTests, DeviceElementGroupRefIdIsCorrect)
{
    // Arrange
    DeviceElementGroup *element = new DeviceElementGroupImpl(
        "1234", "TestDevice", "A hardcoded deviceelement");

    // Act
    std::string testedElement = element->getElementRefId();
    // Assert
    std::string expectedResult = "1234";
    EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(RefIdTests, DeviceElementGroupRefIdIsNotCorrect)
{
    // Arrange
    DeviceElementGroup *element = new DeviceElementGroupImpl(
        "1234", "TestDevice", "A hardcoded deviceelement");

    // Act
    std::string testedElement = element->getElementRefId();
    // Assert
    std::string expectedResult = "1235";
    EXPECT_NE(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

#endif