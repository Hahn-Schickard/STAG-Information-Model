#include "Metric_MOCK.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::AtLeast;

class MetricTest : public ::testing::Test {
protected:
  // NOLINTNEXTLINE
  void SetUp() {
    integer = make_shared<MockMetric>("12345", "Mock Integer",
                                      "Just a Mock Integer", DataType::INTEGER);
  }

  shared_ptr<MockMetric> integer;
};

// NOLINTNEXTLINE
TEST_F(MetricTest, IntegerMetricHasCorrectID) {
  // Act
  string testedElement = integer->getElementId();
  // Assert
  string expectedResult = "12345";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

// NOLINTNEXTLINE
TEST_F(MetricTest, IntegerMetricHasCorrectName) {
  // Act
  string testedElement = integer->getElementName();
  // Assert
  string expectedResult = "Mock Integer";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

// NOLINTNEXTLINE
TEST_F(MetricTest, IntegerMetricHasCorrectDescription) {
  string testedElement = integer->getElementDescription();
  string expectedResult = "Just a Mock Integer";

  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

// NOLINTNEXTLINE
TEST_F(MetricTest, canGetIntegerValue) {
  // Act
  EXPECT_CALL(*integer.get(), getMetricValue()).Times(AtLeast(1));
  // Assert
  ASSERT_NO_THROW(integer->getMetricValue());
}

TEST_F(MetricTest, canGetIntegerType) {
  // Act
  EXPECT_CALL(*integer.get(), getDataType()).Times(AtLeast(1));
  // Assert
  ASSERT_NO_THROW(integer->getDataType());
}