#include "ReadableMock.hpp"

#include <Variant_Visitor/Visitor.hpp>
#include <gtest/gtest.h>
#include <optional>

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

struct ReadableTestParam {
  explicit ReadableTestParam(const DataVariant& value) : value_(value) {}

  ReadableTestParam(DataType type, const ReadableMock::ReadCallback& callback)
      : value_(callback()), type_(type), read_cb_(callback) {}

  bool hasCallback() const { return read_cb_ != nullptr; }

  DataVariant readResult() const { return value_; }

  DataType readType() const { return type_; }

  ReadableMock::ReadCallback readCallback() const { return read_cb_; }

  string testName() const {
    string result;
    if (hasCallback()) {
      result += "callback";
    } else {
      result += "value";
    }
    return result + toSanitizedString(value_);
  }

private:
  DataVariant value_;
  DataType type_ = DataType::None;
  ReadableMock::ReadCallback read_cb_ = nullptr;
};

struct ReadableTests : public TestWithParam<ReadableTestParam> {
  ReadableTests() {
    auto param = GetParam();
    expected = param.readResult();
    if (param.hasCallback()) {
      tested =
          make_shared<ReadableMock>(param.readType(), param.readCallback());
    } else {
      tested = make_shared<ReadableMock>(expected);
    }
  }

  DataVariant expected;
  ReadableMockPtr tested;
};

TEST_P(ReadableTests, returnsDataType) {
  EXPECT_CALL(*tested, dataType).Times(Exactly(1));

  EXPECT_EQ(tested->dataType(), toDataType(expected));
}

TEST_P(ReadableTests, canChangeDataType) {
  EXPECT_CALL(*tested, dataType).Times(Exactly(1));

  auto other_type = Variant_Visitor::match(
      expected,
      [](bool) { return DataType::Opaque; },
      [](intmax_t) { return DataType::Time; },
      [](uintmax_t) { return DataType::String; },
      [](double) { return DataType::Integer; },
      [](DateTime) { return DataType::Unsigned_Integer; },
      [](const vector<uint8_t>&) { return DataType::Boolean; },
      [](const string&) { return DataType::Double; });

  EXPECT_NO_THROW(tested->updateType(other_type););

  EXPECT_NE(tested->dataType(), toDataType(expected));
}

TEST_P(ReadableTests, canRead) {
  EXPECT_CALL(*tested, read).Times(Exactly(1));

  EXPECT_EQ(tested->read(), expected);
}

TEST_P(ReadableTests, canReadTwice) {
  EXPECT_CALL(*tested, read).Times(Exactly(2));

  EXPECT_EQ(tested->read(), expected);
  EXPECT_EQ(tested->read(), expected);
}

DataVariant otherThan(const DataVariant& input) {
  return Variant_Visitor::match(
      input, // NOLINTBEGIN(readability-magic-numbers)
      [](bool) -> DataVariant { return vector<uint8_t>{0x00, 0x01, 0xAB}; },
      [](intmax_t) -> DataVariant { return DateTime(1756130804); },
      [](uintmax_t) -> DataVariant { return string("A new value"); },
      [](double) -> DataVariant { return intmax_t{-698872}; },
      [](DateTime) -> DataVariant { return uintmax_t{9789121}; },
      [](const vector<uint8_t>&) -> DataVariant { return true; },
      [](const string&) -> DataVariant {
        return 20.3512;
      }); // NOLINTEND(readability-magic-numbers)
}

TEST_P(ReadableTests, canChangeReadValue) {
  EXPECT_CALL(*tested, read).Times(Exactly(2));

  EXPECT_NO_THROW(tested->updateValue(otherThan(expected)););

  EXPECT_NE(tested->read(), expected);
  EXPECT_NE(tested->read(), expected);
}

TEST_P(ReadableTests, canChangeCallback) {
  MockFunction<DataVariant()> mock_callable;
  EXPECT_CALL(*tested, read).Times(Exactly(2));
  EXPECT_CALL(mock_callable, Call())
      .Times(Exactly(2))
      .WillRepeatedly(Return(otherThan(expected)));

  tested->updateCallback(mock_callable.AsStdFunction());

  auto read_value = tested->read();
  EXPECT_NE(read_value, expected);
  EXPECT_EQ(read_value, otherThan(expected));
  EXPECT_EQ(tested->read(), otherThan(expected));
}

// NOLINTBEGIN(readability-magic-numbers)
INSTANTIATE_TEST_SUITE_P(ReadableTestsValues,
    ReadableTests,
    Values( // clang-format off
        ReadableTestParam{DataVariant(true)},
        ReadableTestParam{DataVariant(false)},
        ReadableTestParam{DataVariant(intmax_t{15})},
        ReadableTestParam{DataVariant(intmax_t{-20})},
        ReadableTestParam{DataVariant(uintmax_t{69})},
        ReadableTestParam{DataVariant(3.14)},
        ReadableTestParam{DataVariant(-45.33333333)},
        ReadableTestParam{DataVariant(DateTime())},
        ReadableTestParam{DataVariant(vector<uint8_t>{})},
        ReadableTestParam{DataVariant(vector<uint8_t>{0x11,0x22,0x31})},
        ReadableTestParam{DataVariant(string())},
        ReadableTestParam{DataVariant(string("Hello World"))},
        ReadableTestParam{DataType::Boolean,
            [](){ return DataVariant(true);}},
        ReadableTestParam{DataType::Boolean,
            [](){ return DataVariant(false);}},
        ReadableTestParam{DataType::Integer,
            [](){ return DataVariant(intmax_t{15});}},
        ReadableTestParam{DataType::Integer,
            [](){ return DataVariant(intmax_t{-20});}},
        ReadableTestParam{DataType::Unsigned_Integer,
            [](){ return DataVariant(uintmax_t{69});}},
        ReadableTestParam{DataType::Double,
            [](){ return DataVariant(3.14);}},
        ReadableTestParam{DataType::Double,
            [](){ return DataVariant(-45.33333333);}},
        ReadableTestParam{DataType::Time,
            [](){ return DataVariant(DateTime());}},
        ReadableTestParam{DataType::Opaque,
            [](){ return DataVariant(vector<uint8_t>{});}},
        ReadableTestParam{DataType::Opaque,
            [](){ return DataVariant(vector<uint8_t>{0x11,0x22,0x31});}},
        ReadableTestParam{DataType::String,
            [](){ return DataVariant(string());}},
        ReadableTestParam{DataType::String,
            [](){ return DataVariant(string("Hello World"));}}
    ),  // clang-format on 
    [](const TestParamInfo<ReadableTests::ParamType>& info) {
      return info.param.testName();
    });
// NOLINTEND(readability-magic-numbers)
} // namespace Information_Model::testing