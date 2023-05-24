#include "DeviceElement.hpp"
#include "DeviceMockBuilder.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::AtLeast;

struct DeviceElementExpectations {
  const string name_;
  const string desc_;
  const ElementType type_;
  const DataType data_type_;

  DeviceElementExpectations(const DeviceElementExpectations& other) = default;

  DeviceElementExpectations(const string& name,
      const string& desc,
      ElementType type,
      DataType data_type = DataType::UNKNOWN)
      : name_(name), desc_(desc), type_(type), data_type_(data_type) {}
};

using DeviceElementExpectationsPtr = shared_ptr<DeviceElementExpectations>;

ReadFunctor makeDefaultReader(DataType type) {
  return [type]() -> DataVariant { return setVariant(type); };
}

WriteFunctor makeDefaultWriter(DataType type) {
  return [type](DataVariant value) {
    if (!matchVariantType(value, type)) {
      string error_msg = "Incorrect data variant. Metric expects " +
          toString(type) + " data type, but " + toString(toDataType(value)) +
          " was provided";
      throw logic_error(error_msg);
    }
  };
}

class DeviceElementParametersTests
    : public ::testing::TestWithParam<DeviceElementExpectations> {
protected:
  void SetUp() override {
    expectations = make_shared<DeviceElementExpectations>(GetParam());

    auto builder = make_shared<DeviceMockBuilder>();
    builder->buildDeviceBase("1234", "Mocky", "Mocked device");

    switch (expectations->type_) {
    case ElementType::GROUP: {
      builder->addDeviceElementGroup(expectations->name_, expectations->desc_);
      break;
    }
    case ElementType::READABLE: {
      builder->addReadableMetric(expectations->name_,
          expectations->desc_,
          expectations->data_type_,
          makeDefaultReader(expectations->data_type_));
      break;
    }
    case ElementType::WRITABLE: {
      builder->addWritableMetric(expectations->name_,
          expectations->desc_,
          expectations->data_type_,
          makeDefaultReader(expectations->data_type_),
          makeDefaultWriter(expectations->data_type_));
      break;
    }
    default: {
      string error_msg =
          toString(expectations->type_) + " testing is not supported";
      throw logic_error(error_msg);
    }
    }

    device = builder->getResult();
    element = device->getDeviceElement("1234:0");
  }

  DeviceElementExpectationsPtr expectations;
  DeviceElementPtr element;
  DevicePtr device;
};

// NOLINTNEXTLINE
TEST_P(DeviceElementParametersTests, canGetElementType) {
  ElementType tested;
  EXPECT_NO_THROW(tested = expectations->type_);
  EXPECT_EQ(tested, element->getElementType());
}

struct SetDeviceElementTestNameSuffix {
  template <class ParamType>
  string operator()(const ::testing::TestParamInfo<ParamType>& info) const {
    return info.param.name_;
  }
};

vector<DeviceElementExpectations> makeDeviceElementTestParameters() {
  vector<DeviceElementExpectations> params;
  params.emplace_back("ReadableBoolean",
      "Read Only Boolean Metric",
      ElementType::READABLE,
      DataType::BOOLEAN);
  params.emplace_back("ReadableInteger",
      "Read Only Integer Metric",
      ElementType::READABLE,
      DataType::INTEGER);
  params.emplace_back("ReadableUnsignedInteger",
      "Read Only Unsigned Integer Metric",
      ElementType::READABLE,
      DataType::UNSIGNED_INTEGER);
  params.emplace_back("ReadableDouble",
      "Read Only Double Metric",
      ElementType::READABLE,
      DataType::DOUBLE);
  params.emplace_back("ReadableTime",
      "Read Only Time Metric",
      ElementType::READABLE,
      DataType::TIME);
  params.emplace_back("ReadableOpaque",
      "Read Only Opaque Metric",
      ElementType::READABLE,
      DataType::OPAQUE);
  params.emplace_back("ReadableString",
      "Read Only String Metric",
      ElementType::READABLE,
      DataType::STRING);
  params.emplace_back("WritableBoolean",
      "Read and Writable Boolean Metric",
      ElementType::WRITABLE,
      DataType::BOOLEAN);
  params.emplace_back("WritableInteger",
      "Read and Writable Integer Metric",
      ElementType::WRITABLE,
      DataType::INTEGER);
  params.emplace_back("WritableUnsignedInteger",
      "Read and Writable Unsigned Integer Metric",
      ElementType::WRITABLE,
      DataType::UNSIGNED_INTEGER);
  params.emplace_back("WritableDouble",
      "Read and Writable Double Metric",
      ElementType::WRITABLE,
      DataType::DOUBLE);
  params.emplace_back("WritableTime",
      "Read and Writable Time Metric",
      ElementType::WRITABLE,
      DataType::TIME);
  params.emplace_back("WritableOpaque",
      "Read and Writable Opaque Metric",
      ElementType::WRITABLE,
      DataType::OPAQUE);
  params.emplace_back("WritableString",
      "Read and Writable String Metric",
      ElementType::WRITABLE,
      DataType::STRING);
  params.emplace_back("Group", "Group", ElementType::GROUP);
  return params;
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(DeviceElementTests,
    DeviceElementParametersTests,
    ::testing::ValuesIn(makeDeviceElementTestParameters()),
    SetDeviceElementTestNameSuffix());
