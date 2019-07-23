#ifndef DEVICEELEMENTTEST_HPP
#define DEVICEELEMENTTEST_HPP

#include "DeviceElementBuilder.hpp"
#include "DeviceElementGroupImpl.hpp"
#include "DeviceImpl.hpp"
#include "Device.hpp"
#include "DeviceBuilder.hpp"
#include <string>
#include <gtest/gtest.h>

using namespace Model_Factory;
using namespace Information_Model;

TEST(DeviceElementTest, DeviceElementTypeUndefIsCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Undefined);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Undefined;
    EXPECT_EQ(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(DeviceElementTest, DeviceElementTypeUndefIsNotCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Undefined);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Group;
    EXPECT_NE(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(DeviceElementTest, DeviceElementTypeGroupIsCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Group);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Group;
    EXPECT_EQ(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(DeviceElementTest, DeviceElementTypeGroupIsNotCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Group);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Undefined;
    EXPECT_NE(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(DeviceElementTest, DeviceElementTypeReadOnlyIsCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Readonly);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Readonly;
    EXPECT_EQ(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(DeviceElementTest, DeviceElementTypeReadOnlyIsNotCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Readonly);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Undefined;
    EXPECT_NE(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(DeviceElementTest, DeviceElementTypeObservableIsCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Observable);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Observable;
    EXPECT_EQ(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(DeviceElementTest, DeviceElementTypeObservableIsNotCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Observable);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Undefined;
    EXPECT_NE(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(DeviceElementTest, DeviceElementTypeWritableIsCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Writable);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Writable;
    EXPECT_EQ(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(DeviceElementTest, DeviceElementTypeWritableIsNotCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Writable);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Undefined;
    EXPECT_NE(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(DeviceElementTest, DeviceElementTypeFunctionIsCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Function);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Function;
    EXPECT_EQ(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}

TEST(DeviceElementTest, DeviceElementTypeFunctionIsNotCorrect)
{
    // Arrange
    DeviceElement *element = new DeviceElementBuilder(
        "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Function);
    // Act
    ElementType testedElement = element->getElementType();
    // Assert
    ElementType expectedResult = ElementType::Undefined;
    EXPECT_NE(expectedResult, testedElement)
        << "expected: " << expectedResult << std::endl
        << "provided: " << testedElement << std::endl;
}
#endif