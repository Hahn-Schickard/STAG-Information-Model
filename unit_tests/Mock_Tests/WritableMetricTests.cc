#include "WritableMetric_MOCK.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::AtLeast;

class WritableMetricTests : public ::testing::Test {
protected:
  // NOLINTNEXTLINE
  void SetUp() {
    integer = make_shared<MockWritableMetric>(
        "12345", "Mock Integer", "Just a Mock Integer", DataType::INTEGER);
  }

  shared_ptr<MockWritableMetric> integer;
};

// NOLINTNEXTLINE
TEST_F(WritableMetricTests, IntegerMetricHasCorrectID) {
  // Act
  string testedElement = integer->getElementId();
  // Assert
  string expectedResult = "12345";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

// NOLINTNEXTLINE
TEST_F(WritableMetricTests, IntegerMetricHasCorrectName) {
  // Act
  string testedElement = integer->getElementName();
  // Assert
  string expectedResult = "Mock Integer";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

// NOLINTNEXTLINE
TEST_F(WritableMetricTests, IntegerMetricHasCorrectDescription) {
  string testedElement = integer->getElementDescription();
  string expectedResult = "Just a Mock Integer";

  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

// NOLINTNEXTLINE
TEST_F(WritableMetricTests, canGetIntegerValue) {
  // Act
  EXPECT_CALL(*integer.get(), getMetricValue()).Times(AtLeast(1));
  // Assert
  ASSERT_NO_THROW(integer->getMetricValue());
}

// NOLINTNEXTLINE
TEST_F(WritableMetricTests, canSetIntegerValue) {
  // Act
  EXPECT_CALL(*integer.get(), setMetricValue(::testing::_)).Times(AtLeast(1));
  // Assert
  ASSERT_NO_THROW(integer->setMetricValue((DataVariant)10));
}

TEST_F(WritableMetricTests, canGetIntegerType) {
  // Act
  EXPECT_CALL(*integer.get(), getDataType()).Times(AtLeast(1));
  // Assert
  ASSERT_NO_THROW(integer->getDataType());
}
