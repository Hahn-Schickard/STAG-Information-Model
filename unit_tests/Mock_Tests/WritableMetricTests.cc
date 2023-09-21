#include "WritableMetric_MOCK.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::Exactly;

class ParameterizedWritableMetricTests
    : public ::testing::TestWithParam<DataType> {
  DataVariant value_;

  DataVariant read() const { return value_; }

  void write(DataVariant value) { value_ = move(value); }

protected:
  void SetUp() override {
    type = GetParam();
    value_ = getNewVariant();
    tested = make_shared<MockWritableMetric>(type);
  }

  void bindActions() {
    auto reader = bind(&ParameterizedWritableMetricTests::read, this);
    auto writer =
        bind(&ParameterizedWritableMetricTests::write, this, placeholders::_1);
    tested->delegateToFake(writer, reader);
  }

  void unbindActions() { tested->delegateToFake(nullptr, nullptr); }

  DataVariant getNewVariant() const {
    switch (type) {
    // NOLINTBEGIN(readability-magic-numbers)
    case DataType::BOOLEAN:
      return DataVariant((bool)true);
    case DataType::INTEGER:
      return DataVariant((intmax_t)-10);
    case DataType::UNSIGNED_INTEGER:
      return DataVariant((uintmax_t)10);
    case DataType::DOUBLE:
      return DataVariant((double)20.0);
    case DataType::TIME:
      return DataVariant(DateTime(25));
    case DataType::OPAQUE:
      return DataVariant(vector<uint8_t>{1, 2, 3, 4, 5});
    case DataType::STRING:
      return DataVariant(string("Hello World!"));
    case DataType::NONE:
    case DataType::UNKNOWN:
    // NOLINTEND(readability-magic-numbers)
    default:
      throw logic_error(
          "Can not initialise variant value with unknown or none data types");
    }
  }

  DataVariant getReadValue() const { return value_; }

  DataType type; // NOLINT(readability-identifier-naming)
  MockWritableMetricPtr tested; // NOLINT(readability-identifier-naming)
};

TEST_P(ParameterizedWritableMetricTests, canReadDefault) {
  EXPECT_CALL(*tested, getMetricValue()).Times(Exactly(1));

  optional<DataVariant> read_value = nullopt;
  EXPECT_FALSE(read_value.has_value());
  EXPECT_NO_THROW(read_value = tested->getMetricValue());
  EXPECT_TRUE(read_value.has_value());

  auto default_value = setVariant(type).value();
  EXPECT_EQ(default_value, read_value.value());
}

TEST_P(ParameterizedWritableMetricTests, canReadCustom) {
  EXPECT_CALL(*tested, getMetricValue()).Times(Exactly(1));
  bindActions();

  optional<DataVariant> read_value = nullopt;
  EXPECT_FALSE(read_value.has_value());
  EXPECT_NO_THROW(read_value = tested->getMetricValue());
  EXPECT_TRUE(read_value.has_value());

  EXPECT_EQ(getReadValue(), read_value.value());
}

TEST_P(ParameterizedWritableMetricTests, canResetRead) {
  EXPECT_CALL(*tested, getMetricValue()).Times(Exactly(1));
  bindActions();

  optional<DataVariant> read_value = nullopt;
  EXPECT_FALSE(read_value.has_value());
  EXPECT_NO_THROW(read_value = tested->getMetricValue());
  EXPECT_TRUE(read_value.has_value());

  EXPECT_EQ(getReadValue(), read_value.value());
  EXPECT_CALL(*tested, getMetricValue()).Times(Exactly(1));
  unbindActions();

  read_value = nullopt;
  EXPECT_FALSE(read_value.has_value());
  EXPECT_NO_THROW(read_value = tested->getMetricValue());
  EXPECT_TRUE(read_value.has_value());

  auto default_value = setVariant(type).value();
  EXPECT_EQ(default_value, read_value.value());
  EXPECT_EQ(getNewVariant(), getReadValue());
}

TEST_P(ParameterizedWritableMetricTests, canWriteCustom) {
  EXPECT_CALL(*tested, setMetricValue(::testing::_)).Times(Exactly(1));
  bindActions();

  auto default_value = setVariant(type).value();
  EXPECT_NO_THROW(tested->setMetricValue(default_value));

  EXPECT_EQ(default_value, getReadValue());
}

TEST_P(ParameterizedWritableMetricTests, canReadWrittenValue) {
  EXPECT_CALL(*tested, setMetricValue(::testing::_)).Times(Exactly(1));
  bindActions();

  auto new_value = getNewVariant();
  EXPECT_NO_THROW(tested->setMetricValue(new_value));

  EXPECT_CALL(*tested, getMetricValue()).Times(Exactly(1));

  optional<DataVariant> read_value = nullopt;
  EXPECT_FALSE(read_value.has_value());
  EXPECT_NO_THROW(read_value = tested->getMetricValue());
  EXPECT_TRUE(read_value.has_value());
  EXPECT_EQ(new_value, read_value.value());
}

TEST_P(ParameterizedWritableMetricTests, canGetType) {
  EXPECT_EQ(type, tested->getDataType());
}

struct SetWritableMetricTestNameSuffix {
  template <class ParamType>
  string operator()(const ::testing::TestParamInfo<ParamType>& info) const {
    return toSanitizedString(info.param);
  }
};

INSTANTIATE_TEST_SUITE_P(WritableMetricTests,
    ParameterizedWritableMetricTests,
    ::testing::Values(DataType::BOOLEAN,
        DataType::INTEGER,
        DataType::UNSIGNED_INTEGER,
        DataType::DOUBLE,
        DataType::TIME,
        DataType::OPAQUE,
        DataType::STRING),
    SetWritableMetricTestNameSuffix());

TEST(WritableMetricTests, throwsOnUnsupportedType) {
  EXPECT_THROW({ make_shared<MockWritableMetric>(DataType::NONE); },
      std::bad_optional_access);

  EXPECT_THROW({ make_shared<MockWritableMetric>(DataType::UNKNOWN); },
      std::logic_error);
}
