#include "WritableMetric_FAKE.hpp"

#include "gtest/gtest.h"
#include <string>

using namespace std;
using namespace Information_Model;
using namespace Information_Model::testing;

class WritableIntegerTest : public ::testing::Test {
 protected:
  //NOLINTNEXTLINE
  void SetUp() {
    integer = make_shared<FakeWritableMetric<int32_t>>(
        "12345", "Mock Integer", "Just a Mock Integer", 0);
  }
  shared_ptr<Metric> integer;
};

//NOLINTNEXTLINE
TEST_F(WritableIntegerTest, IntegerMetricHasCorrectID) {
  // Act
  string tested_element = integer->getElementRefId();
  // Assert
  string expected_result = "12345";
  EXPECT_STREQ(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << endl
      << "provided: " << tested_element << endl;
}

//NOLINTNEXTLINE
TEST_F(WritableIntegerTest, IntegerMetricHasCorrectName) {
  // Act
  string tested_element = integer->getElementName();
  // Assert
  string expected_result = "Mock Integer";
  EXPECT_STREQ(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << endl
      << "provided: " << tested_element << endl;
}

//NOLINTNEXTLINE
TEST_F(WritableIntegerTest, IntegerMetricHasCorrectDescription) {
  string tested_element  = integer->getElementDescription();
  string expected_result = "Just a Mock Integer";

  EXPECT_STREQ(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << endl
      << "provided: " << tested_element << endl;
}

//NOLINTNEXTLINE
TEST_F(WritableIntegerTest, IntegerMetricSetsCorrectValue) {
  // Arange
  auto integer_cast
      = dynamic_pointer_cast<FakeWritableMetric<int32_t>>(integer);
  // Act
  int tested_element = 10;
  integer_cast->setMetricValue(tested_element);
  // Assert
  ASSERT_NO_THROW(tested_element = integer_cast->getMetricValue().getValue());
  int expected_result = 10;
  EXPECT_EQ(expected_result, tested_element)
      << "expected: " << expected_result << endl
      << "provided: " << tested_element << endl;
}

//NOLINTNEXTLINE
TEST_F(WritableIntegerTest, IntegerMetricReturnsCorrectDataType) {
  // Act
  DataType tested_element = DataType::UNKNOWN;
  // Assert
  ASSERT_NO_THROW(tested_element = integer->getDataType());
  DataType expected_result = DataType::INTEGER;
  EXPECT_EQ(expected_result, tested_element)
      << "expected: " << expected_result << endl
      << "provided: " << tested_element << endl;
}