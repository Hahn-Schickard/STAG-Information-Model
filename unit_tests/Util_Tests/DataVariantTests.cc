#include "DataVariant.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

TEST(DataVariantTests, returnsCorrectSanitizedString) {
  EXPECT_EQ(toSanitizedString(DataType::Boolean), "Boolean");
  EXPECT_EQ(toSanitizedString(DataType::Integer), "SignedInteger");
  EXPECT_EQ(toSanitizedString(DataType::Unsigned_Integer), "UnsignedInteger");
  EXPECT_EQ(toSanitizedString(DataType::Double), "DoubleFloatingPoint");
  EXPECT_EQ(toSanitizedString(DataType::Timestamp), "Timestamp");
  EXPECT_EQ(toSanitizedString(DataType::Opaque), "OpaqueByteArray");
  EXPECT_EQ(toSanitizedString(DataType::String), "String");
  EXPECT_EQ(toSanitizedString(DataType::None), "None");
  EXPECT_EQ(toSanitizedString(DataType::Unknown), "Unknown");
}

TEST(DataVariantTests, returnsCorrectSize) {
  // NOLINTBEGIN(bugprone-unchecked-optional-access)
  EXPECT_EQ(size_of(setVariant(DataType::Boolean).value()), sizeof(bool));
  EXPECT_EQ(size_of(setVariant(DataType::Integer).value()), sizeof(intmax_t));
  EXPECT_EQ(size_of(setVariant(DataType::Unsigned_Integer).value()),
      sizeof(uintmax_t));
  EXPECT_EQ(size_of(setVariant(DataType::Double).value()), sizeof(double));
  EXPECT_EQ(
      size_of(setVariant(DataType::Timestamp).value()), sizeof(Timestamp));
  EXPECT_EQ(
      size_of(setVariant(DataType::Opaque).value()), vector<uint8_t>{}.size());
  EXPECT_EQ(size_of(setVariant(DataType::String).value()), string().size());
  // NOLINTEND(bugprone-unchecked-optional-access)
}

} // namespace Information_Model::testing