#include "ObservableMetric_MOCK.hpp"
#include "TestUtils.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::Exactly;

class ParameterizedObservableMetricTests
    : public ::testing::TestWithParam<DataType> {
  DataVariant value_;

  DataVariant read() const { return value_; }

protected:
  void SetUp() override {
    type_ = GetParam();
    value_ = getDefaultVariant(type_);
    mocked_ = make_shared<MockObservableMetric>(type_);
    tested_ = mocked_;
  }

  void bindActions() {
    auto reader = bind(&ParameterizedObservableMetricTests::read, this);
    mocked_->delegateToFake(reader);
  }

  void unbindActions() { mocked_->delegateToFake(nullptr); }

  DataVariant getReadValue() const { return value_; }

  DataType type_;
  MockObservableMetricPtr mocked_;
  ObservableMetricPtr tested_;
};

TEST_P(ParameterizedObservableMetricTests, canReadDefault) {
  EXPECT_CALL(*mocked_, getMetricValue()).Times(Exactly(1));

  optional<DataVariant> read_value = nullopt;
  EXPECT_FALSE(read_value.has_value());
  EXPECT_NO_THROW(read_value = tested_->getMetricValue());
  EXPECT_TRUE(read_value.has_value());

  auto default_value = setVariant(type_).value();
  EXPECT_EQ(default_value, read_value.value());
}

TEST_P(ParameterizedObservableMetricTests, canReadCustom) {
  EXPECT_CALL(*mocked_, getMetricValue()).Times(Exactly(1));
  bindActions();

  optional<DataVariant> read_value = nullopt;
  EXPECT_FALSE(read_value.has_value());
  EXPECT_NO_THROW(read_value = tested_->getMetricValue());
  EXPECT_TRUE(read_value.has_value());

  EXPECT_EQ(getReadValue(), read_value.value());
}

TEST_P(ParameterizedObservableMetricTests, canResetRead) {
  EXPECT_CALL(*mocked_, getMetricValue()).Times(Exactly(1));
  bindActions();

  optional<DataVariant> read_value = nullopt;
  EXPECT_FALSE(read_value.has_value());
  EXPECT_NO_THROW(read_value = tested_->getMetricValue());
  EXPECT_TRUE(read_value.has_value());

  EXPECT_EQ(getReadValue(), read_value.value());
  EXPECT_CALL(*mocked_, getMetricValue()).Times(Exactly(1));
  unbindActions();

  read_value = nullopt;
  EXPECT_FALSE(read_value.has_value());
  EXPECT_NO_THROW(read_value = tested_->getMetricValue());
  EXPECT_TRUE(read_value.has_value());

  auto default_value = setVariant(type_).value();
  EXPECT_EQ(default_value, read_value.value());
  EXPECT_EQ(getDefaultVariant(type_), getReadValue());
}

TEST_P(ParameterizedObservableMetricTests, canGetType) {
  EXPECT_EQ(type_, tested_->getDataType());
}

TEST_P(ParameterizedObservableMetricTests, canAddObserver) {
  MockMetricObserverPtr observer;
  EXPECT_NO_THROW(observer = make_shared<MockMetricObserver>(mocked_));
  EXPECT_TRUE(tested_->hasListeners());
  observer.reset();
  EXPECT_FALSE(tested_->hasListeners());
}

TEST_P(ParameterizedObservableMetricTests, canObserve) {
  MockMetricObserverPtr observer;
  EXPECT_NO_THROW(observer = make_shared<MockMetricObserver>(mocked_));
  EXPECT_CALL(*observer, handleEvent(::testing::_)).Times(Exactly(1));
  EXPECT_NO_THROW(tested_->observed(getDefaultVariant(type_)));
  this_thread::sleep_for(50ms);
}

INSTANTIATE_TEST_SUITE_P(ObservableMetricTests,
    ParameterizedObservableMetricTests,
    ::testing::Values(DataType::Boolean,
        DataType::Integer,
        DataType::Unsigned_Integer,
        DataType::Double,
        DataType::Time,
        DataType::Opaque,
        DataType::String),
    TestNameSuffixSetter());

TEST(ObservableMetricTests, canBuild) {
  EXPECT_NO_THROW(auto observable = make_shared<MockObservableMetric>());
}

TEST(ObservableMetricTests, throwsOnUnsupportedType) {
  EXPECT_THROW({ make_shared<MockObservableMetric>(DataType::None); },
      std::bad_optional_access);

  EXPECT_THROW({ make_shared<MockObservableMetric>(DataType::Unknown); },
      std::logic_error);
}