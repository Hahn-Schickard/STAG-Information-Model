#include "WritableMetric_MOCK.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::AtLeast;

class ParameterizedWritableMetricTests
    : public ::testing::TestWithParam<DataType> {
  DataVariant value_;

  DataVariant read() { return value_; }

  void write(DataVariant value) { value_ = move(value); }

protected:
  void SetUp() override {
    type = GetParam();
    value_ = setVariant(type).value();
    writable = make_shared<MockWritableMetric>(type);
    auto reader = bind(&ParameterizedWritableMetricTests::read, this);
    auto writer =
        bind(&ParameterizedWritableMetricTests::write, this, placeholders::_1);
    writable->delegateToFake(writer, reader);
  }

  DataType type; // NOLINT(readability-identifier-naming)
  MockWritableMetricPtr writable; // NOLINT(readability-identifier-naming)
};

TEST_P(ParameterizedWritableMetricTests, canRead) {
  EXPECT_CALL(*writable.get(), getMetricValue()).Times(AtLeast(1));
  EXPECT_NO_THROW(writable->getMetricValue());
}

TEST_P(ParameterizedWritableMetricTests, canWrite) {
  EXPECT_CALL(*writable.get(), setMetricValue(::testing::_)).Times(AtLeast(1));
  EXPECT_NO_THROW(writable->setMetricValue(setVariant(type).value()));
}

inline DataVariant setNewVariant(DataType type) {
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

TEST_P(ParameterizedWritableMetricTests, canReadWrittenValue) {
  EXPECT_CALL(*writable.get(), setMetricValue(::testing::_)).Times(AtLeast(1));

  auto new_value = setNewVariant(type);
  EXPECT_NO_THROW(writable->setMetricValue(new_value));

  EXPECT_CALL(*writable.get(), getMetricValue()).Times(AtLeast(1));

  optional<DataVariant> read_value = nullopt;
  EXPECT_FALSE(read_value.has_value());
  EXPECT_NO_THROW(read_value = writable->getMetricValue());
  EXPECT_TRUE(read_value.has_value());
  EXPECT_EQ(new_value, read_value.value());
}

TEST_P(ParameterizedWritableMetricTests, canGetType) {
  EXPECT_EQ(type, writable->getDataType());
}

inline std::string toSanitizedString(DataType type) {
  switch (type) {
  case DataType::BOOLEAN:
    return "Boolean";
  case DataType::INTEGER:
    return "SignedInteger";
  case DataType::UNSIGNED_INTEGER:
    return "UnsignedInteger";
  case DataType::DOUBLE:
    return "Double";
  case DataType::TIME:
    return "Time";
  case DataType::OPAQUE:
    return "OpaqueByteArray";
  case DataType::STRING:
    return "String";
  case DataType::NONE:
    return "None";
  case DataType::UNKNOWN:
  default:
    return "Unknown";
  }
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