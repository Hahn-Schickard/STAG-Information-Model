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
    integer = make_shared<MockWritableMetric>();
  }

  shared_ptr<MockWritableMetric> integer;
};

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
  ASSERT_NO_THROW(integer->setMetricValue((intmax_t)10));
}

TEST_F(WritableMetricTests, canGetIntegerType) {
  // Act
  EXPECT_CALL(*integer.get(), getDataType()).Times(AtLeast(1));
  // Assert
  ASSERT_NO_THROW(integer->getDataType());
}
