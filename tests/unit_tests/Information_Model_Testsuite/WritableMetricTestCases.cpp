#include "WritableMetric_FAKE.hpp"

#include "gtest/gtest.h"
#include <string>

using namespace Information_Model;
using namespace std;

class WritableMetricTest : public ::testing::Test {
 protected:
  //NOLINTNEXTLINE
  void SetUp() {
    integer = new FakeWritableMetric<int>(
        "12345", "Mock Integer", "Just a Mock Integer", 0);
  }

  //NOLINTNEXTLINE
  void TearDown() {
    delete integer;
  }

  FakeWritableMetric<int>* integer;
};

//NOLINTNEXTLINE
TEST_F(WritableMetricTest, IntegerMetricHasCorrectID) {
  // Act
  string testedElement = integer->getElementId();
  // Assert
  string expectedResult = "12345";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

//NOLINTNEXTLINE
TEST_F(WritableMetricTest, IntegerMetricHasCorrectName) {
  // Act
  string testedElement = integer->getElementName();
  // Assert
  string expectedResult = "Mock Integer";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

//NOLINTNEXTLINE
TEST_F(WritableMetricTest, IntegerMetricHasCorrectDescription) {
  string testedElement  = integer->getElementDescription();
  string expectedResult = "Just a Mock Integer";

  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

//NOLINTNEXTLINE
TEST_F(WritableMetricTest, IntegerMetricSetsCorrectValue) {
  // Act
  int testedElement = 10;
  integer->setMetricValue(testedElement);
  // Assert
  ASSERT_NO_THROW(testedElement = integer->getMetricValue().getValue());
  int expectedResult = 10;
  EXPECT_EQ(expectedResult, testedElement)
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}