#ifndef __INFORMATION_MODEL_TEST_UTILITIES_HPP
#define __INFORMATION_MODEL_TEST_UTILITIES_HPP

#include "DataVariant.hpp"

#include "gtest/gtest.h"

#include <stdexcept>

namespace Information_Model {
namespace testing {
inline DataVariant getDefaultVariant(DataType type) {
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
    return DataVariant(std::vector<uint8_t>{1, 2, 3, 4, 5});
  case DataType::STRING:
    return DataVariant(std::string("Hello World!"));
  case DataType::NONE:
  case DataType::UNKNOWN:
  // NOLINTEND(readability-magic-numbers)
  default:
    throw std::logic_error(
        "Can not initialise variant value with unknown or none data types");
  }
}
} // namespace testing
} // namespace Information_Model

struct TestNameSuffixSetter {
  template <class ParamType>
  std::string operator()(
      const ::testing::TestParamInfo<ParamType>& info) const {
    return toSanitizedString(info.param);
  }
};

#endif //__INFORMATION_MODEL_TEST_UTILITIES_HPP