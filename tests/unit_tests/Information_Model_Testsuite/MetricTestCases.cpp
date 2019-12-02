#include "Metric_FAKE.hpp"

#include "gtest/gtest.h"
#include <string>

using namespace Information_Model;
using namespace std;

class MetricTest : public ::testing::Test {
 protected:
  //NOLINTNEXTLINE
  void SetUp() {
    integer = new FakeMetric<int>(
        "12345", "Mock Integer", "Just a Mock Integer", 10);
  }

  //NOLINTNEXTLINE
  void TearDown() {
    delete integer;
  }

  FakeMetric<int>* integer;
};

//NOLINTNEXTLINE
TEST_F(MetricTest, IntegerMetricHasCorrectID) {
  // Act
  string testedElement = integer->getElementRefId();
  // Assert
  string expectedResult = "12345";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

//NOLINTNEXTLINE
TEST_F(MetricTest, IntegerMetricHasCorrectName) {
  // Act
  string testedElement = integer->getElementName();
  // Assert
  string expectedResult = "Mock Integer";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

//NOLINTNEXTLINE
TEST_F(MetricTest, IntegerMetricHasCorrectDescription) {
  string testedElement  = integer->getElementDescription();
  string expectedResult = "Just a Mock Integer";

  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

//NOLINTNEXTLINE
TEST_F(MetricTest, IntegerMetricReturnsCorrectValue) {
  // Act
  int testedElement = 0;
  // Assert
  ASSERT_NO_THROW(testedElement = integer->getMetricValue().getValue());
  int expectedResult = 10;
  EXPECT_EQ(expectedResult, testedElement)
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}